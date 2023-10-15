#!/bin/bash
gcc -o worker worker.c
gcc -o scheduler_sjf scheduler_sjf.c
./scheduler_sjf data.txt
rm worker
rm scheduler_sjf
