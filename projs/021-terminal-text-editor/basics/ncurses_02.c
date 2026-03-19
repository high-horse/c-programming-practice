#include <curses.h>
#include <stdlib.h>
#include <ncurses.h>

int main() {
    int x, y;
    
    initscr();
    clear();
    
    getmaxyx(stdscr, y, x);
    printw("x = %d\ny = %d\n", x, y);
    
    refresh();
    
    y = 5;
    x = 10;
    
    move(y, x);
    printw(" over here : ");
    getch();
    endwin();
    
    return EXIT_SUCCESS;
}