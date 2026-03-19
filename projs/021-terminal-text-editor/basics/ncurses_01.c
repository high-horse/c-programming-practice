#include <stdlib.h>
#include <ncurses.h>

int main() {
    char letter;
    initscr();
    printw("press any key\t:");
    refresh();
    
    letter = getch();
    getch();
    
    printw("you pressed \t'%c'", letter);
    refresh();
    
    getch();
    endwin();
    
    return EXIT_SUCCESS;
}