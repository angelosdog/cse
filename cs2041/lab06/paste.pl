#!/usr/bin/perl -w

if($#ARGV > 0) {
    foreach $f (@ARGV) {
        open(F,"<$f") or die "$0: Can't open $f: $!\n";
        while (<F>) {
            chomp($_);
            print "$_\t"
        }
        print "\n";
        close(F);
    }
} else {
    while(<>) {
        chomp($_);
        print "$_\t";
    }
}
