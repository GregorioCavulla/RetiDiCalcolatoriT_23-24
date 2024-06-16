#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS 1000
#define MAX_VAR_LENGTH 100

typedef struct
{
    char name[MAX_VAR_LENGTH];
    bool used;
} Variable;

Variable variables[MAX_VARS];
int varCount = 0;

void addVariable(const char *name)
{
    for (int i = 0; i < varCount; i++)
    {
        if (strcmp(variables[i].name, name) == 0)
        {
            return; // già esistente
        }
    }
    strcpy(variables[varCount].name, name);
    variables[varCount].used = false;
    varCount++;
}

void markVariableUsed(const char *name)
{
    for (int i = 0; i < varCount; i++)
    {
        if (strcmp(variables[i].name, name) == 0)
        {
            variables[i].used = true;
            return;
        }
    }
}

void findUnusedVariables(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Cannot open file");
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, " \t\n;(){}");
        while (token != NULL)
        {
            if (strcmp(token, "int") == 0 || strcmp(token, "char") == 0 ||
                strcmp(token, "float") == 0 || strcmp(token, "double") == 0)
            {
                token = strtok(NULL, " \t\n;(){}");
                if (token != NULL)
                {
                    addVariable(token);
                }
            }
            else
            {
                markVariableUsed(token);
            }
            token = strtok(NULL, " \t\n;(){}");
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <file.c>\n", argv[0]);
        return 1;
    }

    findUnusedVariables(argv[1]);

    printf("Unused variables:\n");
    bool foundUnused = false;
    for (int i = 0; i < varCount; i++)
    {
        if (!variables[i].used)
        {
            printf("%s\n", variables[i].name);
            foundUnused = true;
        }
    }

    if (!foundUnused)
    {
        printf("No unused variables found.\n");
    }

    return 0;
}
