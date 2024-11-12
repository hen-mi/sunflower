#pragma once 
#include <AST/Expressions.h>
#include <AST/Statements.h>

#include <stdexcept>
#include <Sunflower.h>

namespace Sunflower 
{
	class Parser
	{
	public:

		Parser(std::vector<Token>& Tokens);
		std::vector<std::unique_ptr<Stmt>> parse();

	private:
		
		Token peek();
		Token advance();
		Token previous();
		Token consume(TokenType t, const std::string& message);

		std::unique_ptr<Stmt> block();
		std::unique_ptr<Stmt> statement();
		std::unique_ptr<Stmt> printStmt();
		std::unique_ptr<Stmt> expressionStmt();
		std::unique_ptr<Stmt> declaration();
		std::unique_ptr<Stmt> varDeclaration();
		std::unique_ptr<Expr> assignment();
		std::unique_ptr<Expr> expression();
		std::unique_ptr<Expr> equality();
		std::unique_ptr<Expr> comparison();
		std::unique_ptr<Expr> term();
		std::unique_ptr<Expr> factor();
		std::unique_ptr<Expr> unary();
		std::unique_ptr<Expr> primary();

		bool match(std::vector<TokenType> TokenTypes);
		
		bool check(TokenType type); 
		bool isAtEnd();

		class ParseError : public std::runtime_error {
		public:
			ParseError() : std::runtime_error("") {}
		};

		ParseError error(Token t, const std::string& message);

		void synchronize();

	private:
		std::vector<std::unique_ptr<Stmt>> mStatements;
		const std::vector<Token> mTokens;
		size_t mCurrentToken;
	};
}