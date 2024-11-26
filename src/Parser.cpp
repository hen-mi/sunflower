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
			if (match({ TokenType::FN })) return function();
			return statement();
		}
		catch (ParseError error) 
		{
			synchronize();

			return nullptr;
		};
		
	}

	std::unique_ptr<Stmt> Parser::function() 
	{
		auto name = consume(TokenType::IDENTIFIER, "Expect function name");

		consume(TokenType::LEFT_PARENTHESIS, "Expect '(' after function name");

		std::vector<Token> parameters;

		if(!check(TokenType::RIGHT_PARENTHESIS)) 
		{
			do
			{
				if (parameters.size() >= 255)
				{
					error(peek(), "Can't have more than 255 arguments.");
				}
				parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name"));

			} while (match({ TokenType::COMMA }));
		}

		consume(TokenType::RIGHT_PARENTHESIS, "Expect ')' after parameters");

		consume(TokenType::LEFT_CBRACE, "Expect '{' before function body");

		auto body = block();

		return std::make_unique<FunctionStmt>(name, std::move(parameters), std::move(body));

	}

	std::vector<std::unique_ptr<Stmt>> Parser::block()
	{
		std::vector<std::unique_ptr<Stmt>> statements;

		while(!check(TokenType::RIGHT_CBRACE) && !isAtEnd()) 
		{
			statements.push_back(declaration());
		}

		consume(TokenType::RIGHT_CBRACE, "Expect '}' after block");

		return statements;
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
	{/*

		if (check(TokenType::LEFT_PARENTHESIS)) 
		{
			size_t temp = mCurrentToken;

			if (find({ TokenType::RIGHT_PARENTHESIS, TokenType::IF, TokenType::LEFT_CBRACE })) 
			{
				return ifStmt();
			}

			mCurrentToken = temp;
		}
*/	
		if (match({ TokenType::RETURN })) return returnStmt();
		if (match({ TokenType::WHILE })) return whileStmt();
		if (match({ TokenType::FOR })) return forStmt();
		if (match({ TokenType::IF })) return ifStmt();
		if (match({ TokenType::POUT })) return printStmt();

		if (match({ TokenType::LEFT_CBRACE })) return std::make_unique<BlockStmt>(block());

		return expressionStmt();
	}

	std::unique_ptr<Stmt> Parser::returnStmt() 
	{
		auto keyword = previous();

		std::unique_ptr<Expr> value = nullptr;

		if(!check(TokenType::SEMICOLON)) 
		{
			value = expression();
		}

		consume(TokenType::SEMICOLON, "Expect ';' after return statement value");

		return std::make_unique<ReturnStmt>(std::move(keyword), std::move(value));
	}

	std::unique_ptr<Stmt> Parser::whileStmt() 
	{
		consume(TokenType::LEFT_PARENTHESIS, "Expect '(' before if condition");

		auto condition = expression();

		consume(TokenType::RIGHT_PARENTHESIS, "Expect ')' after if condition");
		
		auto body = statement();

		return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
	}

	std::unique_ptr<Stmt> Parser::forStmt() 
	{
		consume(TokenType::LEFT_PARENTHESIS, "Expect '(' before if condition");

		std::unique_ptr<Stmt> initializer;

		if (match({ TokenType::SEMICOLON })) initializer = nullptr;

		else if (match({ TokenType::LET }))  initializer = varDeclaration();

		else initializer = expressionStmt();

		std::unique_ptr<Expr> condition;

		if (!check(TokenType::SEMICOLON)) condition = expression();

		consume(TokenType::SEMICOLON, "Expect ';' after loop condition");

		std::unique_ptr<Expr> increment;

		if(!check(TokenType::RIGHT_PARENTHESIS)) 
		{
			increment = expression();
		}

		consume(TokenType::RIGHT_PARENTHESIS, "Expect ')' after for clauses");

		auto body = statement();

		if (increment) 
		{
			std::vector<std::unique_ptr<Stmt>> statements;
			statements.push_back(std::move(body));
			statements.push_back(std::make_unique<ExprStmt>(std::move(increment)));
			body = std::make_unique<BlockStmt>(std::move(statements));
		}

		if (!condition) 
		{
			condition = std::make_unique<Literal>(true);
		}
		body = std::make_unique<WhileStmt>(std::move(condition), std::move(body));

		if (initializer) 
		{
			std::vector<std::unique_ptr<Stmt>> statements;
			statements.push_back(std::move(initializer));
			statements.push_back(std::move(body));
			body = std::make_unique<BlockStmt>(std::move(statements));
		}

		return body;

	}
	std::unique_ptr<Stmt> Parser::ifStmt() 
	{
		// "(" expression ")" "->" statement ("else" statement)? ;
		consume(TokenType::LEFT_PARENTHESIS, "Expect '(' before if condition");

		auto condition = expression();

		consume(TokenType::RIGHT_PARENTHESIS, "Expect ')' after if condition");

		auto thenBranch = statement();

		std::unique_ptr<Stmt> elseBranch;
		if (match({ TokenType::ELSE}))
		{
			elseBranch = statement();
		}

		return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
	}

	std::unique_ptr<Stmt> Parser::printStmt() 
	{
		consume(TokenType::LEFT_PARENTHESIS, "Expect '(' after pout statement");

		auto value = expression();
		consume(TokenType::RIGHT_PARENTHESIS, "Expect ')' after pout expression");

		consume(TokenType::SEMICOLON, "Expect ';' after pout statement");

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
		auto expr = or();

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
	std::unique_ptr<Expr> Parser::or() 
	{
		auto expr = Parser::and();

		while(match({TokenType::OR})) 
		{
			auto op = previous();

			auto right = Parser::and();

			expr = std::make_unique<LogicalExpr>(std::move(expr), std::move(op), std::move(right));
		}

		return expr;
	}
	std::unique_ptr<Expr> Parser::and() 
	{
		auto expr = equality();

		while(match({TokenType::AND})) 
		{
			auto op = previous();

			auto right = equality();

			expr = std::make_unique<LogicalExpr>(std::move(expr), std::move(op), std::move(right));
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
			auto op = previous();
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

		return call();
	}
	std::unique_ptr<Expr> Parser::call() 
	{
		auto expr = primary();

		while(match({TokenType::LEFT_PARENTHESIS}))
		{
			expr = finishCall(expr);
		}

		return expr;
	}

	std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr>& callee) 
	{
		std::vector<std::unique_ptr<Expr>> arguments;

		if (!check({TokenType::RIGHT_PARENTHESIS})) 
		{
			do 
			{
				if(arguments.size() >= 255) 
				{
					error(peek(), "Can't have more than 255 arguments.");
				}
				arguments.push_back(expression());

			} while (match({ TokenType::COMMA }));
		}

		auto paren = consume(TokenType::RIGHT_PARENTHESIS, "Expect ')' after arguments");

		return std::make_unique<CallExpr>(std::move(callee), std::move(paren), std::move(arguments));
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

	bool Parser::find(std::vector<TokenType> sequence) 
	{
		size_t temp = mCurrentToken;

		for (auto type : sequence) 
		{

			if (!check(type)) {
				mCurrentToken = temp; // Reset position on failure
				return false;
			}
			advance();
		}

		mCurrentToken = temp; // Reset position after checking
		return true;
	}


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
