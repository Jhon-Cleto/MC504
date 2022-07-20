#ifndef CCDD_IOCTL_H
#define CCDD_IOCTL_H

#define CCD_MGN 'C'

typedef enum cc_mode
{
    ENCODE = 0,
    DECODE
} cc_mode_t;

typedef struct ccd_struct
{
    cc_mode_t mode; // Modo de Operação
    int rot; // Tamanho da Rotação
    int mod_n; // Limitar os valores
    char mem[256]; // memória do Dispositivo
    int mem_size; // Tamanho Máximo
    int mem_used; // Tamanho Utilizado
} ccd_st;


#define CC_GET_MODE _IOR(CCD_MGN, 0, cc_mode_t *)
#define CC_SET_MODE _IOW(CCD_MGN, 1, cc_mode_t *)
#define CC_GET_ROT _IOR(CCD_MGN, 2, int32_t *)
#define CC_SET_ROT _IOW(CCD_MGN, 3, int32_t *)
#define CC_GET_STATE _IOR(CCD_MGN, 4, ccd_st *)
#define CC_RESET_STATE _IO(CCD_MGN, 5)
#define CC_DEBUG_STATE _IOW(CCD_MGN, 6, ccd_st *)

#endif