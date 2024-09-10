#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace Sunflower 
{
    std::string readFromFile(const std::string& filePath) 
    {
        std::ifstream       file(filePath);
        std::stringstream   buffer;


        buffer << file.rdbuf();
        return buffer.str();
    }
}

int main(int argc, char** argv)
{
    std::string filePath = "examples/test.sun";
    std::cout << Sunflower::readFromFile(filePath);
}