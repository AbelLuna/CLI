/*
                         *******************
******************************* PASSONE  *******************************
                         ******************* 
**                                                                   **
** project : CLI                                                     **
** filename : passOne.c                                                **
** version : 1                                                       **
** date : July 29, 2017                                              **
**                                                                   **
***********************************************************************
VERSION HISTORY:
----------------
Version : 1
Date : July 29, 2017
Revised by : Abel Luna
Description : Original version.
*/

/****************************************************************************/
/**                                                                        **/
/**                             MODULES USED                               **/
/**                                                                        **/
/****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "strmap.h"
#include "order.h"
#include "passOne.h"
#include "passTwo.h"

/****************************************************************************/
/**                                                                        **/
/**                            GLOBAL VARIABLES                            **/
/**                                                                        **/
/****************************************************************************/

StrMap *SYMTAB;
StrMap *OPTAB;
char line[100], tempChar[255], label[10], operation[10], operand[10], 
        buffer[255],LOCCTRBuffer[8];
unsigned int LOCCTR, errflag[8], labelCounter, lineNumber,locationStart;
FILE *fp, *fI, *fp_SYMBOLTABLE;

/****************************************************************************/
/**                                                                        **/
/**                             LOCAL FUNCTIONS                            **/
/**                                                                        **/
/****************************************************************************/

/*
 * This function places all recognized mnemonics on a hash table that is
 * to be compared with the mnemonics on an .asm file.
 */
