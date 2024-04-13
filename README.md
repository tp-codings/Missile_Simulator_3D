# Missile_Simulator_3D
This is program which simulates an air defence system with missiles launched from trucks and turrets and their trajectory to moving planes in the air. It is inspired by the Iron Dome. 

## Features
- Turrets find their target automatically and fire if the target is in range
- Missiles from truck launch system can be fired manually -> those missiles are faster with longer range, but not so maneuverable
- freely movable camera in 3D space
- change between missile chase cams (third person view)
- supports terrain generator by heightmap
- particle effects
- missiles can collide during flight
  
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

## GUI Usage
- F: Open/Close GUI
- TimeFactor: Slow Motion Factor (useful for chaotic particle behavior)
- CamSpeed: Moving speed of first person view camera
- Start/Stop: Start/Stop the simulation
- Show Range: Shows range as a dome around city that should be protected
- DirLightColor: Option to modify direction light color
- DirLightDir: Option to modify direction light direction
- AmbientStrength: Defines strength of ambient light in the scene

## Keyboard Shortcuts
- R: Toggle range view
- P: Fire missile from truck
- C: Switch between cams
- K: Fire Canon
- Enter: Start/Stop simulation
- Arrow Keys: Change direction of all planes

# Examples

https://github.com/tp-codings/Missile_Simulator_3D/assets/118997294/dba37930-b11e-4769-8520-d2df164a7189

https://github.com/tp-codings/Missile_Simulator_3D/assets/118997294/5ccf60ed-f1e0-4daa-a814-23193ce88e4d

https://github.com/tp-codings/Missile_Simulator_3D/assets/118997294/50b5ff4a-a596-41ef-9936-150f319c0443

https://github.com/tp-codings/Missile_Simulator_3D/assets/118997294/fe4f253f-210e-453e-b86a-a6977cedeb1e

# Open Issues
To be honest, that list could be very long. I tried out so many things in this project. In many places they didn't work 100%, but I didn't want to spend forever fixing everything, I wanted to keep trying out the things that interested me like particle effects or terrain generation. Looking back, I would probably do a few things differently. The architecture of the application is better than in some previous projects, but it could definitely benefit from suitable design patterns and concepts such as polymorphism. So there is a lot of room for improvement. The following list is, for sure, not complete:
- switching between cameras does not always work, sometimes the cam will not be destroyed after the missile is destroyed
- There are no shadows, what leads to a wierd look of the terrain
- The rotation of the rocket in the tower does not always follow the tower. This can cause the rocket to spin “through the turret.”
- In some cases, the missiles perform a very abrupt turn in the air when the actual target is destroyed or a new one is within closer range and within the missile's detection angle
- Sometimes, trucks are not placed properly on the ground, but hovering above
- The firing system of the canon is not good, I have not yet found an algorithm that positions the cannon sights to match the aircraft in order to hit it (should be like Phalanx CIWS)
- Particle effects are rudimentary
- ...

# Contact Information

Feel free to reach out to me:

- **Email:** [tobipaul50@gmail.com](mailto:tobipaul50@gmail.com)
- **LinkedIn:** [Tobias Paul](https://www.linkedin.com/in/tobias-paul-657513276/)
- **GitHub:** [@tp-codings](https://github.com/tp-codings)

Looking forward to connecting with you! 😊
