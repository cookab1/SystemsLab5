#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include "SymbolList.h"

//lots of methods are missing that you need to add 
//See SymbolList.h for the declarations

//Initializes the first pointer to NULL
SymbolList::SymbolList() {
    first = NULL;
}

//Retrieves the type of the designated symbol name
bool SymbolList::getSymbol(std::string symbolName, char* type) {
    if(findName(symbolName)) {
        *type = iterate->type;
        return true;
    }
    return false;
}

//updates the type of the symbol with name symbolName
void SymbolList::updateSymbol(std::string symbolName, char type) {
    if(findName(symbolName))
        iterate->type = type;    
}

//inserts a symbol with the name symbolName and the type char
//at the END of the linked list
void SymbolList::insertSymbol(std::string symbolName, char type) {
    startIterate();
    //symbolEntry * sl;
    symbolEntry * sl = new symbolEntry();
    sl->type = type;
    sl->name = symbolName;
    sl->next = NULL;
    if(iterate == NULL) {
        first = sl;//{type, symbolName, NULL};//&symbolEntry;
        //first->name = symbolName;
        //first->type = type;
        //first->next = NULL;
    } else {
        while(iterate->next != NULL)
            iterate = iterate->next;
        iterate->next = sl;//{type, symbolName, NULL};//&symbolEntry;
        //iterate = iterate->next;
        //iterate->name = symbolName;
        //iterate->type = type;
        //iterate->next = NULL;
    }
}

//removes the symbolEntry node with the name symbolName
void SymbolList::removeSymbol(std::string symbolName) {
    if(find1Before(symbolName)) {
        iterate->next = iterate->next->next;
    }
    else{
        first = iterate->next;
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
void SymbolList::startIterate() {
    iterate = first;
}

bool SymbolList::findName(std::string symbolName) {
    for(startIterate(); iterate != NULL; iterate = iterate->next) {
        if(iterate->name.compare(symbolName) == 0)
            return true;
    }
    return false;
}

bool SymbolList::find1Before(std::string symbolName) {
    startIterate();
    if(iterate == NULL)
        return false;
    for(; iterate->next != NULL; iterate = iterate->next) {
        if(iterate->next->name.compare(symbolName) == 0)
            return true;
    }
    return false;
}

std::string SymbolList::getNext(char * type){
    std::string nameHolder = "";
    if (iterate != NULL){
        *type = iterate->type;
        nameHolder = iterate->name;
        iterate = iterate->next;
        return nameHolder;
    }
    return "";
}
