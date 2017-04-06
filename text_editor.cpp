#include <ncurses.h>
#include <sys/stat.h>
#include "encryption.h"
#include "text_editor.h"

using namespace std;

text_editor::text_editor(const string& save_file):
		save_file(save_file) {
	initscr();

	cbreak();
	noecho();
	keypad(stdscr, true);

	//start screen output
	refresh();
	
	buffer = "";
}

text_editor::~text_editor() {
	endwin();
}

void text_editor::read_file() {
	struct stat st_buff;
	
	if (stat(save_file.c_str(), &st_buff) == 0) {
		unsigned int x_max, y_max;
		getmaxyx(stdscr, y_max, x_max);

		WINDOW* prompt = newwin(1, x_max - 1, y_max - 1, 0);
		keypad(prompt, true);

		char ch = 0;
		string command = "Enter password to file: ";
		string key = "";
		
		waddstr(prompt, command.c_str());
		waddstr(prompt, key.c_str());
		wrefresh(prompt);
		
		while ((ch = wgetch(prompt)) != '\n') {
			if (ch == 8 || ch == 127 || ch == 7) {
				if (key.length() > 0) {
					key.pop_back();
				}
			}
			else {
				key += ch;
			}

			wclear(prompt);
			waddstr(prompt, command.c_str());
			waddstr(prompt, key.c_str());
			wrefresh(prompt);
		}
		
		wclear(prompt);
		wrefresh(prompt);
		delwin(prompt);
		
		encryptor* ifile = new encryptor(key);
		buffer = ifile->import_file(save_file);
	}
	else {
		buffer = "";
	}
}

void text_editor::write_file() const {
    
}

void text_editor::run_text_editor() {
	string command = "";

	//create top bar window
	unsigned int x_max, y_max;
	getmaxyx(stdscr, y_max, x_max);
	string message = "tedit: " + save_file;
	WINDOW* top_bar = newwin(1, x_max - 1, 0, 0);
	mvwprintw(top_bar, 0, (x_max - message.length()) / 2, "%s", message.c_str());
	wrefresh(top_bar);

	//create text window
	WINDOW* text_window = newwin(y_max - 2, x_max - 1, 1, 0);
	
	//load file into buffer
	read_file();
	wclear(text_window);
	mvwaddstr(text_window, 0, 0, buffer.c_str());
	wmove(text_window, 0, 0);
	wrefresh(text_window);

	int ch = 0;
	while (ch = getch()) {
		unsigned int x, y;
		size_t buffer_size = buffer.length();
		getyx(text_window, y, x);
		getmaxyx(text_window, y_max, x_max);
		size_t buffer_pos = y * x_max + x;

		//check if esc then go to command screen
		if (ch == 27 && command_prompt()) {
			break;
		}
		else {
			switch (ch) {
			case KEY_UP:
				if (y > 0)
					--y;
				break;
			case KEY_RIGHT:
				if (buffer_pos + 1 < buffer_size)
					++x;
				break;
			case KEY_DOWN:
				if (y < y_max && buffer_pos + x_max < buffer_size - 1)
					++y;
				break;
			case KEY_LEFT:
				if (x > 0) 
					--x;
				break;
			default:
				if (ch == 8 || ch == 127 || ch == 7 || ch == KEY_BACKSPACE) {
					buffer.pop_back();
				}
				else {
					buffer += ch;
				}
			}
		}

		wclear(text_window);
		mvwaddstr(text_window, 0, 0, buffer.c_str());
		wmove(text_window, y, x);
		wrefresh(text_window);
	}

	delwin(top_bar);
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
