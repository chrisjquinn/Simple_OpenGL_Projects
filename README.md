# Simple_OpenGL_Projects
Some simple labs and projects in C++ for the **OpenGL** package also using **GLUT**. These include some basic displaying, double-buffering, idle & keyboard handling, fractals and tetris!

## Running
To run a specific file, say `<file>.cpp`. Download the corresponding `Makefile` to your system (Linux or Mac, sorry windows) then type the following:

`make -f Makefile.<linux / OSX> <file>.cpp`

`./<file>`

This should compile and run the program. Brief explanations of the programs are below.


## spiralTube and sprial
Simple displaying of spiral and a spiral tube, use of double buffering and various `glBegin()` types.


## Fractals
My favourite of them all, some fractals are drawn using recursion or iteratively. The fractals drawn are:
`fractalSquare.cpp`   -     Fractual squares filling the space defined, changing depth increases the area filled.
`sierpinski.cpp`      -     The classic sierpinski triangle, done to a depth of 7. This can be changed to show various depths.
`BarnsleyFern.cpp`    -     The barnsley fern fractal, this one is done iteratively instead of recursively, due to around                                                              .                           500,000 points being needed in order to render a nice looking fern.


## Idle & Keyboard Handling
`bounce.cpp` does this, the keyboard arrows change the position of the square, with keyboard keys `r` rotating the square, `q` quitting the program and `a` starting the bounce sequence. This sequence is similar to the classic DVD screensaver (sadly doesn't change colour upon a bounce). The `a` key works as a toggle. Use of *visibility* is also done to print to the terminal.


## 3D
`robot-arm.cpp` is the first to render a 3D model of a robot arm. This allows you to move it around by rotating its arms and elbows. This is done with lighting, display lists, and a predefined cylinder object.


## Tetris
A program of tetris made using 2D rendering. Very buggy and still being worked on, but uses `glutTimerFunc(.)` in order to stage the time delay of shapes.
