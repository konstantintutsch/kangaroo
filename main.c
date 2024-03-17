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
#include <limits.h>
#include <zarg.h>

#include "files.h"
#include "compare.h"

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
    /* List of available arguments */
    Flag ext = {"extension", 'e', 's', "File extension to scan lines of code"};
    Flag dir = {"directory", 'd', 's', "Directories to scan through"};
    Flag rec = {"recursive", 'r', 'b', "Recursion for scanning directories"};

    int recursive = 0;

    int e_arguments = argument_count(argv, &ext);
    int d_arguments = argument_count(argv, &dir);

    char **extensions = malloc(e_arguments * sizeof(char *));
    char **directories = malloc(d_arguments * sizeof(char *));

    unsigned long long int total_lines = 0;

    /* Analyze arguments */
    if (argument_count(argv, &rec) > 0)
        recursive = 1;
    extensions = (char **)argument_value(argv, &ext);
    directories = (char **)argument_value(argv, &dir);

    /* Convert path from arguments to absolute to avoid duplication */
    for (int i = 0; i < d_arguments; i++)
    {
        char buffer[PATH_MAX];
        realpath(directories[i], buffer);

        directories[i] = malloc(PATH_MAX * sizeof(char));
        strcpy(directories[i], buffer);
    }

    /* List all recognized file types */
    printf("File extensions:");
    for (int i = 0; i < e_arguments; i++)
        printf(" %s", extensions[i]);
    printf("\n");

    /* Count lines */
    for (int i = 0; i < d_arguments; i++)
    {
        int duplicate = deduplicate(directories, directories[i]);
        if (duplicate != -1 && duplicate < i)
            continue;

        total_lines += count_directory(directories[i], extensions, recursive);
    }

    printf("\nTotal lines of code: %lld\n", total_lines);
    return (0);
}
