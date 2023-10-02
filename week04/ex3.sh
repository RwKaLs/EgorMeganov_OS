#!/bin/bash
gcc -o ex3 ex3.c

./ex3 3 &
./ex3 5 &

rm ex3
