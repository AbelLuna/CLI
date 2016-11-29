/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   load.h
 * Author: Abel Luna 
 *
 * Created on November 26, 2016, 2:45 PM
 */

#ifndef LOAD_H
#define LOAD_H
#include "sic.h"
struct C{
    ADDRESS address;//
    ADDRESS start;
    BYTE currentByte; //
    BYTE allBytes[31];
};
typedef struct C Loader;

Loader loadObj(FILE *);


#endif /* LOAD_H */

