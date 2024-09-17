#pragma once

#include <Token.h>
#include <string>
#include <vector>

class Lexer 
{
public:

	Lexer(const std::string& source);
	std::vector<Token> tokenize();


private:

	std::string		mSourceCode;
	size_t			mPosition;
	char			mByte;

	char nextChar();
	char peekChar();


};