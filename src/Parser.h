#pragma once 
#include <AST.h>

namespace Sunflower 
{
	class Parser
	{
	public:
		Parser(std::vector<Token>& Tokens);

	private:
		Token peek();
		Token advance();
		Token previous();

		std::unique_ptr<Expr> expression();
		std::unique_ptr<Expr> equality();
		std::unique_ptr<Expr> comparison();
		std::unique_ptr<Expr> term();

		bool match(std::vector<TokenType> &TokenTypes);
		bool check(TokenType type); 
		bool isAtEnd();


	private:

		const std::vector<Token> mTokens;
		size_t mCurrentToken;
	};
}