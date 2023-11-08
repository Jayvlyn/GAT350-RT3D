#include "StringUtils.h"

namespace nc
{
	std::string nc::StringUtils::ToUpper(const std::string& input)
	{
		std::string upper = input;
		for (char& c : upper) c = toupper(c);
		return upper;
	}

	std::string nc::StringUtils::ToLower(const std::string& input)
	{
		std::string lower = input;
		for (char& c : lower) c = tolower(c);
		return lower;
	}

	bool nc::StringUtils::isEqualIgnoreCase(const std::string& str1, const std::string& str2)
	{
		return (ToLower(str1) == ToLower(str2));
	}

	std::string nc::StringUtils::CreateUnique(const std::string& input)
	{
		static uint32_t unique = 0;

		return input + std::to_string(unique++);
	}
}