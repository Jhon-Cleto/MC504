#ifndef PCDD_IOCTL_H
#define PCDD_IOCTL_H

typedef struct pcdd_struct
{
    int repeat;
    char name[64];
} pcdd_st;


#define WR_VALUE _IOW('a', 'a', int32_t *)
#define RD_VALUE _IOR('a', 'b', int32_t *)
#define GREETER  _IOW('a', 'c', pcdd_st *)

#endif