#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include<stdlib.h>
#include<stdio.h>

char* instType(char* mnem);
int file_parser(FILE *ifptr, FILE *ofptr);

#endif