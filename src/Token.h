#pragma once
#include <string>

enum class TokenType 
{
	
};

struct Token final
{
	TokenType		    t;
	std::string		value;
	uint32_t		 line;

};
