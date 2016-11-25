/* 
 * File:   passTwo.c
 * Author: Abel Luna
 * Username: aluna
 *
 * The purpose of passTwo is to create a Listing file and an Object File
 * from a .asm file. passTwo makes use of the files produced in passOne.
 * They are deleted regardless of errors in this pass.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "strmap.h"
#include "order.h"
#include "passTwo.h"

FILE *fList, *fObj, *fp;

/*
 * @param flags These are the errors produced from PassOne in an 8-bit format
 * @param symbolerr This number is either 1 or 0. 1 indicates that a symbol
 * was not found.
 * @return Returns 1 if any errors were produced. Returns 0 otherwise.
 */
int checkFlagError(char* flags, int symbolerr) {
    int n = 0;
    if (flags[0] == '1') {
        fprintf(fList, "**ERROR: Duplicate Labels.\n");
        n = 1;
    }
    if (flags[1] == '1') {
        fprintf(fList, "**ERROR: Illegal Label.\n");
        n = 1;
    }
    if (flags[2] == '1') {
        fprintf(fList, "**ERROR: Illegal operation.\n");
        n = 1;
    }
    if (flags[3] == '1') {
        fprintf(fList, 
        "**ERROR: Missing or illegal operand on data storage directive.\n");
        n = 1;
    }
    if (flags[4] == '1') {
        fprintf(fList, 
        "**ERROR: Missing or illegal operand on START directive.\n");
        n = 1;
    }
    if (flags[5] == '1') {
        fprintf(fList, 
        "**ERROR: Missing or illegal operand on END directive.\n");
        n = 1;
    }
    if (flags[6] == '1') {
        fprintf(fList, "**ERROR: Too many symbols in source program.\n");
        n = 1;
    }
    if (flags[7] == '1') {
        fprintf(fList, "**ERROR: Program too long.\n");
        n = 1;
    }
    if (symbolerr) {
        fprintf(fList, "**ERROR: No Symbol found .\n");
        n=1;
    }
//    if (n == 0) {
//        fprintf(fList, "No errors.\n");
//    }
    return n;
}

/**
 * 
 * @param prgSize The size of the program in hexadecimal. 
 * @param record The struct which will hold the T record of the object file.
 * 
 * The purpose of this function is to check for the START Directive. If it
 * exists, it will write the Header record appropriately. If not, it will
 * write an empty header record with default address of 0. This function
 * also initialized the first T record with the appropriate address.
 */
void checkSTARTDirective(unsigned int prgSize, Record* record) {
    unsigned int length;
    char line[100], LOCCTR[10], operation[10], operand[10], 
         flags[10], firstObjectLine[60];
    fgets(line, sizeof (line), fp);
    fgets(LOCCTR, sizeof (LOCCTR), fp);
    fgets(operation, sizeof (operation), fp);
    fgets(operand, sizeof (operand), fp);
    fgets(flags, sizeof (flags), fp);
    sscanf(line, "%[^\n]", line);
    sscanf(LOCCTR, "%s", LOCCTR);
    sscanf(operation, "%s", operation);
    sscanf(operand, "%s", operand);
    sscanf(flags, "%s", flags);
    fprintf(fList,"Address\t|\tOpcode\t\t|\tSource Line\n");
    if (!strcasecmp("START", operation)) {
        fprintf(fList,"%s\t|\t\t\t|\t%s\n",LOCCTR, line);
        record->err = checkFlagError(flags, 0);
    }
    fprintf(fObj, "H");
    char prgName[7];
    sscanf(line,"%s",prgName);
    fprintf(fObj, "%s", prgName);
    if (length = (strlen(operation)) < 6) {
        int i;
        for (i = 0; i < (length); i++)
            fprintf(fObj, " ");
    }
    if (length = (strlen(LOCCTR)) < 6) {
        int i;
        for (i = 0; i < (length); i++)
            fprintf(fObj, "0");
        fprintf(fObj, "%s", LOCCTR);
    }
    if (length = (snprintf(0, 0, "%u", prgSize)) < 6) {
        int i;
        for (i = 0; i < (length); i++)
            fprintf(fObj, "0");
        fprintf(fObj, "%X\n", prgSize);
    }
    firstObjectLine[0] = '\0';
    firstObjectLine[0] = 'T';
    if ((strlen(LOCCTR)) < 6) {
        int i, j = 1;
        for (i = 0; i < (6-strlen(LOCCTR)); i++) {
            firstObjectLine[j] = '0';
            j++;
        }
    }
        strncat(firstObjectLine, LOCCTR, sizeof (firstObjectLine));
        strncpy(record->text, firstObjectLine, sizeof (record->text));
        strncpy(record->startaddr, LOCCTR, sizeof (record->startaddr));
        strncpy(record->srcline, line, sizeof (record->srcline));
}

