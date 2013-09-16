#!/usr/bin/perl -w
foreach $file (glob "poets/*.txt") {
    open F, "$file" or die;
    $file =~ s/^poets\///;
    %wc = ();
    $length = 0;
    while($line = <F>) {
        $line =~ tr/A-Z/a-z/;
        #$line =~ s/[^a-z]/ /g;
        next if ($line =~ m/^\s*$/);
        @words = split(/[^A-Za-z]+/, $line);
        $length += @words;
        for $word (@words) {
            $wc{$word}++;
        }
    }
    close F;
    $frequency = $wc{$ARGV[0]}/$length;
    printf "%4d/%6d = %.9f %s\n", $wc{$ARGV[0]},$length, $frequency, $file;
}
