#pragma once
#include <Token.h>
#include <string.h>
#include <stdexcept>

namespace Sunflower 
{ 
	class RuntimeError : public std::runtime_error
	{
	public:
		RuntimeError(const Token& token, const std::string& message) :
			std::runtime_error(message), token(token)
		{}

		const Token& getToken() const { return token; }

	private:
		Token token;
	};
}