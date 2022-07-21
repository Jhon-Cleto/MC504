#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "../src/ccdd_ioctl.h"

typedef int int32_t;

#define DEV_FILE_NAME "/dev/ccdd"

int main()
{
    int op, md_op, fd, ret, test_loop = 1;
    char b2[2*BSIZE];
    memset(b2, 0, 2*BSIZE);

    cc_mode_t mode;
    int32_t rot;
    ccd_st d_state;
    d_state.mem_size = BSIZE;
    d_state.mem_used = 0;
    
    ccd_cop_st copst;
    copst.op = APPLY_AND_SAVE;
    memset(copst.buff, 0, BSIZE);

    printf("Caesar Cipher Device App\nOpening the Device file...\n\n");
    fd = open(DEV_FILE_NAME, O_RDWR);

    if (fd == -1)
    {
        printf("Could not open file!\n");
        return 1;
    }

    while (test_loop)
    {
        // printf("\e[1;1H\e[2J");
        printf("----- CCDD Operations -----\n");
        printf("(00) Exit\n");
        printf("(01) Apply Cipher\n");
        printf("(02) Apply Cipher and Save on Device\n");
        printf("(03) Save on Device only\n");
        printf("(04) Read from Device\n");
        printf("(05) Read from Device and Apply Cipher\n");
        printf("(06) Get Cipher Mode\n");
        printf("(07) Set Cipher Mode\n");
        printf("(08) Get Cipher Rotation\n");
        printf("(09) Set Cipher Rotation\n");
        printf("(10) Get Device State\n");
        printf("(11) Reset Device State\n");
        printf("(12) DEBUG: Set Device State\n");
        printf("---------------------------\n");
        printf("\nType one Operation:\n");
        printf(">> ");
        scanf("%d", &op);

        switch (op)
        {

            case 1:
                memset(b2, 0, 2*BSIZE);
                printf("Enter text to apply cipher:\n");
                printf(">> ");
                scanf(" %[^\n]s", b2);
                
                if(strlen(b2) > BSIZE)
                    printf("Invalid content size!\n");                
                
                else
                {
                    memcpy(copst.buff, b2, BSIZE);
                    printf("Applying cipher using the device...\n");
                    copst.op = APPLY_ONLY;
                    ret = ioctl(fd, CC_SET_CONTENT, &copst);
                    if (ret == -1)
                        perror(">> Error");
                    else
                        printf("Resulting text: %s\n", copst.buff);
                    
                    memset(copst.buff, 0, BSIZE);
                }

                break;            

            case 2:
                memset(b2, 0, 2*BSIZE);
                printf("Enter text to apply cipher and save on device:\n");
                printf(">> ");
                scanf(" %[^\n]s", b2);

                if(strlen(b2) > BSIZE)
                    printf("Invalid content size!\n");     

                else
                {
                    memcpy(copst.buff, b2, BSIZE);
                    printf("Writing text on device...\n");
                    copst.op = APPLY_AND_SAVE;
                    ret = ioctl(fd, CC_SET_CONTENT, &copst);
                    if (ret == -1)
                        perror(">> Error");
                    else
                    {
                        printf("The cipher text has been save on the device!\n");
                        printf("Resulting text: %s\n", copst.buff);
                    }
                    
                    memset(copst.buff, 0, BSIZE);
                }

                break;

            case 3:
                memset(b2, 0, 2*BSIZE);
                printf("Enter text to save on device:\n");
                printf(">> ");
                scanf(" %[^\n]s", b2);

                if(strlen(b2) > BSIZE)
                    printf("Invalid content size!\n");     

                else
                {
                    memcpy(copst.buff, b2, BSIZE);
                    printf("Writing text on device...\n");
                    copst.op = SAVE_ONLY;
                    ret = ioctl(fd, CC_SET_CONTENT, &copst);
                    if (ret == -1)
                        perror(">> Error");
                    else
                        printf("The text has been save on the device!\n");
                    
                    memset(copst.buff, 0, BSIZE);
                }

                break;

            case 4:
                printf("Reading the device...\n");
                copst.op = READ_ONLY;
                ret = ioctl(fd, CC_GET_CONTENT, &copst);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("Text on device: %s\n", copst.buff);
                
                memset(copst.buff, 0, BSIZE);
                break;
            
            case 5:
                printf("Reading the device and applying cipher...\n");
                copst.op = READ_AND_APPLY;
                ret = ioctl(fd, CC_GET_CONTENT, &copst);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("Resulting text: %s\n", copst.buff);
                
                memset(copst.buff, 0, BSIZE);
                break;
            
            case 6:
                printf("Reading the device...\n");
                ret = ioctl(fd, CC_GET_MODE, &mode);
                if (ret == -1)
                    perror(">> Error");
                else
                {
                    printf("The cipher mode is ");
                    if (mode == ENCODE)
                        printf("ENCODE\n");
                    else
                        printf("DECODE\n");
                }
                break;

            case 7:
                printf("Available cipher modes:\n(0) ENCODE\n(1) DECODE\n");
                printf("Type the mode:\n");
                printf(">> ");
                scanf(" %d", &md_op);

                if (!md_op)
                    mode = ENCODE;
                else if (md_op == 1)
                    mode = DECODE;
                
                if (md_op && md_op != 1)
                    printf("Invalid mode!\n");

                else
                {
                    printf("Changing cipher mode...\n");
                    ret = ioctl(fd, CC_SET_MODE, &mode);
                    if (ret == -1)
                        perror(">> Error");
                    else
                    {
                        printf("The cipher mode was changed to: ");
                        if (mode == ENCODE)
                            printf("ENCODE\n");
                        else
                            printf("DECODE\n");
                    }
                }
                break;

            case 8:
                printf("Reading the device...\n");
                ret = ioctl(fd, CC_GET_ROT, &rot);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("Cipher Rotation: %d\n", rot);          
                break;
            
            case 9:
                printf("Type the new rotation (>= 0):\n");
                printf(">> ");
                scanf(" %d", &rot);
                printf("Changing cipher mode...\n");
                ret = ioctl(fd, CC_SET_ROT, &rot);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("Cipher rotation was changed to: %d\n", rot);          
                break;                

            case 10:
                printf("Reading the device...\n");
                ret = ioctl(fd, CC_GET_STATE, &d_state);
                if (ret == -1)
                    perror(">> Error");
                else
                {
                    printf("Cipher Mode: ");
                    if (d_state.mode == ENCODE)
                        printf("ENCODE\n");
                    else
                        printf("DECODE\n");
                    printf("Cipher Rotation: %d\n", d_state.rot);
                    printf("Device Content: %s\n", d_state.mem);
                }
                break;
            
            case 11:
                printf("Resetting device state...\n");
                ret = ioctl(fd, CC_RESET_STATE);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("The device has been reset!\n");
                break;

            case 12:
                memset(b2, 0, 2*BSIZE);
                printf("DEBUG operation!\n");
                printf("Type the device state:\n");
                printf("(mode [0: ENCODE, 1: DECODE], rot [>= 0], content [%d chars]).\n", BSIZE);
                printf(">> ");
                scanf(" %d, %d, %[^\n]s", &md_op, &d_state.rot, b2);

                if (md_op && md_op != 1)
                    printf("Invalid cipher mode!\n");
                
                else if(d_state.rot < 0)
                    printf("Invalid rotation!\n");
                
                else if(strlen(b2) > BSIZE)
                    printf("Invalid content!\n");

                else
                {
                    if (!md_op)
                        d_state.mode = ENCODE;
                    else
                        d_state.mode = DECODE;

                    memcpy(d_state.mem, b2, BSIZE);

                    printf("Writing on device...\n");
                    ret = ioctl(fd, CC_DEBUG_STATE, &d_state);
                    if (ret == -1)
                        perror(">> Error");
                    else
                    {
                        printf("The device state was changed to:\n");
                        printf("Cipher Mode: ");
                        if (d_state.mode == ENCODE)
                            printf("ENCODE\n");
                        else
                            printf("DECODE\n");
                        printf("Cipher Rotation: %d\n", d_state.rot);
                        printf("Device Content: %s\n", d_state.mem);                        
                    }               
                }

                break;

            default:
                test_loop = 0;
                break;
        }

        printf("---------------------------\n\n");
    }
    
    printf("App finished!\nClosing the file...\n");
    close(fd);
    printf("Goodby!\n");
    return 0;
}