
#include <sunflower.h>
#include <Lexer.h>
#include <Parser.h>
#include <Evaluator.h>


int main(int argc, char** argv)
{
    const std::string filePath = "examples/fff.txt"; 
    auto t = Sunflower::readFromFile(filePath);
    std::cout << t << '\n';
    Sunflower::Lexer l(t);

    auto temp = l.tokenize();

    //Sunflower::printTokensTable(temp);
    Sunflower::Parser parser{ temp };
    
    Sunflower::Evaluator e(parser.parse());
   
}                       