#include <sys/stat.h>
#include "encryption.h"
#include "text_editor.h"

using namespace std;

/*!
 * Constructor
 * @param save_file - name of file
 */
text_editor::text_editor(const string& save_file):
		save_file(save_file) {
	initscr();

	cbreak();
	noecho();
	keypad(stdscr, true);

	//start screen output
	refresh();
	
	buffer = new vector<string>();
}

text_editor::~text_editor() {
	delete buffer;
	endwin();
}

/*!
 * Loads file into buffer
 */
void text_editor::read_file() {
	struct stat st_buff;
	
	if (stat(save_file.c_str(), &st_buff) == 0) {
		unsigned int x_max, y_max;
		getmaxyx(stdscr, y_max, x_max);

		WINDOW* prompt = newwin(1, x_max - 1, y_max - 1, 0);
		keypad(prompt, true);

		string command = "Enter password to file: ";
		string key = "";
		
		getstring(prompt, command, key);
		
		wclear(prompt);
		wrefresh(prompt);
		delwin(prompt);
		
		encryptor* ifile = new encryptor(key);
		ifile->import_file(save_file, buffer);
		delete ifile;
	}
	else {
		buffer->push_back("");
	}
}

/*!
 * Initializes text editor screen and handles key input
 */
void text_editor::run_text_editor() {
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
	print_buffer(text_window, 0, y_max);
	wmove(text_window, 0, 0);
	wrefresh(text_window);

	int ch = 0;
	size_t p_lines = 0, x = 0, y = 0;
	while (ch = getch()) {
		size_t b_y = y + p_lines;
		size_t buffer_size = buffer->size();
		getmaxyx(text_window, y_max, x_max);

		//check if esc then go to command screen
		if (ch == 27) {
			if (command_prompt()) break;
		}
		else {
			switch (ch) {
			case KEY_RESIZE:
				break;
			case KEY_UP:
				if (x >= x_max) {
					x -= x_max;
				}
				else if (b_y > 0) {
					--y;
					if (x > buffer->at(b_y - 1).length()) {
						x = buffer->at(b_y - 1).length();
					}
				}
				break;
			case KEY_RIGHT:
				if (x < buffer->at(y).length()) {
					++x;
				}
				break;
			case KEY_DOWN:
				if (x + x_max <= buffer->at(b_y).length()) {
					x += x_max;
				}
				else if (b_y < buffer_size - 1) {
					if (x > buffer->at(b_y + 1).length()) {
						x = buffer->at(b_y + 1).length();
					}
					
					++y;
					++b_y;
				}
				break;
			case KEY_LEFT:
				if (x > 0) {
					--x;
				}
				break;
			default:
				if (ch == 8 || ch == 127 || ch == 7 
						|| ch == KEY_BACKSPACE) {
					if (x > 0) {
						buffer->at(b_y).erase(x - 1, 1);
						--x;
					}
					else if (x == 0) {
						if (b_y > 0) {
							x = buffer->at(b_y - 1).length();
							buffer->at(b_y - 1) += buffer->at(b_y);
							buffer->erase(buffer->begin() + b_y);
							--y;
						}
					}
				}
				else if (ch == '\n') {
					if (x == buffer->at(b_y).length()) {
						buffer->insert(buffer->begin() + b_y + 1, "");
					}
					else {
						string tmp;
						tmp = buffer->at(b_y).substr(x, string::npos);
						buffer->at(b_y).erase(x, string::npos);
						buffer->insert(buffer->begin() + b_y + 1, tmp);
					}
					
					++y;
					++b_y;
					x = 0;
				}
				else {
					buffer->at(b_y).insert(x, 1, ch);
					++x;
				}
			}
		}
		
		if (y == y_max) {
			--y;
			++p_lines;
		}
		else if (y > y_max) {
			y = 0;
			--p_lines;
		}

		size_t e_lines = get_extra_lines(b_y, x_max);
		
		print_buffer(text_window, p_lines, p_lines + y_max);
		wmove(text_window, y + x/x_max + e_lines, x % x_max);
		wrefresh(text_window);
	}

	delwin(top_bar);
}

