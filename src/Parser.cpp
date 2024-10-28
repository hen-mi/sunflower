#include <Parser.h>

namespace Sunflower
{

	Parser::Parser(std::vector<Token>& Tokens) : mTokens{ Tokens }, mCurrentToken{ 0 } {}

	std::unique_ptr<Expr> Parser::parse()
	{
		try { return expression(); }
		catch (ParseError error) { return nullptr; }
	};

	std::unique_ptr<Expr> Parser::expression() 
	{
		return equality();
	};
	std::unique_ptr<Expr> Parser::equality() 
	{

		auto expr = comparison();

		while (match({ TokenType::NOT_EQUAL, TokenType::EQUAL_EQUAL }))
		{
			Token op = previous();
			auto right = comparison();
			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}

		return expr;
	};

	std::unique_ptr<Expr> Parser::comparison()
	{
		auto expr = term();

		while(match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) 
		{
			Token op = previous();
			auto right = term();

			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}

		return expr;
	}

	std::unique_ptr<Expr> Parser::term()
	{
		auto expr = factor();

		while (match({TokenType::MINUS, TokenType::PLUS}))
		{
			Token op = previous();
			auto right = factor();

			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}

		return expr;
	};

	std::unique_ptr<Expr> Parser::factor() 
	{
		auto expr = unary();

		while (match({ TokenType::SLASH, TokenType::STAR }))
		{
			Token op = previous();
			auto right = unary();

			expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
		}
		return expr;
	}
	std::unique_ptr<Expr> Parser::unary() 
	{

		if(match({TokenType::NOT, TokenType::MINUS})) 
		{
			Token op = previous();
			auto right = unary();
			
			return std::make_unique<UnaryExpr>(op, std::move(right));
		}

		return primary();
	}
	std::unique_ptr<Expr> Parser::primary() 
	{
		if (match({ TokenType::TRUE })) return std::make_unique<Literal>("true");
		if (match({ TokenType::FALSE })) return std::make_unique<Literal>("false");
		if (match({ TokenType::_NULL })) return std::make_unique<Literal>("null");

		if (match({ TokenType::NUMBER, TokenType::IDENTIFIER})) 
		{
			return std::make_unique<Literal>(previous().lexema);
		}

		if(match({TokenType::LEFT_PARENTHESIS})) 
		{
			auto expr = expression();

			consume(TokenType::RIGHT_PARENTHESIS, "Expect ')' after expression");

			return std::make_unique<GroupingExpr>(std::move(expr));
		}
		else throw error(peek(), "Expected expression.");
	}
	bool Parser::match(std::vector<TokenType> TokenTypes) 
	{
		for(auto& type : TokenTypes)
		{
			if(check(type)) 
			{
				Parser::advance();
				return true;
			}
		}

		return false;
	};

	Token Parser::peek() 
	{
		return mTokens[mCurrentToken];
	}

	bool Parser::isAtEnd() 
	{
		return peek().tokentype == TokenType::_EOF;

	}

	bool Parser::check(TokenType type) 
	{
		if (isAtEnd()) return false;

		return peek().tokentype == type;
	}

	Token Parser::advance() 
	{
		if (!isAtEnd()) ++mCurrentToken;

		return previous();
	}

	Token Parser::previous()
	{
		return mTokens[mCurrentToken-1];
	}

	Token Parser::consume(TokenType t, const std::string& message)
	{
		if (check(t)) return advance();

		throw error(peek(), message);
	}

	Parser::ParseError Parser::error(Token t, const std::string& message) 
	{
		
		Sunflower::error(t, message);


		throw ParseError{};
	}

	void Parser::synchronize()
	{
		advance();

		while (!isAtEnd())
		{
			if (previous().tokentype == TokenType::NEWLINE) return;
			switch (peek().tokentype)
			{
			case TokenType::FN:
			case TokenType::IF:
			case TokenType::WHILE:
			case TokenType::POUT:

			case TokenType::RETURN: return;

			default:
				break;
			}
		}

		advance();
	}
}
