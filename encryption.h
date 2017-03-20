#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

class encryptor {
	const std::string key;
	
public:
	encryptor(const std::string& key);
	void encrypt_file(const std::string& file_name) const;
	void encrypt_file(const std::string& file_name, 
			const std::string& output_name) const;
	void decrypt_file(const std::string& file_name) const;
	void decrypt_file(const std::string& file_name, 
			const std::string& output_name) const;
	bool validate_key(const std::string& user_key) const;
};

#endif
