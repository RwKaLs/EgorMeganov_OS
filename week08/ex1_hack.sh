#!/bin/bash

pid=$(cat /tmp/ex1.pid)
grep rw-p /proc/$pid/maps | awk '{print $1}' | while read m; do
    start=${m%-*}
    end=${m#*-}
    
    sudo dd if=/proc/$pid/mem bs=1 skip=$((16#$start)) count=$((16#$end-16#$start)) 2>/dev/null | strings | grep -o "pass:........" | while read p; do
        echo "Password: $p"
        echo "Memory address: $start"
        kill -9 $pid
        exit
    done
done
