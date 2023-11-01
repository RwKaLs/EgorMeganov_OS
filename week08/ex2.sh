#!/bin/bash

mkdir /tmp/ex2

gcc mmu.c -o mmu
gcc pager.c -o pager

./pager 4 2 &

pid_pager=$!

sleep 3

gnome-terminal -- bash -c "./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid_pager; exec bash"

wait $pid_pager

rm mmu
rm pager
