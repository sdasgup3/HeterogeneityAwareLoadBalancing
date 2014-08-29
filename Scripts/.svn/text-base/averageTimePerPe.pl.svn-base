#!/usr/bin/perl

use warnings;
use strict;
use Getopt::Long;

my $filename = "";
my $num_pe = "";

##################################
#Usage: .pl -f <input file name> -p num_of_cores
#
####################################

GetOptions (
            "file=s"    => \$filename,
            "p=s"    => \$num_pe,
            )
 or die("Error in command line arguments\n");


open(my $fp, "< $filename") or  die("Cannot Open file\n");

my @lines = <$fp>;
close($fp);
my %ghash_count ;
my %ghash_time ;
my ($chareId, $peId, $time) ;

for(my $i = 0 ;  $i < $num_pe; $i ++) {
  $ghash_count{$i}  = 0;
  $ghash_time{$i}   = 0.0;
}


foreach my $line (@lines) {
  chomp $line;
  ($chareId, $peId, $time) = split(/,/, $line);
  chomp $chareId;
  $peId =~ s/^\s+|\s+$//g ;
  $time =~ s/^\s+|\s+$//g ;

  if(defined($peId)) {
    $ghash_count{$peId} += 1;
    $ghash_time{$peId}  += $time;
  }
}

print "Pe\t\taveragetime\n";
for(my $i = 0 ;  $i < $num_pe; $i ++) {
  print "$i\t\t". "$ghash_time{$i} $ghash_count{$i} = ".  eval($ghash_time{$i}/ $ghash_count{$i}). "\n";
}


