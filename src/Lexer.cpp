#include <Lexer.h>

Lexer::Lexer(const std::string& source) 
	: mSourceCode{ source }, mPosition{ 0 }, mByte{source[mPosition]} {}

std::vector<Token> Lexer::tokenize() 
{

};

