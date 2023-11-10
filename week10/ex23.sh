#!/bin/bash

# Link ex1.txt to ex11.txt and ex12.txt
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt

# Compare the content of all files
diff ex1.txt ex11.txt > /dev/null
if [ $? -eq 0 ]; then
    echo "ex1.txt and ex11.txt are the same"
else
    echo "ex1.txt and ex11.txt are different"
fi

# There is no difference in the content of the files ex1.txt, ex11.txt, and ex12.txt.
# They all contain the same data because ex11.txt and ex12.txt are hard links to ex1.txt.

diff ex1.txt ex12.txt > /dev/null
if [ $? -eq 0 ]; then
    echo "ex1.txt and ex12.txt are the same"
else
    echo "ex1.txt and ex12.txt are different"
fi

# Check i-node numbers of all files and save the output to the file output.txt
ls -i ex1.txt ex11.txt ex12.txt > output.txt

# They do not have different i-node numbers. The files ex1.txt, ex11.txt, and ex12.txt
# all have the same i-node number because they are hard links to the same data on the disk.

# Check the disk usage of ex1.txt file
du ex1.txt

# Create a hard link ex13.txt for ex1.txt and move it to the folder /tmp
ln ex1.txt ex13.txt
mv ex13.txt /tmp

# Trace all links to ex1.txt in the current path
find . -inum $(ls -i ex1.txt | cut -d " " -f1)

# Trace all links to ex1.txt in the root path (/)
find / -inum $(ls -i ex1.txt | cut -d " " -f1)

# Check the number of hard links of ex1.txt
stat -c "%h" ex1.txt

# Remove all links from ex1.txt
find . -inum $(ls -i ex1.txt | cut -d " " -f1) -exec rm {} \;

# Searching in the current path will only find links to ex1.txt in the current directory and its subdirectories. Searching in the root path (/) will find links to ex1.txt anywhere on the filesystem. 
