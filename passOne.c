/* 
 * File:   passOne.c
 * Author: Abel Luna
 * Username: aluna
 *
 * The purpose of passOne is to create an intermediate file that is derived from an .asm file containing SIC assembly code. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "strmap.h"
#include "order.h"
StrMap *SYMTAB;
StrMap *OPTAB;
char line[100], tempChar[255], label[10], operation[10], operand[10], buffer[255],LOCCTRBuffer[8];
unsigned int LOCCTR, errflag[8], labelCounter, lineNumber,locationStart;
FILE *fp;
FILE *fI;
FILE *fp_SYMBOLTABLE;
int checkifDirective(void);
/*
 * 
 */
void initializeOPTAB(){
    sm_put(OPTAB,"ADD","18");
    sm_put(OPTAB,"AND","58");
    sm_put(OPTAB,"COMP","28");
    sm_put(OPTAB,"DIV","24");
    sm_put(OPTAB,"J","3C");
    sm_put(OPTAB,"JEQ","30");
    sm_put(OPTAB,"JGT","34");
    sm_put(OPTAB,"JLT","38");
    sm_put(OPTAB,"JSUB","48");
    sm_put(OPTAB,"LDA","00");
    sm_put(OPTAB,"LDCH","50");
    sm_put(OPTAB,"LDL","08");
    sm_put(OPTAB,"LDX","04");
    sm_put(OPTAB,"MUL","20");
    sm_put(OPTAB,"OR","44");
    sm_put(OPTAB,"RD","D8");
    sm_put(OPTAB,"RSUB","4C");
    sm_put(OPTAB,"STA","0C");
    sm_put(OPTAB,"STCH","54");
    sm_put(OPTAB,"STL","14");
    sm_put(OPTAB,"STX","10");
    sm_put(OPTAB,"SUB","1C");
    sm_put(OPTAB,"TD","E0");
    sm_put(OPTAB,"TIX","2C");
    sm_put(OPTAB,"WD","DC");
}



void pass1(Order* obj){
    printf("Assemble Reached\n");
    SYMTAB = sm_new(500);
    OPTAB = sm_new(60);
    initializeOPTAB();
    if((fp=fopen(obj->param1,"r")) != NULL){
            if(feof(fp)) printf("File is empty.\n");
            else {
                fI = fopen("IntemediateFile.txt", "w");
                fp_SYMBOLTABLE =fopen("SymbolTab.txt","w");
                fgets(line,100,fp);
                lineNumber++;
                labelCounter++;
                sscanf(line, "%s %s %s", label, operation, operand);
                if(strcasecmp(operation,"START")==0){
                    
                    if (strlen(operand)){//Checking if operand is missing
                        if(isHex(operand))LOCCTR+=(int)strtol(operand, NULL, 16);//convert to hex
                        else {
                           printf("Line: %u Illegal Operand on START directive\n", lineNumber);
                           strncpy(operand,"0", sizeof(operand));
                           errflag[4]=1;//Illegal operand 
                           LOCCTR+=atoi(operand);
                        }
                    }
                    else {
                        printf("Line: %u Missing Operand on START directive\n", lineNumber);
                        strncpy(operand,"0",sizeof(operand));
                        errflag[4]=1;//Missing operand 
                        LOCCTR+=atoi(operand);
                    }
                    sscanf(LOCCTRBuffer, "%x",&LOCCTR);//Converting hex to String that will be parsed by the hash table.
                    sm_put(SYMTAB,label,LOCCTRBuffer);//Assuming label exists
                    locationStart=LOCCTR;
                    fprintf(fI,"%s%04X\n%s\n%s\n",line,LOCCTR,"$NONE", operand);
                    fprintf(fI,"%u%u%u%u%u%u%u%u\n",errflag[0],errflag[1],errflag[2],errflag[3],errflag[4],errflag[5],errflag[6],errflag[7]);
                    fprintf(fp_SYMBOLTABLE,"%s\t%04X\n",label, LOCCTR);
                    line[0]='\0',operand[0]='\0', label[0]='\0', operation[0]='\0';
                    memset(errflag, 0, sizeof(errflag));//resets integer array to all zeroes.
                }
                
                while(1){
                    if(feof(fp)){
                        printf("END Directive not found.\n");
                        fclose(fp);
                        fclose(fI);
                        fclose(fp_SYMBOLTABLE);
                        break;
                    }
                    else{
                        fgets(line,100,fp);
                        lineNumber++;
                        sscanf(line,"%s",tempChar);//in case there are whitespace before the '.'
                        if(tempChar[0]=='.')fprintf(fI,"%s", line);
                        else if(isspace(line[0])==0){ //label found
                            labelCounter++;
                            sscanf(line,"%s %s %s", label, operation, operand);
                            sscanf(LOCCTRBuffer,"%04X",&LOCCTR);
                            sm_put(SYMTAB,label,LOCCTRBuffer);
                            fprintf(fp_SYMBOLTABLE,"%s\t%04X\n",label, LOCCTR);
                            checkifDirective();
                            if(!sm_exists(SYMTAB,label))errflag[0]=1;//Duplicate Labels
                            if(strlen(label) > 6 || !isalpha(label[0]))errflag[1]=1;//Illegal label
                            if(labelCounter>500)errflag[6]=1;//Too many labels
                            if((LOCCTR-locationStart)>32767)errflag[7]=1;//program too long
                            if(!strcasecmp(operation,"RSUB"))fprintf(fI,"%s\n","$NONE");
                            else fprintf(fI,"%s\n", operand);
                            fprintf(fI,"%u%u%u%u%u%u%u%u\n",errflag[0],errflag[1],errflag[2],errflag[3],errflag[4],errflag[5],errflag[6],errflag[7]);
                            if(strcasecmp(operation,"END")==0){
                                line[0]='\0',operand[0]='\n', label[0]='\0', operation[0]='\0';
                                memset(errflag, 0, sizeof(errflag));//resets integer array to all zeroes.
                                fclose(fp);
                                fclose(fI);
                                break;
                            }
                            line[0]='\0',operand[0]='\n', label[0]='\0', operation[0]='\0';
                            memset(errflag, 0, sizeof(errflag));//resets integer array to all zeroes.
                        }
                        
                        else if(isspace(line[0])!=0){//no label
                            sscanf(line,"%s %s",operation,operand);
                            checkifDirective();
                            if(labelCounter>500)errflag[6]=1;
                            if((LOCCTR-locationStart)>32767)errflag[7]=1;
                            if(!strcasecmp(operation,"RSUB"))fprintf(fI,"%s\n","$NONE");
                            else fprintf(fI,"%s\n", operand);
                            fprintf(fI,"%u%u%u%u%u%u%u%u\n",errflag[0],errflag[1],errflag[2],errflag[3],errflag[4],errflag[5],errflag[6],errflag[7]);
                            memset(errflag, 0, sizeof(errflag));//clears errflags if any
                            if(strcasecmp(operation,"END")==0){
                                line[0]='\0',operand[0]='\n', label[0]='\0', operation[0]='\0';
                                memset(errflag, 0, sizeof(errflag));//resets integer array to all zeroes.
                                fclose(fp);
                                fclose(fI);
                                fclose(fp_SYMBOLTABLE);
                                break;
                            }
                            line[0]='\0',operand[0]='\n', label[0]='\0', operation[0]='\0';
                            memset(errflag, 0, sizeof(errflag));//resets integer array to all zeroes.
                        }
                    }
                }
                
            }
        }
        else errorlog(5);
}

