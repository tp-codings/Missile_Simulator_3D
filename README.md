# Missile_Simulator_3D
This is program which simulates an air defence system with missiles launched from trucks and turrets and their trajectory to moving planes in the air.

## Features
- Turrets find their target automatically and fire if the target is in range
- Missiles from truck launch system can be fired manually -> those missiles are faster with longer range
- freely movable camera in 3D space
- change between missile chase cams (third person view)
- supports terrain generator by heightmap
- particle effects
  
## Project Overview

| Personal Rating | Year of Development | Languages | Tools | Type of Application |
| --- | --- | --- | --- | --- |
| ⭐️⭐️⭐️⭐️⭐️ (5/5) | 2023 | C++ | OpenGL, GLFW, ImGUI, Freetype | Simulation |

# Getting Started

## Prerequisites and basic information

- This application is tested in windows 10/11
- This repo does not provide an executable -> you have to compile your own (I used Visual Studio)
- I don't guarantee that every corner case has been taken into account. The project is part of my learning process for C++ and OpenGL
- All dependencies are included in this repo, so it is almost plug and play :)

1. Clone Repository
`https://github.com/tp-codings/Missile_Simulator_3D.git`
2. Build Executable (e.g. Visual Studio)

# User Manual
## Camera Control in Space
- W: Forward (camera direction)
- A: Left
- S: Backward
- D: Right
- Space: Up
- Shift: Down
- Mouse: Look around
- Scroll wheel: Zoom
