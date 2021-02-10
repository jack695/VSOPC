#include <iostream>
#include "driver.hh"

int main (int argc, char *argv[]){
    if (argc < 2) {
        std::cerr << "Please provide at least a file as input." << std::endl;
        return -1;
    } else if (argc > 4) {
        std::cerr << "Too many arguments provided. " << std::endl;
        return -1;
    } else if (argc == 2){
        driver drv(CODE);
        return  drv.parse (argv[1]);
    } else if (argc == 3) {
        if (argv[1] == std::string ("-lex")){
            driver drv(LEXER);
            return drv.lex(argv[2]);
        }
        else if (argv[1] == std::string ("-parse")) {
            driver drv(PARSER);
            return  drv.parse (argv[2]);
        } else if (argv[1] == std::string ("-check")) {
            driver drv(CHECK);
            return  drv.parse (argv[2]);
        } else if (argv[1] == std::string ("-llvm")){
            driver drv(LLVM);
            return  drv.parse (argv[2]);
        } else {
            std::cerr << "Unknown option: " << argv[1] << std::endl;
            return -1;
        }
    } else {
        if (argv[1] == std::string ("-ext")) {
            std::cerr << "Extension not implemented." << argv[1] << std::endl;
            return -1;
        }
    }
}

