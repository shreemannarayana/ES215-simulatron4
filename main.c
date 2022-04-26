#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileParser.h"

/* Utility functions */

void binToHex(char *bin, char *hex)
{
    char enc[] = "0123456789abcdef";
    for (int i = 0; i < 8; i++)
    { // makes it very specific (only 8 chars!)
        int n = 8 * (int)(bin[4 * i] == '1') + 4 * (int)(bin[4 * i + 1] == '1') + 2 * (int)(bin[4 * i + 2] == '1') + (int)(bin[4 * i + 3] == '1');
        hex[i] = enc[n];
    }
}

/////////////////////////////

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Improper Arguments\n");
        return -1;
    }
    else if (argc > 4)
    {
        printf("Too many arguments\n");
        return -1;
    }
    // Flag for deciding output format
    // false - binary
    // true - hex
    int flag = 1; // defaults to hexadecimal format
    if (argc == 4)
    {
        if (!strcmp(argv[3], "hex"))
        {
            flag = 1;
        }
        else if ((!strcmp((argv[3]), "bin")) || (!strcmp((argv[3]), "binary")))
        {
            flag = 0;
        }
        else
        {
            printf("Inappropriate third argument!\n");
            return -1;
        }
    }

    // Input and output files
    char *input = argv[1];
    char *output = argv[2];

    // Making a duplicate for the final file(in case of hex output)
    char foutput[256];
    strcpy(foutput, output);

    if (flag == 1)
    {
        output = "temp.o";
    }

    FILE *iFile = fopen(input, "r");
    FILE *oFile = fopen(output, "w");

    // Checking if the file does not exist
    if (iFile == NULL)
    {
        printf("Canot open input file\n");
        return -1;
    }

    // test code (to be transported to file parser)
    printf("File opened!\n"); // Just for testing
    int res = file_parser(iFile, oFile);
    if (res == -1)
    {
        printf("fileParser returned status -1\n");
    }
    // Closing the i/p, o/p file
    fclose(iFile);
    fclose(oFile);

    // Changing the o/p file in case requested for hex code
    if (flag == 1)
    {
        FILE *oFile = fopen(output, "r");
        FILE *oFileH = fopen(foutput, "w");
        char *line[32];
        int lineNumber = 1;
        while (fgets(line, 34, oFile) != NULL)
        {
            if (!strcmp(line, ""))
            {
                lineNumber++;
                continue;
            }
            char hex[] = "00000000\n";
            binToHex(line, hex);
            fputs(hex, oFileH);
        }
        fclose(oFile);
        fclose(oFileH);
        remove(output);
    }

    return 0;
}