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
    char foundType = NULL;
    char * nameArray = new char[name.size() + 1];
    std::copy(name.begin(), name.end(), nameArray);
    nameArray[name.size()] = '\0';
     
    if(type != 'U') {
        switch(type) {
            case 'D':
            case 'T':
                if(defined->getSymbol(name, &foundType)) {
                    switch(foundType) {
                        case 'D':
                        case 'T':
                            mds = true; //multiply defined symbol
                        case 'C':
                            defined->updateSymbol(name, type);
                        default:
                            invalid = true; //invalid type
                    }
                } else if (undefined->findName(name)) {
                    undefined->removeSymbol(name);
                    defined->insertSymbol(name, type);
                }
                break;
            case 'C':
                if(!defined->findName(name))
                    defined->insertSymbol(name, type);
                if(undefined->findName(name))
                    undefined->removeSymbol(name);
                break;
            case 'd':
            case 'b':
                strcat(nameArray, "." + num());
                //std::stringstream stream;
                //stream << num();
                //std::string tmpName = name + "." + stream.str();
                
                defined->insertSymbol(name, type);
                break;
            default:
                invalid = true; //invalid type
                break;
        }
    } else {
        if(!undefined->findName(name)) {
            undefined->insertSymbol(name, type);
        }
    }
}

int FileHandler::num() {
    static int num = 0;
    return num++;
}

bool FileHandler::objectFileNeeded(std::string filename){
    
    return 0;
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
    char *buffer = (char*)malloc(80);
    FILE *filePointer;

    //char * filenameArray = new char[filename.size() + 1];


    char nm[10];
    char fileNameArray[50];
    std::copy(filename.begin(), filename.end(),fileNameArray);
    fileNameArray[filename.size()] = '\0';

    strcpy(nm, "nm ");
    //strcpy(fileNameArray, "");

 
    strcat(fileNameArray, nm);
    //std::string nmCommand = strcat("nm ", filename);
    filePointer = popen(fileNameArray, "r");
    if (filePointer == NULL){
        std::cout << "popen failed\n";
        exit(1);
    }
    char type;
    char *name;    
    while(fgets(buffer, 80, filePointer)){
        sscanf(buffer[17], "%c %s ", &type, name); 
        handleObjectSymbol(name, type);
    }
    pclose(filePointer);
             
}

void FileHandler::handleArchive(std::string filename){
    
}


