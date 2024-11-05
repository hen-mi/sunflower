
#include <sunflower.h>
#include <Lexer.h>
#include <Parser.h>
#include <Evaluator.h>


int main(int argc, char** argv)
{
    const std::string filePath = "examples/ff2.txt"; // 4

    std::cout << Sunflower::readFromFile(filePath) << '\n';
   
    {Sunflower::Lexer l(Sunflower::readFromFile(filePath)); }
    Sunflower::Parser parser{ Sunflower::SymbolsTable };
    auto expr = parser.parse();
    Sunflower::Evaluator e(*expr);


    Sunflower::printSymbolsTable();
   
}