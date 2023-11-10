#!/bin/bash

gcc monitor.c -o monitor
gcc ex1.c -o ex1

gnome-terminal -- bash -c "./monitor $1; exec bash"
./ex1 "$1"

rm monitor
rm ex1

