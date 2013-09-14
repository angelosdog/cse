#!/usr/bin/perl -w

foreach $file (glob "poets/*.txt") {

    open F, "$file" or die;
    $words = 0;
    $count = 0;
    while($line = <F>) {
        $words += split(/[^A-Za-z]+/, $line);
        
        $count += $line =~  m/\b$ARGV[0]\b/ig;
    }
    $frequency = log($count+1/$words);
    $file =~ s/^poets\///;
    printf "log((%d+1)/%6d) = %8.4f %s\n", $count,$words, $frequency, $file;
}
