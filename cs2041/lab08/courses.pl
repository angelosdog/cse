#!/usr/bin/perl -w
$url = "http://www.timetable.unsw.edu.au/current/$ARGV[0]KENS.html" or die;
open F, "wget -q -O- $url|" or die;
while($line = <F>) {
    if ($line =~ m/<a.*>$ARGV[0].*<\/a>/) {
        $line=~ /.*($ARGV[0].?[0-9]{4}).*/;
        print "$1\n";
    }
}

