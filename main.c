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


#define EXT_PREFIX 'e'
#define DIR_PREFIX 'd'

#define BASE_LENGTH 64
#define EXT_LENGTH 8
#define DIR_LENGTH 256


int extension_arguments = 0;
int directory_arguments = 0;


/**
 * cmp_extensions - Checks if string ends with string from array of strings
 * 
 * @arg1: string to check
 * @arg2: valid extensions
 *
 * @return: 0 -> Success / 1 -> Failure
 */

int cmp_extensions(char  *file_name,
                   char **extensions)
{
    for (int i = 0; i < extension_arguments; i++)
    {
        if (strncmp(file_name + strlen(file_name) - strlen(extensions[i]), /* End of file_name minus extension length -> extension as string */
                     extensions[i], /* Compare to actual extension */
                     strlen(extensions[i]) /* Only compare actual extension */) != 0)
            continue;

        return (0);
    }

    return (1);
}

/**
 * arg_value - Filter out the value from a command line argument
 *             e. g. `-d./src` -> `./src`
 *
 * @arg1: command line argument
 *
 * @return: value of the argument
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
    memcpy(value, argument + 2, strlen(argument));
    /* End string */
    value[strlen(argument)] = 0;

    return (value);
}

/**
 * file_size - Get file size of file
 *
 * @arg1: file path
 *
 * @return: size of the given file
 */

long int file_size(char *path)
{
    long int size;

    FILE *file;

    file = fopen(path, "r");
    if (file == NULL)
    {
        printf("- Error getting file size of %s: %s\n", path, strerror(errno));
        return (0);
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    fclose(file);
    return (size);
}

/**
 * count_lines - Count how many lines a file has
 *
 * @arg1: file path
 *
 * @return: line count of the given file
 */

int count_lines(char *path)
{
    int lines = 0;

    FILE *file;
    char *buffer = malloc(file_size(path));
    size_t llen = 0;
    ssize_t lread;

    file = fopen(path, "r");
    if (file == NULL)
    {
        printf("- Error opening file %s: %s\n", path, strerror(errno));
        return (-1);
    }

    while ((lread = getline(&buffer, &llen, file)) != -1)
        lines++;

    fclose(file);
    free(buffer);
    return (lines);
}

/**
 * main - Entry point of code
 *
 * @return: 0 -> Success
 */

int main(int   argc,
         char *argv[])
{
    unsigned long total_lines = 0;

    /**
     * Count argument types for dynamic assigning
     * extension_arguments and directory_arguments are definde globally -> access in every for loop
     */
    for (int i = 0; i < argc; i++)
    {
        switch (argv[i][1])
        {
            case EXT_PREFIX:
                extension_arguments++;
                break;
            case DIR_PREFIX:
                directory_arguments++;
                break;
        }
    }
    /* Dynamic assigning of arrays of strings */
    char **extensions;

    extensions = malloc(extension_arguments * sizeof(char *)); /* allocate space for extension_arguments char* pointers*/
    for (int i = 0; i < extension_arguments; i++)
        extensions[i] = malloc(EXT_LENGTH + 1); /* allocate space for EXT_LENGTH + '\0' */

    char **directories;

    directories = malloc(directory_arguments * sizeof(char *));
    for (int i = 0; i < directory_arguments; i++)
        directories[i] = malloc(DIR_LENGTH + 1);


    int counter_exts = 0;
    int counter_dirs = 0;
    for (int i = 1; i < argc; i++)
    {
        char *value_buffer;

        value_buffer = arg_value(argv[i]);
        value_buffer[strlen(value_buffer) + 1] = '\0';
        if (value_buffer == NULL)
            continue;

        switch (argv[i][1])
        {
            case EXT_PREFIX:
                strcpy(extensions[counter_exts], value_buffer);
                counter_exts++;
                break;
            case DIR_PREFIX:
                strcpy(directories[counter_dirs], value_buffer);
                counter_dirs++;
                break;
            default:
                printf("Unknown argument type '%c', skipping.\n", argv[i][1]);
        }

        free(value_buffer); /* free local variable from function arg_value() */
    }


    /* List all recognized file types */
    printf("File extensions:");

    for (int i = 0; i < extension_arguments; i++)
        printf(" %s", extensions[i]);

    printf("\n");


    for (int i = 0; i < directory_arguments; i++)
    {
        DIR *directory;
        struct dirent *element;
  
        directory = opendir(directories[i]);
        if (directory == NULL)
        {
            printf("\nError opening directory %s: %s\n", directories[i], strerror(errno));
            continue;
        }

        printf("\nDirectory %s\n", directories[i]);

        while ((element = readdir(directory)) != NULL)
        {
            if (cmp_extensions(element->d_name, extensions) != 0) /* files extension doesn't match */
                continue;

            int lines;
            char path[DIR_LENGTH + BASE_LENGTH + EXT_LENGTH];

            sprintf(path, "%s/%s", directories[i], element->d_name); /* merge directory path and whole file name */
            lines = count_lines(path);

            if (lines == -1) /* could not get lines */
                continue;

            printf("- %s: %d\n", element->d_name, lines);
            total_lines += lines;
        }
    }

    printf("\nTotal lines of code: %ld\n", total_lines);
    return (0);
}
