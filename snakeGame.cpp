//We are Group 14 

#include <iostream>  //Basic input output
#include <fstream>  //Allows us to work with files
#include <windows.h>  //Windows-specific header file, It contains all of the functions in the Windows API
#include <conio.h>  //Input Output - File handling 
#include <cstdlib>  //several general purpose functions, including dynamic memory management, integer arithmetics, searching, sorting and converting
#include <ctime>  //declares a set of functions, macros and types to work with time
#include <string>  //To use strings
#include <vector>  //To use vectors
#include <cmath>  //To use math func
#include <thread>  //To use thread func

using namespace std;

//Define symbolic constants and macros 
// screen constants
#define WIDTH 80
#define HEIGHT 25
//Arrow Keys for controls
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
//color
#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"

// game functions prototype
void gotxy(int x, int y);
void ScreenSize();
void screenBorder();
void snakegame();
void Instructions();
void Controls();
void drawSnake();
void drawFood();
void drawScore();
void drawGame();
void moveSnake();
void checkFood();
void generateFood();
void checkSnake();
void crew();
void checkGame();
void startGame();
void changeDirection();
void increaseSpeed();
void clrAndStartGame();

// game variables
int score, snakeSize;
int foodX, foodY;
int snakeX[100], snakeY[100];
bool gameOver;
int tailX, tailY;
int gameDelay = 100;

// direction variables
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection direction;

// got xy function
void gotxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// screen size is 80x30
// cursor is not visible
void ScreenSize()
{
    system("mode con cols=80 lines=30");    
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

// function to draw the screen border
void screenBorder()
{
    for (int i = 0; i < WIDTH; i++)
    {
        gotxy(i, 0);
        cout << char(61);
        gotxy(i, HEIGHT - 1);
        cout << char(61);
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        gotxy(0, i);
        cout << char(124);
        gotxy(WIDTH - 1, i);
        cout << char(124);
    }
    gotxy(0, 0);
    cout << char(43);
    gotxy(0, HEIGHT - 1);
    cout << char(43);
    gotxy(WIDTH - 1, 0);
    cout << char(43);
    gotxy(WIDTH - 1, HEIGHT - 1);
    cout << char(43);
}

void snakegame()
{
    gotxy(18,3); cout <<CYN "OOOOOO  OO    O      OO      O    O  OOOOOO";
    gotxy(18,4); cout <<    "O       O O   O     O  O     O  O    O     ";
    gotxy(18,5); cout <<    "OOOOOO  O  O  O    OOOOOO    OOO     OOOOOO";
    gotxy(18,6); cout <<    "     O  O   O O   O      O   O  O    O     ";
    gotxy(18,7); cout <<    "OOOOOO  O    OO  O        O  O    O  OOOOOO" ;
    
    gotxy(20,9); cout <<     "OOOOOO      OO      OO       OO  OOOOOO";
    gotxy(20,10); cout <<    "O          O  O     O O     O O  O     ";
    gotxy(20,11); cout <<    "O OOOO    OOOOOO    O  O   O  O  OOOOOO";
    gotxy(20,12); cout <<    "O    O   O      O   O   O O   O  O     ";
    gotxy(20,13); cout <<    "OOOOOO  O        O  O    O    O  OOOOOO" NC;

}

// function to fetch the multiline instruction from instructions.txt
// display it in the center of the screen
void Instruction()
{
    ifstream file;
    file.open("instructions.txt");
    string line;
    int y = 16;
    while (getline(file, line))
    {
        int x = ((WIDTH / 2) - (line.length() / 2) - 1);
        gotxy(x, y);
        cout << line;
        y++;
    }
    file.close();
    gotxy(WIDTH / 2, y);
    Controls();
    clrAndStartGame();
}

// function to read the controls instructions from controls.txt
// display it in the bottom of game area
void Controls()
{

    ifstream file;
    file.open("controls.txt");
    string line;
    int y = HEIGHT;
    while (getline(file, line))
    {
        int x = ((WIDTH / 2) - (line.length() / 2) - 1);
        gotxy(x, y);
        cout << line;
        y++;
    }
    file.close();
    gotxy(WIDTH / 2, y);
}

// function to draw snake
void drawSnake()
{
    for (int i = 0; i < snakeSize; i++)
    {
        gotxy(snakeX[i], snakeY[i]);
        cout << 'O';
    }
}

// function to draw food
// refresh the border since the upper left corner disappears
void drawFood()
{
    gotxy(foodX, foodY);
    cout <<  char(42);  
    screenBorder();
}

// function to display the score
void drawScore()
{
    gotxy(WIDTH - 15, 0);
    cout << "_Score: " << score <<"_";
}

// function to draw the game
void drawGame()
{
    Controls();
    drawSnake();
    drawFood();
    drawScore();
}

// function to move the snake according to the direction active
void moveSnake()
{
    tailX = snakeX[snakeSize - 1];
    tailY = snakeY[snakeSize - 1];
    changeDirection();
    for (int i = snakeSize - 1; i > 0; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }                                                           
    if (direction == LEFT)
    {
        snakeX[0]--;
    }
    else if (direction == RIGHT)
    {
        snakeX[0]++;
    }
    else if (direction == UP)
    {
        snakeY[0]--;
    }
    else if (direction == DOWN)
    {
        snakeY[0]++;
    }
    drawSnake();
    gotxy(tailX, tailY);
    cout << " ";
}

// function to change the direction of the snake when the user presses the arrow keys
void changeDirection()
{
    
    if (_kbhit())
    {
        char ch = _getch();
        if (ch == 'a' || ch == 'A' || ch == KEY_LEFT)
        {
            if (direction != RIGHT)
            {
                direction = LEFT;
            }
        }
        else if (ch == 'd' || ch == 'D' || ch == KEY_RIGHT)
        {
            if (direction != LEFT)
            {
                direction = RIGHT;
            }
        }
        else if (ch == 'w' || ch == 'W' || ch == KEY_UP)
        {
            if (direction != DOWN)
            {
                direction = UP;
            }
        }
        else if (ch == 's' || ch == 'S' || ch == KEY_DOWN)
        {
            if (direction != UP)
            {
                direction = DOWN;
            }
        }
    }
}

// function to check if the snake eats the food
// if the snake eats the food:
// increase the score by 1
// increase the size of the snake by 1
// increase the speed
// generate a new food
void checkFood()
{
    if (snakeX[0] == foodX && snakeY[0] == foodY)
    {
        score++;
        snakeSize++;
        // increase the speed
        increaseSpeed();
        generateFood();
    }
}

// function to generate the food
// generate the food randomly
// if the snake eats the food, new food is generated
void generateFood()
{
    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
    drawFood();
}

// function to check if the snake hits the wall or itself
void checkSnake()
{
    // if the snake hits the wall, the snake passes through the wall and appear on the other side of the screen
    if (snakeX[0] == 0 || snakeX[0] == WIDTH - 1 || snakeY[0] == 0 || snakeY[0] == HEIGHT - 1)
    {
        if (snakeX[0] == 0)
        {
            gameOver = true;              
        }
        else if (snakeX[0] == WIDTH - 1)
        {
            gameOver = true;               
        }
        else if (snakeY[0] == 0)
        {
            gameOver = true;                
        }
        else if (snakeY[0] == HEIGHT - 1)
        {
            gameOver = true;                          
        }
        screenBorder();
        drawScore();
    }
    // if the snake hits itself, game is over
    for (int i = 1; i < snakeSize; i++)
    {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
        {
            gameOver = true;
        }
    }
}

//Production crew display
void crew()
{
    gotxy(32,2); cout << "Production  Crew\n";
    gotxy(24,3); cout << "IM/2020/098 - Pravil Wijesinghe";
    gotxy(24,4); cout << "IM/2020/037 - Hashinee Perera";
    gotxy(24,5); cout << "IM/2020/002 - Achintha Alahakoon";
    gotxy(24,6); cout << "IM/2020/096 - Kawya Thathsarani";
    gotxy(24,7); cout << "IM/2020/016 - Bhuddhika Hasitha";
}

// function to check if the game is over
void checkGame()
{
    // if the game is over
    if (gameOver)
    {
        system("cls");
        screenBorder();
        crew();
        // display the game over message
        gotxy(WIDTH/2-5, HEIGHT/2-1);
        cout << RED "Game Over!";
        // display the score over message
        gotxy(WIDTH/2-4, HEIGHT/2+1);
        cout << GRN "Score: " << score <<NC;
        Instruction();
        clrAndStartGame();
    }
}

// function to clear the screen and start the game with keyboard press
// wait for the user to press a key
// if the user presses the space bar or key 'n', the game starts with gameDelay=200
// if the user presses the key 'h', the game starts with gameDelay=100
// if the user pressed the key 'e', the game starts with gameDelay=300
// if the user presses the escape key, the game quits
void clrAndStartGame()
{
    while (true)
    {
        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 32 || ch == 'n')
            {
                system("cls");
                screenBorder();
                gameDelay = 200;
                break;
            }
            else if (ch == 'h')
            {
                system("cls");
                screenBorder();
                gameDelay = 100;
                break;
            }
            else if (ch == 'e')
            {
                system("cls");
                screenBorder();
                gameDelay = 300;
                break;
            }
            else if (ch == 27)
            {
                exit(0);
            }
        }
    }
    // start the game
    startGame();
}

