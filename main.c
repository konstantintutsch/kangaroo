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
#include <dirent.h>
#include <errno.h>

#include "files.h"
#include "parse.h"


#define EXT_PREFIX 'e'
#define DIR_PREFIX 'd'

#define BASE_LENGTH 64
#define EXT_LENGTH 8
#define DIR_LENGTH 256


/**
 * main - Entry point of code
 *
 * @return: 0 -> Success
 */

int main(int   argc,
         char *argv[])
{
    unsigned long total_lines = 0;

    /* Dynamic assigning of arrays of strings */
    int e_arguments = arg_type(argv, 'e');
    int d_arguments = arg_type(argv, 'd');

    char **extensions;
    extensions = malloc(e_arguments * sizeof(char *)); /* allocate space for e_arguments char* pointers*/
    for (int i = 0; i < e_arguments; i++)
        extensions[i] = malloc(EXT_LENGTH + 1); /* allocate space for EXT_LENGTH + '\0' */

    char **directories;
    directories = malloc(d_arguments * sizeof(char *));
    for (int i = 0; i < d_arguments; i++)
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

    for (int i = 0; i < e_arguments; i++)
        printf(" %s", extensions[i]);

    printf("\n");


    for (int i = 0; i < d_arguments; i++)
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
