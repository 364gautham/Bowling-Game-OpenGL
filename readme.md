# Computer Graphics Project - Bowling Lane in Nature

Author- Gautham Kashim Adapparaju (Adapted examples written by Willem A. (Vlakkies) Schreuder for the class)

Online references for implementing Fractal tree, Waving flag.

# Project Description

It is bowling game lane in natural surroundings. Playing the game is simple, One needs to press spacebar key to play the game.
User has the options to place the ball at three different pre defined positions using keys 1 ,2 and 3.
(Implementation of pin objects and collision detection)
User can also select various ball colours using key 'c'.

One can navigate around the scene (first person navigation mode is default) using arrows for different camera angle positions and
keys 'q' and 'z' for moving up and down. Keys 'w' for moving forward and 's'for moving backward.

Implemented flowing river below the bowling lane which also is bridge_lane. River is a simple emulation of flowing water.
Water waves emulation are done using constant vertical motion of triangles.

Most beautiful part is the trees implemented in the project. I have used concept of fractal trees (recursive binary branching) to generate
branches of tree. Branches are generated using random generated values for orientation of branches. Each time program is run, the code runs once and
stores the pattern in display list and it is replicated at many places for display in the scene. Two different fractal trees are present in the scene

Simple cone trees using cylinders are also present. Rock like structures are done using spheres.
As a proud CU grad , waving CU flag is flying in the scene.

One can relax in the shelter provided along with the beach style chair

Skycube is adapted from class examples. Various textures and lighting is also applied to objects.

## Build instructions:

Run `make` to generate execution file (.proj)
Run `make clean` to remove  generated files.

Use arrow keys to change viewing angles


Key bindings
  l          Toggles lighting\
  a/A        Decrease/increase ambient Light\
  d/D        Decrease/increase diffuse light\
  s/S        Decrease/increase specular light\
  e/E        Decrease/increase emitted light\
  n/N        Decrease/increase shininess\
  F1         Toggle smooth/flat shading\
  F2         Toggle local viewer mode\
  F8         Change ball increment\
  F9         Invert bottom normal\
  m          Toggles light movement\
  <,>        Light movement\
  ,          decrease light distance\
  .          increase light distance\
  []         Lower/rise light\
  +/-        Change field of view of perspective\
  x          Toggle axes\
  arrows     Change view angle\
  0          Reset view angle\
  q/Q        going up\
  z/Z        going Down\
  w/W        going forward\
  s/S        Going backward\
  spacebar   play Bowling\
  1/2/3      Ball positions\
  c/C        Different Ball colours\
  ESC        Exit
