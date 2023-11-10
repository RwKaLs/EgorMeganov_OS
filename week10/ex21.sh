#!/bin/bash

# Check the file types in the paths /dev and /etc
file /dev/* /etc/* 

# Count the number of directories in the folder /etc
ls -l /etc | grep ^d | wc -l

# Write a hello world program ex1.c
echo '#include <stdio.h>' > ex1.c
echo 'int main() {' >> ex1.c
echo '  printf("Hello, World!\n");' >> ex1.c
echo '  return 0;' >> ex1.c
echo '}' >> ex1.c

# Check the file type of ex1.c before compilation
file ex1.c

# Compile ex1.c
gcc ex1.c -o ex1

# Check the file type of ex1 after compilation
file ex1

# Modify ex1.c to print “Привет, мир!” instead of "Hello world!"
echo '#include <stdio.h>' > ex1.c
echo 'int main() {' >> ex1.c
echo '  printf("Привет, мир!\n");' >> ex1.c
echo '  return 0;' >> ex1.c
echo '}' >> ex1.c

# Check the file type of ex1.c before compilation
file ex1.c

# Compile ex1.c
gcc ex1.c -o ex1

# Check the file type of ex1 after compilation
file ex1

# Remove the compiled executable and source code
rm ex1
rm ex1.c

# The 'file' command in Linux is used to determine the type of a file.
# Obviously, changing the string that's printed in the C program doesn't change the type of the file, so the output of the 'file' command will be the same for both versions of the ex1.c file and its compiled executables.
# However, because russian letters are not ASCII symbols, the encoding type changed to Unicode and UTF-8.

