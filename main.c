#include "defs.h"

int main () {

	// initialize game
	GameType *game = malloc(sizeof(GameType));
	game->win = initNcurses();
	if (sem_init(&game->mutex, 0, 1) < 0) {
		clearWindow(game->win);
		endwin();
		return C_ERROR;
	}
	game->highScore = 0;

	// main game loop
	while(C_TRUE) {
		
		srand((unsigned)time(NULL));
		game->running = malloc(sizeof(int));
		*game->running = C_TRUE;
		char c;
		startGame(game);

		while(C_TRUE) {
			if(*game->running == C_FALSE) {
				break;
			}
			c = wgetch(game->win);
			charToAction(c, game);
		}

		if (game->score > game->highScore)
			game->highScore = game->score;
		cleanupGame(game);
		if(c == 'q') {
			break;
		}
	}

	free(game);
	return 0;
}

/*
  Function:  randm
   Purpose:  generates a pseudorandom number
             in the range [0, max)
        in:  upper range of generated number
   returns:  the randomly generated number
*/
int randm(int max) {
  double r = ( (double)rand() / ((double)(RAND_MAX)+(double)(1)) ); 
  return (int)(r * max);
}

/*
	Function:  cleanupGame
	 Purpose:  deallocate all memory part of the game
		  in:  game pointer
*/
void cleanupGame(GameType *game) {
	pthread_join(game->outputThr, NULL);
	pthread_join(game->gameThr, NULL);
	freeGround(&game->ground);
	free((int*) game->running);
	endwin();
}