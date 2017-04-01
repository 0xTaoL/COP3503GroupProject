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
	string buffer = "";

	initscr();

	cbreak();
	noecho();
	keypad(stdscr, true);

	int ch = 0;
	while (ch = getch()) {
		unsigned int x, y;
		getyx(stdscr, y, x);

		//check if esc then go to command screen
		if (ch == 27 && command_prompt()) {
			break;
		}
		else {
			switch (ch) {
			case KEY_UP:
				--y;
				break;
			case KEY_RIGHT:
				++x;
				break;
			case KEY_DOWN:
				++y;
				break;
			case KEY_LEFT:
				--x;
				break;
			}
		}

		move(y, x);
		refresh();
	}

	endwin();
}

bool text_editor::command_prompt() {
	unsigned int x_max, y_max;
	getmaxyx(stdscr, y_max, x_max);

	WINDOW* p_cmd = newwin(1, x_max - 1, y_max - 1, 0);
	keypad(p_cmd, true);

	char ch = 0;
	string command = "";
	while ((ch = wgetch(p_cmd)) != '\n') {
		if (ch == 8 || ch == 127 || ch == 7) {
			if (command.length() > 0) {
				command.pop_back();
			}
		}
		else {
			command += ch;
		}

		wclear(p_cmd);
		waddstr(p_cmd, command.c_str());
		wrefresh(p_cmd);
	}
	
	wclear(p_cmd);
	wrefresh(p_cmd);
	delwin(p_cmd);
	
	if (command.compare("quit") == 0) {
		return true;
	}
	else {
		return false;
	}
}

void text_editor::print_help() const {
	//TODO
}
