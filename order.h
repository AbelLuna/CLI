/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   order.h
 * Author: Abel Luna
 *
 * Created on September 1, 2016, 10:08 AM
 */

#ifndef ORDER_H
#define ORDER_H

struct A{
    char cmd[30];//Will support commands up to 29 characters in length.
    int id; //Identifies which command is being executed.
    char param1[256]; //Will support filenames up to 255 characters in length.
    char param2[5]; //Will support up to 2 bytes in hexadecimal. 
    int paramN; //Number of parameters 
};
typedef struct A Order;

void help();
void executeOrder(Order* obj);
void getInput(Order* obj);

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* ORDER_H */
