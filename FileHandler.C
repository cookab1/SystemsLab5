#include "FileHandler.h"
#include <cstdlib>
#include <string.h>
#include <stdio.h>

FileHandler::FileHandler(SymbolList * defined_, SymbolList * undefined_){
    defined = defined_;
    undefined = undefined_;
}

void FileHandler::handleObjectSymbol(std::string name, char type){
    
    bool invalid = false;
    bool mds = false; //multiply defined symbol
    char foundType = NULL;
     
    if(type != 'U') {
        switch(type) {
            case 'D':
            case 'T':
                if(defined.getSymbol(name, &foundType)) {
                    switch(foundType) {
                        case 'D':
                        case 'T':
                            mds = true; //multiply defined symbol
                        case 'C':
                            defined.updateSymbol(name, type);
                        case default:
                            invalid = true; //invalid type
                    }
                } else if (undefined.findName(name)) {
                    undefined.removeSymbol(name);
                    defined.insertSymbol(name, type);
                }
                break;
            case 'C':
                if(!defined.findName(name))
                    defined.insertSymbol(name, type);
                if(undefined.findName(name))
                    undefined.removeSymbol(name);
                break;
            case 'd':
            case 'b':
                strcat(name, "." + num());
                defined.insertSymbol(name, type);
                break;
            case default:
                invalid = true; //invalid type
                break;
        }
    } else {
        if(!undefined.findName(name)) {
            undefined.insertSymbol(name, type);
        }
    }
}

int FilHandler::num() {
    static int num = 0;
    return num++;
}

bool FileHandler::objectFileNeeded(std::string filename){

}

bool FileHandler::isArchive(std::string filename){
    int strLength = strlen(filename);
    if (filename[strLength - 2] == '.' && filename[strLength - 1] == 'a'){
        return 1;
    }
    return 0;
}

bool FileHandler::isObjectFile(std::string filename){
    int strLength = strlen(filename);
    if (filename[strLength - 2] == '.' && filename[strLength - 1] == 'o'){
        return 1;
    }
    return 0;
}

void FileHandler::handleObjectFile(std::string filename){
    //unsigned char **buffer = (char**)calloc (80 * 80, sizeof(char*));
    unsigned char *buffer = malloc(80);
    FILE *filePointer;
    std::string nmCommand = strcat("nm ", filename);
    filePointer = popen(nmCommand, "r");
    if (filePointer == NULL){
        std::cout << "popen failed\n";
        exit(1);
    }
    char type;
    std::string name;    
    while(fgets(buffer, 80, filePointer)){
        sscanf(buffer[17], "%c %s ", &type, name); 
        handleObjectSymbol(name, type);
    }
    pclose(filePointer);
             
}

void FileHandler::handleArchive(std::string filename){
    
}


