#!/bin/bash

gcc -pthread -o ex3 ex3.c

for m in 1 2 4 10 100
do
echo "m=$m" >> ex3_res.txt
{ time ./ex3 10000000 $m ; } 2>> ex3_res.txt
done

rm ex3
