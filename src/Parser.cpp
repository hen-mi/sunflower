#include <Parser.h>

namespace Sunflower
{

	Parser::Parser(std::vector<Token>& Tokens) : mTokens{ std::move(Tokens) }, mCurrentToken{ 0 }, mStatements{} {}

	std::vector<std::unique_ptr<Stmt>> Parser::parse()
	{
		while(!isAtEnd()) 
		{
			mStatements.push_back(declaration());
		}

		return std::move(mStatements);
	};

	std::unique_ptr<Stmt> Parser::declaration()
	{
		try 
		{
			if (match({ TokenType::LET})) return varDeclaration();

			return statement();
		}
		catch (ParseError error) 
		{
			synchronize();

			return nullptr;
		};
		
	}
	std::unique_ptr<Stmt> Parser::block() 
	{
		std::vector<std::unique_ptr<Stmt>> statements;

		while(!check(TokenType::RIGHT_CBRACE) && !isAtEnd()) 
		{
			statements.push_back(declaration());
		}

		consume(TokenType::RIGHT_CBRACE, "Expect '}' after block");

		return std::make_unique<BlockStmt>(std::move(statements));
	}
	std::unique_ptr<Stmt> Parser::varDeclaration() 
	{
		auto name = consume(TokenType::IDENTIFIER, "Expect variable name");

		std::unique_ptr<Expr> initializer;


		if(match({TokenType::EQUAL})) 
		{
			initializer = expression();
		}

		consume(TokenType::SEMICOLON, "Expect ; after variable declaration");

		return std::make_unique<VarStmt>(name, std::move(initializer));
	}
	std::unique_ptr<Stmt> Parser::statement() 
	{
		if (match({ TokenType::POUT })) return statement();

		if (match({ TokenType::LEFT_CBRACE })) return block();

		return expressionStmt();
	}
	
	std::unique_ptr<Stmt> Parser::printStmt() 
	{
		auto value = expression();

		consume(TokenType::NEWLINE, "Expected new line");

		return std::make_unique<PrintStmt>(std::move(value));
	}
	std::unique_ptr<Stmt> Parser::expressionStmt() 
	{
		auto expr = expression();

		consume(TokenType::SEMICOLON, "Expect ;");

		return std::make_unique<ExprStmt>(std::move(expr));
	}

	std::unique_ptr<Expr> Parser::assignment()
	{
		auto expr = equality();

		if (match({ TokenType::EQUAL }))
		{
			Token equals = previous();
			auto value = assignment();


			if (auto* varExpr = dynamic_cast<VarExpr*>(expr.get()); varExpr) //instanceof hack -> checks if expr is a VarExpr
			{
				return std::make_unique<AssignExpr>(varExpr->getName(), std::move(value));
			}

			error(equals, "Invalid assignment target");
		}

		return expr;
	}

	std::unique_ptr<Expr> Parser::expression() 
	{
		return assignment();
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

		if (match({ TokenType::IDENTIFIER}))
		{
			return std::make_unique<VarExpr>(previous());
		}

		if(match({TokenType::NUMBER})) 
		{
			return std::make_unique<Literal>(std::stod(previous().lexema));
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
