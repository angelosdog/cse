#!/usr/bin/perl -w

if ($#ARGV+1 < 2) {
    print STDERR "Usage: $0 Not enough arguments\n";
} elsif ($#ARGV+1 > 2) {
    print STDERR "Usage: $0 Too many arguments\n";
} else {
    if($ARGV[0] =~ /^\d+$/) {
        foreach (1..$ARGV[0]) {
            print "$ARGV[1]\n";
        }
    } else {
        print STDERR "Usage: $0\n First argument should be a positive integer";
    }
}
