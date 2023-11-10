#!/bin/bash

# Make an empty file ex5.txt
touch ex5.txt

# Remove write permission for everybody
chmod a-w ex5.txt

# Grant all permissions to owner and others (not group)
chmod u+rwx,o+rwx ex5.txt

# Make group permissions equal to user permissions
chmod g=u ex5.txt

# Print the permissions of ex5.txt
ls -l ex5.txt

# The number 660 is an octal representation of the file permissions. The first digit 6 means the owner has read and write permissions but not execute. The second digit 6 means the group also has read and write permissions. The last digit 0 means others have no permissions. So, 660 means the owner and group can read and write the file, but others cannot access it.
# The number 775 is an octal representation of the file permissions. The first digit 7 means the owner has read, write, and execute permissions. The second digit 7 means the group also has read, write, and execute permissions. The last digit 5 means others have read and execute permissions but not write. So, 775 means the owner and group can read, write, and execute the file, and others can read and execute it.
# The number 777 is an octal representation of the file permissions. Each digit 7 means the corresponding category (owner, group, others) has read, write, and execute permissions. So, 777 means everyone can read, write, and execute the file.
