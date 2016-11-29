/* 
 * File:   load.c
 * Author: Abel Luna
 * Username: aluna
 *
 * The purpose of load is to place the bytes from the object file into the 
 * SIC engine.
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "sic.h"
#include "load.h"

void loadBytes(Loader* loader){
    int i=0;
    for(i=1;i<=loader->allBytes[0];i++){
        PutMem(loader->address,&loader->allBytes[i],0);
        loader->address++;
    }
    
}

void setBytes(char* line, Loader* loader){
    int i, j;
    char address[7];
    for(i=1,j=0;i<=6;i++,j++){
        address[j]=line[i];
    }
    address[7]='\0';
    loader->address=strtol(address,NULL,16);
    for(i=7,j=0;i<strlen(line);i+=2,j++){
        char temp[3];
        temp[0]=line[i];
        temp[1]=line[i+1];
        temp[2]='\0';
        loader->allBytes[j]=strtol(temp,NULL,16);
    }
    
    loadBytes(loader);
}


Loader loadObj(FILE* objectFile){
    Loader loader;
    while(!feof(objectFile)){
    char line[80];
    fgets(line,sizeof line,objectFile);
    sscanf(line,"%[^\n]",line);
    if(line[0]=='H'){
        int i,j;
        char startAddress[7];
        for(i=0,j=7;j<=12;i++,j++){
            startAddress[i]=line[j];
        }
        startAddress[7]='\0';
        loader.start=strtol(startAddress,NULL,16);
    }
    else if(line[0]=='T'){
            setBytes(line,&loader);
        }
    }
    return loader;
    
}