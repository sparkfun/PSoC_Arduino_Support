#!/bin/sh
# This file copies everything you may need from your PSoC Creator Arduino Core
#  project directory to some hardware directory located elsewhere on your
#  computer (defined by the core variable).
core=../Hardware/SparkFun/psoc/cores/arduino
hardware_dir=../Hardware/
sketchbook=/cygdrive/c/dropbox/Projects
# Delete the contents of that directory.
rm $core/* -rf 2>/dev/null


# First, we can just straight-up copy any header files...
#cp core/*.h $core/
# ...or .cpp files. NB: When you change the suffix of the file from .c to .cpp
#  PSoC Creator loses the ability to recognize it for autocomplete and syntax
#  highlighting. Maybe that matters to you, maybe not. If it does..
#cp core/*.cpp $core/
# ...uncomment this next line. It copies all the files in the main directory
#  and renames them from .c to .cpp, which is important for compiling in the
#  Arduino IDE. Note that it will also rename files that you'd rather have
#  retain the .c suffic!!!
#find ./ -maxdepth 1 -name "*.c" -exec cp "{}" "$core/{}pp" \;
# We'll also grabe the "core" directory from this and throw it straight over.
#  Files in there are largely inherited directly from the Arduino
#  implementation of some other platform.
cp ./core/* $core/ -r

# ...and the main.cpp file.
cp main.cpp $core/

# And, libraries...
cp Libraries/SPI $core/../../libraries/ -r
cp Libraries/Wire $core/../../libraries/ -r
cp Libraries/Servo $core/../../libraries/ -r
cp Libraries/WS281x $core/../../libraries/ -r

# These are the generated files that PSoC Creator normally builds with. We
#  need header files, c files (*not* cpp!) and the linker (ld) file.
cp ./codegentemp/*.h $core/
cp ./codegentemp/Cm3Start.c $core/
cp ./codegentemp/*.ld $core/

cp ./CortexM3/ARM_GCC_484/Release/Uno_blocks.a $core/../../variants/standard/

# There are some assembly (.s) files that need to be copied over. Arduino
#  looks for .S (uppercase), and that case sensitivity is hard-coded in the
#  IDE, so as we copy the files (only the ones with gnu somewhere in the name)
#  we should uppercase the s.
find ./codegentemp -maxdepth 1 -name "*nu.s" |\
  sed -e "s%\(\./codegentemp/\)\(.*\)\.s%cp \1\2.s $core\/\2.S%g" |\
  sh
# Finally, there's are a couple of .inc files that are needed during linking. 
find ./codegentemp -maxdepth 1 -name "*nu*inc" -exec cp "{}" "$core/" \;

# Last of all, grab the hex file and throw it into the core.
cp ./CortexM3/ARM_GCC_484/Release/Uno_blocks.hex $hardware_dir/SparkFun/psoc/Bootloader.hex  

chmod -R 777 $core/../../*
cp $hardware_dir/ $sketchbook/ -r

