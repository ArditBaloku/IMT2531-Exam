# IMT2531-Exam
Repository for the exam deliverables to IMT2531 - Graphics Programming

# Checklist:

## Task 1

- [x] Load a heightmap, color it based on elevation and render trees.
- [x] Illuminate the scene using the Phong model.

Additional features:

- [x] Generate custom heightmap
- [ ] Add shadows
- [x] Render clouds
- [ ] Introduce rain or snow

## Task 2

- [x] Provide a free moving camera controlled with mouse and WASD
- [x] Allow switching between global camera, 1st and 3rd perspective
- [x] Day/night cycle and different light color based on sun/moon position
- [ ] Emulate ripples on the water using a normal map

Additional features:

- [x] Allow the user to control the speed of the day/night cycle with GUI
- [x] Integrate zooming for the camera
- [ ] Add minimap
- [x] Dim light sources around the terrain
- [ ] Sound that changes appropriately with the scene

## Task 3

- [ ] Load air, land and water animals. Make them move around.
- [x] Avoid objects moving through the ground
- [ ] Allow the user to switch between the objects and control them

Additional features:

- [ ] Make your own custom object loader
- [ ] Make the air model move in 3D
- [ ] Move the objects systematically
- [ ] Make the fish jump in and out of water, or the duck dive under
- [ ] Animate a model
- [ ] Add sound to some models
- [ ] Let the user choose between following the movement of the objects or control the model

# How to setup

Make sure you have [cmake](https://cmake.org/) version 3.14 or later installed.
Navigate to the root folder of the project and execute the following commands

On linux:
```
mkdir build
cd build
cmake ..
make
./Exam
```

On windows:
```
mkdir build
cd build
cmake ..
cmake --build .
cd ..
cd out/build/x64-Debug
Exam.exe
```

# Controls

W - Move camera forward

A - Move camera left

S - Move camera backwards

D - Move camera right

T - Switch between first-person, third-person and global view

LShift - Sprint

Esc - Exit

Arrow keys - Navigate the menu

Space - Make a selection in the menu