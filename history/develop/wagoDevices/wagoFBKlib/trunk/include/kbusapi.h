#include <asm/types.h>

#define IOCTL_KBUSUPDATE           _IOWR(253 ,1, void *)
#define IOCTL_GETBININPUTOFFSET    _IOWR(253, 2, void *)
#define IOCTL_GETBINOUTPUTOFFSET   _IOWR(253, 3, void *)
#define IOCTL_CHANGE_UPDATE_SPEED  _IOWR(253, 4, void *)

#define PAB_SIZE 1020
typedef union
{
     struct   /* Zugriff auf PAB als u16 */
     {
       __u16  Pab [PAB_SIZE];       /* Prozess-Abbild */
     
       #if PFC
         __u16  Var [PFC_VAR_SIZE]; /* Variablen-Bereich, direkt hinter PAB */
       #endif
     
     } us;
     
     struct   /* Zugriff auf PAB als u8 */
     {
       __u8 Pab [sizeof (__u16)*PAB_SIZE];       /* Prozess-Abbild */
     
       #if PFC
         __u8 Var [sizeof (__u16)*PFC_VAR_SIZE]; /* Variablen-Bereich, direkt hinter PAB */
       #endif
     
     } uc;
     
}T_PabVarUnion;

#define PABIN (volatile T_PabVarUnion *) 0xFFE060;
#define PABOUT (volatile T_PabVarUnion *) (0xFFE060+sizeof(T_PabVarUnion));
static volatile T_PabVarUnion * pstPabOUT = PABOUT;
static volatile T_PabVarUnion * pstPabIN = PABIN;



extern int KbusOpen(void);
extern int KbusUpdate(void);
extern int KbusClose(void);
extern int KbusGetBinaryInputOffset(void);
extern int KbusGetBinaryOutputOffset(void);
