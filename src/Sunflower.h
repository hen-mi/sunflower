#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Token.h>

namespace Sunflower
{
	std::string readFromFile(const std::string& filePath);
	void printSymbolsTable();

	inline std::vector<Token> SymbolsTable;

}