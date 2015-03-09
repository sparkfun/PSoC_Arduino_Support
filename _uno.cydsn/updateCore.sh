#!/bin/sh
core=~/Dropbox/Projects/Hardware/SparkFun/psoc/cores/arduino
cp *.h $core/
find ./ -maxdepth 1 -name "*.c" -exec cp "{}" "$core/{}pp" \;
find ./codegentemp -maxdepth 1 -name "*.c" -exec cp "{}" "$core/" \;
find ./codegentemp -maxdepth 1 -name "*.h" -exec cp "{}" "$core/" \;
find ./codegentemp -maxdepth 1 -name "*.ld" -exec cp "{}" "$core/" \;
find ./codegentemp -maxdepth 1 -name "*.h" -exec cp "{}" "$core/" \; 
find ./codegentemp -maxdepth 1 -name "*nu.s" |\
  sed -e "s%\(\./codegentemp/\)\(.*\)\.s%cp \1\2.s $core\/\2.S%g" |\
  sh
find ./codegentemp -maxdepth 1 -name "*nu*inc" -exec cp "{}" "$core/" \;
rm $core/sketch.*

