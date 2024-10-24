#pragma once
#include <string>
#include <unordered_map>
enum class TokenType 
{
	//SINGLE CHARACTER
	LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_CBRACE, RIGHT_CBRACE,
	COLON, LEFT_SQUAREBR, RIGHT_SQUAREBR, SLASH, STAR, PLUS, COMMA,
	//SINGLE OR DOUBLE VARIANT 
	EQUAL, EQUAL_EQUAL,
	MINUS, IF, 
	LESS, LESS_EQUAL,
	GREATER, GREATER_EQUAL,
	NOT, NOT_EQUAL,

	//LITERALS
	IDENTIFIER, STRING, NUMBER, _NULL,

	//KEYWORDS

	FN, POUT, TRUE, FALSE, WHILE,

   _EOF
};

inline std::unordered_map<std::string, TokenType> keyword
{
	{"fn", TokenType::FN},
	{"true", TokenType::TRUE},
	{"false", TokenType::FALSE},
	{"pout", TokenType::POUT},
	{"null", TokenType::_NULL},
	{"while", TokenType::WHILE}
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
	{TokenType::IF, "IF"},
	{TokenType::_EOF, "EOF"},
	{TokenType::MINUS, "MINUS"},
	{TokenType::EQUAL, "EQUAL"},
	{TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"}
};

struct Token
{

	TokenType  tokentype;
	std::string	  lexema;
	size_t		    line;

};
