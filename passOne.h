/*
                         *******************
******************************* PASSONE  *******************************
                         ******************* 
**                                                                   **
** project : CLI                                                     **
** filename : passOne.h                                                **
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

#ifndef PASSONE_H
#define PASSONE_H

/****************************************************************************/
/**                                                                        **/
/**                             MODULES USED                               **/
/**                                                                        **/
/****************************************************************************/

#include "strmap.h"
#include "order.h"

/****************************************************************************/
/**                                                                        **/
/**                       DEFINITIONS AND MACROS                           **/
/**                                                                        **/
/****************************************************************************/

#define MAX_LINE 100
#define MAX_TEMP_CHAR  257
#define MAX_CHAR  10
#define MAX_LOCCTR 8
#define MAX_FLAGS 8

/****************************************************************************/
/**                                                                        **/
/**                       TYPEDEFS AND STRUCTURES                          **/
/**                                                                        **/
/****************************************************************************/

struct AB{
    char line[MAX_LINE],
    tempChar[MAX_TEMP_CHAR],
    label[MAX_CHAR],
    operation[MAX_CHAR], 
    operand[MAX_CHAR],
    buffer[MAX_CHAR],
    LOCCTRBuffer[MAX_LOCCTR];
    unsigned int LOCCTR, errflag[MAX_FLAGS], labelCounter, lineNumber, locationStart;
};
typedef struct AB SourceCodeInfo;

/****************************************************************************/
/**                                                                        **/
/**                         EXPORTTED VARIABLES                            **/
/**                                                                        **/
/****************************************************************************/

int checkifDirective(void);
void initializeOPTAB(void);
extern StrMap test;
extern struct StrMap OPTAB2;
unsigned int pass1(Order*);
int handleNonLabel(void);
int handleLabel(void);
void checkifSTARTexists(void);
int endDirectiveExists(void);
void handleLabelErrors(void);
void pass2(unsigned int,StrMap*,StrMap*);

#endif /* PASSONE_H */

/****************************************************************************/
/**                                                                        **/
/**                                 EOF                                    **/
/**                                                                        **/
/****************************************************************************/