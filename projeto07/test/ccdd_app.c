#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "../src/ccdd_ioctl.h"

typedef int int32_t;

#define DEV_FILE_NAME "/dev/ccdd"

#define BSIZE 256

int main()
{
    int op, md_op, fd, ret, test_loop = 1;
    cc_mode_t mode;
    int32_t rot;
    ccd_st d_state;
    d_state.mem_size = BSIZE;
    d_state.mem_used = 0;
    
    char buffer[BSIZE];
    memset(buffer, 0, BSIZE);

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
        printf("---- CCDD Operations ----\n");
        printf("(1) Read from Device\n");
        printf("(2) Write on Device\n");
        printf("(3) Get Cipher Mode\n");
        printf("(4) Set Cipher Mode\n");
        printf("(5) Get Cipher Rotation\n");
        printf("(6) Set Cipher Rotation\n");
        printf("(7) Get Device State\n");
        printf("(8) Reset Device State\n");
        printf("(9) DEBUG: Set Device State\n");
        printf("(0) Exit\n");
        printf("\nType one Operation:\n");
        printf(">> ");
        scanf("%d", &op);

        switch (op)
        {
            case 1:
                printf("Reading the device...\n");
                ret = read(fd, buffer, BSIZE);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("Text on device: %s\n", buffer);
                
                memset(buffer, 0, BSIZE);
                break;
            
            case 2:
                printf("Enter text to write on device:\n");
                printf(">> ");
                scanf(" %[^\n]s", &buffer);
                printf("Writing text on device...\n");
                ret = write(fd, buffer, strlen(buffer));
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("The text has been written to the device!\n");
                
                memset(buffer, 0, BSIZE);
                break;
            
            case 3:
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

            case 4:
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

            case 5:
                printf("Reading the device...\n");
                ret = ioctl(fd, CC_GET_ROT, &rot);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("Cipher Rotation: %d\n", rot);          
                break;
            
            case 6:
                printf("Type the new rotation:\n");
                printf(">> ");
                scanf(" %d", &rot);
                printf("Changing cipher mode...\n");
                ret = ioctl(fd, CC_SET_ROT, &rot);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("Cipher rotation was changed to: %d\n", rot);          
                break;                

            case 7:
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
            
            case 8:
                printf("Resetting device state...\n");
                ret = ioctl(fd, CC_RESET_STATE);
                if (ret == -1)
                    perror(">> Error");
                else
                    printf("The device has been reset!\n");
                break;

            case 9:
                char b2[512];
                memset(b2, 0, 512);
                printf("DEBUG operation!\n");
                printf("Type the device state:\n");
                printf("(mode [0: ENCODE, 1: DECODE], rot [>= 0], content [%d chars]).\n", BSIZE);
                printf(">> ");
                scanf(" %d, %d, %[^\n]s", &md_op, &d_state.rot, &b2);

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

        printf("\n---------------------------\n");
    }
    
    printf("App finished!\nClosing the file...\n");
    close(fd);
    printf("Goodby!\n");
    return 0;
}