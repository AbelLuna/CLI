/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   passOne.h
 * Author: abel
 *
 * Created on October 20, 2016, 8:16 PM
 */

#ifndef PASSONE_H
#define PASSONE_H
#include "strmap.h"
int checkifDirective();
void initializeOPTAB();
extern StrMap test;
extern struct StrMap OPTAB2;
unsigned int pass1(Order*);

#endif /* PASSONE_H */

