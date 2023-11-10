#!/bin/bash

# Delete ./tmp folder if it exists
if [ -d "./tmp" ]; then
    rm -r ./tmp
fi

# Create a symbolic link tmp1 for the folder ./tmp
ln -s ./tmp tmp1

# Run ls -li
ls -li

# Before creating the ./tmp directory, the symbolic link tmp1 points to a non-existent location, so it appears in red color when we run ls -li. After creating the ./tmp directory, the symbolic link tmp1 now points to an existing directory, so it appears in the default color.

# Create the folder ./tmp
mkdir ./tmp

# Run ls -li
ls -li

# Create a file ex1.txt using gen.sh
bash gen.sh 10 ex1.txt

# Add it to ./tmp
mv ex1.txt ./tmp

# Check the folder ./tmp1
ls -l ./tmp1

# Create another symbolic link tmp1/tmp2 to the folder ./tmp
ln -s /absolute/path/to/your/directory/tmp tmp1/tmp2

# Create another file ex1.txt using the same generator gen.sh
bash gen.sh 10 ex1.txt

# Add it to the folder ./tmp1/tmp2
mv ex1.txt ./tmp1/tmp2

# Check the content of the sub-folders
ls -l ./tmp1/tmp2

# Try to access the sub-folders ./tmp1/tmp2/tmp2/tmp2/...
ls -l ./tmp1/tmp2/tmp2/tmp2

# Trying to access the sub-folders ./tmp1/tmp2/tmp2/tmp2/... results in an infinite loop because tmp2 is a symbolic link to itself. The filesystem keeps trying to resolve the symbolic link, which just points back to itself, creating an endless chain of directories that doesn’t actually exist.

# Delete the folder ./tmp
rm -r ./tmp

# Check the symbolic links again
ls -li

# Delete all other symbolic links you created
find . -type l -delete
