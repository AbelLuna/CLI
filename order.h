/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   order.h
 * Author: abelluna
 *
 * Created on September 1, 2016, 10:08 AM
 */

#ifndef ORDER_H
#define ORDER_H

struct A{
    char cmd[30];
    int id;
    char param1[256]; //Filename 
    char param2[256]; 
    int paramN;//Number of parameters 
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

