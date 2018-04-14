#!/bin/bash
#Raspberry pi compliling script

echo "compiling..."

  g++ -c -g `pkg-config --cflags alsa` -o main.o ALSAtest.cpp
  g++ -o main main.o -lasound
  chmod +x main.o
  echo "finished" 
  