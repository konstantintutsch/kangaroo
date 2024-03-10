/**
 * Copyright (c) 2023-2024 Konstantin Tutsch.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define MAX_ARRAY_LENGTH 16
#define MAX_FILEEXT_LENGTH 8
#define MAX_DIRNAME_LENGTH 256

/**
 * cmp_extensions - Checks if string ends with string from array of strings
 * @arg1: string to check
 * @arg2: valid extensions
 *
 * Return - 0 Success
 *          1 Failure
 */

int cmp_extensions (char *file_name,
                    char  extensions[MAX_ARRAY_LENGTH][MAX_FILEEXT_LENGTH])
{
    int status = 1;

    for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        if (extensions[i][0] == 0)
            continue;

        if (strncmp (file_name + strlen(file_name) - strlen(extensions[i]), /* End of file_name minus extension length -> extension as string */
                     extensions[i], /* Compare to actual extension */
                     strlen(extensions[i]) /* Only compare actual extension */) == 0)
            status = 0;
    }

    return (status);
}

/**
 * arg_value - Filter out the value from a command line argument 
 *             e. g. `-d./src` -> `./src`
 * @arg1: command line argument
 *
 * Return - String
 */

char *arg_value(char *argument)
{
    /* Arguments should always start with a dash */
    if (argument[0] != '-')
    {
        printf("Missing dash. Not an argument: %s\n", argument);
        return (NULL);
    }

    char *value = malloc(strlen(argument));

    /* Write string to value, skip first two characters */
    memcpy (value, argument + 2, strlen(argument));
    /* End string */
    value[strlen(argument)] = 0;

    return (value);
}

/**
 * file_size - Get file size of file
 * @arg1: file path
 *
 * Return - Long Int
 */

long int file_size(char *path)
{
    long int size;

    FILE *file;

    file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Error getting file size of %s: %s\n", path, strerror(errno));
        return (0);
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    fclose(file);
    return (size);
}

/**
 * count_lines - Count how many lines a file has
 * @arg1: file path
 *
 * Return - Int
 */

int count_lines(char *path)
{
    int lines = 0;

    FILE *file;
    char *buffer = malloc (file_size(path));
    size_t llen = 0;
    ssize_t lread;

    file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Error opening file %s: %s\n", path, strerror(errno));
        return (-1);
    }

    while ((lread = getline(&buffer, &llen, file)) != -1)
        lines++;
                
    fclose (file);
    free(buffer);
    return (lines);
}

/**
 * main - Entry point of code
 *
 * Return - 0 Success
 */

int main (int   argc,
          char *argv[])
{
    unsigned long total_lines = 0;
    char extensions[MAX_ARRAY_LENGTH][MAX_FILEEXT_LENGTH];
    char directories[MAX_ARRAY_LENGTH][MAX_DIRNAME_LENGTH];

    /*  */
    for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        extensions[i][0] = 0;
        directories[i][0] = 0;
    }

    for (int i = 1; i < argc; i++)
    {
        char *value_buffer;
        char type_buffer;

        int counter_exts = 0;
        int counter_dirs = 0;

        value_buffer = arg_value(argv[i]);
        if (value_buffer == NULL)
            continue;

        switch (argv[i][1])
        {
            case 'f':
                strcpy(extensions[i], value_buffer);
                counter_exts++;
                break;
            case 'd':
                strcpy(directories[i], value_buffer);
                counter_dirs++;
                break;
            default:
                printf("Unknown argument type '%c', skipping.\n", argv[i][1]);
                type_buffer = 0;
        }

        free(value_buffer);
    }

    /* List all recognized file types */
    printf ("File types:");
    for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        if (extensions[i][0] == 0)
            continue;
        
        printf (" %s", extensions[i]);
    }
    printf ("\n");

    for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
    {
        if (directories[i][0] == 0)
            continue;

        DIR *directory;
        struct dirent *element;
  
        directory = opendir(directories[i]);
        if (directory == NULL)
        {
            printf ("Error opening directory %s: %s\n", directories[i], strerror(errno));
            continue;
        }

        printf ("Directory %s\n", directories[i]);
        while ((element = readdir(directory)) != NULL)
        {
            if (cmp_extensions (element->d_name, extensions) != 0)
                continue;

            int lines;
            char path[MAX_DIRNAME_LENGTH + 32 + MAX_FILEEXT_LENGTH];

            sprintf (path, "%s/%s", directories[i], element->d_name); /* merge directory path and whole file name */
            lines = count_lines(path);

            if (lines == -1)
                continue;
            
            printf ("- %s: %d\n", element->d_name, lines);
            total_lines += lines;
        }
    }

    printf ("Total lines of code: %ld\n", total_lines);
    return (0);
}
