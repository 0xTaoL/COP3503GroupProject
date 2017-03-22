#include <string>
#include <stdexcept>
#include <fstream>
#include "encryption.h"

using namespace std;

encryptor::encryptor(const string& key):
		key(key) {};

bool encryptor::validate_key(const string& user_key) const {
	return (!user_key.compare(key)) ? true : false;
}

void encryptor::encrypt_file(const string& file_name) const {
	fstream file(file_name, ios::binary | ios::in);
	
	if (!file.is_open()) {
		file.close();
		throw invalid_argument("File does not exist");
	}
	
	string contents;
	while (file.good()) {
		contents += file.get();
	}
	
	if (file.bad()) {
		file.close();
		throw runtime_error("IO Error");
	}
	
	file.close();
	file.open(file_name, ios::trunc | ios::out | ios::binary);
	
	for (size_t i = 0; i < key.length(); ++i) {
		file.put(key[i] >> 1);
	}
	
	file.put('\n');
	
	for (size_t i = 0; i < contents.length() - 1; ++i) {
		file.put(contents[i] ^ key[i % key.length()]);
	}
	
	file.close();
}

void encryptor::decrypt_file(const string& file_name) const {
	fstream file(file_name, ios::binary | ios::in);
	
	if (!file.is_open()) {
		file.close();
		throw invalid_argument("File does not exist");
	}
	
	string hashed_key;
	getline(file, hashed_key);
	
	if (key.length() == hashed_key.length()) {
		for (size_t i = 0; i < key.length(); ++i) {
			if ((key[i] >> 1) != hashed_key[i]) {
				throw runtime_error("Password is invalid");
			}
		}
	}
	else {
		throw runtime_error("Password is invalid");
	}
	
	string contents;
	while (file.good()) {
		contents += file.get();
	}
	
	if (file.bad()) {
		file.close();
		throw runtime_error("IO Error");
	}
	
	file.close();
	file.open(file_name, ios::trunc | ios::out | ios::binary);
	
	for (size_t i = 0; i < contents.length() - 1; ++i) {
		file.put(contents[i] ^ key[i % key.length()]);
	}
	
	file.close();
}
