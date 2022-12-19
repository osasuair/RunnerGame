#include "defs.h"

/* 
	Function:  output
	 Purpose:  loops for each frame and prints game to screen
		  in:  game pointer
 */
void *output(void *game) {
	while(C_TRUE) {
		if(*((GameType*) game)->running == C_FALSE) {
    		printToWindow((GameType*) game);
    		pthread_exit(NULL);
    	}
		printToWindow((GameType*) game);
		usleep(SPEED);
	}
}

/* 
	Function:  printToWindow
	 Purpose:  converts game to array of strings, prints that string to window
		  in:  game pointer
 */
void printToWindow(GameType *game) {
	clearWindow(game->win);
	char **str;
	initGameString(&str);

	if (sem_wait(&game->mutex) < 0) {
		*game->running = C_FALSE;
		pthread_exit(NULL);
	}

	gameToString(game, str);

	if(*game->running == C_FALSE)
		gameOverString(str);

	if (sem_post(&game->mutex) < 0) {
		*game->running = C_FALSE;
		pthread_exit(NULL);
	}

	scrPrt(game->win, str);
	freeGameString(&str);
}
