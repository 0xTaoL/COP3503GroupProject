#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <stdexcept>

class encryptor {
	const std::string key;
	
public:
	encryptor(const std::string& key);
	void encrypt_file(const std::string&) const;
	void decrypt_file(const std::string&) const;
	bool validate_key(const std::string&) const;
	std::string import_file(const std::string&) const;
	void export_file(const std::string&, std::string&) const;
};

#endif
