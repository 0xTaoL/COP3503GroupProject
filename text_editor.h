#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <string>
#include <vector>
#include <ncurses.h>

class text_editor {
	const std::string temp_file, save_file;
	std::vector<std::string>* buffer;

	void read_file();
	bool command_prompt();
	void print_help() const;
	void save();
	void getstring(WINDOW*, const std::string&, std::string&);
	void print_buffer(WINDOW*, size_t, size_t);
	
public:
	text_editor(const std::string& save_file);
	~text_editor();
	void run_text_editor();
};

#endif
