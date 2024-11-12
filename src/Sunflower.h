#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Token.h>
#include <stack>
namespace Sunflower
{
	void error(Token token, const std::string& message);
	void report(size_t line, const std::string& where, const std::string& message);

	std::string readFromFile(const std::string& filePath);
	void printTokensTable(const std::vector<Token>& tokenTable);

}