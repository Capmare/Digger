# Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Design choices
## Map generation
To generate the map i decided to create a single file script that that converts a black and white image to the correct tile type and exports as a txt format.
## Map creation && digging
The map is being created by using multiple textures tiles that are 20x4, i take all those textures at start and combine them into a singular texture, to dig trough the dirt we just change the pixel color.
## Pathfinding
The AI uses 2 different algorithms, the first one is BFS, it will be used for when the AI cannot dig, it also helps with avoiding getting stuck in the corners, when it changes its state it starts using the A* for faster lookup since it doesnt care about getting stuck.
## Animation
The animation is being controlled by the ```AnimControllerComponent``` this specific component holds all the states of each object in the world and decides which animation should play on the current state.

https://github.com/Capmare/Digger/tree/origin/GameBranch