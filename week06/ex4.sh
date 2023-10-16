#!/bin/bash
gcc -o scheduler_rr scheduler_rr.c
gcc -o worker worker.c
./scheduler_rr data.txt
rm scheduler_rr
rm worker
