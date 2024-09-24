#include <sunflower.h>
namespace Sunflower 
{

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