#!/usr/bin/perl -w
foreach $file (glob "poets/*.txt") {
    open F, "$file" or die;
    $words = 0;
    $length = 0;
    %wc = ();
    while($line = <F>) {
        $line =~ tr/A-Z/a-z/;
        $line =~ s/[^a-z]/ /;
        next if ($line =~ m/^\s*$/);
        chomp $line;
        @words = split(/[^A-Za-z]+/, $line);
        $length += @words;
        for $word (@words) {
            $wc{$word}++;
        }
    }
    close F;
    $frequency = log(($wc{$ARGV[0]}+=1)/$length);
    $file =~ s/^poets\///;
    printf "log((%d+1)/%6d) = %8.4f %s\n",  $wc{$ARGV[0]}-=1,$length, $frequency, $file;
}
