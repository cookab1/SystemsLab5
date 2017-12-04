#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdio.h>
#include "SymbolList.h"
#include "FileHandler.h"
#include "Resolve.h"
#include <string.h>
#include <sstream>

FileHandler::FileHandler(SymbolList * defined, SymbolList * undefined){
    FileHandler::defined = defined;
    FileHandler::undefined = undefined;
}

void FileHandler::handleObjectSymbol(std::string name, char type){
    
    bool invalid = false;
    bool mds = false; //multiply defined symbol
    char foundType;
    char * nameArray = new char[name.size() + 2];
    std::copy(name.begin(), name.end(), nameArray);
    nameArray[name.size()] = '\0';
    int numTwo = 0;

    if(type != 'U') {
        switch(type) {
            case 'D':
               if(defined->getSymbol(name, &foundType)) {
                    switch(foundType) {
                        case 'D':
                            mds = true;
                            std::cout << ": multiple definition of " << name << "\n";
                        case 'T':
                            mds = true; //multiply defined symbol
                            std::cout << ": multiple definition of " << name << "\n";
                        case 'C':
                            defined->updateSymbol(name, type);
                        default:
                            invalid = true; //invalid type
                    }
                } else if (undefined->getSymbol(name, &foundType)) {
                    undefined->removeSymbol(name);
                    defined->insertSymbol(name, type);
                }
                else{
                    defined->insertSymbol(name, type);
                }
                break; 
            case 'T':
                if(defined->getSymbol(name, &foundType)) {
                    switch(foundType) {
                        case 'D':
                            mds = true;
                            std::cout << ": multiple definition of " << name << "\n";
                        case 'T':
                            mds = true; //multiply defined symbol
                            std::cout << ": multiple definition of " << name << "\n";
                        case 'C':
                            defined->updateSymbol(name, type);
                        default:
                            invalid = true; //invalid type
                    }
                } else if (undefined->getSymbol(name, &foundType)) {
                    //printf("name: %s\n", name.c_str());
                    undefined->removeSymbol(name);
                    //printf("undefindedNext: %s", (undefined->getNext(&foundType)).c_str());
                    //undefined->printSymbols(name);
                    defined->insertSymbol(name, type);
                }
                else{
                    defined->insertSymbol(name, type);
                }
                break;
            case 'C':
                if(!defined->getSymbol(name, &foundType))
                    defined->insertSymbol(name, type);
                if(undefined->getSymbol(name, &foundType))
                    undefined->removeSymbol(name);
                    //undefined->printSymbols(name);
                break;
            case 'd':
                numTwo = num();
                sprintf(nameArray, "%s.%d", nameArray, numTwo);
                defined->insertSymbol(nameArray, type);
                break;
            case 'b':
                //strcat(nameArray, );
                numTwo = num();
                sprintf(nameArray, "%s.%d", nameArray, numTwo);
                defined->insertSymbol(nameArray, type);
                break;
            default:
                invalid = true; //invalid type
                break;
        }
    } else {
        if(!undefined->findName(name) && !defined->findName(name)) {
            undefined->insertSymbol(name, type);
        }
    }
}

int FileHandler::num() {
    static int num = 0;
    //char tmpChar = num + '0';
    //num++;
    return num++;
}

bool FileHandler::objectFileNeeded(std::string filename){
    //checks if the .o file needs to go to the undefined/defined list
    //Bascially does the .o file define something that is currently undefined?
    FILE *fp;
    const char * fpString = ("nm " + filename).c_str();
    fp = popen(fpString, "r");
    char buffer[130];
    char * currLine = fgets(buffer, sizeof(buffer), fp);
    char type;
    char name[60];
    //int value = 0;
    char *typePointer = NULL;
    while(currLine != NULL){
        if (currLine[9] == 'U'){
            sscanf(buffer + 17, "%c %s ", &type, name);
        }
        else{
            sscanf(buffer + 17, "%c %s", &type, name);
        }
        if (undefined->getSymbol(name, typePointer)){
            return true;
        }
        else if (defined->getSymbol(name, typePointer)){
            if (*typePointer == 'C' && (type == 'T' || type == 'D')){
                return true;
            }
        }

        currLine = fgets(buffer, sizeof(buffer), fp);
    }
    return false;
}

bool FileHandler::isArchive(std::string filename){
    char * filenameArray = new char[filename.size() + 1];
    std::copy(filename.begin(), filename.end(),filenameArray);
    filenameArray[filename.size()] = '\0';
 
    int strLength = strlen(filenameArray);
    if (filename[strLength - 2] == '.' && filename[strLength - 1] == 'a'){
        return 1;
    }
    return 0;
}

bool FileHandler::isObjectFile(std::string filename){
    char * filenameArray = new char[filename.size() + 1];
    std::copy(filename.begin(), filename.end(),filenameArray);
    filenameArray[filename.size()] = '\0';

    int strLength = strlen(filenameArray);
    if (filename[strLength - 2] == '.' && filename[strLength - 1] == 'o'){
        return 1;
    }
    return 0;

}

void FileHandler::handleObjectFile(std::string filename){
    char *buffer = (char*)malloc(130);
    FILE *filePointer = NULL;
    const char * filePointerString = ("nm " + filename).c_str();
    filePointer = popen(filePointerString, "r");
    
    char * currLine;


    if (filePointer == NULL){
        std::cout << "popen failed\n";
        exit(1);
    }
    char type;
    //std::string name = "";
    char name[80];     
    //char value[30];
    currLine = fgets(buffer, 130, filePointer);
    while(currLine != NULL){
        if (currLine[17] == 'U'){
            sscanf(buffer + 17, "%c %s ", &type, name);
        }
        else{
            sscanf(buffer + 17, "%c %s ", &type, name);
        }
        handleObjectSymbol(name, type);
        currLine = fgets(buffer, 130, filePointer);
    }
    pclose(filePointer);
             
}

void FileHandler::handleArchive(std::string filename){

   FILE *fp;
   system("mkdir tmp");
   const char * systemCall = ("cp " + filename + " ./tmp/tmp.a").c_str();
   system(systemCall);
   system("cd tmp; ar -x tmp.a");
   system("rm tmp/tmp.a");
   fp = popen("ls tmp", "r");
   if (fp == NULL){
       std::cout << "popen failed \n";
       exit(1);
   }
   char buffer[130];
   char * currLine;
   bool changed = false;
   do{
       changed = false;
       currLine = fgets(buffer, sizeof(buffer), fp);
       while(currLine != NULL){
           //add symbols of Oi to defined/undefined lists (updated as necessary)
           if (objectFileNeeded(currLine)){
               handleObjectFile(currLine);
           }
           changed = true;
           currLine = fgets(buffer, sizeof(buffer), fp);
      }
   }while (changed == true);
   system("rm -f -r tmp");
}


