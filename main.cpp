#include <iostream>
#include <string>
#include <cstring>
#include "text_editor.h"
#include "encryption.h"

using namespace std;

void print_help(const char* const& prog_name) {
	cout << "Help: " << prog_name << "\n"
		 << "-d [file] decrypt the specified file\n"	
		 << "-e [file] encrypt the specified file\n"
		 << "-h show this help screen" << endl;
}

int main(int argc, char** argv) {
	string name;
	size_t option;
	
	if (argc == 1) {
		name = ".tmp.sav";
		option = 0;
	} else if (argc == 2) {
		if (!strcmp(argv[1], "-h")) {
			print_help(argv[0]);
			return 0;
		} else if (!strcmp(argv[1], "-e") || !strcmp(argv[1], "-d")) {
			cout << "Invalid usage" << endl;
			print_help(argv[0]);
			return 0;
		}
		
		name = argv[1];
		option = 0;
	} else if (argc == 3) {
		if (!strcmp(argv[1], "-e")) {
			option = 1;
			name = argv[2];
		} else if (!strcmp(argv[1], "-d")) {
			option = 2;
			name = argv[2];
		}
	} else {
		cout << "Invalid usage." << endl;
		print_help(argv[0]);
		return 1;
	}
	
	switch (option) {
	case 0: //text editor
	{
		text_editor* text = new text_editor(name);
		text->run_text_editor();
		
		delete text;
		break;
	}
	case 1: //encrypt
	{
		string key;
		
		cout << "Enter a password: ";
		cin >> key;
		
		encryptor* encrypt = new encryptor(key);
		
		try {
			encrypt->encrypt_file(name);
		} catch (exception& e) {
			cerr << "Error: " << e.what() << endl;
			
			delete encrypt;
			return -1;
		}
		
		delete encrypt;
		break;
	}
	case 2: //decrypt
	{
		string key;
		
		cout << "Enter the password: ";
		cin >> key;
		
		encryptor* decrypt = new encryptor(key);
		
		try {
			decrypt->decrypt_file(name);
		} catch (exception& e) {
			cerr << "Error: " << e.what() << endl;
			
			delete decrypt;
			return -1;
		}
		
		delete decrypt;
		break;
	}
	}
}
