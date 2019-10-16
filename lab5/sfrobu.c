#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>

int f_opt = 0;

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
    
    if (f_opt)
    {
        return (toupper((*a)^42) < toupper((*b)^42) ? -1:1);
    } else
    {
        return (((*a)^42) < ((*b)^42) ? -1:1);
    }
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
        printf("IO Error detected.");
        return 1;
    }
    return 0;
}

size_t init_buffer(char** arr, char* buffer, size_t size)
{
    size_t i, lineNum;
    char* line = buffer;
    for (i = 0, lineNum = 0; i < size; i++)
    {
        if (buffer[i] == ' ')
        {
            arr[lineNum++] = line;
            line = buffer + (i + 1);
        }
    }
    return lineNum;
}


int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "Incorrect number of arguments.\n");
        exit(1);
    } else if (argc == 2)
    {
        if (strcmp(argv[1], "-f") == 0)
        {
            f_opt = 1;
        } else
        {
            fprintf(stderr, "Invalid option. Only -f is allowed.\n");
            exit(1);
        }
    }

    ssize_t ret;
    struct stat buf;
    size_t fileSize;
    char* input;

    int (*cmp) (const void*, const void*);
    cmp = &pre_frobcmp;
    ret = fstat(STDIN_FILENO, &buf);
    fileSize = buf.st_size + 1;
    input = (char*) malloc(sizeof(char) * fileSize);

    char** ptrarray;
    int letterCount = 0;
    int wordCount = 0;
    if (errorExists() || input == NULL)
    {
        free(input);
        exit(1);
    }

    size_t line_length = 0;
    size_t buffer_size = 0;
    size_t num_lines = 0;
    int file_end = 0;
    int charSpace;
    char read_buffer[2];
    char curr;
    char* mem_tmp;
    while (!file_end)
    {
        ret = read(STDIN_FILENO, read_buffer, 1);
        if (errorExists())
        {
            fprintf(stderr, "IO Error\n");
            exit(1);
        }
        curr = read_buffer[0];
        charSpace = (curr == ' ');
        file_end = !ret;
        if (!line_length && charSpace)
        {
            continue;
        }
        if (buffer_size == fileSize)
        {
            fileSize *= 2;
            mem_tmp = (char*) realloc(input, sizeof(char) * fileSize);
            if (mem_tmp == NULL)
            {
                fprintf(stderr, "Memory Error");
                exit(1);
            }
            input = mem_tmp;
        }
        
        if (!file_end)
        {
            input[buffer_size++] = curr;
            line_length++;
            if (!charSpace)
            {
                continue;
            }
        } else
        {
            if (buffer_size == 0)
            {
                free(input);
                return 0;
            }
            if (input[buffer_size - 1] != ' ')
            {
                input[buffer_size++] = ' ';
            }
            if (line_length == 0)
            {
                break;
            }
        }
        num_lines++;
        line_length = 0;
    }
    ptrarray = (char**) malloc(sizeof(char*) * num_lines);
    if (ptrarray == NULL)
    {
        fprintf(stderr, "Memory Error.\n"); 
        exit(1);
    }
    if (num_lines != init_buffer(ptrarray, input, buffer_size))
    {
        fprintf(stderr, "Line Number is incorrect.\n");
        exit(1);
    }
    qsort(ptrarray, num_lines, sizeof(char*), cmp);
    size_t i;
    for (i = 0; i < num_lines; i++)
    {
        while (1)
        {
            ret = write(STDOUT_FILENO, ptrarray[i], 1);
            if (errorExists())
            {
                fprintf(stderr, "IO Error.\n");
                exit(1);
            }
            if (*ptrarray[i]++ == ' ')
            {
                break;
            }
        }
    }
    free(ptrarray);
    free(input);
    return 0;
}
