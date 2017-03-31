#include <ncurses.h>
#include "display.h"

using namespace std;

void display_editor_mode(string file_name) {
    initscr();
    
    cbreak();
    noecho();
    keypad(stdscr, true);

    int height, width;
    getmaxyx(stdscr, height, width);



    endwin();
}

void prompt_command() {

}
