#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char* argv[])
{
    const char* from;
    const char* to;

    if (argc != 3)
    {
        fprintf(stderr, "Error: Incorrect number of arguments: %d\n", argc);
        exit(1);
    }

    from = argv[1];
    to = argv[2];

    if (strlen(from) != strlen(to))
    {
        fprintf(stderr, "Arguments must hav the same length.\n");
        exit(1);
    }

    size_t i;
    size_t j;
    for (i = 0; i < strlen(from); i++)
    {
        for (j = i + 1; j < strlen(from); j++)
        {
            if (from[i] == from[j])
            {
                fprintf(stderr, "Operand \"from\" contains duplicate bytes.\n");
                exit(1);
            }
        }
    }

    char c = getchar();
    size_t k;
    while (c != EOF)
    {
        for (k = 0; k < strlen(from); k++)
        {
            if (c == from[k])
            {
                c = to[k];
                break;
            }
        }
        putchar(c);
        c = getchar();
    }
    return 0;
}
