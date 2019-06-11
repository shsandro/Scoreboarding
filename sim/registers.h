#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CLOCK_COUNT 9 /*Registrador que armazena os ciclos de clock*/

/*Estrutura do registrador  */
typedef struct{
    bool busy;
    int data;
}Register;

/*Banco de registradores MIPS32*/
Register registers[32];
/*Registradores PC e IR*/
Register PC, IR;

void initialize_registers();
int read_register(int register);
bool write_register(int data, int register);
bool get_status(int register);
int set_status(int register);

#endif