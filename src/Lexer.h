#pragma once
#include <sunflower.h>
#include <Token.h>


namespace Sunflower
{
	class Lexer
	{	
	public:

		Lexer(const std::string& source);
		void tokenize();
		~Lexer();

	private:

		char nextChar();
		char peekChar();
		bool isAtEnd();
		bool findMatch(char expected);
		void addToken(TokenType type);
		

	private:

		const std::string mSourceCode;
		size_t				mPosition;
		size_t					mLine;
		std::string	   mCurrentLexema;
		char					mByte;

	};

}