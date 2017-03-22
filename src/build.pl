#!/usr/bin/perl

##
# Automate application build.
#
# Execution call:
#
# perl build.pl
#
# To a verbose build:
#
# perl build.pl --verbose
#

use strict;
use warnings;
use Cwd;
my $rc;

my $buildDir;

my $cmakeGenerator;
my $buildToolMake;
my $buildToolMakeArguments;
my $buildType;


sub usage {
  print STDERR "\n";
  print STDERR "$0 [Parameters]\n";
  print STDERR "\n";
  print STDERR "Parameters:\n";
  print STDERR "-help\n  Display this help\n";
  print STDERR "-builddir\n  The build directory\n";
  print STDERR "-buildtype\n  The build type (Release / Debug)\n";
  print STDERR "\n";

  0;
}

#
# Main routine
#
BEGIN {
  $buildDir       = 'build';
  $cmakeGenerator = '';
  $buildToolMake  = '';
  $buildToolMakeArguments  = '';
  $buildType      = 'Release';

  print STDERR "\n";
  print STDERR "Building..\n";
  print STDERR "\n";

  for (my $i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] =~ /-{1,2}help/) {
        usage();
        exit 0;
    }
    elsif ($ARGV[$i] =~ /-{1,2}builddir/) {
      ++$i;

      if ($i > $#ARGV) {
        print "[Error] Invalid build directory.\n";
        usage();
        exit 1;
      }

      $buildDir = $ARGV[$i];
    }
    elsif ($ARGV[$i] =~ /-{1,2}buildtype/) {
      ++$i;

      if ($i > $#ARGV || ($ARGV[$i] ne 'Release' && $ARGV[$i] ne 'Debug')) {
        print "[Error] Invalid build type.\n";
        usage();
        exit 1;
      }

      $buildType = $ARGV[$i];
    }
    elsif ($ARGV[$i] =~ /-g|-{1,2}buildgenerator/) {
      ++$i;

      if ($i > $#ARGV || ($ARGV[$i] ne 'Release' && $ARGV[$i] ne 'Debug')) {
        print "[Error] Invalid CMake generator.\n";
        usage();
        exit 1;
      }

      $cmakeGenerator = $ARGV[$i];
    }
    elsif ($ARGV[$i] =~ /-{1,2}verbose/) {
      $buildToolMakeArguments .= ' VERBOSE=1';
    }
    elsif ($ARGV[$i] =~ /-{1,2}clean/) {
      $buildToolMakeArguments .= ' clean';
    }
  }

  print STDERR 'OS: ', $^O, "\n";
  print STDERR 'Build directory: ', $buildDir, "\n";
  print STDERR 'Build type: ', $buildType, "\n";

  my $dirSeparator = '';

  if ($^O eq 'linux' || $^O eq 'msys') {
    $dirSeparator = '/';
  }
  else {
    $dirSeparator = '\\';
  }

  #
  # Creating (if needed) the build directory)
  #
  if (! -e ($buildDir . $dirSeparator . $^O)) {
    print STDERR "Trying to creating the build directory.\n";

    mkdir $buildDir || die("Cannot create the build directory '$buildDir'");

    if (! -e ($buildDir. $dirSeparator . $^O)) {
      mkdir($buildDir . $dirSeparator . $^O ) || die('Cannot create directory '.($^O . $dirSeparator . $buildDir));
    }
  }

  $buildDir = ($buildDir . $dirSeparator . $^O);

  if (! -e ($buildDir)) {
    print 'Error the build directory '.($buildDir).'cannot be created.'."\n";
    exit 1;
  }

  if (! -e $buildDir) {
    print STDERR "The build directory doesn't exists.\n";
    exit 1;
  }

  # Entering in the build directory.
  chdir $buildDir || die("Cannot go to the build directory '$buildDir'");

  my $currDir = getcwd();

  print "Current directory: ", $currDir, "\n";
  print "Build directory: ", $buildDir, "\n";

  my $pattern = $buildDir;
  $pattern =~ s/\\/\//g;

  if ($currDir !~ m/$pattern$/) {
    print STDERR "Seems that was not possible to enter in the build directory.\n";
    exit 1;
  }

  if ($cmakeGenerator =~ /msys/i) {
    print 'Generator for Msys', "\n";

    $cmakeGenerator = 'MSYS Makefiles';
    $buildToolMake  = 'make';
  }
  elsif ($cmakeGenerator =~ /unix/i) {
    print 'Generator for Unix', "\n";

    $cmakeGenerator = 'Unix Makefiles';
    $buildToolMake  = 'make';
  }
  else {
    # Determining the configuration based on operating system type
    if ($^O =~ 'MSWin32') {
      print 'Generator for MinGW', "\n";

      $cmakeGenerator = 'MinGW Makefiles';
      $buildToolMake  = 'mingw32-make';
    }
    elsif ($^O =~ 'msys') {
      print 'Generator for MSYS', "\n";

      $cmakeGenerator = 'MSYS Makefiles';
      $buildToolMake  = 'make';
    }
    else {
      print 'Generator for Unix', "\n";

      $cmakeGenerator = 'Unix Makefiles';
      $buildToolMake  = 'make';
    }
  }


  # Begin time
  my $time_begin = time();

  # CMake Instruction
  my $cmakeInstruction = "cmake -G\"$cmakeGenerator\" -DCMAKE_BUILD_TYPE:STRING=$buildType ..".$dirSeparator."..";

  print $cmakeInstruction, "\n";


  # Calling CMake
  $rc = system($cmakeInstruction);

  if ($rc != 0) {
    print STDERR "Error at CMake execution.\n";
  }

  my $makeCmd = $buildToolMake.' '.$buildToolMakeArguments;

  print 'Make command: ', $makeCmd, "\n";

  # Making project
  system($makeCmd);


  # End time
  my $time_end = time();

  print STDERR "\n\n";

  my $time_total = $time_end - $time_begin;

  printf "Building duration: %.2f seconds (%.2f minutes or %.2f hours).\n", $time_total, $time_total / 60, $time_total / 3600;

  print STDERR "\n";

  0;
}
