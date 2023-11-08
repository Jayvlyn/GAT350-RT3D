#pragma once
#include <string>

namespace nc
{
	class StringUtils
	{
	public:
		std::string ToUpper(const std::string& input);
		std::string ToLower(const std::string& input);
		bool isEqualIgnoreCase(const std::string& str1, const std::string& str2);
		std::string CreateUnique(const std::string& input);

	};
}