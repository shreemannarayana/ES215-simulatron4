#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "fileParser.h"
#include <ctype.h>

// The map structure
struct map
{
	char *s;
	char *e;
};

/* Utility Functions */

int isNumeric(const char *s)
{
	while (*s)
	{
		if (isdigit(*s++) == 0)
			return 0;
	}

	return 1;
}



// Trimming the string
char *trim(char *str)
{
	size_t len = 0;
	char *frontp = str;
	char *endp = NULL;

	if (str == NULL)
	{
		return NULL;
	}
	if (str[0] == '\0')
	{
		return str;
	}

	len = strlen(str);
	endp = str + len;

	while (isspace((unsigned char)*frontp))
	{
		++frontp;
	}
	if (endp != frontp)
	{
		while (isspace((unsigned char)*(--endp)) && endp != frontp)
		{
		}
	}

	if (frontp != str && endp == frontp)
		*str = '\0';
	else if (str + len - 1 != endp)
		*(endp + 1) = '\0';

	endp = str;
	if (frontp != str)
	{
		while (*frontp)
		{
			*endp++ = *frontp++;
		}
		*endp = '\0';
	}

	return str;
}

// Binary Search function
int binarySearch(struct map arr[], int l, int r, char *x)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;
		if (strcmp(arr[mid].s, x) == 0)
			return mid;
		if (strcmp(arr[mid].s, x) > 0)
			return binarySearch(arr, l, mid - 1, x);
		return binarySearch(arr, mid + 1, r, x);
	}
	return -1;
}

// Converting strings to lower case
void toLower(char *str)
{
	int n = strlen(str);
	for (int i = 0; i < n; i++)
	{
		str[i] = tolower(str[i]);
	}
}

// Decimal to Binary Converter
void decToBinary(int n, char *a)
{
	int size = strlen(a);
	int *binaryNum = (int *)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
	{
		*(binaryNum + i) = 0;
	}
	int i = 0;
	while (n > 0)
	{
		*(binaryNum + i) = n % 2;
		n = n / 2;
		i++;
	}
	for (int i = size - 1; i >= 0; i--)
	{
		*(a + i) = '0' + (*(binaryNum + size - 1 - i));
	}
	a[size] = '\0';
	free(binaryNum);
}

// string compare function for sorting the maps (according to first string)
int mapCompare(const struct map *p1, const struct map *p2)
{
	return strcmp(p1->s, p2->s);
}

// Encoding map for the Registers
struct map regMap[] = {
	{"$zero", "00000"},
	{"$at", "00001"},
	{"$v0", "00010"},
	{"$v1", "00011"},
	{"$a0", "00100"},
	{"$a1", "00101"},
	{"$a2", "00110"},
	{"$a3", "00111"},
	{"$t0", "01000"},
	{"$t1", "01001"},
	{"$t2", "01010"},
	{"$t3", "01011"},
	{"$t4", "01100"},
	{"$t5", "01101"},
	{"$t6", "01110"},
	{"$t7", "01111"},
	{"$s0", "10000"},
	{"$s1", "10001"},
	{"$s2", "10010"},
	{"$s3", "10011"},
	{"$s4", "10100"},
	{"$s5", "10101"},
	{"$s6", "10110"},
	{"$s7", "10111"},
	{"$t8", "11000"},
	{"$t9", "11001"},
	{"$k0", "11010"},
	{"$k1", "11011"},
	{"$gp", "11100"},
	{"$sp", "11101"},
	{"$fp", "11110"},
	{"$ra", "11111"},
};

// Opcode map for the R-type instructions
struct map rMap[] = {
	{"add", "000000"},
	{"sub", "000000"},
	{"and", "000000"},
	{"or", "000000"},
	{"nor", "000000"},
	{"sll", "000000"},
	{"srl", "000000"},
	{"slt", "000000"},
	{"sltu", "000000"},
	{"jr", "000000"},
};

// func encoding map for R-type instructions

