#!/usr/bin/perl -w

$flag = 0;
if($ARGV[0] eq "-d") {
    shift @ARGV
    $flag = 1;
} elsif ($ARGV[0] eq "-t") {
    shift @ARGV
    $t_flag = 2;
}
$url = "http://www.timetable.unsw.edu.au/2013/$ARGV[0].html" or die;
open F, "wget -q -O- $url|" or die;