// function to increase the speed of the game
// if the gameDelay is greather than 200, decrease the gameDelay by 8
// else if the gameDelay is greater than 100, decrease the gameDelay by 7
// else if the gameDelay is greater than 50, decrease the gameDelay by 4
// else if the gameDelay is greater than 40, decrease the gameDelay by 3
void increaseSpeed()
{
    if (gameDelay > 200)
    {
        gameDelay -= 8;
    }
    else if (gameDelay > 100)
    {
        gameDelay -= 7;
    }
    else if (gameDelay > 50)
    {
        gameDelay -= 4;
    }
    else if (gameDelay > 40)
    {
        gameDelay -=3;
    }
}

// function to control the game
void controlGame()
{
    // move the snake
    moveSnake();
    // check if the snake eats the food
    checkFood();
    // check if the snake hits the wall or itself
    checkSnake();
    // check if the game is over
    checkGame();
}

// function to start the game
void startGame()
{
    // set the starting direction
    direction = RIGHT;

    // set the starting position of the snake
    snakeX[0] = WIDTH / 2;
    snakeY[0] = HEIGHT / 2;

    // set the starting position of the food
    foodX = (rand() % (WIDTH - 2)) + 1;
    foodY = (rand() % (HEIGHT - 2)) + 1;

    // set the starting score
    score = 0;

    // set the starting size of the snake
    snakeSize = 1;

    // set the starting gameOver to false
    gameOver = false;

    // control the game
    while (!gameOver)
    {
        controlGame();

        // delay the game
        Sleep(gameDelay);

        // draw the game
        drawGame();
    }
}

int main()
{
    // set the screen size
    ScreenSize();

    // set the screen border
    screenBorder();

    snakegame();

    // read the instructions
    Instruction();

    // start the game
    startGame();

}