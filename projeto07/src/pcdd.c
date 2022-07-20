#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

#include <linux/device.h>
#include <linux/cdev.h>

#include <linux/uaccess.h>

#include <linux/ioctl.h>
#include "pcdd_ioctl.h"

// Meta Dados
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jhonatan Cléto");
MODULE_DESCRIPTION("Pseudo Character Device Driver");

#define DRIVER_NAME "pcdd"
#define DRIVER_CLASS "PCDDClass"

static dev_t pcdd_nr; // Device Number
static struct cdev pcdd_dev; // Character Device Struct
static struct class *pcdd_cls; //Device Class

// Buffer do Driver
static char pcdd_buf[256];
static int pcdd_buf_ptr = 0;
static char c;

// Device Driver File Abstraction
static int pcdd_open(struct inode *i, struct file *f);
static int pcdd_close(struct inode *i, struct file *f);
static ssize_t pcdd_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t pcdd_write(struct file *f, const char __user *buf, size_t len, loff_t *off);

// ioctl Function

static long int pcdd_ioctl(struct file *f, unsigned cmd, unsigned long arg);

static struct file_operations pcdd_fops = 
{
    .owner = THIS_MODULE,
    .open = pcdd_open,
    .release = pcdd_close,
    .read = pcdd_read,
    .write = pcdd_write,
    .unlocked_ioctl = pcdd_ioctl
};

// Constructor
static int __init pcdd_init(void)
{
    printk(KERN_INFO "PCDD was registered!\n");
    
    if (alloc_chrdev_region(&pcdd_nr, 0, 1, DRIVER_NAME) < 0)
        return -1;
    
    if ((pcdd_cls = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
    {
        unregister_chrdev_region(pcdd_nr, 1);
        return -1;
    }

    if (device_create(pcdd_cls, NULL, pcdd_nr, NULL, DRIVER_NAME) == NULL)
    {
        class_destroy(pcdd_cls);
        unregister_chrdev_region(pcdd_nr, 1);
        return -1;
    }

    cdev_init(&pcdd_dev, &pcdd_fops);

    if (cdev_add(&pcdd_dev, pcdd_nr, 1) == -1)
    {
        device_destroy(pcdd_cls, pcdd_nr);
        class_destroy(pcdd_cls);
        unregister_chrdev_region(pcdd_nr, 1);
        return -1;
    }

    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n",\
           MAJOR(pcdd_nr), MINOR(pcdd_nr));
    return 0;
}

// Destructor
static void __exit pcdd_exit(void)
{
    cdev_del(&pcdd_dev);
    device_destroy(pcdd_cls, pcdd_nr);
    class_destroy(pcdd_cls);
    unregister_chrdev_region(pcdd_nr, 1);
    printk(KERN_INFO "PCDD was unregistered!\n");
}


static int pcdd_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "PCDD: open()\n");
    return 0;
}

static int pcdd_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "PCDD: close()\n");
    return 0;
}

static ssize_t pcdd_read(struct file *f, char __user *buf, size_t
                         len, loff_t *off)
{
    printk(KERN_INFO "PCDD: read()\n");

    if (copy_to_user(buf, &c, 1) != 0)
        return -EFAULT;

    return 1;
}

static ssize_t pcdd_write(struct file *f, const char __user *buf,
                          size_t len, loff_t *off)
{
    // int min_sz = (len > pcdd_buf_ptr) ? pcdd_buf_ptr : len;
    printk(KERN_INFO "PCDD: write()\n");
    
    if (copy_from_user(&c, buf+len-1, 1) != 0)
        return -EFAULT;
    
    // pcdd_buf_ptr = min_sz;
    return len;
}

static long int pcdd_ioctl(struct file *f, unsigned cmd, unsigned long arg)
{
    switch (cmd)
    {
    case WR_VALUE:
        printk(KERN_INFO "PCDD: ioctl write\n");
        break;
    
    case RD_VALUE:
        printk(KERN_INFO "PCDD: ioctl read\n");
        break;
    
    case GREETER:
        printk(KERN_INFO "PCDD: ioctl greeter\n");
        break;
    }

    return 0;
}

module_init(pcdd_init);
module_exit(pcdd_exit);