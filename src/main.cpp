
#include <sunflower.h>
#include <Lexer.h>


int main(int argc, char** argv)
{
    std::string filePath = "examples/helloworld.sun";

    std::cout << Sunflower::readFromFile(filePath) << '\n';
   
    Sunflower::Lexer l(Sunflower::readFromFile(filePath));

    Sunflower::printSymbolsTable();
   
}