#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include "SymbolList.h"

//lots of methods are missing that you need to add 
//See SymbolList.h for the declarations

/* printSymbols
 * prints a header and then the contents of the linked list
 */
void SymbolList::printSymbols(std::string header)
{
    symbolEntry * ptr = first;
    std::cout << header << " Symbol Table\n";
    std::cout << "-----------------------\n";
    while (ptr != NULL)
    {
       
       std::cout << std::setw(32) << std::left 
                 << ptr->name << " " << ptr->type 
                 << std::endl;
       ptr = ptr->next;
    }
}

