
/* 
 * File:   passTwo.c
 * Author: Abel Luna
 * Username: aluna
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "strmap.h"
#include "order.h"
#include "passTwo.h"

FILE *fList, *fObj, *fp;
/*
 * 
 */
//void checkFlagError(char * flags, FILE);

int checkFlagError(char* flags) {
    int n = 0;
    if (flags[0] == '1') {
        fprintf(fList, "ERROR: Duplicate Labels.\n");
        n = 1;
    }
    if (flags[1] == '1') {
        fprintf(fList, "ERROR: Illegal Label.\n");
        n = 1;
    }
    if (flags[2] == '1') {
        fprintf(fList, "ERROR: Illegal operation.\n");
        n = 1;
    }
    if (flags[3] == '1') {
        fprintf(fList, "ERROR: Missing or illegal operand on data storage directive.\n");
        n = 1;
    }
    if (flags[4] == '1') {
        fprintf(fList, "ERROR: Missing or illegal operand on START directive.\n");
        n = 1;
    }
    if (flags[5] == '1') {
        fprintf(fList, "ERROR: Missing or illegal operand on END directive.\n");
        n = 1;
    }
    if (flags[6] == '1') {
        fprintf(fList, "ERROR: Too many symbols in source program.\n");
        n = 1;
    }
    if (flags[7] == '1') {
        fprintf(fList, "ERROR: Program too long.\n");
        n = 1;
    }
    if (n == 0) {
        fprintf(fList, "No errors.\n");
    }
    return n;
}

void checkSTARTOperand(unsigned int prgSize, Record* record) {
    unsigned int length;
    char line[100], LOCCTR[10], operation[10], operand[10], flags[10], temp[60];
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
    if (!strcasecmp("START", operation)) {
        fprintf(fList, "%s\n", line);
        //fprintf(fList, "\n");
        record->err = checkFlagError(flags);
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
    temp[0] = '\0';
    temp[0] = 'T';
    if ((strlen(LOCCTR)) < 6) {
        int i, j = 1;
        for (i = 0; i < (6-strlen(LOCCTR)); i++) {
            temp[j] = '0';
            j++;
        }
    }
        strncat(temp, LOCCTR, sizeof (temp));
        strncpy(record->text, temp, sizeof (record->text));
        strncpy(record->startaddr, LOCCTR, sizeof (record->startaddr));
        strncpy(record->srcline, line, sizeof (record->srcline));
    
}

void newTRecord(Record* record) {
    memset(record->text, 0, sizeof(record->text));
    record->text[0] = '\0';
    record->text[0] = 'T';
    strncat(record->text, "00", sizeof (record->text));
    strncat(record->text, record->LOCCTR, sizeof (record->text));
}

void printTRecord(Record* record) {
    //write length
    char length[3];
    int len = (strlen(record->text) - 9) / 2;
    printf("%d\n",len);
    if (len != -1) {//making sure I don't print an empty record.
        sprintf(length, "%02X", len);
        record->text[7] = length[0];
        record->text[8] = length[1];
        record->text[strlen(record->text)] = '\0';
        fprintf(fObj, "%s\n", record->text);
    }
    newTRecord(record);

}

void printERecord(Record* record) {
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
    fclose(fp_SymbolTAB);
    fclose(fList);
    fclose(fObj);
    fclose(fp);

}

void printToListing(Record* record) {
    fprintf(fList, "%s\n", record->srcline);
    fprintf(fList, "OPCODE= %s\n", record->opcode_addr);
    fprintf(fList, "ADDRESS= %s\n", record->LOCCTR);
    record->err = checkFlagError(record->flags);
}

void toRecord(Record* record, char * str) {
    if ((69 - strlen(record->text)) >= strlen(str)) {
        if (strlen(record->text) == 7)
            strncat(record->text, "  ", sizeof (record->text));
        strncat(record->text, str, sizeof (record->text));
        printf("toRecord %s\n",record->text);
    } else {
        printTRecord(record);
        strncat(record->text, "  ", sizeof (record->text));
        strncat(record->text, str, sizeof (record->text));
        printf("toRecord %s\n",record->text);
    }
    printToListing(record);
}

void test() {
    fclose(fList);
    fclose(fObj);
    fclose(fp);
}

int endofFile() {
    if (feof(fp)) {
        //remove("objectFile.txt");
        puts("No Object code generated\n");
        puts("No End directive found\n");
        fclose(fList);
        fclose(fObj);
        fclose(fp);
        return 1;
    } else return 0;
}

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


void handleLine2(Record* record, char* operation, char* operand, StrMap* OPTAB, StrMap* SYMTAB) {
    char address[20], opcode[20], X[10];
    address[0]='\0',opcode[0]='\0', X[0]='\0';
    int n, hasX=0;
    if (sm_exists(OPTAB, operation)) {//if  exists
        //Checking if it has ",X" to it
        if (operand[strlen(operand) - 1] == 'X' || operand[strlen(operand) - 1] == 'x') {
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
                strncpy(record->opcode_addr, opcode, sizeof (record->opcode_addr));
                toRecord(record, record->opcode_addr);
            } else if (!strcasecmp("RSUB", operation)) {
                char temp[8] = "4C0000\0";
                strncpy(address, temp, sizeof (address));
                strncpy(record->opcode_addr, temp, sizeof (record->opcode_addr));
                toRecord(record, record->opcode_addr);
            } else { //symbol not found
                sm_get(OPTAB, operation, opcode, sizeof (opcode));
                strncat(opcode, "0000", sizeof (opcode));
                n = 1; //error.no symbol
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
}

//not accounting for comments in beginning.
//fix prgsize
void pass2(unsigned int prgSize, StrMap* OPTAB, StrMap* SYMTAB) {
    printf("le size: %X\n",prgSize);
    char line[100], addr[10], operation[10], operand[10], flags[10];
    line[0] = '\0', addr[0] = '\0', operation[0] = '\0', operand[0] = '\0', flags[0];
    int error;
    Record record;
    record.LOCCTR[0] = '\0', record.flags[0] = '\0', record.opcode_addr[0] = '\0', record.srcline[0] = '\0';
    record.startaddr[0] = '\0', record.text[0] = '\0';
    fList = fopen("ListingFile.txt", "w");
    fObj = fopen("ObjectFile.txt", "w");
    if ((fp = fopen("IntemediateFile.txt", "r")) != NULL) {
        if (feof(fp)) printf("ERROR: File is empty.\n");
        else {
            checkSTARTOperand(prgSize, &record);
            printf("%s\n", line);
            while (1) {
                if (removeComments())break;
                if (endofFile())break;
                //prep lines//
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
                //end prep//ass
                printf("Operation: %s\n",operation);
                if (strcasecmp("END", operation)) {//not end
                    handleLine2(&record, operation, operand, OPTAB, SYMTAB);
                } else {//When End Directive is found.
                    printERecord(&record);
                    break;
                }
            }
        }

    }
}



