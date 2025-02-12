#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

// Window dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int CELL_SIZE = 20;

// Snake movement directions
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
// Game states
enum GameState { MENU, PLAYING, GAME_OVER };

// Game variables
bool gameOver = false;
int score = 0;
Direction direction = STOP;
std::vector<SDL_Point> snake;
SDL_Point food;
GameState gameState = MENU;

// SDL variables
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;
Mix_Chunk* eatSound = nullptr;
Mix_Chunk* gameOverSound = nullptr;

// Function to initialize SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("arial.ttf", 28);
    if (!font) {
        std::cout << "Font could not be loaded! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    eatSound = Mix_LoadWAV("eat.wav");
    if (!eatSound) {
        std::cout << "Eat sound could not be loaded! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    gameOverSound = Mix_LoadWAV("gameover.mp3");
    if (!gameOverSound) {
        std::cout << "Game over sound could not be loaded! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    return true;
}

// Function to spawn food
void spawnFood() {
    food.x = (rand() % (SCREEN_WIDTH / CELL_SIZE)) * CELL_SIZE;
    food.y = (rand() % (SCREEN_HEIGHT / CELL_SIZE)) * CELL_SIZE;
}

// Function to handle input
void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) gameOver = true;
        
        if (event.type == SDL_KEYDOWN) {
            if (gameState == MENU && event.key.keysym.sym == SDLK_RETURN) {
                gameState = PLAYING;
                snake.clear();
                snake.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
                score = 0;
                spawnFood();
                direction = STOP;
            }
            else if (gameState == GAME_OVER && event.key.keysym.sym == SDLK_RETURN) {
                gameState = MENU;
            }
            else if (gameState == PLAYING) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: if (direction != DOWN) direction = UP; break;
                    case SDLK_DOWN: if (direction != UP) direction = DOWN; break;
                    case SDLK_LEFT: if (direction != RIGHT) direction = LEFT; break;
                    case SDLK_RIGHT: if (direction != LEFT) direction = RIGHT; break;
                }
            }
        }
    }
}

// Function to update the game
void updateGame() {
    if (gameState != PLAYING) return;
    
    SDL_Point newHead = snake.front();
    switch (direction) {
        case LEFT: newHead.x -= CELL_SIZE; break;
        case RIGHT: newHead.x += CELL_SIZE; break;
        case UP: newHead.y -= CELL_SIZE; break;
        case DOWN: newHead.y += CELL_SIZE; break;
        default: return;
    }

    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH || newHead.y < 0 || newHead.y >= SCREEN_HEIGHT) {
        gameState = GAME_OVER;
        Mix_PlayChannel(-1, gameOverSound, 0);
        return;
    }

    for (auto& segment : snake) {
        if (newHead.x == segment.x && newHead.y == segment.y) {
            gameState = GAME_OVER;
            Mix_PlayChannel(-1, gameOverSound, 0);
            return;
        }
    }
    
    snake.insert(snake.begin(), newHead);
    
    if (newHead.x == food.x && newHead.y == food.y) {
        score++;
        Mix_PlayChannel(-1, eatSound, 0);
        spawnFood();
    } else {
        snake.pop_back();
    }
}

// Function to render text
void renderText(std::string text, int x, int y) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Function to render the game
void renderGame() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (gameState == MENU) {
        renderText("Press ENTER to Start", SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2);
    } else if (gameState == PLAYING) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (auto& segment : snake) {
            SDL_Rect rect = { segment.x, segment.y, CELL_SIZE, CELL_SIZE };
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect foodRect = { food.x, food.y, CELL_SIZE, CELL_SIZE };
        SDL_RenderFillRect(renderer, &foodRect);

        renderText("Score: " + std::to_string(score), 20, 20);
    } else if (gameState == GAME_OVER) {
        renderText("Game Over!", SCREEN_WIDTH / 2.5, SCREEN_HEIGHT / 2 - 50);
        renderText("Press ENTER to Restart", SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2);
        renderText("Final Score: " + std::to_string(score), SCREEN_WIDTH / 2.5, SCREEN_HEIGHT / 2 + 50);
    }

    SDL_RenderPresent(renderer);
}

void closeSDL() {
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(gameOverSound);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

int main() {
    srand(time(0));
    if (!initSDL()) return -1;
    while (!gameOver) {
        handleInput();
        updateGame();
        renderGame();
        SDL_Delay(100);
    }
    closeSDL();
    return 0;
}