struct map rfMap[] = {
	{"add", "100000"},
	{"sub", "100010"},
	{"and", "100100"},
	{"or", "100101"},
	{"nor", "100111"},
	{"sll", "000000"},
	{"srl", "000010"},
	{"slt", "101010"},
	{"sltu", "101011"},
	{"jr", "001000"},
};

// Opcode map for the I-type instructions
struct map iMap[] = {
	{"addi", "001000"},
	{"lw", "100011"},
	{"sw", "101011"},
	{"lh", "100001"},
	{"lhu", "100101"},
	{"sh", "101001"},
	{"lb", "100000"},
	{"lbu", "100100"},
	{"sb", "101000"},
	{"ll", "110000"},
	{"sc", "111000"},
	{"lui", "001111"},
	{"andi", "001100"},
	{"ori", "001101"},
	{"beq", "000100"},
	{"bne", "000101"},
	{"slti", "001010"},
	{"sltiu", "001011"},
};

// Opcode map for the J-Type instructions
struct map jMap[] = {
	{"j", "000010"},
	{"jal", "000011"},
};

// Opcode map for the Memory access Type instructions
struct map maMap[] = {
	{"lw", "100011"},
	{"sw", "101011"},
	{"lh", "100001"},
	{"lhu", "100101"},
	{"sh", "101001"},
	{"lb", "100000"},
	{"lbu", "100100"},
	{"sb", "101000"},
	{"ll", "110000"},
	{"sc", "111000"}};

// Defining a flag for efficiency
int notSorted = 1;

// Defining the sizes of all the maps
int iMap_size = sizeof(iMap) / sizeof(struct map);
int jMap_size = sizeof(jMap) / sizeof(struct map);
int rMap_size = sizeof(rMap) / sizeof(struct map);
int rfMap_size = sizeof(rfMap) / sizeof(struct map);
int regMap_size = sizeof(regMap) / sizeof(struct map);
int maMap_size = sizeof(maMap) / sizeof(struct map);

// Returns instruction type for detecting the mnem
//
// Encoding - 1 : R-type
// 			  2 : I-type
// 			  3 : J-type
//			  4 : None (Probably report the error)
char *instType(char *mnem)
{
	if (notSorted)
	{
		// Sorting all the maps to do better(binary) search
		notSorted = 0;
		qsort(jMap, jMap_size, sizeof(struct map), mapCompare);
		qsort(iMap, iMap_size, sizeof(struct map), mapCompare);
		qsort(rMap, rMap_size, sizeof(struct map), mapCompare);
		qsort(regMap, regMap_size, sizeof(struct map), mapCompare);
		qsort(rfMap, rfMap_size, sizeof(struct map), mapCompare);
		qsort(maMap, maMap_size, sizeof(struct map), mapCompare);
	}
	if (binarySearch(jMap, 0, jMap_size - 1, mnem) != -1)
		return 3;
	else if (binarySearch(iMap, 0, iMap_size - 1, mnem) != -1)
		return 2;
	else if (binarySearch(rMap, 0, rMap_size - 1, mnem) != -1)
		return 1;
	else
		return 4;
}

// label addresses
int la[256];
int lptr = 0;

// Symbol table structure
struct sym_table
{
	char *name[32]; // Only 32 char names
	int value;		// as 32 bits would be enough
					// value represents address for label and data in case of constants
};

int getValue(char* s, struct sym_table st[]){
	for(int i = 0; i < 256; i++){
		if (!strcmp(st[i].name, s)){
			return st[i].value;
		}
	}
	return -1;
}

