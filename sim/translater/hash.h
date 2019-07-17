#ifndef HASH_H
#define HASH_H

#define HASH_SIZE 256
#define NONE -1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    int data;
    char* key;
}Hash;

Hash* Hash_Table;

void init_hash();
unsigned int hash(char* key);
int insert_label(char* key, int data);
int get_data(char* key);

#endif