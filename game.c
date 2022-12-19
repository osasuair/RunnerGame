#include "defs.h"

/* 
    Function:  startGame
     Purpose:  initializes game variables and starts game threads
         out:  game pointer
*/
void startGame(GameType *game) {
    game->action = RUN;
    game->inaction = 0;
    game->score = 1;
    game->cloudN = 0;
    game->crateN = 0;
    initGround(&game->ground);

    pthread_create(&game->outputThr, NULL, output, game);
    pthread_create(&game->gameThr, NULL, updateGame, game);
}

/* 
    Function: charToAction
     Purpose: converts char to actionType and sets game action to that
          in: char
         out: game
 */
void charToAction(char c, GameType *game) {

    if (sem_wait(&game->mutex) < 0) {
		*game->running = C_FALSE;
		pthread_exit(NULL);
	}

    switch(c) {
        case 'w':
            if(game->action == JUMP && game->inaction > 0) {
                break;
            }
            game->action = JUMP;
            game->inaction = JUMP_TIME;
            break;
        case 's':
            game->action = RUN;
            game->inaction = 0;
            break;
        case 'd':
            if(game->action == PUNCH && game->inaction > 0) {
                break;
            }
            game->action = PUNCH;
            game->inaction = PUNCH_TIME;
            break;
        case 'q':
            *game->running = C_FALSE;
            break;
    }

    if (sem_post(&game->mutex) < 0) {
		*game->running = C_FALSE;
		pthread_exit(NULL);
	}
}