void initializeOPTAB(void){
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

/*
 * @param obj The struct in which the filename exists.
 * 
 * This function creates an intermediate file and symbol table based on 
 * the an .asm file provided by obj. If the file is not found, it prints
 * an error to the console.
 */
unsigned int pass1(Order* obj){
    LOCCTRBuffer[0]='\0';
    LOCCTR=0;
    SYMTAB = sm_new(500);
    OPTAB = sm_new(60);
    initializeOPTAB();
    int checkContinue;
    if((fp=fopen(obj->param1,"r")) != NULL){
            if(feof(fp)) printf("File is empty.\n");
            else {
                fI = fopen("IntemediateFile.txt", "w");
                fp_SYMBOLTABLE =fopen("SymbolTab.txt","w");
                fgets(line,100,fp);
                lineNumber++;
                labelCounter++;
                sscanf(line, "%s %s %s", label, operation, operand);
                
                checkifSTARTexists();
                
                while(1){
                    if(feof(fp)){
                        printf("EROR: END Directive not found.\n");
                        fclose(fp);
                        fclose(fI);
                        fclose(fp_SYMBOLTABLE);
                        lineNumber=0,labelCounter=0;
                        break;
                    }
                    else{
                        fgets(line,100,fp);
                        lineNumber++;
                        sscanf(line,"%s",tempChar);//remove whitespace
                        if(tempChar[0]=='.')fprintf(fI,"%s", line);
                        
                        else if(isspace(line[0])==0){ //label found
                            if(handleLabel())break;
                        }
                        
                        else if(isspace(line[0])!=0){//no label
                            if(handleNonLabel())break;
                        }
                    }
                }        
            }
        }
        else {
        errorlog(5);
        checkContinue=5;
        }
    if(checkContinue!=5){
        pass2(LOCCTR-locationStart, OPTAB, SYMTAB);
    }
    return LOCCTR;
}
/* 
 * @return Returns 1 if "END" Directive is found. Else, it returns 0.
 * 
 * This function increments the label counter, places the label inside the 
 * SYMBOLTABLE using sm_put, writes the label and location counter to
 *  the SYMBOLTAB.txt file,  
 */
int handleLabel(void){
    sscanf(line,"%s %s %s", label, operation, operand);
    sprintf(LOCCTRBuffer,"%04X",LOCCTR);
    if(sm_exists(SYMTAB,label))errflag[0]=1;//Duplicate Labels
    else{
        sm_put(SYMTAB,label,LOCCTRBuffer);
        labelCounter++;
        fprintf(fp_SYMBOLTABLE,"%s\t%04X\n",label, LOCCTR);
    }
    checkifDirective();
    handleLabelErrors();
    fprintf(fI,"%u%u%u%u%u%u%u%u\n",errflag[0],errflag[1],errflag[2],
            errflag[3],errflag[4],errflag[5],errflag[6],errflag[7]);
    if(endDirectiveExists()) return 1;
    line[0]='\0',operand[0]='\n', label[0]='\0', operation[0]='\0';
    memset(errflag, 0, sizeof(errflag));//resets integer array to all zeroes.
    return 0;
}

/*
 * This function handles all errors that can be associated in the case
 * of a label being present in an .asm file. It also writes to the 
 * intermediate file in the special case of an RSUB instruction to be
 *  present.
 */
void handleLabelErrors(void){
    if(strlen(label) > 6 || !isalpha(label[0]))errflag[1]=1;//Illegal label
    if(labelCounter>500)errflag[6]=1;//Too many labels
    if((LOCCTR-locationStart)>32767)errflag[7]=1;//program too long
    if(!strcasecmp(operation,"RSUB"))fprintf(fI,"%s\n","0000");
    else if(!strcasecmp(operation,"BYTE")){ //do nothing
    }
    else if(!strcasecmp(operation,"WORD")){
        //do nothing
    }
    else if(!strcasecmp(operation,"RESB")){
        //do nothing
    }
    else fprintf(fI,"%s\n", operand);
}

/*
 * This function handles all errors that can be associated in the case
 * of a label being absent in an .asm file. It also writes to the 
 * intermediate file in the special case of an RSUB instruction to be
 *  present.
 */
void handleNonLabelErrors(void){
    if(labelCounter>500)errflag[6]=1;
    if((LOCCTR-locationStart)>32767)errflag[7]=1;
    if(!strcasecmp(operation,"RSUB"))fprintf(fI,"%s\n","0000");
    else if(!strcasecmp(operation,"BYTE")){ //do nothing
    }
    else if(!strcasecmp(operation,"WORD")){
        //do nothing
    }
    else if(!strcasecmp(operation,"RESB")){
        //do nothing
    }
    else fprintf(fI,"%s\n", operand);
}

/* 
 * @return Returns 1 if the "END" directive is found. Else returns 0.
 * 
 * This function compares the operation in an .asm file to the "END"
 * directive. If it is found, it closes all FILE streams, and resets all 
 * values.
 */
int endDirectiveExists(void){
    if(strcasecmp(operation,"END")==0){
        line[0]='\0',operand[0]='\n', label[0]='\0', operation[0]='\0';
        labelCounter=0, lineNumber=0;
        memset(errflag, 0, sizeof(errflag));//resets int array to all zeroes.
        fclose(fp);
        fclose(fI);
        fclose(fp_SYMBOLTABLE);
        
        return 1;
    }
    else return 0;
}

/* 
 * @return Returns 1 if "END" directive exists. Else, it returns 0.
 * 
 * This function writes the errorflags associated to an .asm file
 * to an intermediate file. It then resets the flags and all other 
 * values.
 */
int handleNonLabel(void){
    sscanf(line,"%s %s",operation,operand);
    checkifDirective();
    handleNonLabelErrors();
    fprintf(fI,"%u%u%u%u%u%u%u%u\n",errflag[0],errflag[1],errflag[2],
            errflag[3],errflag[4],errflag[5],errflag[6],errflag[7]);
    memset(errflag, 0, sizeof(errflag));//clears errflags if any
    if(endDirectiveExists())return 1;
    line[0]='\0',operand[0]='\n', label[0]='\0', operation[0]='\0';
    memset(errflag, 0, sizeof(errflag));//resets integer array to all zeroes.
    return 0;
}

/*
 * This function checks to see if the "START" Directive exists in 
 * the first line of an .asm file.
 * 
 * If it is found, it writes the line, location counter, operand, 
 * and errorflags to the intermediate file. It also writes to the 
 * SYMBOLTABLE with the corresponding address provided.
 */
void checkifSTARTexists(void){
    if(!strcasecmp(operation,"START")){
        if (strlen(operand)){//Checking if operand is missing
            if(isHex(operand))LOCCTR+=(int)strtol(operand, NULL, 16);//read as hex
            else {
               printf("Line %u: Illegal Operand on START directive\n",lineNumber);
               strncpy(operand,"0", sizeof(operand));
               errflag[4]=1;//Illegal operand 
               LOCCTR+=atoi(operand);
            }
        }
        else {
            printf("Line %u: Missing Operand on START directive\n", lineNumber);
            strncpy(operand,"0",sizeof(operand));
            errflag[4]=1;//Missing operand 
            LOCCTR+=atoi(operand);
        }
    sprintf(LOCCTRBuffer,"%04X",LOCCTR);
    sm_put(SYMTAB,label,LOCCTRBuffer);//Assuming label exists.
    locationStart=LOCCTR; //start location to be compared later.
    fprintf(fI,"%s%04X\n%s\n%s\n",line,LOCCTR,"START", operand);
    fprintf(fI,"%u%u%u%u%u%u%u%u\n",errflag[0],errflag[1],errflag[2],
            errflag[3],errflag[4],errflag[5],errflag[6],errflag[7]);
    fprintf(fp_SYMBOLTABLE,"%s\t%04X\n",label, LOCCTR);
    line[0]='\0',operand[0]='\0', label[0]='\0', operation[0]='\0';
    memset(errflag, 0, sizeof(errflag));//resets int array to all zeroes.
    }
}


/* 
 * @return Returns 0 if Directive "WORD","BYTE","RESW","RESB" or "END"
 * is found or if Operation does not exist. Else, it returns 1.
 * 
 * This function compares the operation to Directives, if they match,
 * the location counter is incremented accordingly, and '$NONE' is written
 * to the intermediate file as Directives have no mnemonic value.
 * 
 * If the Directives do not match, the operation is then compared to the
 * OPTABLE to check if the operation even exists. If it does not, an error
 * is printed to the console along with the line number and '$NONE'
 * is written on the intermediate file as their is no mnemonic value. If
 * they do match, the mnemonic value is written to the intermediate file
 * and returns 1.
 * 
 * Location Counter is not incremented in the case of illegal operands
 * or illegal operations. The previous location is written instead.
 */
int checkifDirective(void){
    if(line[strlen(line)-1]=='\n')fprintf(fI,"%s%04X\n",line, LOCCTR);
    else fprintf(fI,"%s\n%04X\n",line, LOCCTR);
    
    if(!strcasecmp(operation,"WORD")){
        if(strlen(operand)==0) errflag[3]=1;
        LOCCTR+=3;
        fprintf(fI,"%s\n","WORD");
        sprintf(operand,"%06X",atoi(operand));
        fprintf(fI,"%s\n",operand);
        return 0;
    }
    else if(!strcasecmp(operation,"RESW")){
        if(strlen(operand)==0) errflag[3]=1;//illegal operand
        else { 
            if(isHex(operand))LOCCTR += 3*atoi(operand);
        } 
        fprintf(fI,"%s\n","RESW");
        return 0;
    }
    else if(!strcasecmp(operation,"RESB")){
        fprintf(fI,"%s\n","RESB");
        if(strlen(operand)==0) errflag[3]=1;//missing operand
        else { 
            char temp[10];
            if(isHex(operand))LOCCTR += atoi(operand);
            sprintf(temp,"%04X",LOCCTR);
            fprintf(fI,"%s\n",temp);
        } 
        
        return 0;
    }
    else if(!strcasecmp(operation,"BYTE")){
        if(strlen(operand)==0){
           errflag[3]=1;//missing operand 
           fprintf(fI,"%s\n","BYTE");
        } 
        else {
            fprintf(fI,"%s\n","BYTE");
            char hexstr[10];
            if(operand[0]=='C'){//does not check complete format
                LOCCTR+=strlen(operand)-3; //-3 for the C ' and '
                int i,j=0;
                for(i=2;i<(strlen(operand)-1);i++){
                    sprintf(hexstr+j*2,"%02X",operand[i]);
                    j++;
                }
                fprintf(fI,"%s\n", hexstr);
            }
            else if(operand[0]=='X'){
                LOCCTR+=(strlen(operand)-3)/2;//Divide by 2 because its byte
                int i;
                for(i=2;i<(strlen(operand)-1);i++){
                    fprintf(fI,"%c",operand[i]);
                }
               fprintf(fI,"\n");
            }
            else {
                errflag[3]=1;//illegal operand}
                fprintf(fI,"$NONE");
            }
        }
        return 0;
    }
    else if(!strcasecmp(operation,"END")){
    fprintf(fI,"%s\n","END");
    return 0;
    }
    else if(!sm_exists(OPTAB,operation)){//If OPCODE does not exist
    errflag[2]=1;//Illegal operation
    fprintf(fI,"%s\n","$NONE");
    printf("Opcode %s on line: %u doesn't exist\n", operation, lineNumber);
    return 0;
    }
    else {//If opcode exists
        fprintf(fI,"%s\n",operation);//
        LOCCTR+=3;
        return 1;
    }
} 

/****************************************************************************/
/**                                                                        **/
/**                                 EOF                                    **/
/**                                                                        **/
/****************************************************************************/