#pragma once
#include <string>
#include <unordered_map>
enum class TokenType 
{
	//SINGLE CHARACTER
	LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_BRACE, RIGHT_BRACE,

	//SINGLE OR DOUBLE VARIANT 
	EQUAL, EQUAL_EQUAL,

	//LITERALS
	IDENTIFIER, STRING,

	//KEYWOARDS

	FN, POUT,

   _EOF
};

inline std::unordered_map<std::string, TokenType> keyword{ {"fn", TokenType::FN} };

struct Token
{

	TokenType  tokentype;
	std::string	  lexema;
	size_t		    line;

};
