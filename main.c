#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <unistd.h>
#include <errno.h>

#define ROWS 20
#define COLS 50
#define WALL '#'
#define SNAKE_CHAR 'O'
#define FOOD_CHAR 'F'
#define INITIAL_LENGTH 3

typedef struct {
    int X;
    int Y;
} Position;

typedef struct {
    Position body[ROWS * COLS];
    int length;
    int direction;
} Snake;

Position food;
Snake snake;
int gameOver = 0;

void hideCursor() {
    printf("\e[?25l");
}

void clrscr() {
    printf("\e[1;1H\e[2J");
}

void gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void drawBoundaries() {
    for (int row = 0; row < ROWS + 2; row++) {
        for (int col = 0; col < COLS + 2; col++) {
            if (row == 0 || row == ROWS + 1 || col == 0 || col == COLS + 1) {
                printf("%c", WALL);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void drawSnake() {
    for (int i = 0; i < snake.length; i++) {
        gotoxy(snake.body[i].X, snake.body[i].Y);
        printf("%c", SNAKE_CHAR);
    }
}

void clearSnake() {
    for (int i = 0; i < snake.length; i++) {
        gotoxy(snake.body[i].X, snake.body[i].Y);
        printf(" ");
    }
}

void placeFood() {
    food.X = rand() % COLS + 1;
    food.Y = rand() % ROWS + 1;
    gotoxy(food.X, food.Y);
    printf("%c", FOOD_CHAR);
}

void initializeGame() {
    srand(time(0));
    clrscr();
    hideCursor();
    drawBoundaries();

    snake.length = INITIAL_LENGTH;
    snake.body[0].X = COLS / 2;
    snake.body[0].Y = ROWS / 2;
    snake.direction = 'd'; // Start moving right

    for (int i = 1; i < snake.length; i++) {
        snake.body[i].X = snake.body[i - 1].X - 1;
        snake.body[i].Y = snake.body[i - 1].Y;
    }

    placeFood();
}

int checkSelfCollision(Position pos) {
    for (int i = 1; i < snake.length; i++) {
        if (pos.X == snake.body[i].X && pos.Y == snake.body[i].Y) {
            return 1;
        }
    }
    return 0;
}

void moveSnake() {
    Position newHead = snake.body[0];

    if (snake.direction == 'w') newHead.Y--;
    else if (snake.direction == 's') newHead.Y++;
    else if (snake.direction == 'a') newHead.X--;
    else if (snake.direction == 'd') newHead.X++;

    // Implement wrapping around the borders
    if (newHead.X <= 0) newHead.X = COLS;
    else if (newHead.X > COLS) newHead.X = 1;
    if (newHead.Y <= 0) newHead.Y = ROWS;
    else if (newHead.Y > ROWS) newHead.Y = 1;

    // Check if the snake collides with itself
    if (checkSelfCollision(newHead)) {
        gameOver = 1;
        return;
    }

    // Check if the snake eats the food
    if (newHead.X == food.X && newHead.Y == food.Y) {
        snake.length++;
        placeFood(); // Place a new piece of food
    } else {
        clearSnake(); // Clear tail if no growth
    }

    // Move the body
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }
    snake.body[0] = newHead;
    drawSnake();
}

void processInput() {
    char ch = getch();  // Wait for input
    if (ch == 'w' && snake.direction != 's') snake.direction = 'w';
    else if (ch == 's' && snake.direction != 'w') snake.direction = 's';
    else if (ch == 'a' && snake.direction != 'd') snake.direction = 'a';
    else if (ch == 'd' && snake.direction != 'a') snake.direction = 'd';
}

int main() {
    initializeGame();

    while (!gameOver) {
        processInput();   // Wait for user input before each move
        moveSnake();      // Move the snake only after an input
    }

    gotoxy(0, ROWS + 3);
    printf("Game Over! Your score is: %d\n", snake.length - INITIAL_LENGTH);
    return 0;
}
