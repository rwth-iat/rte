
# Skript:   start_server.tcl
# Skript zum Starten des Servers dieser Dom�ne

# ***************************************************************************************************
# **** Hinweise  
#      29.03.2013   - Vorlage (epple)
# 
# ***************************************************************************************************
# **** Das Skript "start_server.tcl" sollte sich in der Dom�ne ${THISACPLTSYSTEM}/tools/dbmanagement befinden
#	   Das Skript startet den Server auf der Datenbasis "databasename" im Ordner "${THISSERVER}/database" 
#      Die Datenbasis muss bereits exisitieren. 
#      Der Server erh�lt den Namen der Serverdom�ne
#
#      folgende Umgebungsvariablen werden gesetzt:
#      lokal
#        THISACPLTSYSTEM     r/w   Verzeichnis in dem das gesamte ACPLT-System liegt
#                                 
#        THISSERVER          r/w   Verzeichnis in dem alles was zu dem speziellen Server geh�rt
#                                  liegt
#        OLDPATH             r/w   gesicherter Systempfad
#      prozessweit
#        ACPLT_HOME          r/w   Aktuelles Home-Verzeichnis f�r die ACPLT-Tools 
#                                  (Bemerkung: ist von der Benennung nicht gl�cklich da speziell
#                                  f�r Server)
#        OV_Library_PATH     r/w   Verzeichnis in dem die zu ladenden Bibliotheken liegen m�ssen
#                                  hier standardm��ig "${THISACPLTSYSTEM}/user/libs"
#        INSTALLPATH         r/w   Pfad in dem die Systemprogramme liegen
#                                  hier standardm��ig "${THISACPLTSYSTEM}/bin"
#        PATH                r/w   Systempfad
#        REGISTRYLANGUAGEGERMAN r  Sprache
#        ERRORLEVEL             r  Fehlercode
#
#      folgende Variablen werden gesetzt:
#        SERVERNAME                Name des Servers
#                                  hier standardm��ig der Name der Dom�ne
#        DATABASENAME              Name der Datenbasis
#                                  hier standardm��ig "db"
#
# ***************************************************************************************************
namespace path {::tcl::mathop ::tcl::mathfunc}
#
# ***************************************************************************************************
#  Ermitteln des Verzeichnisses "THISSERVER"
# ***************************************************************************************************#cd F:/130314_demostick/acplt
set THISSERVER [pwd]
puts "Aktives Serververzeichnis ist: ${THISSERVER}"
#
#  Ermitteln des Verzeichnisses "THISACPLTSYTEM"
#
cd ../
set THISACPLTSYSTEM [pwd]
puts "ACPLT Systemverzeichnis ist: ${THISACPLTSYSTEM}"
#
#  Setzen der Installationsdatei als DIR#  Dieses Verzeichnis wird auch aktive Directory
#
cd ../
set INSTDIR [pwd]
puts "Das Installationsverzeichnis ist: ${INSTDIR}"
cd acplt
#
#  Ermitteln des Verzeichnisses "MODELDIR" in dem sich die zu ladenden
#  Modellordner befinden
#
set MODELDIR ${THISACPLTSYSTEM}/user/models
puts "Das Modellverzeichnis ist: ${MODELDIR}"
#
#  Setzen der Prozess-Umgebungsvariablen 
#
set env(ACPLT_HOME) ${THISACPLTSYSTEM}
set env(OV_LIBRARY_PATH) "${THISACPLTSYSTEM}/bin;${THISACPLTSYSTEM}/user/libs;$env(PATH)"
#
#  Bestimmung des Servernamens 
#
set SERVERNAME [file tail ${THISSERVER}]
puts "Name des servers: ${SERVERNAME}"
#
#  Setzen des Datenbasisnamens
#
set DATABASENAME db_${SERVERNAME}
#
#  Pr�fen ob eine Datenbasis existiert
#
puts "Name der Datenbasis: ${DATABASENAME}"
if {
[file exists ${THISACPLTSYSTEM}/database/$DATABASENAME.ovd] == 0 } { 
puts "Datenbasis exisitiert nicht"
return
} else { puts "Datenbasis gefunden"}
#
#  Starten des Servers
#

