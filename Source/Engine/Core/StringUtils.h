#pragma once
#include <string>

namespace nc {
	//static int count;
	static std::string ToUpper(std::string str);
	static std::string ToLower(std::string str);
	static bool IsEqualIgnoreCase(std::string str1, std::string str2);
	static std::string CreateUnique(std::string str);
};