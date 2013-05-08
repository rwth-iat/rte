# ***********************************************************************************
#      ACPLT                                                                        *
#      Lehrstuhl für Prozessleittechnik                                             *
#      RWTH Aachen                                                                  *
#                                                                                   *
# ***********************************************************************************
#      Datei:            start_server.tcl                                           *
#                                                                                   *
#      Historie:		 2013-03-29   Epple     erstellt                            *
#                                                                                   *
#      Beschreibung:     Skript zum Starten des Servers dieser Domäne               *
#                        und zum Nachladen von Bibliotheken und Templates           *
#                                                                                   *
# ***********************************************************************************
#      Hinweis:          -  Das Skript start_server.tcl" befindet sich in           *
#                           der Domäne acplt/system/systools                        *
#                        -  Die Datenbasis db.ovd muss im Ordner                    *
#                           acplt/servers/$THISSERVER vorhanden sein.               *
#                        -  Der Server erhält den Namen der Domäne                  *
# ***********************************************************************************
#                                                                                   *
#      verwendete Unterprogramme:                                                   *
#      ov_runtimeserver, fb_dbcommands                                              *
#                                                                                   *
#      verwendete Umgebungsvariablen                                                *
#      lokal                                                                        *
#        THISACPLTSYSTEM   set     Verzeichnis in dem das gesamte ACPLT-System      *
#                                  liegt (wird automatisch ermittelt)               *
#        THISSERVER        set     Verzeichnis in dem alles abgelegt wird           *
#                                  was zu diesem Server gehört.                     * 
#        SERVERNAME        set     Name des Servers                                 *
#                                  hier standardmäßig der Name der Domäne           *
#        DATABASENAME      set     Name der Datenbasis                              *
#                                  hier standardmäßig "db"                          *
#      prozessweit                                                                  *
#        ACPLT_HOME        set     Aktuelles Home-Verzeichnis für das ACPLT-System  *
#                                  entspricht $THISACPLTSYSTEM$                     *
#        PATH              set     Pfad in dem die zu ladenden Bibliotheken         * 
#                                  liegen müssen:                                   *
#                                  "${THISACPLTSYSTEM}/system/userlibs/bin" und     *
#                                  "${THISACPLTSYSTEM}/system/userlibs/bin"         *
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
# ***********************************************************************************
namespace path {::tcl::mathop ::tcl::mathfunc}

#
# ***********************************************************************************
#  Ermitteln des Verzeichnisses "THISSERVER"
# ***********************************************************************************
set THISSERVER [pwd]
puts "serverdomain:        ${THISSERVER}"
#
#  Ermitteln des Verzeichnisses "THISACPLTSYTEM"
#
set MODELDIR $THISSERVER/modelinstances
puts "modelinstance-domain:${MODELDIR}"
#
#  Ermitteln des Verzeichnisses "THISACPLTSYTEM"
#
cd ../../
set THISACPLTSYSTEM [pwd]
puts "ACPLT systemdomain:  ${THISACPLTSYSTEM}"
#
#  Ermitteln des Verzeichnisses "LIBDIR" und "TEMPLATEDIR" in dem sich die zu ladenden
#  Bibliotheks- und Templateordner befinden
#
set USERBINDIR ${THISACPLTSYSTEM}/system/addonlibs
set PATH1 ${USERBINDIR}
puts "bin-path userlibs    ${PATH1}"
set PATH2 ${THISACPLTSYSTEM}/system/sysbin
puts "bin-path system:     ${PATH2}"
set BINPATH "${PATH1};${PATH2};$env(PATH)"
set TEMPLATEDIR ${THISACPLTSYSTEM}/templates
puts "templatedomain:      ${TEMPLATEDIR}"
#
#  Setzen der Prozess-Umgebungsvariablen 
#
set env(ACPLT_HOME) ${THISACPLTSYSTEM}
set env(PATH) ${BINPATH}
#
#  Setzen des LD_LIBRATY_PATH fuer linux
#
set env(LD_LIBRARY_PATH) "${THISACPLTSYSTEM}/system/sysbin;${THISACPLTSYSTEM}/system/addonlibs;$env(LD_LIBRARY_PATH)"
#
#  Bestimmung des Servernamens 
#
set SERVERNAME [file tail  ${THISSERVER}]
#
#  Setzen des Datenbasisnamens
#
set DATABASENAME db
#
#  Prüfen ob eine Datenbasis existiert
#
puts "databasename:        ${DATABASENAME}"
if {
[file exists ${THISSERVER}/$DATABASENAME.ovd] == 0 } { 
puts "database doesn't exist"
return
} else { puts "database found"}
#
#  Starten des Servers
#

