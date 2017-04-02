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

	//start screen output
	refresh();

	//create top bar window
	unsigned int x_max, y_max;
	getmaxyx(stdscr, y_max, x_max);
	string message = "tedit: " + save_file;
	WINDOW* top_bar = newwin(1, x_max - 1, 0, 0);
	mvwprintw(top_bar, 0, (x_max - message.length()) / 2, "%s", message.c_str());
	wrefresh(top_bar);

	//set start position to y=1 to not overlap
	move(1, 0);

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
				if (y > 1)
					--y;
				break;
			case KEY_RIGHT:
				++x;
				break;
			case KEY_DOWN:
				if (y < y_max - 2)
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

	delwin(top_bar);
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
