/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "order.h"

void help(){
    printf("-\n");
    printf("load filename : will call the load function to load the specified file.\n");
    printf("execute : will allow you to execute in debug mode.\n");
    printf("dump start end : will call the dump function, passing the values of start and end. Start and end will be hexadecimal values.\n");
    printf("help : Will print out a list of available commands. If an unrecognized command is typed at the command level, a message to type help should be displayed.\n");
    printf("assemble filename : will assemble and SIC assembly language program into a load module and store it in a file.\n");
    printf("directory : will list the files stored in the current directory.\n");
    printf("exit : will exit from the simulator.\n");
    printf("-\n");
}

void load(){
    printf("Load Reached\n");
}

void assemble(){
    printf("Assemble Reached\n");
}

void directory(){
    system("ls");
}

void execute(){
    printf("Execute Reached\n");
}

void debug(){
    printf("Debug Reached\n");
}

void dump(){
    printf("Dump Reached\n");
}
void errorlog(int errorID){
    switch(errorID){
        case 1:printf("Too many parameters.\n");break;
        case 2:printf("Too few parameters.\n");break;
        case 3:printf("Parameters not in hexadecimal format.\n");break;
        default: printf("An unexpected error occurred.\n");
    }
    
}

int homebrewScanf(char* input, Order* obj){
    int counter=0, out1=0, out2=0, out3=0, i;
    int looplen= strlen(input)-1;
    char command[30], param1[256], param2[256];
    command[0]='\0', param1[0]='\0', param2[0]='\0';
    input[looplen]='\0', obj->cmd[0]='\0', obj->param1[0]='\0', obj->param2[0]='\0';
    for(i=0; i<=looplen;i++){
        if(isalnum(input[i])!=0 && counter==0 ){
            command[out1]=input[i];
            out1++;
            if(isalnum(input[i+1])==0) {counter++; command[out1]='\0';}
            //else out1++;
        }
        else if(isalnum(input[i])!=0 && counter==1){
            param1[out2]=input[i];
            out2++;
            if(isalnum(input[i+1])==0){counter++; param1[out2]='\0';}
        }
        else if(isalnum(input[i])!=0 && counter==2){
            param2[out3]=input[i];
            out3++;
            if(isalnum(input[i+1])==0){counter++; param2[out3]='\0';}
        }
        else{}
    }
    strncpy(obj->cmd,command,sizeof(obj->cmd));
    strncpy(obj->param1,param1,sizeof(obj->param1));
    strncpy(obj->param2,param2,sizeof(obj->param2));
    return counter;
}

void getInput(Order* obj){
    char input[100];
    fgets(input,100,stdin);
    obj->paramN = homebrewScanf(input,obj)-1;
    //obj->paramN = sscanf(input,"%s %s %s %s",obj->cmd,obj->param1,obj->param2, extra) - 1;
}

int validateHex(Order* obj){
    int n=0;
    if (obj->param1[strspn(obj->param1, "0123456789abcdefABCDEF")] != 0)
    {
        n++;
    }
    if(obj->param2[strspn(obj->param2, "0123456789abcdefABCDEF")] != 0){
        n++;
    }
    if(n==0){
        dump();
    }
    else errorlog(3);
    //dump();
}

void testparam(Order* obj){
    //1 == too many parameters
    //2 == too few parameters
    switch (obj->id){
        case 1: if(obj->paramN == 0) help();
                else errorlog(1); break;
        case 2: if(obj->paramN == 1) load();
                else if (obj->paramN < 1) errorlog(2);
                else errorlog(1);break;
        case 3: if(obj->paramN == 1) assemble(); 
                else if (obj->paramN < 1) errorlog(1);
                else errorlog(1);break;
        case 4: if(obj->paramN == 0) directory();
                else errorlog(1);break;
        case 5: if(obj->paramN == 0) execute();
                else errorlog(1); break;
        case 6: if(obj->paramN == 0) debug();
                else errorlog(1); break;
        case 7: if(obj->paramN == 2) validateHex(obj);
                else if(obj->paramN < 2) errorlog(2);
                else errorlog(1);break;
        case 8: if(obj->paramN>=1) errorlog(1);
                else obj->id=99; break;
        default: printf("ERROR\n");
    }
}

void executeOrder(Order* obj){
    if (strcasecmp(obj->cmd,"help")==0) obj->id=1;
    else if (strcasecmp(obj->cmd,"load")==0) obj->id=2;
    else if (strcasecmp(obj->cmd,"assemble")==0) obj->id=3;
    else if (strcasecmp(obj->cmd,"directory")==0) obj->id=4;
    else if (strcasecmp(obj->cmd,"dir")==0)obj->id=4;
    else if (strcasecmp(obj->cmd,"execute")==0) obj->id=5;
    else if (strcasecmp(obj->cmd,"debug")==0) obj->id=6;
    else if (strcasecmp(obj->cmd,"dump")==0) obj->id=7;
    else if (strcasecmp(obj->cmd,"exit")==0) obj->id=8;
    else {printf("Unrecognized command. Type 'help' for a list of available commands.\n"); obj->id=0;}
    if(obj->id!=0)testparam(obj);
    
}