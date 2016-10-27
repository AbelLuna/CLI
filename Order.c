
/* 
 * File:   Order.c
 * Author: Abel Luna
 * CMPE 3334.01
 * Username: aluna
 * Created on September 1, 2016, 10:08 AM
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "order.h"
#include "passOne.h"
void errorlog();
int validateHex(char*);
int checkifDirective(char*, char*,char*,unsigned int*, unsigned int*,FILE*);

/*Help menu. This function returns void and takes no parameters.*/
void help(){
    printf("-\n");
    printf("load filename : will call the load function to load the specified"
            "file.\n");
    printf("execute : will allow you to execute in debug mode.\n");
    printf("dump start end : will call the dump function, passing the values"
            "of start and end. Start and end will be hexadecimal values.\n");
    printf("help : Will print out a list of available commands.\n");
    printf("assemble filename : will assemble and SIC assembly language "
            "program into a load module and store it in a file.\n");
    printf("directory : will list the files stored in the current "
            "directory.\n");
    printf("exit : will exit from the simulator.\n");
    printf("-\n");
}
/*Load function stub*/
void load(){
    printf("Load Reached\n");
}


/*Assemble function stub*/
void assemble(Order* obj){
    pass1(obj);
    }

/*Directory function stub*/
void directory(){
    system("ls");
}
/*Execute function stub*/
void execute(){
    printf("Execute Reached\n");
}
/*debug function stub*/
void debug(){
    printf("Debug Reached\n");
}
/*Dump function stub*/
void dump(){
    printf("Dump Reached\n");
}


/*
 *Takes a parameter of int type and returns void. This function is an error 
 * log. It will print an error to the console based on the number given.
 */
void errorlog(int errorID){
    switch(errorID){
        case 1: printf("Too many parameters.\n");break;
        case 2: printf("Too few parameters.\n");break;
        case 3: printf("Parameters not in hexadecimal format.\n");break;
        case 4: printf("Unrecognized command. Type 'help' for a list of"
                "available commands.\n");break;
        case 5: printf("Could not open file.\n");break;
        default: printf("An unexpected error occurred.\n");
    }
}

/*
 * Takes parameters of char* type and Order* type and returns and int. Order* 
 * is defined as a struct. This function takes the raw string given and
 *  the struct pointer passed to it.The for loop splits the string into
 * 3 separate strings and copies each string into the struct. The loop adds 
 * to counter every time it detects a space in between chars. After the
 *  counter is incremented a third time, it ignores the rest of the string.
 */
int homebrewScanf(char* input, Order* obj){
    int counter=0, out=0, i;
    int len = strlen(input)-1;
    char command[30], param1[256], param2[256];
    //Initializing char arrays as blank.
    command[0]='\0', param1[0]='\0', param2[0]='\0';
    //Initializing char arrays in struct as blank.
    obj->cmd[0]='\0', obj->param1[0]='\0', obj->param2[0]='\0';
    
    if(input[len]=='\n')
    input[len]='\0'; 
    else {while(fgetc(stdin)!='\n');//Clears stdin buffer.
    input[len]='\0';}
    
    for(i=0; i<=len;i++){
        if(isspace(input[i])==0 && counter==0 ){
            command[out]=input[i];
            out++;
            if(isspace(input[i+1])!=0) {counter++; command[out]='\0'; out=0;}
        }
        else if(isspace(input[i])==0 && counter==1){
            param1[out]=input[i];
            out++;
            if(isspace(input[i+1])!=0){counter++; param1[out]='\0'; out=0;}
        }
        else if(isspace(input[i])==0 && counter==2){
            param2[out]=input[i];
            out++;
            if(isspace(input[i+1])!=0){counter++; param2[out]='\0'; out=0;}
        }
        else{}
    }
    //Copy contents from loop to struct
    strncpy(obj->cmd,command,sizeof(obj->cmd));
    strncpy(obj->param1,param1,sizeof(obj->param1));
    strncpy(obj->param2,param2,sizeof(obj->param2));
    return counter;
}
/*
 * Takes a struct pointer as type Order* and returns void. This function gets 
 * a string from the user. The input is a maximum of 256 in order to prevent
 * stack smashing when using homebrewScanf(char*,Order*). 
 */
void getInput(Order* obj){
    char input[256];
    fgets(input,256,stdin);
    obj->paramN = homebrewScanf(input,obj);
}
/*
 * Takes parameter of a struct of type Order* and returns void. This function
 *  validates the parameters in the struct to ensure they are in hexadecimal 
 * format. If they are not in hexadecimal format int n is incremented and 
 * calls the function errorlog(int) which prints out an error.
 */
int isHex(char* str){
    if (str[strspn(str, "0123456789abcdefABCDEF")] != 0) return 0;
    else return 1;
}

/*
 * Takes a paramater of struct type Order* and returns void. This function 
 * checks the parameters required for each command that is recognized by the 
 * interpretor. The int in obj->paramN has the number of parameters the 
 * command has been given by the user. If the number of parameters matches 
 * the number of parameters they should have in the conditional statements, 
 * then the program calls the function associated with that command. If not, 
 * the function errorlog(int) is called. 
 */
void testparam(Order* obj){
    //1 == too many parameters
    //2 == too few parameters
    switch (obj->id){
        case 1: if(obj->paramN == 0) help();    //help
                else errorlog(1); break;
        case 2: if(obj->paramN == 1) load();    //load
                else if (obj->paramN < 1) errorlog(2);
                else errorlog(1);break;
        case 3: if(obj->paramN == 1) assemble(obj);   //assemble
                else if (obj->paramN < 1) errorlog(2);
                else errorlog(1);break;
        case 4: if(obj->paramN == 0) directory();   //directory
                else errorlog(1);break;
        case 5: if(obj->paramN == 0) execute();  //execute
                else errorlog(1); break;
        case 6: if(obj->paramN == 0) debug();   //debug
                else errorlog(1); break;
        case 7: if(obj->paramN == 2) {if(isHex(obj->param1) && isHex(obj->param2)) dump(); else errorlog(3);} //dump
                else if(obj->paramN < 2) errorlog(2);
                else errorlog(1);break;
        case 8:  obj->id=99; break; //exit
        default: printf("Unexpected Error.\n");
    }
}
/*
 * Takes a parameter of struct type Order*. This function compares the char 
 * array inside the struct called cmd to commands that the interpretor 
 * recognizes and assigns an number of type int to the struct id. If none 
 * match, the function errorlog(int errorID) is called. If one does match,
 * another function testparam(Object* obj) is called to further test 
 * conditions required.
 */
void executeOrder(Order* obj){
    if (strcasecmp(obj->cmd,"help")==0) obj->id=1;
    else if (strcasecmp(obj->cmd,"load")==0) obj->id=2;
    else if (strcasecmp(obj->cmd,"assemble")==0) obj->id=3;
    else if (strncasecmp(obj->cmd,"directory",3)==0) obj->id=4;//dir works too
    else if (strcasecmp(obj->cmd,"execute")==0) obj->id=5;
    else if (strcasecmp(obj->cmd,"debug")==0) obj->id=6;
    else if (strcasecmp(obj->cmd,"dump")==0) obj->id=7;
    else if (strcasecmp(obj->cmd,"exit")==0) obj->id=8;
    else {errorlog(4); obj->id=0;}
    if(obj->id!=0)testparam(obj);
}