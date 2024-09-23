
#include <sunflower.h>
#include <Lexer.h>


int main(int argc, char** argv)
{
    std::string filePath = "examples/h.sun";

    std::cout << Sunflower::readFromFile(filePath);
   
    Sunflower::Lexer l(filePath);
    Sunflower::printSymbolsTable();
   
}