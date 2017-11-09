//Fill in the following:
//
//Team member 1: Brenner Harris
//Team member 2: Andy Cook
//
#include <iostream>
#include <cstdlib>
#include "SymbolList.h"
#include "FileHandler.h"
#include "Resolve.h"

//You don't need to modify this file
int main(int argc, char * argv[])
{

    if (argc <= 1)
    {
       std::cout << "resolve: no input files\n";
       exit(1);
    }
    Resolve resolve(argc, argv);
}
