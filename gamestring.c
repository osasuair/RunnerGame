#include "defs.h"

/* 
  Function:  gameToString 
   Purpose:  converts GameType to array of strings
        in:  game to be converted
       out:  newly created array of strings 
*/
void gameToString(GameType *game, char **str) {

    for(int i = 0; i<game->cloudN; ++i) {
        addCloud(str, game->clouds[i]);
    }
    for(int i = 0; i<game->crateN; ++i) {
        addCrate(str, game->crates[i]);
    }
    addRunner(str, game->action);

    for(int i = 0; i< GROUND_ROW; ++i) {
        strcpy(str[HEIGHT-GROUND_ROW+i], game->ground[i]);
    }
    addScore(str, game->score, game->highScore);
}

/* 
    Function:  addCloud
     Purpose:  adds cloud to string array at position pos
      in/out:  string array with cloud added
          in:  position of cloud
*/
void addCloud(char **str, PositionType pos) {
    str[pos.row+0][pos.col+3] = '_';
    str[pos.row+1][pos.col+1] = '_';
    str[pos.row+2][pos.col+1] = '_';
    str[pos.row+2][pos.col+2] = '_';
    str[pos.row+2][pos.col+3] = '_';
    str[pos.row+2][pos.col+4] = '_';
    str[pos.row+1][pos.col+2] = '(';
    str[pos.row+2][pos.col+0] = '(';
    str[pos.row+1][pos.col+4] = ')';
    str[pos.row+2][pos.col+5] = ')';
}

/* 
    Function:  addRunner
     Purpose:  adds runner to string array at position pos
      in/out:  string array with runner added
          in:  action of runner
*/
void addRunner(char **str, ActionType action) {
    int row = RUNNER_ROW;
    int col = RUNNER_COL;
    static int run = 0;

    if(action == RUN) {
        if(run == 0) {
            str[row+0][col+1] = 'o';
            str[row+1][col+0] = 'r';
            str[row+1][col+1] = '|';
            str[row+1][col+2] = 'v';
            str[row+2][col+0] = '/';
            str[row+2][col+2] = '>';
            run = 1;
        } else if(run == 1) {
            str[row+0][col+1] = 'o';
            str[row+1][col+0] = '/';
            str[row+1][col+1] = '|';
            str[row+1][col+2] = '\\';
            str[row+2][col+0] = '>';
            str[row+2][col+2] = '>';
            run = 2;
        } else {
            str[row+0][col+1] = 'o';
            str[row+1][col+0] = '/';
            str[row+1][col+1] = '|';
            str[row+1][col+2] = 'v';
            str[row+2][col+0] = '>';
            str[row+2][col+2] = '\\';
            run = 0;
        }
    }
    else if(action == JUMP) {
        row-= 4;
        str[row+0][col+1] = 'o';
        str[row+1][col+0] = '/';
        str[row+1][col+1] = '|';
        str[row+1][col+2] = 'v';
        str[row+2][col+0] = '>';
        str[row+2][col+2] = '>';
    }
    else if(action == PUNCH) {
        str[row+1][col+0] = '<';
        str[row+2][col+0] = '/';
        str[row+0][col+1] = 'o';
        str[row+1][col+1] = '|';
        str[row+0][col+2] = '_';
        str[row+2][col+2] = '}';
    }
}

/* 
    Function:  addCrate
     Purpose:  adds crate to string array at position pos
      in/out:  string array with crate added
          in:  Crate to be added
*/
void addCrate(char **str, CrateType crate) {
    int row = crate.pos.row;
    int col = crate.pos.col;
    for(int i = 0; i< 2; ++i) {
        str[row+0][col+1] = '-';
        str[row+0][col+2] = '-';
        str[row+0][col+3] = '-';
        str[row+3][col+1] = '-';
        str[row+3][col+2] = '-';
        str[row+3][col+3] = '-';
        str[row+1][col+0] = '|';
        str[row+2][col+0] = '|';
        str[row+1][col+4] = '|';
        str[row+2][col+4] = '|';
        if(crate.crate == FILLED) {
            str[row+1][col+1] = '#';
            str[row+1][col+2] = '#';
            str[row+1][col+3] = '#';
            str[row+2][col+1] = '#';
            str[row+2][col+2] = '#';
            str[row+2][col+3] = '#';
            break;
        }
        row-=CRATE_HEIGHT-1; 
    }    
}