/**
 * 
 * @param record This is where the new T record is to be written in.
 * 
 * This function clears the current T record located in the struct and
 * initializes a new one with the appropriate address in the program.
 */
void newTRecord(Record* record) {
    memset(record->text, 0, sizeof(record->text));
    record->text[0] = '\0';
    record->text[0] = 'T';
    strncat(record->text, "00", sizeof (record->text));
    strncat(record->text, record->LOCCTR, sizeof (record->text));
}

/**
 * 
 * @param record The struct that contains the record that is to be printed.
 * 
 * This function prints out the T record inside the record struct, if it is
 * not blank. It then calls a function to create a new record.
 */
void printTRecord(Record* record) {
    char length[3];
    int len = (strlen(record->text) - 9) / 2;
    if (len != -1) {//making sure I don't print an empty record.
        sprintf(length, "%02X", len);
        record->text[7] = length[0];
        record->text[8] = length[1];
        record->text[strlen(record->text)] = '\0';
        fprintf(fObj, "%s\n", record->text);
    }
    newTRecord(record);

}

/**
 * 
 * @param record The struct that the E record will be printed on.
 * @param error Indicates if there were any errors throughout the program.
 * 
 * This function prints the E record to the object file. If there were any
 * errors throughout the program, it deletes the object file and advises the
 * user to check the Listing file.
 */
void printERecord(Record* record, int error) {
    FILE *fp_SymbolTAB;
    printTRecord(record);
    memset(record->text, 0, sizeof(record->text));
    record->text[0] = 'E';
    strncat(record->text, "00", sizeof (record->text));
    strncat(record->text, record->startaddr, sizeof (record->text));
    fprintf(fObj,"%s",record->text);
    if ((fp_SymbolTAB = fopen("SymbolTab.txt", "r")) != NULL) {
        fprintf(fList, "\n\n\nSymbolTable:::\n");
        while (!feof(fp_SymbolTAB)) {
            char line[50];
            fgets(line, sizeof (line), fp_SymbolTAB);
            fprintf(fList, "%s", line);
        }
    }
    if(error) {
        remove("ObjectFile.txt");
        puts("No object file produced. Check Listing file for errors.\n");
    }
    remove("IntemediateFile.txt");
    remove("SymbolTab.txt");
    fclose(fp_SymbolTAB);
    fclose(fList);
    fclose(fObj);
    fclose(fp);
}

/**
 * 
 * @param record Contains the values to be printed to the listing file.
 * 
 * This function prints the source line, opcode, address, and any errors 
 * produced to the listing file.
 */
void printToListing(Record* record) {
    fprintf(fList,"%s\t|\t%s\t\t|\t%s\n",record->LOCCTR,record->opcode_addr,record->srcline);
    record->err = checkFlagError(record->flags, record->err);
}

/**
 * 
 * @param record Contains T record that is to be modified.
 * @param opcode The opcode that is to be added to the T record.
 * 
 * This function checks if the T record is full. If it is, it calls a
 * function to print it and create a new one. It also calls a function to
 * write to the Listing file.
 */
void toRecord(Record* record, char * opcode) {
    
    if ((69 - strlen(record->text)) >= strlen(opcode)) {
        if (strlen(record->text) == 7)
            strncat(record->text, "  ", sizeof (record->text));
        strncat(record->text, opcode, sizeof (record->text));
    } else {
        printTRecord(record);
        strncat(record->text, "  ", sizeof (record->text));
        strncat(record->text, opcode, sizeof (record->text));
    }
    printToListing(record);
}
/**
 * 
 * @return Returns 1 if the END Directive was not found. Otherwise returns 0.
 * 
 * This function checks if the end of the file has been reached without 
 * finding the END directive.
 */
int endofFile() {
    if (feof(fp)) {
        remove("objectFile.txt");
        puts("No Object code generated\n");
        puts("No End directive found\n");
        fclose(fList);
        fclose(fObj);
        fclose(fp);
        return 1;
    } else return 0;
}
/**
 * 
 * @return Returns 1 if the end of the File is reached. Otherwise returns 0.
 * 
 * This function is meant to skip all comments in the .asm file in order
 * not to process them later.
 */
int removeComments() {
    int c;
    while ((c = fgetc(fp)) == '.') {//Print comments
        if (endofFile())return 1;
        char comment[100];
        ungetc(c, fp);
        fgets(comment, sizeof (comment), fp);
        fprintf(fList, "%s", comment);
    }
    ungetc(c, fp);
    return 0;
}

/**
 * 
 * @param record Where the opcode, address, and record are kept.
 * @param operation This is the Mnemonic.
 * @param operand This is the operand given to the mnemonic.
 * @param OPTAB This is the table where all the recognized mnemonics are.
 * @param SYMTAB This is the table where all the symbols are.
 * 
 * This is where the mnemonics from the .asm file are processed. It
 * first checks if the operation exists in the OPTAB table. If it does not,
 * it checks to see if it is a Directive. If none are found, the object file
 * will not be created. If it is found, it checks if the ",X" is present in
 * the operand. If it is, it modifies the char * operand so it will be
 * recognized by the SYMTAB, and it adds 1000H to the address. If it is not
 * found, the program moves forward to check if the symbol exists. If the
 * symbol is found, the appropriate address is assigned. If not, an error is
 * to be printed out to the Listing file.
 */
