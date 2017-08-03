/*
                         *******************
******************************* PASSTWO  *******************************
                         ******************* 
**                                                                   **
** project : CLI                                                     **
** filename : passTwo.h                                                **
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

#ifndef PASSTWO_H
#define PASSTWO_H

/****************************************************************************/
/**                                                                        **/
/**                             MODULES USED                               **/
/**                                                                        **/
/****************************************************************************/

#include "strmap.h"

/****************************************************************************/
/**                                                                        **/
/**                       DEFINITIONS AND MACROS                           **/
/**                                                                        **/
/****************************************************************************/

#define MAX_TEXT_RECORD_LENGTH 70
#define MAX_TEXT_RECORD_ADDRESS  7
#define MAX_SOURCE_CODE_LINE_LENGTH  100
#define MAX_OPCODE_LENGTH 10
#define NUMBER_OF_FLAGS 9

/****************************************************************************/
/**                                                                        **/
/**                       TYPEDEFS AND STRUCTURES                          **/
/**                                                                        **/
/****************************************************************************/

struct B{
    char text[MAX_TEXT_RECORD_LENGTH];//Will support records up to 69 columns in length.
    char startaddr[MAX_TEXT_RECORD_ADDRESS];//The address to be written to the End record.
    char LOCCTR[MAX_TEXT_RECORD_ADDRESS];//The address that the text record is to start with.
    char srcline[MAX_SOURCE_CODE_LINE_LENGTH];//Entire line being read.
    char opcode_addr[MAX_OPCODE_LENGTH];//Opcode 
    char flags[NUMBER_OF_FLAGS];//The errors inside the intermediate file.
    int err;//To be checked at the end of the program if it is set.
};
typedef struct B Record;

#endif /* PASSTWO_H */

/****************************************************************************/
/**                                                                        **/
/**                                 EOF                                    **/
/**                                                                        **/
/****************************************************************************/