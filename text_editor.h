#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <string>

class text_editor {
	const std::string temp_file, save_file;
	bool write_mode;

        void read_file();
        void write_file();
        bool command_prompt();
	
public:
	text_editor(const std::string& save_file);
	void issue_command();
	void run_text_editor();
	void print_help();
	void save();
};

#endif
