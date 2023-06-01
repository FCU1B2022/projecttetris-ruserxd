#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable:4996)

#define LEFT_KEY 0x25     // The key to move left, default = 0x25 (left arrow)
#define RIGHT_KEY 0x27    // The key to move right, default = 0x27 (right arrow)
#define ROTATE_KEY 0x26   // The key to rotate, default = 0x26 (up arrow)
#define DOWN_KEY 0x28     // The key to move down, default = 0x28 (down arrow)
#define FALL_KEY 0x20     // The key to fall, default = 0x20 (spacebar)
#define LEAVE_KEY 0x1B    // The key to leave
#define FALL_DELAY 500    // The delay between each fall, default = 500
#define RENDER_DELAY 100  // The delay between each frame, default = 100

#define LEFT_FUNC() GetAsyncKeyState(LEFT_KEY) & 0x8000
#define RIGHT_FUNC() GetAsyncKeyState(RIGHT_KEY) & 0x8000
#define ROTATE_FUNC() GetAsyncKeyState(ROTATE_KEY) & 0x8000
#define DOWN_FUNC() GetAsyncKeyState(DOWN_KEY) & 0x8000
#define FALL_FUNC() GetAsyncKeyState(FALL_KEY) & 0x8000
#define Leave_FUNC() GetAsyncKeyState(LEAVE_KEY)&0x8000
#define CANVAS_WIDTH 10
#define CANVAS_HEIGHT 20

typedef enum {
    RED = 41,
    GREEN,
    YELLOW,
    BLUE,
    PURPLE,
    CYAN,
    WHITE,
    BLACK = 0,
}Color;

typedef enum {
    EMPTY = -1,
    I,
    J,
    L,
    O,
    S,
    T,
    Z
}ShapeId;

typedef struct {
    ShapeId shape;
    Color color;
    int size;
    char rotates[4][4][4];
}Shape;

typedef struct
{
    int x;
    int y;
    int score;
    int rotate;
    int fallTime;
    ShapeId queue[4];
}State;

typedef struct {
    Color color;
    ShapeId shape;
    bool current;
}Block;

Shape shapes[7] = {
    {
        .shape = I,
        .color = CYAN,
        .size = 4,
        .rotates =
        {
            {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0}
            },
            {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0}
            }
        }
    },
    {
        .shape = J,
        .color = BLUE,
        .size = 3,
        .rotates =
        {
            {
                {1, 0, 0},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 1},
                {0, 1, 0},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {0, 0, 1}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {1, 1, 0}
            }
        }
    },
    {
        .shape = L,
        .color = YELLOW,
        .size = 3,
        .rotates =
        {
            {
                {0, 0, 1},
                {1, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {1, 0, 0}
            },
            {
                {1, 1, 0},
                {0, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = O,
        .color = WHITE,
        .size = 2,
        .rotates =
        {
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            },
            {
                {1, 1},
                {1, 1}
            }
        }
    },
    {
        .shape = S,
        .color = GREEN,
        .size = 3,
        .rotates =
        {
            {
                {0, 1, 1},
                {1, 1, 0},
                {0, 0, 0}
            },
            {
                {0, 1, 0},
                {0, 1, 1},
                {0, 0, 1}
            },
            {
                {0, 0, 0},
                {0, 1, 1},
                {1, 1, 0}
            },
            {
                {1, 0, 0},
                {1, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = T,
        .color = PURPLE,
        .size = 3,
        .rotates =
        {
            {
                {0, 1, 0},
                {1, 1, 1},
                {0, 0, 0}
            },

                {{0, 1, 0},
                {0, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 1, 0},
                {1, 1, 0},
                {0, 1, 0}
            }
        }
    },
    {
        .shape = Z,
        .color = RED,
        .size = 3,
        .rotates =
        {
            {
                {1, 1, 0},
                {0, 1, 1},
                {0, 0, 0}
            },
            {
                {0, 0, 1},
                {0, 1, 1},
                {0, 1, 0}
            },
            {
                {0, 0, 0},
                {1, 1, 0},
                {0, 1, 1}
            },
            {
                {0, 1, 0},
                {1, 1, 0},
                {1, 0, 0}
            }
        }
    },
};

void setBlock(Block* block, Color color, ShapeId shape, bool current)
{
    block->color = color;
    block->shape = shape;
    block->current = current;
}

void resetBlock(Block* block)
{
    block->color = BLACK;
    block->shape = EMPTY;
    block->current = false;
}

bool move(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int originalX, int originalY, int originalRotate, int newX, int newY, int newRotate, ShapeId shapeId) {
    Shape shapeData = shapes[shapeId];
    int size = shapeData.size;

    // check if the new position is valid to place the block
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (shapeData.rotates[newRotate][i][j]) {
                if (newX + j < 0 || newX + j >= CANVAS_WIDTH || newY + i < 0 || newY + i >= CANVAS_HEIGHT) {
                    return false;
                }
                if (!canvas[newY + i][newX + j].current && canvas[newY + i][newX + j].shape != EMPTY) {
                    return false;
                }
            }
        }
    }

    // remove the old position
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (shapeData.rotates[originalRotate][i][j]) {
                resetBlock(&canvas[originalY + i][originalX + j]);
            }
        }
    }

    // move the block
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (shapeData.rotates[newRotate][i][j]) {
                setBlock(&canvas[newY + i][newX + j], shapeData.color, shapeId, true);
            }
        }
    }

    return true;
}

