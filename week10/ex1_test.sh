#!/bin/bash

cd "$1"

if [ -e link_to_file2.txt ]; then
    rm link_to_file2.txt
fi

echo "Hello, world!" > file1.txt
cp file1.txt file2.txt
cat file1.txt
echo "Hello, world 2.0!" >> file1.txt
chmod +x file1.txt
cat file1.txt
mv file1.txt file3.txt
ln -s file2.txt link_to_file2.txt
rm file2.txt
rm link_to_file2.txt
rm file3.txt
