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
set USERBINDIR "${THISACPLTSYSTEM}/system/addonlibs ${THISACPLTSYSTEM}/system/syslibs"
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
set env(PATH) "${THISACPLTSYSTEM}/system/sysbin;${THISACPLTSYSTEM}/system/addonlibs;${THISACPLTSYSTEM}/system/syslibs;$env(PATH)"
#
#  Setzen des LD_LIBRATY_PATH fuer linux
#
if {$tcl_platform(os) == "Linux"} then {
   if {[info exists env(LD_LIBRARY_PATH)]} {
         set env(LD_LIBRARY_PATH) "${THISACPLTSYSTEM}/system/addonlibs;${THISACPLTSYSTEM}/system/sysbin;${THISACPLTSYSTEM}/system/syslibs;$env(LD_LIBRARY_PATH)"
   } else {
         set env(LD_LIBRARY_PATH) "${THISACPLTSYSTEM}/system/addonlibs;${THISACPLTSYSTEM}/system/sysbin;${THISACPLTSYSTEM}/system/syslibs"
   }
}

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
set COMMAND "ov_runtimeserver -c [file nativename ${THISSERVER}/ov_server.conf] -l ${LOGFILE}"
catch {exec ov_runtimeserver -c [file nativename ${THISSERVER}/ov_server.conf] -l ${LOGFILE} &} pid
set tries 0

#Prüfen ob server hochgfährt
while {$tries<2000} {
		if {
			[file exists ${LOGFILE}] == 1 } {
			set in [open ${LOGFILE} r]
			set line 0
			set tries [+ $tries 1]
			while {[gets $in line] != -1} {
			
				if {[regexp "(.*)Server started(.*)" $line] } then {
					puts $tries
					# pid of the process
					
					puts "server ${SERVERNAME} is running with PID $pid"
					set tries 2000
				}
			}
			close $in 
		}
		after 10 
	}
	

	


# pid of the process


proc sleep {time} {
    after $time set end 1
    vwait end
}
  
# ********************************************************************************
# ********************************************************************************
# ********************************************************************************
# ********************************************************************************
#
#  Eingabeschleife
#
set in1 1;
set k 0
while {$k<1} {

# Prüfung ob server läuft
fconfigure stdin -blocking 0
if {[lsearch $tcl_platform(os) "Windows"] >= 0} then {
	if { [catch {file rename ${LOGFILE} ${LOGFILE}.test}] } {
		#puts "running"
	} else {
		file rename ${LOGFILE}.test ${LOGFILE}
		puts "Server was terminated"
		break
	}
} else { 	
	#probing if the process is still alive
	set running [exec ps -A | grep $pid]
	if { $running == "" || [regexp "(.*)defunct(.*)" $running]} {
		puts "Server was terminated"
		break
	}
}
if { $in1 > -1 } then {
puts "-----------------------------------------------------"
puts "ONLINE-Eingabemöglichkeit: m = load modelinstance , t = load template , quit = shutdown server"
}
after 1000
gets stdin in1
fconfigure stdin -blocking 1
# ********************************************************************************
# ********************************************************************************
#
# Nachladen von Templates
#
# ********************************************************************************
if {[eq ${in1} "quit"] == 1} {
if {$tcl_platform(os) == "Linux"} then {
		set COMMAND1 "kill $pid" 
	} else {
	set LOADFILE "$THISACPLTSYSTEM/system/systools/kill.fbd"
 	set COMMAND1 "${THISACPLTSYSTEM}/system/sysbin/fb_dbcommands -s localhost:7509/${SERVERNAME} -load -f ${LOADFILE}"
	}
	puts "Initated shutdown, waiting for server to unmap the database..."
    set PRX [open "|$COMMAND1" "RDWR"]
	
	set tries 0
	while {$tries<60} {
		set in [open ${LOGFILE} r]
		set line 0
		set tries [+ $tries 1]
		while {[gets $in line] != -1} {
			if {[regexp "(.*)Database unmapped(.*)" $line] || $tries > 50} then {
				exit
			}
		}
		after 500 
	}
	close $in
}
if {[eq ${in1} t] == 1} {
#
#  Einlesen der vorhandenen Templateordner
#
#
set i 1
set out ""

set content1 [glob -types d -tails -nocomplain -directory ${TEMPLATEDIR} "*"]

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
set content2 [glob -types f -tails -nocomplain -directory ${TEMPLATEDIR}/$DIRNAME "*.fbd"]
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



