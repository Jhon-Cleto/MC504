// Kernel headers
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

// CCDD header
#include "ccdd_ioctl.h"

// Meta Dados
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jhonatan Cléto");
MODULE_DESCRIPTION("Caesar Cipher Device Driver");

#define DRIVER_NAME "ccdd"
#define DRIVER_CLASS "CCDDClass"

static dev_t ccdd_nr; // Device Number
static struct class *ccdd_cls; //Device Class
static struct cdev ccdd_dev; // Character Device Struct

// Device Data Structure
ccd_st dds;

// Device Driver File Abstraction
static int ccdd_open(struct inode *i, struct file *f);
static int ccdd_close(struct inode *i, struct file *f);
static ssize_t ccdd_read(struct file *f, char __user *buf, size_t size, loff_t *off);
static ssize_t ccdd_write(struct file *f, const char __user *buf, size_t size, loff_t *off);

// ioctl Function
static long int ccdd_ioctl(struct file *f, unsigned cmd, unsigned long arg);

// Private API
char get_rot_value(char xo);
void default_state(void);

static struct file_operations ccdd_fops = {
    .owner = THIS_MODULE,
    .open = ccdd_open,
    .release = ccdd_close,
    .read = ccdd_read,
    .write = ccdd_write,
    .unlocked_ioctl = ccdd_ioctl
};

// Constructor
static int __init ccdd_init(void)
{
    
    if (alloc_chrdev_region(&ccdd_nr, 0, 1, DRIVER_NAME) < 0)
        return -1;
    
    if ((ccdd_cls = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
    {
        unregister_chrdev_region(ccdd_nr, 1);
        return -1;
    }

    if (device_create(ccdd_cls, NULL, ccdd_nr, NULL, DRIVER_NAME) == NULL)
    {
        class_destroy(ccdd_cls);
        unregister_chrdev_region(ccdd_nr, 1);
        return -1;
    }

    cdev_init(&ccdd_dev, &ccdd_fops);

    if (cdev_add(&ccdd_dev, ccdd_nr, 1) == -1)
    {
        device_destroy(ccdd_cls, ccdd_nr);
        class_destroy(ccdd_cls);
        unregister_chrdev_region(ccdd_nr, 1);
        return -1;
    }

    printk(KERN_INFO "Caesar cipher device driver (CCDD) was registered!\n");
    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(ccdd_nr), MINOR(ccdd_nr));
    
    // Configuração inicial do Device
    printk(KERN_INFO "Booting device in ROT13 mode!\n");
    default_state(); // Estado Padrão

    return 0;
}

// Destructor
static void __exit ccdd_exit(void)
{
    cdev_del(&ccdd_dev);
    device_destroy(ccdd_cls, ccdd_nr);
    class_destroy(ccdd_cls);
    unregister_chrdev_region(ccdd_nr, 1);
    printk(KERN_INFO "CCDD was unregistered!\n");
}


static int ccdd_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "CCDD: open()\n");
    return 0;
}

static int ccdd_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "CCDD: close()\n");
    return 0;
}

static ssize_t ccdd_read(struct file *f, char __user *buf, size_t
                         size, loff_t *off)
{
    printk(KERN_INFO "CCDD: read()\n");
    if (*off >= dds.mem_size)
        return 0;

    if (*off + size > dds.mem_size)
        size = dds.mem_size - *off;

    if (copy_to_user(buf, dds.mem, size))
        return -EFAULT;

    *off += size;
    return size;
}

static ssize_t ccdd_write(struct file *f, const char __user *buf,
                          size_t size, loff_t *off)
{
    int i;
    printk(KERN_INFO "CCDD: write()\n");
    if (*off >= dds.mem_size)
        return 0;

    if (*off + size > dds.mem_size)
        size = dds.mem_size - *off;

    if (copy_from_user(dds.mem, buf, size))
        return -EFAULT;
    
    // Aplica a Cifra de Cesar
    for (i = 0; i < size; i++)
        dds.mem[i] = get_rot_value(dds.mem[i]);

    // Limpa as posições não utilizadas
    for (i = size; i < dds.mem_used; i++)
        dds.mem[i] = '\0';

    dds.mem_used = size; 
    *off += size;
    return size;
}

