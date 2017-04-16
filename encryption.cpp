#include <string>
#include <vector>
#include <fstream>
#include "encryption.h"

using namespace std;

/*!
 * Constructor
 * @param key - password used to encrypt and decrypt
 */
encryptor::encryptor(const string& key):
		key(key) {};

/*!
 * Compares given password with password used to encrypt
 * @param user_key - password to verify
 * @return - true if passwords match, otherwise false
 */
bool encryptor::validate_key(const string& user_key) const {
	return (!user_key.compare(key)) ? true : false;

}

/*!
 * Encrypts file
 * @param file_name - name of file to be encrypted
 */
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

/*!
 * Decrypts file
 * @param file_name - name of file to be decrypted
 */
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

/*!
 * Imports file
 * @param file_name - name of file to be imported
 * @param buffer - where file is read into
 */
void encryptor::import_file(const string& file_name, vector<string>* buffer) const {
	fstream file(file_name, ios::binary | ios::in);
	buffer->push_back("");
	
	if (!file.is_open()) {
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
	
	size_t line = 0;
	char ch;
	for (size_t i = 0; file.good(); ++i) {
		ch = file.get() ^ key[i % key.length()];
		
		if (ch == '\n') {
			++line;
			buffer->push_back("");
		}
		else {
			buffer->at(line) += ch;
		}
	}
	buffer->back().pop_back();
	
	if (file.bad()) {
		file.close();
		throw runtime_error("IO Error");
	}
	
	file.close();
}

/*!
 * Exports file
 * @param filename - name of file to be exported
 * @param data - file contents
 */
void encryptor::export_file(const string& filename, vector<string>* data) const {
	fstream file(filename, ios::binary | ios::trunc | ios::out);
	
	if (!file.is_open()) {
		throw invalid_argument("File does not exist");
	}
	
	for (size_t i = 0; i < key.length(); ++i) {
		file.put(key[i] >> 1);
	}
	
	file.put('\n');
	
	size_t count = 0;
	for (size_t i = 0; i < data->size(); ++i) {
		for (size_t j = 0; j < data->at(i).length(); ++j) {
			file.put(data->at(i).at(j) ^ key[count % key.length()]);
			
			++count;
		}
		
		if (i != data->size() - 1 || data->at(i).length() != 0) {
			file.put('\n' ^ key[count % key.length()]);
			++count;
		}
	}
	
	file.close();
}
