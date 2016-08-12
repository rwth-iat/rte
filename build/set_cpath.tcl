#sets environment's CPATH
proc set_cpath {cpath} {
        global env
        if {[info exists env(CPATH)]} {
                set env(CPATH) $cpath:$env(CPATH)
        } else {
                set env(CPATH) $cpath
        }
}

