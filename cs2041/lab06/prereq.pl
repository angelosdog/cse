#!/usr/bin/perl -w
$recursive = 0;
if ($#ARGV < 0) {
    die;
} elsif ($ARGV[0] eq "-r") {
    shift @ARGV;
    $recursive = 1;
}
if ($ARGV[0] !~ /^([A-Z]{4}\d{4})/) {
    die;
}

sub find_prereq
{
    my $course = $_[0];
    my @prereq = ();
    my @p = ();
    $url = "http://www.handbook.unsw.edu.au/postgraduate/courses/2013/$course.html";
    open F, "wget -q -O- $url|" or die;
    while ($line = <F>) {
        if ($line =~ /<p>Prerequisite:/) {
            @p = split('</p>', $line);
            @p = $p[0] =~ m/[A-Z]{4}\d{4}/g;
            push @prereq, @p;
   
        }
    }
    close F;
    $url = "http://www.handbook.unsw.edu.au/undergraduate/courses/2013/$course.html";
    open F, "wget -q -O- $url|" or die;
    while ($line = <F>) {
        if ($line =~ /<p>Prerequisite:/) {
            @p = split('</p>', $line);
            @p = $p[0] =~ m/[A-Z]{4}\d{4}/g;
            push @prereq, @p;
        }

    }
    close F;
    push @prereqs, @prereq;

    if ($recursive == 1) {
        foreach (@prereq) {
            find_prereq($_);
        }
    }
}

find_prereq($ARGV[0]);
%hash   = map { $_, 1 } @prereqs;
@prereqs = keys %hash;
@prereqs = sort @prereqs;
print join("\n", @prereqs);
print ("\n");
