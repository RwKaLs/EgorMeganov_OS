#!/bin/bash
gcc -o worker worker.c
gcc -o scheduler scheduler.c
./scheduler data.txt
rm worker
rm scheduler
