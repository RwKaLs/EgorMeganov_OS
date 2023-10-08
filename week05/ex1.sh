#!/bin/bash

gcc -o publisher publisher.c

n=$1

gnome-terminal -- ./publisher $n

gcc -o subscriber subscriber.c

for ((i=1;i<=n;i++)); do
    gnome-terminal -- ./subscriber $i
done

