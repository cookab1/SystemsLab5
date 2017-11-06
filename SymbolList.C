#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include "SymbolList.h"

//lots of methods are missing that you need to add 
//See SymbolList.h for the declarations

//Initializes the first pointer to NULL
SymbolList() {
    first = NULL;
}

//Retrieves the type of the designated symbol name
bool getSymbol(std::string symbolName, char* type) {
    if(findName(symbolName)) {
        *type = iterate->type;
        return true;
    }
    return false;
}

//updates the type of the symbol with name symbolName
void updateSymbol(std::string symbolName, char type) {
    if(findName(symbolName))
        iterate->type = type;    
}

//inserts a symbol with the name symbolName and the type char
//at the END of the linked list
void insertSymbol(std::string symbolName, char type) {
    startIterate();
    if(iterate == NULL) {
        first = &symbolEntry;
        first->name = symbolName;
        first->type = type;
        first->next = NULL;
    } else {
        while(iterate->next != NULL)
            iterate = iterate->next;
        iterate->next = &symbolEntry;
        iterate = iterate->next;
        iterate->name = symbolName;
        iterate->type = type;
        iterate->next = NULL;
    }
}

//removes the symbolEntry node with the name symbolName
void removeSymbol(std::string symbolName) {
    if(find1Before(symbolName)) {
        iterate->next = iterate->next->next;
    }
}

/* printSymbols
 * prints a header and then the contents of the linked list
 */
void SymbolList::printSymbols(std::string header) {
    symbolEntry * ptr = first;
    std::cout << header << " Symbol Table\n";
    std::cout << "-----------------------\n";
    while (ptr != NULL) {       
       std::cout << std::setw(32) << std::left 
                 << ptr->name << " " << ptr->type 
                 << std::endl;
       ptr = ptr->next;
   }
}

//sets iterate to the beginning of the 
void startIterate() {
    iterate = first;
}

bool findName(std::string symbolName) {
    for(startIterate(); iterate != NULL; iterate = iterate->next) {
        if(iterate->name.compare(symbolName) == 0)
            return true;
    }
    return false;
}

bool find1Before(std::string symbolName) {
    startIterate();
    if(iterate == NULL)
        return false;
    for(; iterate->next != NULL; iterate = iterate->next) {
        if(iterate->next->name.compare(symbolName) == 0)
            return true;
    }
    return false;
}
