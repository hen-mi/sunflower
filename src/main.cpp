
#include <sunflower.h>
#include <Lexer.h>
#include <Parser.h>

int main(int argc, char** argv)
{
    const std::string filePath = "examples/ff.txt";

    std::cout << Sunflower::readFromFile(filePath) << '\n';
   
    {Sunflower::Lexer l(Sunflower::readFromFile(filePath));}
    Sunflower::Parser parser{ Sunflower::SymbolsTable };
    auto expr = parser.parse();
   
    if(expr)
    {
        PrintVisitor printer;
        expr->accept(printer);

    }
    //Sunflower::printSymbolsTable();
   
}