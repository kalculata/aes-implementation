#pragma once

class AES {
public:
	static std::string aes128(std::string key, std::string message);
	static std::string aes192(std::string key, std::string message);
	static std::string aes256(std::string key, std::string message);
};