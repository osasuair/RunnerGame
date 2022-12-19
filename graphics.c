#include "defs.h"

/*
  Function:  initNcurses
   Purpose:  initializes ncurses library
*/
WINDOW *initNcurses() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	WINDOW *win = newwin(HEIGHT+2, WIDTH+1, START_Y, START_X);
	refresh();
	
	box(win, 0, 0);
	char *intro = "Press any key to start...";
	mvwprintw(win, HEIGHT/2, (WIDTH-strlen(intro))/2, "%s", intro);
	wrefresh(win);
	wgetch(win);
	return win;
}

/*
  Function:  scrPrt
   Purpose:  prints out a string to a given row and col on the screen
		in:  string to be printed out
		in:  row where the string is printed out
		in:  column where the string is printed out
*/
void scrPrt(WINDOW *win, char **str) {
	box(win, 0, 0);
	for(int i = 0; i < HEIGHT; ++i) {
		mvwprintw(win, i+1, 1, "%s", str[i]);
  	}
	wrefresh(win);
}

/*
  Function:  clearWindow
   Purpose:  clears the window
		in:  window to be cleared
*/
void clearWindow(WINDOW *win) {
	wclear(win); 
	box(win, 0, 0);
}