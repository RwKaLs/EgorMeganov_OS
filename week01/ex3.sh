#!/bin/bash
mkdir ~/week01/
cd ~/week01/
echo $(date)
sleep 3
mkdir root_folder
sleep 3
ls -l -t -r / > root_folder/root.txt
echo $(date)
sleep 3
mkdir home_folder
sleep 3
ls -l -t -r ~ > home_folder/home.txt

echo "Root folder content:":
cat root_folder/root.txt
echo "Home folder content:"
cat home_folder/home.txt
echo "Root folder items:":
ls root_folder
echo "Home folder items:":
ls home_folder
