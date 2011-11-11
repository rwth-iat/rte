//
//  -----------------------------------------------------------------------------
// |  WAGO Kontakttechnik GmbH & Co. KG |                                        |
// |  Hansastr. 27                      |  Technical Support                     |
// |  D-32423 Minden                    |                                        |
// |  Tel.: +49(0)571 / 887 - 0         |  Tel.: +49(0)571 / 887 - 555           |
// |  Fax.: +49(0)571 / 887 - 169       |  Fax.: +49(0)571 / 887 - 8555          |
// |  Mail: info@wago.com               |  Mail: support@wago.com                |
// |  www : http://www.wago.com         |                                        |
//  -----------------------------------------------------------------------------
/// \file       wagoset.c       
/// \version    0.02
/// \date       18-MAY-2006
/// \author     Florian Reckmann
///
/// \description :
/// This programm set the configuration of the Fieldbus-Controller in the Flash
/// The Flash is reading from the Bootloader if it start the Linux-Kernel 
///
/// \par History:
/// \history 01.12.2004  Init(LF)
//

#include <asm/types.h>

#define IOCTL_KBUSUPDATE           1
#define IOCTL_GETBININPUTOFFSET    2
#define IOCTL_GETBINOUTPUTOFFSET   3
#define IOCTL_CHANGE_UPDATE_SPEED  4

typedef enum { FALSE, TRUE } BOOL;

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
