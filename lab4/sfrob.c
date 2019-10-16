#include <stdio.h>
#include <stdlib.h>

char unfrob(const char c)
{
    return c ^ 42;
}

int frobcmp(const char *a, const char *b)
{
    while (1)
    {
        if (*a != *b)
            break;
        if (*a == ' ' && *b == ' ')
            return 0;
        a++;
        b++;
    }
    
    if (unfrob(*a) > unfrob(*b))
        return 1;
    else
        return -1;
}

int pre_frobcmp(const void *x, const void *y)
{
    const char* a = *(const char**)x;
    const char* b = *(const char**)y;
    return frobcmp(a, b);
}

int errorExists()
{
    if (ferror(stdin))
    {
        printf("I/O error detected.");
        return 1;
    }
    return 0;
}

int main()
{
    char* curr;
    curr = (char*)malloc(sizeof(char));
    char** ptrarray;
    ptrarray = (char**)malloc(sizeof(char*));
    int letterCount = 0;
    int wordCount = 0;
    curr[letterCount] = getchar();
    if (errorExists() || curr == NULL)
    {
        free(curr);
        exit(1);
    }
    
    while (curr[letterCount] != EOF && !errorExists())
    {
        char nextChar = getchar();
        if (errorExists())
        {
            free(curr);
            free(ptrarray);
            exit(1);
        }
        
        if (curr[letterCount] == ' ')
        {
            char** tmpword = realloc(ptrarray, wordCount*sizeof(char*) + sizeof(char*));
            if (tmpword == NULL)
            {
                free(curr);
                free(ptrarray);
                exit(1);
            } else
            {
                ptrarray = tmpword;
                ptrarray[wordCount] = curr;
                wordCount++;
                letterCount = -1;
                curr = (char*)malloc(sizeof(char));
            }
            if (nextChar == EOF)
                break;
            while (nextChar == ' ')
            {
                nextChar = getchar();
                if (errorExists())
                {
                    free(curr);
                    free(ptrarray);
                    exit(1);
                }
            }
        } else if (nextChar == EOF)
        {
            nextChar = ' ';
        }
        
        letterCount++;
        char* tmpchar = realloc(curr, letterCount*sizeof(char) + sizeof(char));
        if (tmpchar == NULL)
        {
            free(curr);
            free(ptrarray);
            exit(1);
        } else
        {
            curr = tmpchar;
            curr[letterCount] = nextChar;
        }
    }
    
    qsort(ptrarray, wordCount, sizeof(char*), pre_frobcmp);
    
    size_t i;
    size_t j;
    for (i = 0; i < wordCount; i++)
    {
        for (j = 0; ; j++)
        {
            putchar(ptrarray[i][j]);
            if (ptrarray[i][j] == ' ')
                break;
        }
    }
    
    size_t k;
    for (k = 0; k < wordCount; k++)
        free(ptrarray[k]);
    free(ptrarray);
    return 0;
}

