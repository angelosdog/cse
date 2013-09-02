#!/usr/bin/perl -w

if($#ARGV > 0) {
    foreach $f (@ARGV) {
        open(F,"<$f") or die "$0: Can't open $f: $!\n";
        @lines = <F>;
        chomp(@lines); 
        print join("\t", @lines);
        print "\n";
        close(F);
    }
} else {
    @lines = <>;
    chomp(@lines);
    print join("\t", @lines);
    print "\n";
}
