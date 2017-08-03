/*
                         *******************
******************************* MAIN  *******************************
                         ******************* 
**                                                                   **
** project : CLI                                                     **
** filename : main.c                                                 **
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
#include "order.h"
#include "passOne.h"
#include "sic.h"


int main(void) {
    printf("Welcome to the command line interpretor.\n");
    help();
    SICInit();
    int loop = TRUE;
    while(loop){
        printf("$>");
        Order *cmd = malloc(sizeof(Order));
        getInput(cmd);
        executeOrder(cmd);
        if(cmd->id==99) loop = FALSE; //id condition means exit.
        free(cmd);
    }
    return (EXIT_SUCCESS);
}
/****************************************************************************/
/**                                                                        **/
/**                                 EOF                                    **/
/**                                                                        **/
/****************************************************************************/
