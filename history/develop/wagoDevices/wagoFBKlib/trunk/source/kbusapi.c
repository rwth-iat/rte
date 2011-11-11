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
///

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <asm/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "kbusapi.h"

int iFD;

static void modifyPABforEffect(void)
{ 
  pstPabOUT->us.Pab[0] = pstPabOUT->us.Pab[0] << 1;
  if(pstPabOUT->us.Pab[0] > 0xFFF) pstPabOUT->us.Pab[0] = 1; 
  return;
}

// -------------------------------------------------------------------------------
/// Oeffnet einen Kanal zum Kbus-Treiber und initialisert die Bibliothek.
/// \retval 0 Kbus-Schnittstelle bereit
/// \retval EACCES Kein Zugriff auf /dev/kbus moeglich
/// \retval ENODEV /dev/kbus existiert nicht auf dem Dateisystem
/// \retval ENOMEM nicht genug Speicher verfuegbar
/// \retval EMFILE zuviele Dateien auf dem System bereits geoeffnet
/// \retval EIO low-level IO Fehler
// -------------------------------------------------------------------------------
int KbusOpen()
{
  iFD = open("/dev/kbus", O_WRONLY);
  
  if(iFD < 1) {
    printf("KBUSAPI: Failed opening fifo for writing: %s", strerror(errno));
    return errno;
  }
  return 0;
}

// -------------------------------------------------------------------------------
/// Aktualisiert Prozessdaten und Prozessabbild. Kann nur nach KbusOpen()
/// aufgerufen werden.
/// \retval 0 Aktualisierungsnachricht erfolgreich gesendet
/// \retval EINVAL Kanal zum Kbus wurde noch nicht oder fehlerhaft geoeffnet
// -------------------------------------------------------------------------------
int KbusUpdate()
{
  int iBytes=0;
  int iTmp;
  
  if(iFD < 1) return -EINVAL;

  iBytes = ioctl(iFD, IOCTL_KBUSUPDATE, &iTmp);
  if (0 >= iBytes)
    return -EINVAL;
  return 0;
}

// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------
int KbusGetBinaryInputOffset()
{
  int iBytes = 0;
  int iInputOffset = 0;
  
  if(iFD < 1) return -EINVAL;

  iBytes = ioctl(iFD, IOCTL_GETBININPUTOFFSET, &iInputOffset);
  if (0 >= iBytes)
    return -EINVAL;
  return iInputOffset;
}

// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------
int KbusGetBinaryOutputOffset()
{
  int iBytes = 0;
  int iOutputOffset = 0;
  
  if(iFD < 1) return -EINVAL;

  iBytes = ioctl(iFD, IOCTL_GETBINOUTPUTOFFSET, &iOutputOffset);
  if (0 >= iBytes)
    return -EINVAL;
  return iOutputOffset;
}

// -------------------------------------------------------------------------------
/// Schliesst den Kanal zum Kbus und gibt allozierte Resourcen wieder frei.
/// \retval 0 Kanal geschlossen
/// \retval EINVAL Kbus-Kanal war nicht (erfolgreich) geoeffnet
// -------------------------------------------------------------------------------
int KbusClose()
{
  /* Close /dev/kbus */
  close(iFD);
  return 0;
}
