#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <asm/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "kbusapi.h"

int iFD = -1;

// -------------------------------------------------------------------------------
/// Oeffnet einen Kanal zum Kbus-Treiber und initialisiert die Bibliothek.
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
  
  if(iFD < 0) {
    printf("KBUSAPI: Failed opening fifo for writing: %s\n", strerror(errno));
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
  
  if(iFD < 0) return -EINVAL;

  iBytes = ioctl(iFD, IOCTL_KBUSUPDATE, &iTmp);
  if (0 >= iBytes)
  {
    printf("KBUSAPI: Failed sending update request: %s\n", strerror(errno));
    return errno;
  }
  return 0;
}

// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------
int KbusGetBinaryInputOffset()
{
  int iBytes = 0;
  int iInputOffset = 0;
  
  if(iFD < 0) return -EINVAL;

  iBytes = ioctl(iFD, IOCTL_GETBININPUTOFFSET, &iInputOffset);
  if (0 >= iBytes)
  { 
    printf("KBUSAPI: Failed getting Bin InputOffset: %s\n", strerror(errno));
    return errno;
  }
  return iInputOffset;
}

// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------
int KbusGetBinaryOutputOffset()
{
  int iBytes = 0;
  int iOutputOffset = 0;
  
  if(iFD < 0) return -EINVAL;

  iBytes = ioctl(iFD, IOCTL_GETBINOUTPUTOFFSET, &iOutputOffset);
  if (0 >= iBytes)
  {
     printf("KBUSAPI: Failed getting BinOutputOffset: %s\n", strerror(errno));
     return errno;
  }
  return iOutputOffset;
}

// -------------------------------------------------------------------------------
/// Schliesst den Kanal zum Kbus und gibt allokierte Ressourcen wieder frei.
/// \retval 0 Kanal geschlossen
/// \retval EINVAL Kbus-Kanal war nicht (erfolgreich) geoeffnet
// -------------------------------------------------------------------------------
int KbusClose()
{
  /* Close /dev/kbus */
  close(iFD);
  return 0;
}
