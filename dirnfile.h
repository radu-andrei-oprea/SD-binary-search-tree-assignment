//OPREA RADU-ANDREI 335 CB
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef DIR_N_FILE
#define DIR_N_FILE

typedef struct dir {
	char * nume;
	TArb parinte;
	TArb files;
	TArb directories;
} Dir;


typedef struct file {
	char * nume;
	TArb parinte;
} File;


Dir * createDir(char * s);
File * createFile(char * s);

void distrDir(Dir * dir);
void distrFile(File * file);

int compNume(char * a, char *b);

#endif