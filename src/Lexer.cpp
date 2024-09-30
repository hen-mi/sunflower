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

		while(!isAtEnd()) 
		{
			char c = nextChar();
			
			switch (c)
			{
			//single characters
			case '{': 
				mCurrentLexema += c; addToken(TokenType::LEFT_CBRACE);

				break;

			case '}': mCurrentLexema += c; addToken(TokenType::RIGHT_CBRACE);

				break;

			case '(': mCurrentLexema += c; addToken(TokenType::LEFT_PARENTHESIS); break;

			case ')': mCurrentLexema += c; addToken(TokenType::RIGHT_PARENTHESIS); break;
	
			case '[': mCurrentLexema += c; addToken(TokenType::LEFT_SQUAREBR); break;

			case ']': mCurrentLexema += c; addToken(TokenType::RIGHT_SQUAREBR); break;
			
			case '/': mCurrentLexema += c; addToken(TokenType::SLASH); break;

			case '*': mCurrentLexema += c; addToken(TokenType::STAR); break;

			case ':': mCurrentLexema += c; addToken(TokenType::COLON); break;

			case '+': mCurrentLexema += c; addToken(TokenType::PLUS); break;

			case ',': mCurrentLexema += c; addToken(TokenType::COMMA); break;

			//single or double characters
			case '-': 
				
				if (findMatch('-')) //comment line
				{					
					while (peekChar() != '\n' && !isAtEnd()) { nextChar(); }
					nextChar();
					break;
				}
				
				else if (findMatch('>')) { addToken(TokenType::IF); break; }
			
				else { mCurrentLexema += c; addToken(TokenType::MINUS); } break;
						
			case '=': addToken(findMatch('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;

			case '>': addToken(findMatch('=') ? TokenType::LESS_EQUAL  : TokenType::LESS);  break;

			case '<': addToken(findMatch('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

			case '!': addToken(findMatch('=') ? TokenType::NOT_EQUAL : TokenType::NOT); break;
			//Literals
			case '"': string();  break;

			//whitespace 
			case ' ':	break;
			case '\0':	break;
			case '\r':	break;
			case '\t':	break;
			
			case '\n': mLine++; break;

			default:
			//identifier or keyword
				if(std::isalpha(c)) 
				{
					mByte = c;
					Lexer::identifier();
				}

				else if(std::isdigit(c)) 
				{
					//TODO
				}
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
		return Lexer::isAtEnd() ? '\n' : mSourceCode[mPosition + 1];
	}

	bool Lexer::findMatch(char expected) 
	{
		if (isAtEnd()) return false;

		if (mSourceCode.at(mPosition) != expected) return false;
		
		mPosition++;

		return true;
	}

	void Lexer::addToken(TokenType type) 
	{
		
		Sunflower::SymbolsTable.push_back
		(
			{ type, mCurrentLexema, mLine}
		);
		mCurrentLexema = "";
	}

	void Lexer::string() 
	{
		
		while(Lexer::peekChar() != '"' && !isAtEnd())
		{
			
			if (peekChar() == '\n')
				mLine++;

			mCurrentLexema += Lexer::nextChar();
			
		}
		mCurrentLexema += Lexer::nextChar();
		Lexer::nextChar();
		addToken(TokenType::STRING);
	}

	void Lexer::identifier() 
	{
		mCurrentLexema += mByte;

		while(std::isalpha(Lexer::peekChar())) 
		{
			
			mCurrentLexema += Lexer::nextChar();
		}
		mCurrentLexema += Lexer::nextChar();

		if(keyword.find(mCurrentLexema) != keyword.end()) 
		{
			addToken(keyword[mCurrentLexema]);
		}
		else 
		{
			addToken(TokenType::IDENTIFIER);
		}
		

	}
	Lexer::~Lexer() {}
}

