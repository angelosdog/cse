#!/usr/bin/perl -w

if ($#ARGV > 0 && $ARGV[0] =~ /-\d+/) {
    $lines = abs(shift @ARGV);
} else {
    $lines = 10;
}
foreach $i (0..$lines-1) {
    $output[$i] = "";
}

$count = $lines;
if ($#ARGV > 0) {
    foreach $f (@ARGV) {
        open(F,"<$f") or die "$0: Can't open $f: $!\n";
        print "==> $f <==\n";
        while (<F>) {
            $line = $_;
            shift @output;
            $output[$count-1] = $line;
        }
        print @output;
        close(F);
        foreach $i (0..$lines-1) {
            $output[$i] = "";
        } 
        $count = $lines;
    }
} else {
    while (<>) {
        $line = $_;
        shift @output;
        $output[$count-1] = $line;
    }
    print @output;
}
