#include <string>
#include <stdexcept>
#include <fstream>
#include <cstdio>
#include "encryption.h"

using namespace std;

encryptor::encryptor(const string& key):
		key(key) {};

bool encryptor::validate_key(const string& user_key) const {
	return (!user_key.compare(key)) ? true : false;
}

void encryptor::encrypt_file(const string& file_name) const {
	ifstream file(file_name, ios::binary | ios::in);
        string tmp_name = file_name + ".tmp";
        ofstream tmp_file(tmp_name, ios::binary | ios::out);
	
	if (!file.is_open()) {
		throw invalid_argument("File does not exist");
	}

        for (size_t i = 0; i < key.length(); ++i) {
                tmp_file.put(key[i] >> 1);
        }
        tmp_file.put('\n');

        size_t count = 0;
	while (file.good()) {
                tmp_file.put(file.get() ^ key[count % key.length()]);
                ++count;
        }
        
	file.close();
        tmp_file.close();
        remove(file_name.c_str());
        rename(tmp_name.c_str(), file_name.c_str());
}

void encryptor::decrypt_file(const string& file_name) const {
	ifstream file(file_name, ios::binary | ios::in);
        string tmp_name = file_name + ".tmp";
        ofstream tmp_file(tmp_name, ios::binary | ios::out);

        if (!file.is_open()) {
                throw invalid_argument("File does not exist");
        }

        char test_key[256];
        file.getline(test_key, 256, '\n');
        for (size_t i = 0; i < key.length(); ++i) {
                if ((key[i] >> 1) != test_key[i]) {
                        throw invalid_argument("Incorrect Password");
                }
        }

        size_t count = 0;
        while (file.good()) {
                tmp_file.put(file.get() ^ key[count % key.length()]);
                ++count;
        }

        file.close();
        tmp_file.close();
        remove(file_name.c_str());
        rename(tmp_name.c_str(), file_name.c_str());
}