static long int ccdd_ioctl(struct file *f, unsigned cmd, unsigned long arg)
{
    int i;
    ccd_cop_st copst, *uarg;

    switch (cmd)
    {
        case CC_GET_CONTENT: // Retorna o texto armazenado no device
            
            printk(KERN_INFO "CCDD: Reading device content...\n");
            
            uarg = (ccd_cop_st *) arg;

            if (copy_from_user(&copst, uarg, sizeof(copst)))
                return -EFAULT;
            
            if (copst.op == READ_AND_APPLY)
            {
                printk(KERN_INFO "CCDD: Applying cipher...\n");
                // Aplica a Cifra de Cesar
                for (i = 0; i < dds.mem_size; i++)
                    copst.buff[i] = get_rot_value(dds.mem[i]);

                if (copy_to_user(uarg->buff, copst.buff, dds.mem_size))
                    return -EFAULT;
            }

            else if(copst.op == READ_ONLY)
            {
                printk(KERN_INFO "CCDD: Reading device content only...\n");
                if (copy_to_user(uarg->buff, dds.mem, dds.mem_size))
                    return -EFAULT;
            }
            
            break;

        case CC_SET_CONTENT: // copia o texto para o device e/ou aplica a cifra
            
            uarg = (ccd_cop_st *) arg;
            
            if (copy_from_user(&copst, uarg, sizeof(copst)))
                return -EFAULT;
            
            if (copst.op == APPLY_ONLY)
            {
                printk(KERN_INFO "CCDD: Applying cipher only...\n");
                // Aplica a Cifra de Cesar
                for (i = 0; i < dds.mem_size; i++)
                    copst.buff[i] = get_rot_value(copst.buff[i]);

                if (copy_to_user(uarg->buff, copst.buff, dds.mem_size))
                    return -EFAULT;                           
            }
        
            else if (copst.op == APPLY_AND_SAVE)
            {
                // Aplica a Cifra de Cesar
                printk(KERN_INFO "CCDD: Applying cipher...\n");
                for (i = 0; i < dds.mem_size; i++)
                    dds.mem[i] = get_rot_value(copst.buff[i]);            
                
                printk(KERN_INFO "CCDD: Saving cipher text on device...\n");

                if (copy_to_user(uarg->buff, dds.mem, dds.mem_size))
                    return -EFAULT;                  
            }

            else if (copst.op == SAVE_ONLY)
            {
                // Salva a Mensagem apenas
                printk(KERN_INFO "CCDD: Overwriting device content...\n");
                printk(KERN_INFO "CCDD: Saving text on device...\n");
                for (i = 0; i < dds.mem_size; i++)
                    dds.mem[i] = copst.buff[i];
            }
            
            break;

        case CC_GET_MODE: // Informa o Modo de Operação
            
            printk(KERN_INFO "CCDD: Reading cipher mode...\n");
            
            if (copy_to_user((cc_mode_t *) arg, &dds.mode, sizeof(dds.mode)))
                return -EFAULT;
            
            break;

        case CC_SET_MODE: // Altera o Modo de Operação
            
            printk(KERN_INFO "CCDD: Overwriting cipher mode...\n");
            
            if (copy_from_user(&dds.mode, (cc_mode_t *) arg, sizeof(dds.mode)))
                return -EFAULT;
            
            break;

        case CC_GET_ROT: // Informa a Rotação
            
            printk(KERN_INFO "CCDD: Reading cipher rotation...\n");
            if (copy_to_user((int32_t *) arg, &dds.rot, sizeof(dds.rot)))
                return -EFAULT;            
            break;

        case CC_SET_ROT: // Altera a Rotação
            
            printk(KERN_INFO "CCDD: Overwriting cipher rotation...\n");
            
            if (copy_from_user(&dds.rot, (int32_t *) arg, sizeof(dds.rot)))
                return -EFAULT;
            
            break;
        
        case CC_GET_STATE: // Informa o Estado do Dispositivo
            
            printk(KERN_INFO "CCDD: Reading device state...\n");
            
            if (copy_to_user((ccd_st *) arg, &dds, sizeof(dds)))
                return -EFAULT;
            
            break;

        case CC_RESET_STATE: // Estado Padrão
            
            printk(KERN_INFO "CCDD: Resetting device...\n");
            default_state();
            break;

        case CC_DEBUG_STATE: // DEBUG: Modifica o estado do Dispositivo
            
            printk(KERN_INFO "CCDD: (DEBUG) Setting device state...\n");
            
            if (copy_from_user(&dds, (ccd_st *) arg, sizeof(dds)))
                return -EFAULT;
            
            break;

        default:
            printk(KERN_INFO "Nothing happened!\n");
            return -EINVAL;
    }

    return 0;
}

#define ROT_N(xo, base) (base + (((xo - base) + dds.rot) % dds.mod_n))
#define UNROT_N(xo, base) (base + (((xo - base) + (dds.mod_n - dds.rot)) % dds.mod_n))
char get_rot_value(char xo)
{
    if (xo >= 'A' && xo <= 'Z')
    {
        if (dds.mode == ENCODE)
            return ROT_N(xo, 'A');
        return UNROT_N(xo, 'A');
    }

    else if (xo >= 'a' && xo <= 'z')
    {
        if (dds.mode == ENCODE)
            return ROT_N(xo, 'a');
        return UNROT_N(xo, 'a');
    }

    return xo;
}

void default_state(void)
{
    int i;
    dds.rot = 13; // ROT13
    dds.mod_n = 26; // Limitar ao Alfabeto Romano
    dds.mode = ENCODE; // Encode Mode
    dds.mem_size = BSIZE; // Capacidade do Dispositivo
    dds.mem_used = 0; // Memória totalmente vazia

    // Zerar a memória
    for (i = 0; i < dds.mem_size; i++)
        dds.mem[i] = '\0';
}

module_init(ccdd_init);
module_exit(ccdd_exit);