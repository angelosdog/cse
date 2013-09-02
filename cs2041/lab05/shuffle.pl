#!/usr/bin/perl -w

$size = 0;
while ($line = <STDIN>) {
    $input[$size] = $line;
    $size++;
}

while ($size > 0) {
    $index = rand($size);
    print $input[$index];
    splice (@input, $index, 1);
    $size--;
}
