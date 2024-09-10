#pragma once

#include <string>
enum class TokenType 
{
	
};

struct Token 
{
	TokenType		    t;
	std::string		value;
	uint32_t		 line;

} using Token;
