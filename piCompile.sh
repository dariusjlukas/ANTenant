#!/bin/bash
#Raspberry pi compliling script

echo "compiling..."

  g++ -c -g `pkg-config --cflags alsa` -o main.o ALSAtest.cpp
  g++ -o main main.o -lasound
  chmod +x main.o
  gcc -o alsa-record-example -lasound alsa-record-example.c && ./alsa-record-example hw:0
  echo "finished" 
  