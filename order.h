/*
                         *******************
******************************* ORDER  *******************************
                         ******************* 
**                                                                   **
** project : CLI                                                     **
** filename : order.h                                                **
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

#ifndef ORDER_H
#define ORDER_H

/****************************************************************************/
/**                                                                        **/
/**                       DEFINITIONS AND MACROS                           **/
/**                                                                        **/
/****************************************************************************/

#define MAX_COMMAND_SIZE 30
#define MAX_PARAM1_SIZE  257
#define MAX_PARAM2_SIZE  5

/****************************************************************************/
/**                                                                        **/
/**                       TYPEDEFS AND STRUCTURES                          **/
/**                                                                        **/
/****************************************************************************/

struct A{
    char cmd[MAX_COMMAND_SIZE] ;//Will support commands up to 29 characters in length.
    int id; //Identifies which command is being executed.
    char param1[MAX_PARAM1_SIZE]; //Will support filenames up to 256 characters in length.
    char param2[MAX_PARAM2_SIZE]; //Will support up to 2 bytes in hexadecimal. 
    int paramN ; //Number of parameters 
};
typedef struct A Order;

/****************************************************************************/
/**                                                                        **/
/**                         EXPORTTED VARIABLES                            **/
/**                                                                        **/
/****************************************************************************/
void errorlog(int);
int validateHex(char*);
//int checkifDirective(char*, char*,char*,unsigned int*, unsigned int*,FILE*);
//void pass2(unsigned int);
void help(void);
void executeOrder(Order*);
void getInput(Order*);
int isHex(char*);
void errorlog(int);
#endif /* ORDER_H */

/****************************************************************************/
/**                                                                        **/
/**                                 EOF                                    **/
/**                                                                        **/
/****************************************************************************/