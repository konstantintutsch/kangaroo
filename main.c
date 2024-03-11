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
#include <errno.h>

#include "files.h"
#include "parse.h"
#include "compare.h"


#define EXT_PREFIX 'e'
#define DIR_PREFIX 'd'

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
    unsigned long int total_lines = 0;
    int recursive = 0;

    if (arg_type(argv, 'r') > 0)
        recursive = 1;

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
            realpath(value_buffer, directories[counter_dirs]);
            counter_dirs++;
            break;
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
        int duplicate = deduplicate(directories, directories[i]);
        if (duplicate != -1 && duplicate < i)
            continue;

        total_lines += count_directory(directories[i], extensions, recursive);
    }

    printf("\nTotal lines of code: %ld\n", total_lines);
    return (0);
}
