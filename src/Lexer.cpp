#include <Lexer.h>

namespace Sunflower
{
	Lexer::Lexer(const std::string& source)
		: mSourceCode{ source }, mPosition{ 0 }, mByte{ source[mPosition] }, 
		  mLine{ 1 }, mCurrentLexema{""} 
	{
		
		Lexer::tokenize();
		
	}

	void Lexer::tokenize()
	{

		while(isAtEnd()) 
		{
			char c = nextChar();

			switch (c)
			{
			case '(': addToken(TokenType::LEFT_PARENTHESIS); break;
			case ')': addToken(TokenType::RIGHT_PARENTHESIS); break;


			//whitespace 
			case ' ':	break;
			case '\0':	break;
			case '\r':	break;
			case '\t':	break;

			case '\n': mLine++; break;

			default:
				break;
			}
		}
		Lexer::addToken(TokenType::_EOF);
	}

	bool Lexer::isAtEnd() 
	{
		return mPosition >= mSourceCode.size();
	}
	char Lexer::nextChar() 
	{
		return mSourceCode[mPosition++];
	}

	char Lexer::peekChar() 
	{
		return Lexer::isAtEnd() ? mSourceCode[mPosition + 1] : '\n';
	}

	bool Lexer::findMatch(char expected) 
	{
		if (Lexer::isAtEnd() || expected != mSourceCode.at(mPosition + 1))
			return false;

		mPosition++;

		return true;
	}

	void Lexer::addToken(TokenType type) 
	{
		
		Sunflower::SymbolsTable.push_back
		(
			{ type, mCurrentLexema, mLine }
		);
		mCurrentLexema = "";
		mPosition++;
		mLine++;

	}

	Lexer::~Lexer() {}
}

