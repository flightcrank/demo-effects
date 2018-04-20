
# Metaballs-2D
This is a simple attempt at creating an "iso-surface" with metaballs
This program is coded in C using SDL to open a window to draw to. So you
will need to have SDL (version 2) installed on your system for this program to work.
This program should compile and run with no or very little modification and on 
any platform like, Linux, Windows, or MacOS. Consult documentaion for those platforms on
compilation instructions.

Compilation instructions on a Linux system are as follows..

	gcc main.c renderer.c -lm `sdl2-config --cflags --libs` -o metaballs-2d
    

## Images 
![animation](https://i.imgur.com/mIeGoZM.gif)
