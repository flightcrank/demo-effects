

# Vector-balls
This is a simple attempt at creating Vector-balls in a simple 3d engine. Which are sprites that are placed in 3d space 
and can be moved and rotated around and morph into different shapes.
This program is coded in C using SDL to open a window to draw to. It can run in any resolution by 
changing the SCREEN_WIDTH and SCREEN_HEIGHT defines in main.c. 

You will need to have SDL (version 2) installed on your system for this program to work.
This program should compile and run with no or very little modification and on 
any platform like, Linux, Windows, or MacOS. Consult documentation for those platforms on
compilation instructions.

Compilation instructions on a Linux system are as follows..

	$ gcc main.c renderer.c vector.c -lm `sdl2-config --cflags --libs` -o vectorballs 

## Images 
![animation](https://i.imgur.com/HwMoFlA.gif)
