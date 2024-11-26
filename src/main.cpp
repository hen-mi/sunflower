
#include <sunflower.h>
#include <Lexer.h>
#include <Parser.h>
//#include <Resolver.h>
#include <Evaluator.h>


int main(int argc, char** argv)
{
    const std::string filePath = "examples/double.txt"; 
    auto t = Sunflower::readFromFile(filePath);
    std::cout << t << '\n';
    Sunflower::Lexer l(t);
    Sunflower::Evaluator evaluator{};

    auto temp = l.tokenize();
    Sunflower::Parser parser{ temp };
    //Sunflower::printTokensTable(temp);
    
    evaluator.run(parser.parse());
}                       