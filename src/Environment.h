#pragma once 
#include <any>
#include <unordered_map>
#include <string>
#include <Token.h>
#include <RuntimeError.h>

namespace Sunflower 
{
	class Environment
	{
	public:
		Environment();
		Environment(Environment* enclosing);
		~Environment() = default;

		std::any getValue(const Token& name);
		void define(const std::string& name, const std::any& value);
		void assign(const Token& name, const std::any& value);
		
	private:
		Environment* mEnclosing;
		std::unordered_map<std::string, std::any> mNamedValues;
		
	};
}