/*!
 * Prompts user for command input
 * @return - true if program should terminate, otherwise false
 */
bool text_editor::command_prompt() {
	unsigned int x_max, y_max;
	getmaxyx(stdscr, y_max, x_max);

	WINDOW* p_cmd = newwin(1, x_max - 1, y_max - 1, 0);
	keypad(p_cmd, true);

	string command;
	getstring(p_cmd, ":", command);
	
	if (command.compare("quit") == 0) {
		delwin(p_cmd);
		return true;
	}
	else if (command.compare("save") == 0) {
		string save, password;
		
		if (save_file.compare(".tmp.sav") == 0) {
			getstring(p_cmd, "Save name: ", save);
			getstring(p_cmd, "Password: ", password);
		}
		else {
			save = save_file;
			getstring(p_cmd, "Password: ", password);
		}
		
		encryptor* export_data = new encryptor(password);
		
		try {
			export_data->export_file(save, buffer);
		}
		catch (invalid_argument& e) {
			waddstr(p_cmd, "Error in file output. Press any key to continue.");
			wrefresh(p_cmd);
			getch(); 
			
			wclear(p_cmd);
			wrefresh(p_cmd);
			delwin(p_cmd);
			delete export_data;
			return false;
		}
		
		delwin(p_cmd);
		delete export_data;
		return true;
	}
	else if (command.compare("help") == 0) {
		print_help();
		delwin(p_cmd);
		return false;
	}
	else {
		delwin(p_cmd);
		return false;
	}
}

/*!
 * Prompts for input on specified window and sets input to response
 * @param win - window to prompt
 * @param prompt - description string of input
 * @param input - variable to store response in
 */
void text_editor::getstring(WINDOW* win, const string& prompt, string& input) {
	wclear(win);
	waddstr(win, prompt.c_str());
	wrefresh(win);
	
	char ch;
	input = "";
	while ((ch = wgetch(win)) != '\n') {
		if (ch == 8 || ch == 127 || ch == 7) {
			if (input.length() > 0) {
				input.pop_back();
			}
		}
		else {
			input += ch;
		}

		wclear(win);
		waddstr(win, prompt.c_str());
		waddstr(win, input.c_str());
		wrefresh(win);
	}	
	
	wclear(win);
	wrefresh(win);
}

/*!
 * Prints buffer to window
 * @param win - window to transfer contents of buffer to
 * @param start - start index of contents to be displayed
 * @param end - end index of contents to be displayed
 */
void text_editor::print_buffer(WINDOW* win, size_t start, size_t end) {
	if (end > buffer->size()) {
		end = buffer->size();
	}
	if (start > buffer->size()) {
		start = buffer->size() - 1;
	}
	
	wclear(win);
	
	for (size_t i = start; i < end; ++i) {
		waddstr(win, (buffer->at(i) + '\n').c_str());
	}
	
	wrefresh(win);
}

size_t text_editor::get_extra_lines(size_t line, size_t x_max) {
	size_t e_lines = 0;
	
	for (size_t i = 0; i < line && i < buffer->size(); ++i) {
		e_lines += buffer->at(i).length() / x_max;
	}
	
	return e_lines;
}

/*!
 * Displays help dialog
 */
void text_editor::print_help() const {
	unsigned int x_max, y_max;
	getmaxyx(stdscr, y_max, x_max);

	WINDOW* help_win = newwin(6, 40, (y_max - 6) / 2, (x_max - 40) / 2);

	string message =	"Commands:\n"
						"help - display this text\n"
						"save - save the file\n"
						"quit - exit the program (does NOT save)\n\n"
						"Press any key to exit this dialog";

	waddstr(help_win, message.c_str());
	wrefresh(help_win);

	getch();
	wclear(help_win);
	wrefresh(help_win);

	delwin(help_win);
}
