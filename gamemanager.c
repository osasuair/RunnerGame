#include "defs.h"

/* 
    Function:  updateGame
     Purpose:  loops through game logic/ base function for game thread
      in/out:  game pointer
*/
void *updateGame(void *game) {
    while ((C_TRUE)) {
        if(*((GameType*) game)->running == C_FALSE) {
            pthread_exit(NULL);
        }

        if (sem_wait(&((GameType*)game)->mutex) < 0){
            *((GameType*) game)->running = C_FALSE;
            pthread_exit(NULL);
        }

        advanceGame((GameType*) game);
        collision((GameType*) game);
        ((GameType*)game)->score += 1;

        if (sem_post(&((GameType*)game)->mutex) < 0){
            *((GameType*) game)->running = C_FALSE;
            pthread_exit(NULL);
        }
            
        usleep(SPEED);
    }
}

/* 
    Function:  advanceGame
     Purpose:  advances game by one frame
      in/out:  game pointer
*/
void advanceGame(GameType *game) {

    for(int i = 0; i<game->cloudN; ++i) {
        game->clouds[i].col -= 1;
        if(game->clouds[i].col == 0) {
            deleteCloud(game, i);
        }
    }
    for(int i = 0; i<game->crateN; ++i) {
        game->crates[i].pos.col -= 1;
        if(game->crates[i].pos.col == 0) {
            deleteCrate(game, i);
        }
    }

    newClouds(game);
    newCrates(game);
    shiftGround(game->ground);

    // if runner is not jumping or punching, runner runs
    if (game->inaction > 0) {
        game->inaction -= 1;
    } else {
        game->action = RUN;
    }
}

/* 
    Function:  collision
     Purpose:  checks for collision between runner and crates
      in/out:  game pointer
*/
void collision(GameType *game) {
    for(int i = 0; i < game->crateN; ++i) {
        if(game->crates[i].pos.col > (RUNNER_COL - CRATE_WIDTH) && game->crates[i].pos.col < (RUNNER_COL + RUNNER_WIDTH-1)) { 
            if(game->action == RUN){
                *game->running = C_FALSE;
            }
            else if(game->action == JUMP && game->crates[i].crate == EMPTY) {
                *game->running = C_FALSE;
            }
            else if(game->action == PUNCH) {
                if(game->crates[i].crate == EMPTY) {
                    game->score += 100;
                    deleteCrate(game, i);
                } else {
                    *game->running = C_FALSE;
                }
            }
        }
    }   
}

/* 
    Function:  shiftGround
     Purpose:  shifts ground to the left by one iteration and randomly generates new column of ground
      in/out:  ground pointer
*/
void shiftGround(char **ground) {
    for (int i = 1; i < GROUND_ROW; ++i) {
        for (int j = 0; j < GROUND_COL - 2; ++j) {
            ground[i][j] = ground[i][j+1];
        }
    }

    for(int i = 0; i < 3; ++i) {
        int newRand = randm(4);
        if(newRand == 0) {
            ground[1+i][GROUND_COL - 2] = '`';
        } else {
            ground[1+i][GROUND_COL - 2] = ' ';
        }
    }
}

/* 
    Function:  newClouds
     Purpose:  adds new clouds to game if conditions are met
      in/out:  game pointer
 */
void newClouds(GameType *game) {
    static long int lastCloudAdded = 100; // Initially an arbitrary large number

    if (game->cloudN < MAX_CLOUDS) {
        int newRand = randm(2*lastCloudAdded);
        if(newRand > 8 && lastCloudAdded > CLOUD_WIDTH +2) {
            game->clouds[game->cloudN].row = randm(3);
            game->clouds[game->cloudN].col = GROUND_COL - (CLOUD_WIDTH +2);
            game->cloudN += 1;
            lastCloudAdded = 0;
        }
    }

    lastCloudAdded += 1;
}

/* 
    Function: newCrates
     Purpose: adds new crates to game if conditions are met
      in/out: game pointer    
*/
void newCrates(GameType *game) {
    static long int lastCrateAdded = 100;

    if (game->crateN < MAX_CRATE) {
        int newRand = randm(3);
        if(newRand == 0 && lastCrateAdded > (CRATE_WIDTH + 10)) {
            game->crates[game->crateN].pos.row = 11;
            game->crates[game->crateN].pos.col = WIDTH - (CRATE_WIDTH + 1);
            newRand = randm(2);
            if(newRand == 0) {
                game->crates[game->crateN].crate = FILLED;
            } else {
                game->crates[game->crateN].crate = EMPTY;
            }
            game->crateN += 1;
            lastCrateAdded = 0;
        }
    }

    lastCrateAdded += 1;
}

/* 
    Function:  deleteCloud
     Purpose:  deletes cloud from GameType game
      in/out:  game pointer
          in:  index of cloud to be deleted
 */
void deleteCloud(GameType *game, int index) {
    for(int i = index; i < game->cloudN - 1; ++i) {
        game->clouds[i] = game->clouds[i+1];
    }
    game->cloudN -= 1;
}

/* 
    Function:  deleteCrate
     Purpose:  deletes crate from GameType game
      in/out:  game pointer
          in:  index of crate to be deleted
*/
void deleteCrate(GameType *game, int index) {
    for(int i = index; i < game->crateN - 1; ++i) {
        game->crates[i] = game->crates[i+1];
    }
    game->crateN -= 1;
}