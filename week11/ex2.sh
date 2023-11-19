#!/bin/bash

# Compile the create_fs.c program
gcc create_fs.c -o create_fs

# Create the file system
./create_fs disk0

# Compile the ex2.c program
gcc ex2.c -o ex2

# Process the input from the input file
./ex2 disk0 input.txt

rm create_fs
rm ex2