void printCanvas(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], State* state)
{
    printf("\033[0;0H\n");
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        printf("|");
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            printf("\033[%dm\u3000", canvas[i][j].color);
        }
        printf("\033[0m|\n");
    }

    Shape shapeData = shapes[state->queue[1]];
    printf("\033[%d;%dHNext:", 3, CANVAS_WIDTH * 2 + 5);
    for (int i = 1; i <= 3; i++)
    {
        shapeData = shapes[state->queue[i]];
        for (int j = 0; j < 4; j++) {
            printf("\033[%d;%dH", i * 4 + j, CANVAS_WIDTH * 2 + 15);
            for (int k = 0; k < 4; k++) {
                if (j < shapeData.size && k < shapeData.size && shapeData.rotates[0][j][k]) {
                    printf("\x1b[%dm  ", shapeData.color);
                }
                else {
                    printf("\x1b[0m  ");
                }
            }
        }
    }
    return;
}

int clearLine(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH]) {
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            if (canvas[i][j].current) {
                canvas[i][j].current = false;
            }
        }
    }

    int linesCleared = 0;

    for (int i = CANVAS_HEIGHT - 1; i >= 0; i--)
    {
        bool isFull = true;
        for (int j = 0; j < CANVAS_WIDTH; j++)
        {
            if (canvas[i][j].shape == EMPTY) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            linesCleared += 1;

            for (int j = i; j > 0; j--)
            {
                for (int k = 0; k < CANVAS_WIDTH; k++)
                {
                    setBlock(&canvas[j][k], canvas[j - 1][k].color, canvas[j - 1][k].shape, false);
                    resetBlock(&canvas[j - 1][k]);
                }
            }
            i++;
        }
    }


    return linesCleared;
}

int logic(Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH], State* state)
{
    if (ROTATE_FUNC()) {
        int newRotate = (state->rotate + 1) % 4;
        if (move(canvas, state->x, state->y, state->rotate, state->x, state->y, newRotate, state->queue[0]))
        {
            state->rotate = newRotate;
        }
    }
    else if (LEFT_FUNC()) {
        if (move(canvas, state->x, state->y, state->rotate, state->x - 1, state->y, state->rotate, state->queue[0]))
        {
            state->x -= 1;
        }
    }
    else if (RIGHT_FUNC()) {
        if (move(canvas, state->x, state->y, state->rotate, state->x + 1, state->y, state->rotate, state->queue[0]))
        {
            state->x += 1;
        }
    }
    else if (DOWN_FUNC()) {
        state->fallTime = FALL_DELAY;
    }
    else if (FALL_FUNC()) {
        state->fallTime += FALL_DELAY * CANVAS_HEIGHT;
    }
    else if (Leave_FUNC()) {
        system("cls");
        Sleep(100);
        return -1;
    }
    state->fallTime += RENDER_DELAY;

    while (state->fallTime >= FALL_DELAY) {
        state->fallTime -= FALL_DELAY;

        if (move(canvas, state->x, state->y, state->rotate, state->x, state->y + 1, state->rotate, state->queue[0])) {
            state->y++;
        }
        else {
            state->score += clearLine(canvas);

            state->x = CANVAS_WIDTH / 2;
            state->y = 0;
            state->rotate = 0;
            state->fallTime = 0;
            state->queue[0] = state->queue[1];
            state->queue[1] = state->queue[2];
            state->queue[2] = state->queue[3];
            state->queue[3] = rand() % 7;

            if (!move(canvas, state->x, state->y, state->rotate, state->x, state->y, state->rotate, state->queue[0]))
            {
                printf("\033[%d;%dH\x1b[41m GAME OVER \x1b[0m\033[%d;%dH", CANVAS_HEIGHT - 3, CANVAS_WIDTH * 2 + 5, CANVAS_HEIGHT + 5, 0);
                exit(0);
            }
        }
    }
    return 0;
}

