#!/bin/sh

gcc -g ./file_listing.c -O3 -o addFile
gcc -g ./main.c -O3 -o fileShare

./fileShare
