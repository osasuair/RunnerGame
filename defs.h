#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <curses.h>

#define C_TRUE        1
#define C_FALSE       0
#define C_ERROR      -1

#define HEIGHT       19
#define WIDTH        65
#define START_X       0
#define START_Y       0

#define SPEED     50000
#define JUMP_TIME    12
#define PUNCH_TIME   10
#define RUNNER_ROW   12
#define RUNNER_COL   30
#define MAX_CRATE     3
#define MAX_CLOUDS    6
#define RUNNER_WIDTH  3
#define CRATE_WIDTH   5
#define CRATE_HEIGHT  4
#define CLOUD_WIDTH   5
#define GROUND_ROW    4
#define GROUND_COL   65

#define GAME_OVER     1

typedef enum {RUN, JUMP, PUNCH} ActionType;
typedef enum {EMPTY, FILLED} CrateEnumType;

typedef struct {
    int row;
    int col;
} PositionType;

typedef struct {
    PositionType pos;
    CrateEnumType crate;
} CrateType;

typedef struct {
    volatile int *running;
    ActionType action;
    int inaction;
    CrateType crates[MAX_CRATE];
    PositionType clouds[MAX_CLOUDS]; 
    char **ground;
    int crateN;
    int cloudN;
    long int score;
    long int highScore;
    WINDOW *win;
    pthread_t outputThr;
    pthread_t gameThr;
    sem_t mutex;
} GameType;

int randm(int max);
void cleanupGame(GameType *game);

void startGame(GameType *game);
void charToAction(char c, GameType *game);

WINDOW *initNcurses();
void scrPrt(WINDOW *win, char **str);
void clearWindow(WINDOW *win);

void *output(void *game);
void printToWindow(GameType *game);

void *updateGame(void *game);
void collision(GameType *game);
void advanceGame(GameType *game);
void shiftGround(char **ground);
void newClouds(GameType *game);
void newCrates(GameType *game);
void deleteCloud(GameType *game, int index);
void deleteCrate(GameType *game, int index);

void gameToString(GameType *game, char **str);
void gameOverString(char **str);
void addCloud(char **str, PositionType pos);
void addRunner(char **str, ActionType action);
void addCrate(char **str, CrateType);
void addScore(char **str, long int score, long int highScore);
void initGround(char ***ground);
void initGameString(char ***str);
void freeGameString(char ***str);
void freeGround(char ***ground);
