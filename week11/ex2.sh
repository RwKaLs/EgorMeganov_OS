#!/bin/bash

gcc create_fs.c -o create_fs
./create_fs disk0

gcc ex2.c -o ex2
./ex2 disk0 input.txt

rm create_fs
rm ex2
