#!/bin/bash

# Create a file lofs.img with 50 MiB size
dd if=/dev/zero of=lofs.img bs=1M count=50

# Setup a loop device on the lofs.img
LOOP=$(sudo losetup --find --show lofs.img)

# Create a Loop File System (LOFS) ext4 on the lofs.img
sudo mkfs -t ext4 $LOOP

# Create a new empty directory ./lofsdisk if it doesn't exist
if [ ! -d "lofsdisk" ]; then
# Create a dir
  mkdir lofsdisk
fi

# Mount the created filesystem on the mount point ./lofsdisk
sudo mount $LOOP lofsdisk

# Add file file1 to the LOFS
echo "Egor" > lofsdisk/file1
# Add file file2 to the LOFS 
echo "Meganov" > lofsdisk/file2

# Define a function get_libs() in the script to return the paths of all shared libraries of a binary file
get_libs() {
# The ldd command lists the shared libraries used by the binary file passed as an argument ($1)
    ldd $1 | awk 'BEGIN{ORS=" "}$1~/^\//{print $1}$3~/^\//{print $3}' | sed 's/,$/\n/'
}

# Get shared libraries of commands bash, cat, echo, ls and add them with their shared libraries to the LOFS
for cmd in bash cat echo ls; do
# The cp command copies the command binary to the lofsdisk directory
    cp $(which $cmd) lofsdisk
    for lib in $(get_libs $(which $cmd)); do
    # The mkdir -p command creates the necessary directories in lofsdisk to hold the library
        mkdir -p lofsdisk$(dirname $lib)
        # The cp command copies the library to the corresponding directory in lofsdisk
        cp $lib lofsdisk$lib
    done
done

# Compile the ex1.c file
gcc ex1.c -o ex1

# Copy the ex1 executable to the new filesystem
cp ex1 lofsdisk/

# Change the root directory of the process to the mount point of the created LOFS and run the program ex1
sudo chroot lofsdisk /ex1 > ex1.txt

# Separate the outputs in ex1.txt for better visualizing in ex1.txt
echo "--------------------------" >> ex1.txt

# Run the same program again (not change the root directory of the process)
./ex1 >> ex1.txt

# Add explanations to the ex1.txt
echo -e "\nIn the chroot environment, the ex1 program can only access the files and directories inside the lofsdisk directory. Outside the chroot environment, the ex1 program can access the entire filesystem." >> ex1.txt

# Remove the executable ex1 file
rm ex1

