#!/bin/bash
#Raspberry pi compliling script

echo "complieing..."

  g++ -c -g `pkg-config --cflags alsa` -o main.o ALSAtest.cpp
  g++ -o main main.o
  g++ -o main main.o -l asound
  