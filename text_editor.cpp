#include <iostream>
#include <ncurses.h>
#include "text_editor.h"

using namespace std;

text_editor::text_editor(const string& save_file):
		save_file(save_file) {
	write_mode = false;
}

void text_editor::read_file() const {
    //TODO
}

void text_editor::write_file() const {
    //TODO
}

void text_editor::run_text_editor() {
	string command = "";

        initscr();

        cbreak();
        noecho();
        keypad(stdscr, true);

        int ch = 0;
        while (ch = getch()) {
                if (ch == 27) {
                        //turn on no delay to check if esc or alt
                        nodelay(stdscr, true);

                        //check if esc then go to command screen
                        if (getch() == -1) {
                                if (command_prompt()) break;
                        }

                    //turn off nodelay to wait for next keypress
                        nodelay(stdscr, false);
                }
                else {
                        switch (ch) {
                                case KEY_UP:
                                    break;
                        }

        }

        endwin();
}

bool text_editor::command_prompt() {

}

void text_editor::print_help() const {
	cout << "Commands:\n"
		 << "i    -- start input\n"
		 << "help -- this screen" << endl;
}
