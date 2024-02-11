## Visual Representation Guide

In the ecosystem simulation, each entity is represented by a specific color and shape to make it easily distinguishable within the environment. Here's a guide to understanding these visual cues:

- **Grass**
  - **Color:** Green (`GREEN` for healthy grass, `DARKGREEN` when trampled, and `YELLOW` when health is below half).
  - **Shape:** Rectangle. The size may represent the health level or growth stage of the grass.

- **Sheep**
  - **Color:**
    - `LIGHTGRAY` when wandering.
    - `DARKGRAY` when going to food.
    - `DARKBLUE` when still.
    - `PURPLE` when breeding.
    - `SKYBLUE` when evading wolves.
  - **Shape:** Rectangle. The size may vary to represent the health level of the sheep.

- **Wolves**
  - **Color:**
    - `RED` when wandering.
    - `ORANGE` when going to food or in a still position.
    - `PINK` when breeding.
  - **Shape:** Circle. The size may vary to represent the health level of the wolf.

### Interpretation
- The visual cues help in quickly assessing the state and health of each entity within the simulation.
- The behavior of sheep and wolves (such as evading, hunting, or breeding) can be observed through changes in their color.
- The growth and condition of grass patches are indicated by their color, from green (healthy) to yellow (withering).

## Features
- **Grass Growth and Spread:** Grass grows over time and can spread to adjacent spaces.
- **Sheep Behavior:** Sheep move around the environment, eat grass to gain health, evade wolves, and breed.
- **Wolf Behavior:** Wolves hunt sheep, eat to gain health, and breed.
- **Emergent Ecosystem Dynamics:** Interaction between grass, sheep, and wolves leads to complex ecosystem dynamics.

## Setup
### Requirements
- [Raylib](https://www.raylib.com/) library for graphical output and interaction handling.
- C++ compatible compiler.

### Installation
1. Ensure Raylib is installed and configured on your system.
2. Clone this repository to your local machine.
3. Navigate to the cloned directory and compile the project.
4. Run the compiled executable¨.

## Usage
- **Starting the Simulation:** Execute the compiled program to start the simulation. The ecosystem will initialize with predefined numbers of grass, sheep, and wolves.
- **Interacting with the Simulation:** Currently, the simulation runs automatically. Future versions may include user interactions to alter parameters in real-time.
