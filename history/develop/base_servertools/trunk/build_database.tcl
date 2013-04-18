
# ***********************************************************************************
#      ACPLT                                                                        *
#      Lehrstuhl für Prozessleittechnik                                             *
#      RWTH Aachen                                                                  *
#                                                                                   *
# ***********************************************************************************
#      Datei:            build_database.tcl                                         *
#                                                                                   *
#      Historie:		 2913-03-29   Epple     erstellt                            *
#                                                                                   *
#      Beschreibung:     Skript zum Anlegen einer neuen Datenbasis                  *
#                        für den Server dieser Domäne                               *
#                                                                                   *
#                                                                                   *
# ***********************************************************************************
#      Hinweis:          -  Das Skript "build_database.tcl" sollte sich in          *
#                           der Domäne acplt/tools/dbmanagemnt befinden             *
#                        -  Das Skript legt eine neue Datenbasis im                 *
#                           Ordner acplt/database an.                               *
#                        -  Die Datenbasis erhält den Namen db_$servername           *
#                        -  Eine vorhandene Datenbasis wird gelöscht.               *
# ***********************************************************************************
#      verwendete Unterprogramme:                                                   *
#      ov_dbutil                                                                    *
#                                                                                   *
#      verwendete Umgebungsvariablen                                                *
#      lokal                                                                        *
#        THISACPLTSYSTEM   r/w     Verzeichnis in dem das gesamte ACPLT-System      *
#                                  liegt(muss individuell gesetzt werden)           *
#        THISSERVER        r/w     Verzeichnis in dem alles abgelegt wird           *
#                                  was zu diesem Server gehört.                     * 
#      prozessweit                                                                  *
#        ACPLT_HOME        r/w     Aktuelles Home-Verzeichnis für die ACPLT-Tools   *
#                                  (Bemerkung: ist von der Benennung nicht          *
#                                  glücklich da speziell für Server)                *
#        OV_Library_PATH   r/w     Verzeichnis in dem die zu ladenden Bibliotheken  * 
#                                  liegen müssen                                    *
#                                  standardmäßig "${THISACPLTSYSTEM}/user/libs"     *
#        INSTALLPATH       r/w     Pfad in dem die Systemprogramme liegen           *
#                                  hier standardmäßig "${THISACPLTSYSTEM}/bin"      *
#                                                                                   *
#      folgende Variablen werden gesetzt:                                           *
#        DATABASENAME      r/w     Name der Datenbasis                              *
#                                  standardmäßig db_$servername                     *
#        databasesize      r/w     Größe der Datenbasis                             *
#                                  Setzen durch Eingabe                             *
#                                  für einen "unbeschwerten" Betrieb werden         *
#                                  mindestens 1MB empfohlen                         *
# ***********************************************************************************
#
#  Ermitteln des Verzeichnisses "THISSERVER"
# 
set THISSERVER [pwd]
puts "Aktives Serververzeichnis ist: ${THISSERVER}"
#
#  Ermitteln des Verzeichnisses "THISACPLTSYTEM"
#  Dieses Verzeichnis wird auch aktive Directory
#
cd ../
set THISACPLTSYSTEM [pwd]
puts "Aktives Systemverzeichnis ist: ${THISACPLTSYSTEM}"
#
#  Setzen der Prozess-Umgebungsvariablen 
#
set env(ACPLT_HOME) ${THISACPLTSYSTEM}
set env(OV_LIBRARY_PATH) ${THISACPLTSYSTEM}/user/libs
set env(INSTALLPATH) ${THISACPLTSYSTEM}/bin
#
#  Bestimmung des Servernamens 
#
set SERVERNAME [file tail ${THISSERVER}]
puts "Name des servers: ${SERVERNAME}"
#
#  Festlegung des Namens der Datenbasis 
#
set DATABASENAME "db_${SERVERNAME}"
puts "Bitte geben Sie die gewünschte Größe der Datenbasis $DATABASENAME an:"
gets stdin databasesize
puts "Größe der Datenbasis: ${databasesize}"
#
#  Löschen einer eventuell vorhandenen alten Datenbasis
#
if {
[file exists ${THISACPLTSYSTEM}/database/${DATABASENAME}.ovd] == 1
} then {file delete ${THISACPLTSYSTEM}/database/${DATABASENAME}.ovd
}
#
#   Erstellen der neuen Datenbasis
#
# !!!! Achtung, es ist nicht klar, ob und wie hier die Fehlerinfo übergeben wird.
#
set env(ERRORLEVEL) 0
exec bin/ov_dbutil -c ${databasesize} -f ${DATABASENAME}.ovd -l ${SERVERNAME}/log_builddb.txt
if { $env(ERRORLEVEL) == 1} then { puts "failed (create)!" } 
if { $env(ERRORLEVEL) == 1} then { set env(OV_ERROR) 1 }
#
puts "Database ${DATABASENAME} erstellt"
