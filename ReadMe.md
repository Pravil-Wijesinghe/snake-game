# Snake Game (SDL2)

A simple Snake game implemented using SDL2, SDL_ttf, and SDL_mixer.

## Prerequisites

Ensure you have the following installed on your system:
- SDL2
- SDL2_ttf
- SDL2_mixer
- MinGW-w64 (for Windows)
- g++ compiler

## Installation and Running the Game

### 1. Compile the Game
Run the following command in the terminal or command prompt:
```sh
g++ snakeGameSDL.cpp -o snakeGame -IC:/msys64/mingw64/include -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -g
```

### 2. Run the Game
After compilation, execute the game using:
```sh
./snakeGame
```

## Game Controls
- **Arrow Keys**: Move the snake (Up, Down, Left, Right)
- **Enter**: Start/Restart the game
- **Close Window**: Exit the game

## Features
- Simple snake movement
- Food spawning
- Score tracking
- Game over screen
- Sound effects for eating food and game over

## Dependencies
- [SDL2](https://github.com/libsdl-org/SDL)
- [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
- [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer)

## License
This project is open-source and available under the MIT License.

---
Enjoy the game! 🎮