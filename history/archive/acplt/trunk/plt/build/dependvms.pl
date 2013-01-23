#! /usr/bin/perl

while (<>) {
    s/\.\./-/g;
    s:([-a-zA-Z.0-9_/]+)/([a-zA-Z.0-9_]*):[$1]$2:g;
    s:/:.:g;
    s/^(.+)\.o:(.*)$/$1.obj:$2/;
    print;
}
