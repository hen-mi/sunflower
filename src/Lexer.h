#pragma once
#include <sunflower.h>

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
		char nextPeekChar();
		bool isAtEnd();
		bool findMatch(char expected);
		void addToken(TokenType type);
		void string();
		void identifier();
		void number();

	private:

		const std::string mSourceCode;
		size_t				mPosition;
		size_t					mLine;
		std::string	   mCurrentLexema;
		char					mByte;

	};

}