#pragma once 
#include <AST.h>
#include <stdexcept>
#include <Sunflower.h>

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
		Token consume(TokenType t, const std::string& message);

		std::unique_ptr<Expr> expression();
		std::unique_ptr<Expr> equality();
		std::unique_ptr<Expr> comparison();
		std::unique_ptr<Expr> term();
		std::unique_ptr<Expr> factor();
		std::unique_ptr<Expr> unary();
		std::unique_ptr<Expr> primary();

		bool match(std::vector<TokenType> &TokenTypes);
		
		bool check(TokenType type); 
		bool isAtEnd();

		class ParseError : public std::runtime_error {
		public:
			ParseError() : std::runtime_error("") {}
		};

		ParseError error(Token t, const std::string& message);



	private:

		const std::vector<Token> mTokens;
		size_t mCurrentToken;
	};
}