puts "starting server      ${SERVERNAME}"
# crashes otherwise, if there is a space in the path
if {[lsearch $tcl_platform(os) "Windows"] >= 0} then {
	set THISACPLTSYSTEM [file attributes $THISACPLTSYSTEM -shortname]
	set THISSERVER [file attributes $THISSERVER -shortname]
}
set LOGFILE ${THISSERVER}/logfiles/log_start_server.txt
set COMMAND "${THISACPLTSYSTEM}/system/sysbin/ov_runtimeserver -f ${THISSERVER}/${DATABASENAME}.ovd -s ${SERVERNAME} -w ksserv -w fb -w ksservtcp -w ksservhttp -l ${LOGFILE}"
set ACPLT_PROCESS [open "|$COMMAND" "RDWR"]
# pid of the process
set pid [pid $ACPLT_PROCESS]
puts "server ${SERVERNAME} is running with $pid"

proc sleep {time} {
    after $time set end 1
    vwait end
}
  
proc sigint {pid} {
	global tcl_platform
	global THISACPLTSYSTEM
	if {$tcl_platform(os) == "Linux"} then { 
		catch {exec kill -s SIGINT $pid}
	} elseif {[lsearch $tcl_platform(os) "Windows"] >= 0} then {
		catch {exec "${THISACPLTSYSTEM}/system/systools/SendSignal.exe" "$pid"}
	}
	puts "sleeping for 10 seconds"
	sleep 10000
}

# ********************************************************************************
# ********************************************************************************
# ********************************************************************************
# ********************************************************************************
#
#  Eingabeschleife
#
set k 0
while {$k<1} {
puts "-----------------------------------------------------"
puts "ONLINE-Eingabemöglichkeit: m = load modelinstance , t = load template"
gets stdin in1
# ********************************************************************************
# ********************************************************************************
#
# Nachladen von Templates
#
# ********************************************************************************
if {[eq ${in1} x] == 1} {
	sigint $pid
}
if {[eq ${in1} t] == 1} {
#
#  Einlesen der vorhandenen Templateordner
#
#
set i 1
set out ""

set content1 [glob -tails -nocomplain -directory ${TEMPLATEDIR} "*"]

foreach item $content1 {
	   append out "$i  "
	   append out $item
       append out "\n"
	   set i [+ $i 1]
	   }
if {[> $i 1]} {
puts "template groups found:"
puts "\n"
puts "$out"
#
#  Auswahl des Templateordners
#
puts "please enter the groupnumber"  
gets stdin DIRNUMBER
# Templatenummer prüfen
if {[>= ${DIRNUMBER} $i] == 1} {
puts "number not allowed"
continue}
if {[< ${DIRNUMBER} 1] == 1} {
puts "number not allowed"
continue}
# Name des gewählten Templateordners bestimmen
set DIRNAME [lindex $content1 ${DIRNUMBER}-1]
set content2 [glob -tails -nocomplain -directory ${TEMPLATEDIR}/$DIRNAME "*.fbd"]
# content2 enthält alle Files mit Endung .fbd
foreach item $content2 {
	   set out "-  "
	   append out $item
       puts "$out laden ? (j/n) \n"
       gets stdin JN
       if {[| [eq ${JN} J] [eq ${JN} j]] == 0} {
       continue}
	   set LOADFILE "templates/${DIRNAME}/${item}"
 	   set COMMAND1 "${THISACPLTSYSTEM}/system/sysbin/fb_dbcommands -s localhost:7509/${SERVERNAME} -load -f ${LOADFILE}"
       set PRX [open "|$COMMAND1" "RDWR"]
puts "template loaded:     ${item} \n"
}
} else {
puts "no templates found"
}
}

# ********************************************************************************
# ********************************************************************************
#
# Nachladen von Modellinstanzen
#
# ********************************************************************************
if {[eq ${in1} m] == 1} {
#
#  Einlesen der vorhandenen Modelle
#
#
set i 1

set content1 [glob -tails -nocomplain -directory ${MODELDIR} "*.fbd"]
foreach item $content1 {
	   append out "$i  "
	   append out $item
       append out "\n"
	   set i [+ $i 1]
	   }
if {[> $i 1]} {
puts "modelinstances found:"
puts "\n"
puts $out
#
#  Auswahl des zu ladenden Modells
#
gets stdin MODELNUMBER
# Modellnummer prüfen
if {[>= ${MODELNUMBER} $i] == 1} {
puts "number not allowed"
continue}
if {[< ${MODELNUMBER} 1] == 1} {
puts "number not allowed"
continue}
# Name des gewählten Modells bestimmen
set MODELNAME [lindex $content1 ${MODELNUMBER}-1]
set LOADFILE "${MODELDIR}/${MODELNAME}"
set COMMAND1 "${THISACPLTSYSTEM}/system/sysbin/fb_dbcommands -s localhost:7509/${SERVERNAME} -load -f ${LOADFILE}"
set PRX [open "|$COMMAND1" "RDWR"]
puts "model loaded:        ${MODELNAME}  \n"


} else {
	puts "no modelinstances found"
	}
	}
}

return