void handleLine2(Record* record, char* operation, char* operand, 
        StrMap* OPTAB, StrMap* SYMTAB) {
    char address[20], opcode[20], X[10];
    address[0]='\0',opcode[0]='\0', X[0]='\0';
    int n, hasX=0;
    if (sm_exists(OPTAB, operation)) {//if  exists
        //Checking if it has ",X" to it
        if (operand[strlen(operand) - 1] == 'X' || 
                operand[strlen(operand) - 1] == 'x') {
                if (operand[strlen(operand) - 2] == ',') {
                    operand[strlen(operand) - 2] = '\0';
                    hasX=1;
            }
        }
        if (operand[0] != '$') {
            if (sm_exists(SYMTAB, operand)) {
                sm_get(OPTAB, operation, opcode, sizeof (opcode));
                sm_get(SYMTAB, operand, address, sizeof (address));
                if(hasX){
                    int tempaddr = (int) strtol(address, NULL, 16);
                    sprintf(address, "%04X", tempaddr + 32768); //32768==8000H
                }
                strncat(opcode, address, sizeof (opcode));
                strncpy(record->opcode_addr, opcode, 
                        sizeof (record->opcode_addr));
                toRecord(record, record->opcode_addr);
            } else if (!strcasecmp("RSUB", operation)) {
                char temp[8] = "4C0000\0";
                strncpy(address, temp, sizeof (address));
                strncpy(record->opcode_addr, temp, 
                        sizeof (record->opcode_addr));
                toRecord(record, record->opcode_addr);
            } else { //symbol not found
                sm_get(OPTAB, operation, opcode, sizeof (opcode));
                strncat(opcode, "0000", sizeof (opcode));
                record->err=1; //error.no symbol
                toRecord(record, opcode);
            }

        }
    } else if (!strcasecmp("BYTE", operation)) {
        toRecord(record, operand);
    } else if (!strcasecmp("WORD", operation)) {
        strncpy(record->opcode_addr, operand, sizeof (record->opcode_addr));
        toRecord(record, operand);
    } else if (!strcasecmp("RESW", operation)) {
        printTRecord(record);
    }
    else if (!strcasecmp("RESB", operation)) {
        strncpy(record->LOCCTR,operand,sizeof(record->LOCCTR));
        printTRecord(record);
    }
    else {
        memset(record->opcode_addr,0,sizeof(record->opcode_addr));
        printToListing(record);
    }
}

/**
 * 
 * @param prgSize The size of the program given from the .asm file.
 * @param OPTAB The table containing all mnemonics recognized.
 * @param SYMTAB The table containing all symbols recognized.
 * 
 * This function gets the line from the IntermediateFile produced in passOne
 * and calls the function to handle it if the "END" directive is not found.
 */
void pass2(unsigned int prgSize, StrMap* OPTAB, StrMap* SYMTAB) {
    char line[100], addr[10], operation[10], operand[10], flags[10];
    line[0] = '\0', addr[0] = '\0', operation[0] = '\0', operand[0] = '\0',
    flags[0];
    int error=0;
    Record record;
    fList = fopen("ListingFile.txt", "w");
    fObj = fopen("ObjectFile.txt", "w");
    if ((fp = fopen("IntemediateFile.txt", "r")) != NULL) {
        if (feof(fp)) printf("ERROR: File is empty.\n");
        else {
            checkSTARTDirective(prgSize, &record);
            while (1) {
                line[0] = '\0', addr[0] = '\0', operation[0] = '\0',
                        operand[0] = '\0',flags[0];
                if(record.err)error=1;
                record.err=0;
                if (removeComments())break;
                if (endofFile())break;
                fgets(line, sizeof (line), fp);
                fgets(addr, sizeof (addr), fp);
                fgets(operation, sizeof (operation), fp);
                fgets(operand, sizeof (operand), fp);
                fgets(flags, sizeof (flags), fp);
                sscanf(line, "%[^\n]", line);
                sscanf(addr, "%s", addr);
                sscanf(operation, "%s", operation);
                sscanf(operand, "%s", operand);
                sscanf(flags, "%s", flags);
                strncpy(record.srcline, line, sizeof (record.srcline));
                strncpy(record.LOCCTR, addr, sizeof (record.LOCCTR));
                if (strcasecmp("END", operation)) {//not end
                    handleLine2(&record, operation, operand, OPTAB, SYMTAB);
                } else {//When End Directive is found.
                    printERecord(&record, error);
                    break;
                }
            }
        }

    }
}