//
//選擇要玩的遊戲
int drawArcadeMenu()
{
    printf("========== 街機遊戲選單 ==========\n");
    printf("1. ooxx\n");
    printf("2. 俄羅斯方塊\n");
    printf("0. 可直接退出遊戲\n");
    printf("==================================\n");
    printf("輸入想玩啥?\n");
    int n = 1;
    scanf("%d", &n);
    if (n == 0)exit(0);
    Sleep(1000);
    return n;
}
//
//
//OOXX的遊戲
bool jg_win(char array[9]) {
    if (array[0] == 'o' && array[1] == 'o' && array[2] == 'o')return true;
    else if (array[3] == 'o' && array[4] == 'o' && array[5] == 'o')return true;
    else if (array[6] == 'o' && array[7] == 'o' && array[8] == 'o')return true;
    else if (array[0] == 'o' && array[4] == 'o' && array[8] == 'o')return true;
    else if (array[2] == 'o' && array[4] == 'o' && array[6] == 'o')return true;
    else if (array[0] == 'o' && array[3] == 'o' && array[6] == 'o')return true;
    else if (array[1] == 'o' && array[4] == 'o' && array[7] == 'o')return true;
    else if (array[2] == 'o' && array[5] == 'o' && array[8] == 'o')return true;
    if (array[0] == 'x' && array[1] == 'x' && array[2] == 'x')return true;
    else if (array[3] == 'x' && array[4] == 'x' && array[5] == 'x')return true;
    else if (array[6] == 'x' && array[7] == 'x' && array[8] == 'x')return true;
    else if (array[0] == 'x' && array[4] == 'x' && array[8] == 'x')return true;
    else if (array[2] == 'x' && array[4] == 'x' && array[6] == 'x')return true;
    else if (array[0] == 'x' && array[3] == 'x' && array[6] == 'x')return true;
    else if (array[1] == 'x' && array[4] == 'x' && array[7] == 'x')return true;
    else if (array[2] == 'x' && array[5] == 'x' && array[8] == 'x')return true;
    return false;
}
void print_gaming(char array[9], int meo[9]) {
    system("cls");
    for (int i = 0; i < 9; i++) {
        printf("\033[43;30m");
        if (meo[i] != 0)
            printf("\033[31;1m%c \033[0m", array[i]);
        else
            printf("%d \033[0m", i);
        if (i % 3 == 2)printf("\n");
        printf("\033[0m");
    }
}
int edge_wrong_byOOXX(int x) {
    if (x < 0 || x>8) {
        printf("You exceed the edge please try again!!!\n");
        return 0;
    }
    return 1;
}

int OOXX() {
    char array[9] = { 0 };
    int meo[9] = { 0 };
    printf("Hello I will like to start a game name for OOXX\n");
    int want_to_play = 1;//0==o 1==x
    while (want_to_play) {
        int n = 0;
        memset(array, '0', sizeof(array));
        memset(meo, 0, sizeof(meo));
        print_gaming(array, meo);
        while (!jg_win(array)) {
            int x;
            scanf("\n%d", &x);
            if (edge_wrong_byOOXX) {
                if (n % 2 == 0)array[x] = 'o';
                else if (n % 2 == 1)array[x] = 'x';
                meo[x] = 1;
                n++;
            }
            print_gaming(array, meo);
        }
        if (n == 9 && !jg_win(array))printf("平手\n");
        else if (n % 2 == 1)printf("O winner\n");
        else if (n % 1 == 0) printf("x winner\n");
        printf("Do you wnat to play again??(Yes==1,No==0)\n");
        scanf("%d", &want_to_play);
    }
    return 0;
}
//內部機器設定
int edge_bymachine(int x) {
    if (x != 0 && x != 1) {
        printf("您輸入的未在要求內喔!!\n");
        return -1;
    }
    return 1;
}
int main()
{
    int want_to_play = 2;
    while (want_to_play) {
        system("cls");
        int n = drawArcadeMenu();
        printf("%d\n", n);
        srand((unsigned)time(NULL));
        if (n == 1) {
            system("cls");
            OOXX();
        }
        else if (n == 2) {
            State state = {
                .x = CANVAS_WIDTH / 2,
                .y = 0,
                .score = 0,
                .rotate = 0,
                .fallTime = 0
            };

            for (int i = 0; i < 4; i++)
            {
                state.queue[i] = rand() % 7;
            }

            Block canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
            for (int i = 0; i < CANVAS_HEIGHT; i++)
            {
                for (int j = 0; j < CANVAS_WIDTH; j++)
                {
                    resetBlock(&canvas[i][j]);
                }
            }

            system("cls");

            move(canvas, state.x, state.y, state.rotate, state.x, state.y, state.rotate, state.queue[0]);

            while (1)
            {
                int judge_over = logic(canvas, &state);
                printCanvas(canvas, &state);
                printf("\n\n\n\n\n\n\n\n\n\n\n\Your score:%d", state.score);
                printf("\nESC can leave the gaming if you don't want to play!");
                Sleep(100);
                if (judge_over == -1)break;
            }
        }
    }
}