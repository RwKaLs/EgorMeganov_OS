#!/bin/bash

# This command will print the inode number of ex1
ls -i ex1

# This command will print the number of blocks and the block size of ex1
stat -c "%b - %B" ex1

# This command will print the size in bytes and permissions of ex1
stat -c "%s - %A" ex1

cp ex1 ex2

# This command will print the number of links of ex2
stat -c "%h" ex2

# These commands will print the inode numbers of ex1 and ex2
ls -i ex1
ls -i ex2

# This command will print the files in /etc that have 3 links
stat -c "%h - %n" /etc/* | grep ^3

# The number of links (3 in this case) represents the number of hard links to the file. A hard link is essentially a reference to the data on the disk, so a file with 3 links has 3 references in the filesystem.