void checkifSTART(void){
    
}

int checkifDirective(void){
    if(line[strlen(line)-1]=='\n')fprintf(fI,"%s%04X\n",line, LOCCTR);
    else fprintf(fI,"%s\n%04X\n",line, LOCCTR);
    
    if(strcasecmp(operation,"WORD")==0){
        if(strlen(operand)==0) errflag[3]=1;
        LOCCTR+=3;
        fprintf(fI,"%s\n","$NONE");
        return 0;
    }
    else if(strcasecmp(operation,"RESW")==0){
        if(strlen(operand)==0) errflag[3]=1;//illegal operand
        else { 
            if(isHex(operand))LOCCTR += 3*atoi(operand);
        } 
        fprintf(fI,"%s\n","$NONE");
        return 0;
    }
    else if(strcasecmp(operation,"RESB")==0){
        if(strlen(operand)==0) errflag[3]=1;//missing operand
        else { 
            if(isHex(operand))LOCCTR += atoi(operand);
        } 
        fprintf(fI,"%s\n","$NONE");
        return 0;
    }
    else if(strcasecmp(operation,"BYTE")==0){
        if(strlen(operand)==0) errflag[3]=1;//missing operand
        else {
            if(operand[0]=='C'){//does not check complete format
                LOCCTR+=strlen(operand)-3; //-3 for the C ' and '
            }
            else if(operand[0]=='X')
                LOCCTR+=(strlen(operand)-3)/2;//Divide by 2 because hex
            else errflag[3]=1;//illegal operand
        }
        fprintf(fI,"%s\n","$NONE");
        return 0;
    }
    else if(!strcasecmp(operation,"END")){
    fprintf(fI,"%s\n","$NONE");
    }
    else if(!sm_exists(OPTAB,operation)){//Checking if Opcode exists//Does not exists
    errflag[2]=1;//Illegal operation
    fprintf(fI,"%s\n","$NONE");
    printf("Opcode %s doesn't exist\n", operation);
    return 0;
    }
    else {//If opcode exists
        sm_get(OPTAB,operation,buffer,sizeof(buffer));
        fprintf(fI,"%s\n",buffer);//value of mneumonic
        LOCCTR+=3;
        return 1;
    }
} 


