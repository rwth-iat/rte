set ID=1
set DBNAME=server%ID%
del C:\Programme\iFBSpro\server\database\%DBNAME%.ovd
fb_dbutil -c 1000000 -d %DBNAME%
start fb_server -d %DBNAME% -lib ksapi -lib msgsys