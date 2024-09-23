#pragma once
#include <string>

enum class TokenType 
{
	//SINGLE CHARACTER
	LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_BRACE,

	//SINGLE OR DOUBLE VARIANT 
	EQUAL, EQUAL_EQUAL,

	//LITERALS
	IDENTIFIER, STRING,

	//KEYWOARDS

	FN, 

   _EOF
};

struct Token
{

	TokenType		    tokentype;
	std::string			   lexema;
	size_t					 line;

};
