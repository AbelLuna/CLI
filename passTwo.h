/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   passTwo.h
 * Author: Abel Luna 
 *
 * Created on November 20, 2016, 4:59 PM
 */

#ifndef PASSTWO_H
#define PASSTWO_H
#include "strmap.h"
struct B{
    char text[70];//Will support records up to 69 columns in length.
    char startaddr[7];//The address to be written to the End record.
    char LOCCTR[7];//The address that the text record is to start with.
    char srcline[100];//Entire line being read.
    char opcode_addr[10];//Opcode 
    char flags[9];//The errors inside the intermediate file.
    int err;//To be checked at the end of the program if it is set.
};
typedef struct B Record;

#endif /* PASSTWO_H */

