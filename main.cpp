#include <iostream>
#include <fstream>
#include <sstream>

#include "PrePro.hpp"
#include "Parser.hpp"
#include "ObjectManager.hpp"

int main(int argc, char** argcv) {
    // No argument provided
    if (argc <= 1) {
        std::cout << "[ERROR]: No file provided. Please specify a .sol file." << std::endl;
        throw 0;
    }

    std::ifstream solFile;
    try {
        solFile.open(argcv[1]);
    }
    catch (std::system_error& error) {
        std::cout << "[ERROR]: Error while loading file " << argcv[1] << ": " << error.code().message() << std::endl;
        throw 0;
    }

    std::stringstream programBuffer;
    programBuffer << solFile.rdbuf();

    std::string processedStr = PrePro::filter(programBuffer.str());

    std::stringstream filePath = std::stringstream(argcv[1]);
    std::string fileName = "";
    while (std::getline(filePath, fileName, '/'));

    std::string solExtension = ".sol";
    size_t solPos = fileName.find(solExtension);
    if (solPos != std::string::npos) fileName.erase(solPos, solExtension.length());

    ObjectManager::file = fileName + ".asm";
    ObjectManager::prepareFile();
    
    ASTNode* root = Parser::run(processedStr);
    root->evaluate(&Parser::symbolTable);
    
    ObjectManager::closeFile();
    return 1;
}
