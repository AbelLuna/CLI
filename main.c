/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Abel Luna
 *
 * Created on September 1, 2016, 10:08 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "order.h"

/*
 * 
 */
int main(int argc, char** argv) {
    printf("Welcome to the command line interpretor\n");
    help();
    while(1){
        printf(">");
        Order cmd;
        getInput(&cmd);
        executeOrder(&cmd);
        if(cmd.id==99) break;
    }

    return (EXIT_SUCCESS);
}

