

/*      SIC Assembler
 * File:   main.c
 * Author: Abel Luna
 * CMPE 3334.01
 * Username: aluna
 * 
 * This program will assemble assembly code recognized by the SIC engine and output object code.
 */

#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include "passOne.h"
#include "sic.h"

/*
 * The main function prints out a welcome message and the help menu.
 * It creates a struct called Order and calls the function getInput(Order*)
 * to break up the string and calls the function executeOrder(Order*) to 
 * call the appropriate command. cmd.id is assigned in the function
 * executeOrder(Order*). 99 represents the command "exit".
 */
int main(int argc, char** argv) {
    printf("Welcome to the command line interpretor.\n");
    help();
    SICInit();
    while(1){
        printf(">");
        Order cmd;
        getInput(&cmd);
        executeOrder(&cmd);
        if(cmd.id==99) break;
    }

    return (EXIT_SUCCESS);
}

