#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define HIGH 32
#define LOW 33
#define NONE -1

/*Estrutura do registrador  */
typedef struct{
    bool busy;
    int data;
    int fu; //unidade funcional que produzirá o resultado do registrador
}Register;

/*Banco de registradores MIPS32*/
Register registers[34]; /*Registrador zero sempre armazena o valor 0*/
/*Registradores PC e IR*/
Register PC, IR;

void initialize_registers();
int read_register(int register);
bool write_register(int data, int register);
bool get_status(int register);
int set_status(int register);

#endif