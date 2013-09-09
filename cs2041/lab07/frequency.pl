#!/usr/bin/perl -w

foreach $file (glob "poets/*.txt") {
    open(F, "$file") or die;
    $words = 0;
    $count = 0;
    while($line = <F>) {
        chomp $line;
        $words += scalar(split(/[A-Za-z]+/, $line));
        $count += $line =~  m/$ARGV[0]/ig;
    }
    print "$words $count \n";
}