int file_parser(FILE *ifptr, FILE *ofptr)
{
	char line[256];
	unsigned int lineNumber = 1;
	// making all la 0
	for(int i = 0; i < 256; i++){
		la[i] = -1;
	}
	// parse 1
	// initialize the symbol table
	struct sym_table st[256]; // a max of 256 entries
	while (fgets(line, 256, ifptr) != NULL && strcmp(trim(line), ".data"))
	{
		lineNumber++;
	}
	// supporting only word size data
	int st_ptr = 0; // symbol table ptr

	// Initializing all the st table values to zero, and empty strings
	while (st_ptr < 256)
	{
		st[st_ptr].value = 0;
		strcpy(st[st_ptr].name, "");
		st_ptr++;
	}
	st_ptr = 0;
	// updating the constants
	while (fgets(line, 256, ifptr) != NULL && strcmp(trim(line), ".text"))
	{
		if (!strcmp(trim(line), ""))
		{
			lineNumber++;
			continue;
		}
		char tokens[3][32] = {"\0", "\0", "\0"}; // size of each token - 32, size of array = 3

		char *token = strtok(line, " ");
		strcpy(tokens[0], token);
		int i = 1;
		while (token != NULL && i <= 2)
		{
			token = strtok(NULL, " ");
			if (token != NULL)
				strcpy(tokens[i++], trim(token));
		}
		
		if (tokens[0] == '\0' || tokens[1] == '\0' || tokens[2] == '\0')
		{
			printf("l%d: Syntax error!", lineNumber);
			return -1;
		}
		tokens[0][strlen(tokens[0]) - 1] = '\0';
		if (st_ptr < 256)
		{
			st[st_ptr].value = atoi(tokens[2]);
			strcpy(st[st_ptr].name, tokens[0]);
			st_ptr++;
			
		}
		else
		{
			printf("Symbol table storage limit exceeded!\n");
			return -1;
		}
		lineNumber++;
	}

	
	// Updating the labels
	rewind(ifptr);
	lineNumber = 1;
	while (fgets(line, 256, ifptr) != NULL && strcmp(trim(line), ".text"))
	{
		lineNumber++;
	}

	// while look for condition (NOT EOF ) && (line not empty)
	while (fgets(line, 256, ifptr) != NULL && strcmp(trim(line), ".data"))
	{
		if (!strcmp(trim(line), ""))
		{
			lineNumber++;
			continue;
		}
		strcpy(line, trim(line));
		if (line[strlen(line) - 1] == ':')
		{
			if (st_ptr < 256)
			{
				line[strlen(line) - 1] = '\0';
				strcpy(line, trim(line));
				// eliminate the repeating error
				for (int i = 0; i < st_ptr; i++)
				{
					if (!strcmp(line, st[i].name))
					{
						printf("l%d: label repeated multiple times!\n", lineNumber);
						return -1;
					}
				}
				st[st_ptr].value = lineNumber - 1;
				strcpy(st[st_ptr].name, line);
				la[lptr] = lineNumber;
				lptr++;
				st_ptr++;
			}
			else
			{
				printf("Symbol table storage limit exceeded!");
				return -1;
			}
		}
		lineNumber++;
	}

	// resetting to the start of file
	rewind(ifptr);
	lineNumber = 1;

	// Parse 2
	// Start at the .text

	while (fgets(line, 256, ifptr) != NULL && strcmp(trim(line), ".text"))
	{
		lineNumber++;
	}

	// while look for condition (NOT EOF ) && (line not empty)
	while (fgets(line, 256, ifptr) != NULL && strcmp(trim(line), ".data"))
	{
		if (!strcmp(trim(line), ""))
		{
			lineNumber++;
			continue;
		}
		int isLabelLine = 0;
		for(int i = 0; i < 256; i++){
			if (lineNumber == la[i]){
				isLabelLine = 1;
				lineNumber++;
				break;
			}
		}
		if (isLabelLine) continue;
		// Generating tokens array
		char tokens[4][10] = {"\0", "\0", "\0", "\0"}; // size of each token - 10, size of array = 4
		char mnem[10];

		char *token = strtok(line, " ");
		strcpy(mnem, trim(token));
		toLower(mnem);
		strcpy(tokens[0], mnem);
		int i = 1;
		while (token != NULL && i <= 3)
		{
			token = strtok(NULL, ",");
			if (token != NULL)
				strcpy(tokens[i++], trim(token));
		}

		// testing
		for (int i = 0; i < 4; i++)
		{
			printf("%s ", tokens[i]);
		}
		printf("\n");
		// testing ends

		int instructionType = instType(mnem);
		if (instructionType == 1)
		{
			// R-type instruction
			// take care about shift operations
			// handle jr properly

			if (!strcmp(mnem, "jr"))
			{

				int t0 = binarySearch(rfMap, 0, rfMap_size - 1, tokens[0]);
				int t1 = binarySearch(regMap, 0, regMap_size - 1, tokens[1]);
				if ((t1 + 1))
				{
					fputs("000000", ofptr);
					fputs(regMap[t1].e, ofptr);
					fputs("000000000000000", ofptr);
					fputs(rfMap[t0].e, ofptr);
					fputs("\n", ofptr);
				}
				else
				{
					printf("l%d: syntax-error\n", lineNumber);
					return -1;
				}
			}
			else if (!strcmp(mnem, "sll") || !strcmp(mnem, "srl"))
			{
				int t1 = binarySearch(regMap, 0, regMap_size - 1, tokens[1]);
				int t2 = binarySearch(regMap, 0, regMap_size - 1, tokens[2]);
				if ((t1 + 1) && (t2 + 1))
				{
					fputs("000000", ofptr);
					fputs("00000", ofptr);
					fputs(regMap[t2].e, ofptr);
					fputs(regMap[t1].e, ofptr);
					char shamt[] = "00000";
					/* failed to check whether the third token is a number */
					if (isNumeric(tokens[3]))
						decToBinary(atoi(tokens[3]), shamt);
					else{
						if (getValue(tokens[3], st) != -1){
							decToBinary((getValue(tokens[3], st)), shamt);
						}
						else{
							printf("l%d: label not found!", lineNumber);
							return -1;
						}
					}
					fputs(shamt, ofptr);
					fputs(rfMap[binarySearch(rfMap, 0, rfMap_size - 1, tokens[0])].e, ofptr);
					fputs("\n", ofptr);
				}
				else
				{
					printf("l%d: syntax-error\n", lineNumber);
					return -1;
				}
			}
			else
			{
				int t1 = binarySearch(regMap, 0, regMap_size - 1, tokens[1]);
				int t2 = binarySearch(regMap, 0, regMap_size - 1, tokens[2]);
				int t3 = binarySearch(regMap, 0, regMap_size - 1, tokens[3]);
				if ((t1 + 1) && (t2 + 1) && (t3 + 1))
				{
					fputs("000000", ofptr);
					fputs(regMap[t2].e, ofptr);
					fputs(regMap[t3].e, ofptr);
					fputs(regMap[t1].e, ofptr);
					fputs("00000", ofptr);
					fputs(rfMap[binarySearch(rfMap, 0, rfMap_size - 1, tokens[0])].e, ofptr);
					fputs("\n", ofptr);
				}
				else
				{
					printf("l%d: syntax-error\n", lineNumber);
					return -1;
				}
			}
		}
		else if (instructionType == 2)
		{
			// I-type
			// handle the memory access type instructions properly
			if (binarySearch(maMap, 0, maMap_size - 1, tokens[0]) == -1)
			{
				if (!strcmp(tokens[0], "lui"))
				{
					int t0 = binarySearch(iMap, 0, iMap_size - 1, tokens[0]);
					int t1 = binarySearch(regMap, 0, regMap_size - 1, tokens[1]);

					if ((t1 + 1))
					{
						fputs(iMap[t0].e, ofptr);
						fputs("00000", ofptr);
						fputs(regMap[t1].e, ofptr);
						char imm[] = "0000000000000000";
						/* failed to check whether the third token is a number */
						if (isNumeric(tokens[2]))
							decToBinary(atoi(tokens[2]), imm);
						else{
							
							if (getValue(tokens[2], st) != -1)
								decToBinary(getValue(tokens[2], st), imm);
							else{
								printf("l%d: label not found!\n", lineNumber);
								return -1;
							}
						}
						fputs(imm, ofptr);
						fputs("\n", ofptr);
					}
					else
					{
						printf("l%d: syntax-error\n", lineNumber);
						return -1;
					}
				}
				else
				{
					int t0 = binarySearch(iMap, 0, iMap_size - 1, tokens[0]);
					int t1 = binarySearch(regMap, 0, regMap_size - 1, tokens[1]);
					int t2 = binarySearch(regMap, 0, regMap_size - 1, tokens[2]);
					if ((t1 + 1) && (t2 + 1))
					{
						fputs(iMap[t0].e, ofptr);
						// keep the order in the case of beq ans BNE
						if (!strcmp(mnem, "beq") || !strcmp(mnem, "bne"))
						{
							fputs(regMap[t1].e, ofptr);
							fputs(regMap[t2].e, ofptr);
						}
						// else reverse
						else
						{
							fputs(regMap[t2].e, ofptr);
							fputs(regMap[t1].e, ofptr);
						}

						char imm[] = "0000000000000000";
						/* failed to check whether the third token is a number */
						if (isNumeric(tokens[3]))
							decToBinary(atoi(tokens[3]), imm);
						else{
							if (getValue(tokens[3], st) != -1)
								decToBinary((!strcmp(mnem, "beq") || !strcmp(mnem, "bne"))?(getValue(tokens[3], st) - lineNumber + 1):(getValue(tokens[3], st)), imm);
							else{
								printf("l%d: label not found!", lineNumber);
								return -1;
							}
						}
						fputs(imm, ofptr);
						fputs("\n", ofptr);
					}
					else
					{
						printf("l%d: syntax-error\n", lineNumber);
						return -1;
					}
				}
			}

			else
			{
				int t0 = binarySearch(iMap, 0, iMap_size - 1, tokens[0]);
				int t1 = binarySearch(regMap, 0, regMap_size - 1, tokens[1]);
				char *imm_str = strtok(tokens[2], "(");
				char *reg_value = strtok(NULL, ")");
				int t2 = binarySearch(regMap, 0, regMap_size - 1, reg_value);
				if ((t1 + 1) && (t2 + 1))
				{
					fputs(iMap[t0].e, ofptr);
					fputs(regMap[t2].e, ofptr);
					fputs(regMap[t1].e, ofptr);
					char imm[] = "0000000000000000";
					/* failed to check whether the third token is a number */
					if (isNumeric(imm_str))
							decToBinary(atoi(imm_str), imm);
						else{
							if (getValue(imm_str, st) != -1)
								decToBinary(getValue(imm_str, st), imm);
							else{
								printf("l%d: label not found!", lineNumber);
								return -1;
							}
						}
					fputs(imm, ofptr);
					fputs("\n", ofptr);
				}
				else
				{
					printf("l%d: syntax-error\n", lineNumber);
					return -1;
				}
			}
		}
		else if (instructionType == 3)
		{
			// J-type
			int t0 = binarySearch(jMap, 0, jMap_size - 1, tokens[0]);
			fputs(jMap[t0].e, ofptr);
			char imm[] = "00000000000000000000000000";
			/* failed to check whether the third token is a number */
			if (isNumeric(tokens[1]))
				decToBinary(atoi(tokens[1]), imm);
			else{
				if (getValue(tokens[1], st) != -1)
					decToBinary(getValue(tokens[1], st), imm);
				else{
					printf("l%d: label not found!\n", lineNumber);
					return -1;
				}
			}
			fputs(imm, ofptr);
			fputs("\n", ofptr);
		}
		else
		{
			printf("l%d: syntax-error\n", lineNumber);
			return -1;
		}
		lineNumber++;
	}
	return 0;
}

/*           V2            */
// Width checking
// Adding additional flags(eg: hex) // finished
// labels, data - symbol table
//