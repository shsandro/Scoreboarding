#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdlib.h>
#include <stdio.h>

/*Estrutura do registrador  */
typedef struct{
    bool busy;
    int data;
}Register;

Register registradores[32];
Register PC, IR;



#endif