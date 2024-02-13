# Ecosystem Simulation Project

## Overview

This project simulates an ecosystem consisting of grass, sheep, wolves, and now crows. The simulation models basic interactions within a simplified environment, demonstrating concepts such as growth, predation, reproduction, and flocking behavior. Each entity in the ecosystem follows simple rules defined by their finite state machines, contributing to the emergent behavior observed in the simulation.

## Features

- **Grass Growth and Spread:** Grass grows over time and can spread to adjacent spaces.
- **Sheep Behavior:** Sheep move around the environment, eat grass to gain health, evade wolves, and breed.
- **Wolf Behavior:** Wolves hunt sheep, eat to gain health, and breed.
- **Crow Flocking:** Crows fly around the environment, exhibiting complex flocking behavior based on the Boids algorithm.
- **Emergent Ecosystem Dynamics:** Interaction between grass, sheep, wolves, and crows leads to complex ecosystem dynamics.

## Visual Representation Guide

In the ecosystem simulation, each entity is represented by a specific color and shape to make it easily distinguishable within the environment. Here's a guide to understanding these visual cues:

### Grass
- **Color:** Green (`GREEN` for healthy grass, `DARKGREEN` when trampled, and `YELLOW` when health is below half).
- **Shape:** Rectangle. The size may represent the health level or growth stage of the grass.

### Sheep
- **Color:** Varies based on behavior (`LIGHTGRAY` when wandering, `DARKGRAY` when going to food, etc.).
- **Shape:** Rectangle. The size may vary to represent the health level of the sheep.

### Wolves
- **Color:** Varies based on behavior (`RED` when wandering, `ORANGE` when hunting, etc.).
- **Shape:** Circle. The size may vary to represent the health level of the wolf.

### Crows
- **Color:** Violet (`VIOLET`), representing their integration into the ecosystem.
- **Shape:** Triangle, to distinguish their bird-like form and direction of movement.
- **Behavior:** Exhibits flocking behavior, dynamically changing direction and speed based on the positions of nearby crows.

## Setup

### Requirements
- [Raylib](https://www.raylib.com/) library for graphical output and interaction handling.
- C++ compatible compiler.

### Installation
1. Ensure Raylib is installed and configured on your system.
2. Clone this repository to your local machine.
3. Navigate to the cloned directory and compile the project.
4. Run the compiled executable to start the simulation.

## Usage
- **Starting the Simulation:** Execute the compiled program. The ecosystem initializes with predefined numbers of grass, sheep, wolves, and crows.
- **Observing Dynamics:** Watch as the simulation unfolds, displaying the complex interactions and behaviors of the ecosystem's entities.
