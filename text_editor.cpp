#include <iostream>
#include <string>
#include "text_editor.h"

using namespace std;

text_editor::text_editor(const string& save_file):
		save_file(save_file) {
	write_mode = false;
}

void text_editor::run_text_editor() {
	string command = "";
	
	while (command.compare("quit")) {
		if (write_mode == false) {
			cout << "Enter a command: ";
			cin >> command;
			
			if (!command.compare("i")) {
				write_mode = true;
			} 
			else if (!command.compare("help")) {
				print_help();
			}
		} 
		else {
			//char input = (char)NULL;
		}
	}
}

void text_editor::print_help() {
	cout << "Commands:\n"
		 << "i    -- start input\n"
		 << "help -- this screen" << endl;
}
