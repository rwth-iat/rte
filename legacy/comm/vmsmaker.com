$!
$! vmsmaker.com -- DCL helper script which changes the current directory
$!                 and then executes a command with parameter(s)
$!
$ set def 'P1'
$ 'P2' 'P3' 'P4' 'P5' 'P6' 'P7' 'P8' 'P9'
