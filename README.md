# C++ Snake Game

A simple 2D Snake game built from scratch in C++ using SFML.
The game demonstrates a basic game loop, real-time keyboard input, collision detection, scoring, high score saving, and clean object-oriented structure.

## Features

* Fixed-size game window: 800x600
* Main menu screen
* Real-time keyboard movement
* Snake grows when food is collected
* Collision detection with walls
* Collision detection with snake body
* Score display
* High score saved locally
* Pause and resume functionality
* Restart after game over
* Increasing difficulty over time
* Clean class-based structure using `Game`, `Snake`, and `Food`

## Controls

| Key           | Action                  |
| ------------- | ----------------------- |
| Space / Enter | Start game              |
| Arrow Keys    | Move snake              |
| W A S D       | Alternative movement    |
| P             | Pause / Resume          |
| R             | Restart after game over |
| Esc           | Quit game               |

## Requirements

This project uses SFML.

On Ubuntu or WSL, install SFML using:

```bash
sudo apt update
sudo apt install libsfml-dev -y
```

## How to Compile

Compile the game with:

```bash
g++ main.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
```

## How to Run

Run the game using:

```bash
./game
```

## Project Structure

```text
cpp-snake-game/
├── main.cpp
├── README.md
└── .gitignore
```

## Game Rules

The snake moves continuously around the game area.
The player must collect food to increase the score and grow the snake.
The game ends if the snake hits the wall or collides with itself.
After game over, the player can restart the game by pressing `R`.

## Code Structure

The code is organized into separate classes:

* `Game` handles the main loop, input, update logic, rendering, game states, score, and high score.
* `Snake` handles snake movement, growth, direction, rendering, and self-collision.
* `Food` handles food spawning and rendering.

## Bonus Features Implemented

* Main menu
* Pause and resume
* Restart mechanism
* High score saved to file
* Increasing difficulty
* Custom text rendering using simple shapes
* Clean object-oriented design

## Author

Veena Sadasivan Nair