puts "Server ${SERVERNAME} wird gestartet"
set env(PATH) ${THISACPLTSYSTEM}
set LOGFILE ${THISACPLTSYSTEM}/${SERVERNAME}/log_serverstart.txt
set COMMAND "${THISACPLTSYSTEM}/bin/ov_runtimeserver.exe -f ${DATABASENAME}.ovd -s ${SERVERNAME} -w ksserv -w fb -w ksservtcp -w ksservhttp -l $LOGFILE"
set ACPLT_PROCESS [open "|$COMMAND" "RDWR"]
puts "-----------------------------------------------------"
puts "Server ${SERVERNAME} l�uft"
# ********************************************************************************
# ********************************************************************************
# ********************************************************************************
# ********************************************************************************
#
#  Eingabeschleife
#
set k 0
while {$k<1} {
puts "ONLINE-Eingabem�glichkeit: l = Load Lib , m = load model"
gets stdin in1
# ********************************************************************************
# ********************************************************************************
#
# Nachladen von Modellen
#
# ********************************************************************************
if {[eq ${in1} m] == 1} {
#
#  Ermitteln des Verzeichnisses "MODELDIR" in dem sich die zu ladenden
#  Modelle befinden
#
set MODELDIR ${THISACPLTSYSTEM}/user/models
puts "Das Modellverzeichnis ist: ${MODELDIR}"
puts "-----------------------------------------------------"
#
#  Einlesen der vorhandenen Modellordner
#
puts "-----------------------------------------------------"
puts "Folgende Modellordner stehen zur Verf�gung:"
puts "\n"
#
set i 1
set content1 [glob -tails -directory ${MODELDIR} "*"]
foreach item $content1 {
	   append out "$i  "
	   append out $item
       append out "\n"
	   set i [+ $i 1]
	   }
puts $out
#
#  Auswahl des Modellordners
#
puts "Bitte geben Sie die Nummer des gew�nschten Modellverzeichnisses ein 
\n ------------------------------------------------------"
gets stdin DIRNUMBER
# Modelnummer pr�fen
if {[>= ${DIRNUMBER} $i] == 1} {
puts "Unzul�ssige Modellnummer"
continue}
if {[< ${DIRNUMBER} 1] == 1} {
puts "Unzul�ssige Modellnummer"
continue}
# Name des gew�hlten Modellordners bestimmen
set DIRNAME [lindex $content1 ${DIRNUMBER}-1]
puts "-----------------------------------------------------"

puts "Modellordner: ${DIRNAME}"
set content2 [glob -tails -directory ${MODELDIR}/$DIRNAME "*.fbd"]
# content2 enth�lt alle Files mit Endung .fbs
foreach item $content2 {
	   set out "-  "
	   append out $item
       puts "$out laden ? (j/n) \n"
       gets stdin JN
       if {[| [eq ${JN} J] [eq ${JN} j]] == 0} {
       continue}
	   set LOADFILE $MODELDIR/$DIRNAME/$item
	   set COMMAND1 "${THISACPLTSYSTEM}/tools/ltsoftbin/fb_dbcommands -s localhost:7509/$SERVERNAME -load -f ${LOADFILE}"
       set PRX [open "|$COMMAND1" "RDWR"]
puts "OK  loaded: $item "
}
}
puts "-------------------------------------------------------"

# ********************************************************************************
# ********************************************************************************
#
# Nachladen von Bibliotheken
#
# ********************************************************************************
if {[eq ${in1} l] == 1} {
#
#  Ermitteln des Verzeichnisses "libs" in dem sich die zu ladenden
#  Modelle befinden
#
set LIBDIR ${THISACPLTSYSTEM}/user/libs
puts "Das Bibliotheksverzeichnis ist: ${LIBDIR}"
#
#  Einlesen der vorhandenen Bibliotheken
#
puts "-----------------------------------------------------"
puts "Folgende Bibliotheken stehen zur Verf�gung:"
puts "\n"
#
set i 1
set content31 [glob -tails -directory ${LIBDIR} "*.dll"]
foreach item $content31 {
	   append out "$i  "
	   append out $item
       append out "\n"
	   set i [+ $i 1]
	   }
puts $out
#
#  Auswahl der zu ladenden Bibliothek
#
puts "Bitte geben Sie die Nummer der zu ladenden Bibliothek ein 
\n ------------------------------------------------------"
gets stdin LIBNUMBER
# Nummer pr�fen
if {[>= ${LIBNUMBER} $i] == 1} {
puts "Unzul�ssige Nummer"
continue}
if {[< ${LIBNUMBER} 1] == 1} {
puts "Unzul�ssige Nummer"
continue}
# Name der gew�hlten Bibliothek bestimmen
set LIBNAME [lindex $content31 ${LIBNUMBER}-1]
puts "Soll die Bibliothek $LIBNAME geladen werden? (j/n)"
puts "-----------------------------------------------------"
gets stdin JN
if {[| [eq ${JN} J] [eq ${JN} j]] == 0} {
puts "Laden abgebrochen"
continue
}
set FULLLIBNAME $LIBDIR/$LIBNAME
set env(ERRORLEVEL) 0
set COMMAND1 "bin/ov_dbutil -f ${DATABASENAME}.ovd -w $FULLLIBNAME -l ${SERVERNAME}/log_builddb.txt"
set PRNAME [open "|$COMMAND1" "RDWR"]
if { $env(ERRORLEVEL) == 1} then { puts "failed (loading)!" } 
if { $env(ERRORLEVEL) == 1} then { set env(OV_ERROR) 1 
puts "Bibliothek $LIBNAME wird geladen"}
# set COMMAND1 "${THISACPLTSYSTEM}/tools/ltsoftbin/fb_dbcommands -s localhost:7509/$SERVERNAME -lib $FULLLIBNAME -load"
# set PRNAME [open "|$COMMAND1" "RDWR"]
puts "OK  loaded: $LIBNAME "
}
puts "-------------------------------------------------------"

}

}
return



