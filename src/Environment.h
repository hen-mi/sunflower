#pragma once 
#include <any>
#include <unordered_map>
#include <string>
#include <Token.h>
#include <RuntimeError.h>
#include <memory>
namespace Sunflower 
{
	class Environment
	{
	public:
		Environment();
		explicit Environment(std::shared_ptr<Environment> enclosing);
		~Environment() = default;

		std::any getValue(const Token& name);
		void define(const std::string& name, const std::any& value);
		void assign(const Token& name, const std::any& value);
		
	private:
		std::shared_ptr<Environment> mEnclosing;
		std::unordered_map<std::string, std::any> mNamedValues;
		
	};
}