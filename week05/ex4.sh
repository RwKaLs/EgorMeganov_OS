#!/bin/bash

gcc -pthread -o ex4 ex4.c

for m in 1 2 4 10 100
do
    echo "m=$m" >> ex4_res.txt
    { time ./ex4 10000000 $m ; } 2>> ex4_res.txt
done

rm ex4