/* 
    Function:  addScore
     Purpose:  adds score to string array
      in/out:  string array with score added
          in:  score 
          in:  high score
*/
void addScore(char **str, long int score, long int highScore) {
    char *scoreStr = "SCORE: ";
    int len = strlen(scoreStr);
    for(int i = 0; i<len; ++i) {
        str[0][WIDTH-(len+10) - 1 + i] = scoreStr[i];
    }

    char scoreNum[11];
    sprintf(scoreNum, "%10ld", score);
    for(int i = 0; i<10; ++i) {
        str[0][WIDTH - 10 -1+ i] = scoreNum[i];
    }

    char *highScoreStr = "HIGHEST SCORE: ";
    len = strlen(highScoreStr);
    for(int i = 0; i<len; ++i) {
        str[0][i] = highScoreStr[i];
    }

    char highScoreNum[11];
    sprintf(highScoreNum, "%10ld", highScore);
    for(int i = 0; i<10; ++i) {
        str[0][len + i] = highScoreNum[i];
    }
}

/* 
    Function:  gameOverString
     Purpose:  add "TYPE 'q' to QUIT, PRESS ANY KEY TO START AGAIN." to string array
      in/out:  string array with game over string added
*/
void gameOverString(char **str) {
    char *gameOver = "TYPE 'q' to QUIT, PRESS ANY KEY TO START AGAIN.";
    int len = strlen(gameOver);
    int start = (WIDTH-len)/2;
    for(int i = 0; i<len; ++i) {
        str[HEIGHT/2][start+i] = gameOver[i];
    }
}

/*  Function:  initGround
     Purpose:  initialize the ground string
         out:  string array with ground added     
*/
void initGround(char ***ground) {
    (*ground) = malloc(GROUND_ROW * sizeof(char*));
    for(int i = 0; i<GROUND_ROW; ++i) {
        (*ground)[i] = malloc(WIDTH * sizeof(char));
    }
    for(int i = 0; i<WIDTH-1; ++i) {
        (*ground)[0][i] = '-';
        (*ground)[1][i] = ' ';
        (*ground)[2][i] = ' ';
        (*ground)[3][i] = ' ';
    }
    (*ground)[0][WIDTH-1] = '\0';
    (*ground)[1][WIDTH-1] = '\0';
    (*ground)[2][WIDTH-1] = '\0';
    (*ground)[3][WIDTH-1] = '\0';
}

/* 
    Function: initGameString
     Purpose:  initialize string array
         out:  string array    
*/
void initGameString(char ***str) {
    (*str) = calloc(HEIGHT, sizeof(char*));

    char spaces[WIDTH];
    for(int i = 0; i<WIDTH-1; ++i) {
        spaces[i] = ' ';
    }
    spaces[WIDTH-1] = '\0';

    for(int i = 0; i<HEIGHT; ++i) {
        (*str)[i] = malloc(sizeof(char[WIDTH]));
        strcpy((*str)[i], spaces);
    }
}

/* 
    Function:  freeGameString
     Purpose:  free string array
          in:  string array
*/
void freeGameString(char ***str) {
    for(int i = 0; i<HEIGHT; ++i) {
        free((*str)[i]);
    }
    free(*str);
}

/* 
    Function:  freeGround
     Purpose:  free ground string
          in:  ground string
*/
void freeGround(char ***ground) {
    for(int i = 0; i<GROUND_ROW; ++i) {
        free((*ground)[i]);
    }
    free(*ground);
}