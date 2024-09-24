#pragma once
#include <string>
#include <unordered_map>
enum class TokenType 
{
	//SINGLE CHARACTER
	LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_CBRACE, RIGHT_CBRACE,

	//SINGLE OR DOUBLE VARIANT 
	EQUAL, EQUAL_EQUAL,

	//LITERALS
	IDENTIFIER, STRING,

	//KEYWORDS

	FN, POUT,

   _EOF
};

inline std::unordered_map<std::string, TokenType> keyword
{
	{"fn", TokenType::FN},
	{"pout", TokenType::POUT}
};

inline std::unordered_map<TokenType, std::string> TokenTypeToString
{
	{TokenType::LEFT_PARENTHESIS, "LEFT_PARENTHESIS"},
	{TokenType::RIGHT_PARENTHESIS, "RIGHT_PARENTHESIS"},
	{TokenType::LEFT_CBRACE, "LEFT_CBRACE"},
	{TokenType::RIGHT_CBRACE, "RIGHT_CBRACE"},
	{TokenType::FN, "FN"},
	{TokenType::POUT, "POUT"},
	{TokenType::IDENTIFIER, "IDENTIFIER"},
	{TokenType::STRING, "STRING"},
	{TokenType::_EOF, "EOF"}
};

struct Token
{

	TokenType  tokentype;
	std::string	  lexema;
	size_t		    line;

};
