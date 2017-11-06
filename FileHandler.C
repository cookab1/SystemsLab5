#include "FileHandler.h"
#include <cstdlib>
#include <string.h>
#include <stdio.h>

FileHandler::FileHandler(SymbolList * defined_, SymbolList * undefined_){
    defined = defined_;
    undefined = undefined_;
}

void FileHandler::handleObjectSymbol(std::string filename, char type){

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
    unsigned char * buffer = (char*)malloc (80 * 80 * sizeof(char));
    FILE *filePointer;
    std::string nmCommand = strcat("nm ", filename);
    filePointer = popen(nmCommand, "r");
    if (filePointer == NULL){
        std::cout << "popen failed\n";
        exit(1);
    }
    int i = 0;    
    while(fgets(buffer, sizeof(buffer), filePointer)){

    }
    pclose(filePointer);
             
}

void FileHandler::handleArchive(std::string filename){
    
}


