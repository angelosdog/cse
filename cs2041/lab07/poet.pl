#!/usr/bin/perl

%poems = ();
foreach $file (glob "poets/*.txt") {
    open F, "$file" or die;
    $file =~ s/^poets\///;
    $file =~ s/\.txt$//;
    $file =~ s/_/ /g;
    $length = 0;
    while($line = <F>) {
        $line =~ tr/A-Z/a-z/;
        @words = split(/[^A-Za-z]+/, $line);
        @words = grep(!/^\s*$/, @words);
        $poems{$file}{$length} += @words;
        for $word (@words) {
            $poems{$file}{$wc}{$word}+=1;
        }
    }
    close F;
}

$debug = 0;
if ($ARGV[0] eq "-d") {
    shift @ARGV;
    $debug = 1;
}

foreach $f (@ARGV) {
    %words = ();
    open(F,"<$f") or die "$0: Can't open $f: $!\n";
    while($line = <F>) {
        $line =~ tr/A-Z/a-z/;
        @words_in_line = split(/[^A-Za-z]+/, $line);
        @words_in_line = grep(!/^\s*$/, @words_in_line);
        for $word (@words_in_line) {
            if($words{$word} == 0) {
                $words{$word}++;
            }
            
        }
    }
    close F;

    %log_probabilities = ();
    foreach $poet (keys %poems) {
        $probability = 0;
        foreach $word (keys %words) {
            $probability += log(($poems{$poet}{$wc}{$word}+=1)/($poems{$poet}{$length}));
        }
        $log_probabilities{$poet} = $probability;
    }
    @logs = sort{$log_probabilities{$a} cmp $log_probabilities{$b}} keys %log_probabilities;
    if ($debug == 1) {
        $size = @logs;
        foreach $i (0..$size-1) {
            print "$f: log_probability of $log_probabilities{$logs[$i]} for $logs[$i]\n";
        }
    }
    print "$f most resembles the work of $logs[0] (log-probablity=$log_probabilities{$logs[0]})\n";

}