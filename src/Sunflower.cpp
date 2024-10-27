#include <sunflower.h>
namespace Sunflower 
{
    void Sunflower::report(int line, const std::string& where, const std::string& message)  
    {
        std::cerr << "[line: " << line << " ]" << "Error: " << where << " " << message << "\n";
    }

    void Sunflower::error(Token token, const std::string& message) 
    {
        if(token.tokentype == TokenType::_EOF) 
        {
            report(token.line, "at end", message);

        } else {
            report(token.line, token.lexema + "'", message);
        }
    }

    std::string Sunflower::readFromFile(const std::string& filePath)
    {
        std::ifstream       file(filePath);
        std::stringstream   buffer;


        buffer << file.rdbuf();
        return buffer.str();
    }

    void Sunflower::printSymbolsTable()
    {
        for (const auto& token : Sunflower::SymbolsTable) 
        {
            
            std::cout << TokenTypeToString[token.tokentype] <<" " << token.lexema << " " << token.line << "\n";
        }
    }
}