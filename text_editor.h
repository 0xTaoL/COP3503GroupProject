#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <string>
#include <ncurses.h>

class text_editor {
	const std::string temp_file, save_file;
	std::string buffer;

	void read_file();
	void write_file() const;
	bool command_prompt();
	void print_help() const;
	void save();
	
public:
	text_editor(const std::string& save_file);
	~text_editor();
	void run_text_editor();
};

#endif
