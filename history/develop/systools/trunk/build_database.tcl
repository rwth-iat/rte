
# ***********************************************************************************
#      ACPLT                                                                        *
#      Lehrstuhl für Prozessleittechnik                                             *
#      RWTH Aachen                                                                  *
#                                                                                   *
# ***********************************************************************************
#      Datei:            build_database.tcl                                         *
#                                                                                   *
#      Historie:		 2013-03-29   Epple     erstellt                            *
#                                                                                   *
#      Beschreibung:     Skript zum Anlegen einer neuen Datenbasis                  *
#                        für den Server dieser Domäne                               *
#                                                                                   *
#                                                                                   *
# ***********************************************************************************
#      Hinweis:          -  Das Skript "build_database.tcl" befindet sich in        *
#                           der Domäne acplt/system/tools                           *
#                        -  Das Skript legt eine neue Datenbasis im                 *
#                           Ordner acplt/servers/$THISSERVER an.                    *
#                        -  Die Datenbasis erhält den Namen "db"                    *
#                        -  Eine vorhandene Datenbasis wird gelöscht.               *
# ***********************************************************************************
#      verwendete Unterprogramme:                                                   *
#      ov_dbutil                                                                    *
#                                                                                   *
#      verwendete Umgebungsvariablen                                                *
#      lokal                                                                        *
#        THISACPLTSYSTEM   set     Verzeichnis in dem das gesamte ACPLT-System      *
#                                  liegt (wird automatisch ermittelt)               *
#        THISSERVER        set     Verzeichnis in dem alles abgelegt wird           *
#                                  was zu diesem Server gehört.                     * 
#      prozessweit                                                                  *
#        ACPLT_HOME        set     Aktuelles Home-Verzeichnis für das ACPLT-System  *
#                                  entspricht $THISACPLTSYSTEM$                     *
#        PATH              set     Pfad in dem die zu ladenden Bibliotheken         * 
#                                  liegen müssen:                                   *
#                                  "${THISACPLTSYSTEM}/system/addonlib" und         *
#                                  "${THISACPLTSYSTEM}/system/sysbin"               *
#                                                                                   *
#      folgende Variablen werden gesetzt:                                           *
#        DATABASENAME      set     Name der Datenbasis                              *
#                                  standardmäßig db                                 *
#        databasesize      get     Größe der Datenbasis                             *
#                                  Setzen durch Eingabe                             *
#                                  für einen "unbeschwerten" Betrieb werden         *
#                                  mindestens 1MB empfohlen                         *
# ***********************************************************************************
#
#  Ermitteln des Verzeichnisses "THISSERVER"
# 
set THISSERVER [pwd]
puts "serverdomain:       ${THISSERVER}"
#
#  Ermitteln des Verzeichnisses "THISACPLTSYTEM"
#  Dieses Verzeichnis wird auch aktive Directory
#
cd ../../
set THISACPLTSYSTEM [pwd]
puts "ACPLT systemdomain: ${THISACPLTSYSTEM}"
#
#  Setzen der Prozess-Umgebungsvariablen 
#
set env(ACPLT_HOME) ${THISACPLTSYSTEM}
set env(PATH) "${THISACPLTSYSTEM}/system/sysbin;${THISACPLTSYSTEM}/system/addonlibs;$env(PATH)"
#
#  Setzen des LD_LIBRATY_PATH fuer linux
#
if {$tcl_platform(os) == "Linux"} then { 
  set env(LD_LIBRARY_PATH) "${THISACPLTSYSTEM}/system/sysbin;${THISACPLTSYSTEM}/system/addonlibs;$env(LD_LIBRARY_PATH)"
} 


#
#  Bestimmung des Servernamens 
#
set SERVERNAME [file tail ${THISSERVER}]
puts "servername:         ${SERVERNAME}"
#
#  Festlegung des Namens der Datenbasis 
#
set DATABASENAME "db"
puts "Please choose the size of the database (in byte)"
gets stdin databasesize
#
#  Löschen einer eventuell vorhandenen alten Datenbasis
#
if {
[file exists ${THISSERVER}/${DATABASENAME}.ovd] == 1
} then {file delete ${THISSERVER}/${DATABASENAME}.ovd
}
#
#   Erstellen der neuen Datenbasis
#
# !!!! Achtung, es ist nicht klar, ob und wie hier die Fehlerinfo übergeben wird.
#
set env(ERRORLEVEL) 0
exec system/sysbin/ov_dbutil -c ${databasesize} -f /servers/${SERVERNAME}/${DATABASENAME}.ovd -l ${THISSERVER}/logfiles/log_builddb.txt
if { $env(ERRORLEVEL) == 1} then { puts "failed (create)!" } 
if { $env(ERRORLEVEL) == 1} then { set env(OV_ERROR) 1 }
#
if { $env(ERRORLEVEL) == 0} then {puts "database ${DATABASENAME} created with ${databasesize} byte "}
