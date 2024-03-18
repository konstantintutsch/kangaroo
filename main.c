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
    Flag ext = {"extension", 'e', 1, "File extension to scan lines of code"};
    Flag dir = {"directory", 'd', 1, "Directories to scan through"};
    Flag rec = {"recursive", 'r', 0, "Recursion for scanning directories"};
    Flag ign = {"ignore",    'i', 1, "Exclude a directory from being scanned"};

    int recursive = 0;

    int e_arguments = flag_count(argv, &ext);
    int d_arguments = flag_count(argv, &dir);
    int i_arguments = flag_count(argv, &ign);

    char **extensions = malloc(e_arguments * sizeof(char *));
    char **directories = malloc(d_arguments * sizeof(char *));
    char **ignores = malloc(i_arguments * sizeof(char *));

    unsigned long long int total_lines = 0;

    /* Analyze arguments */
    if (flag_count(argv, &rec) > 0)
        recursive = 1;
    extensions = flag_value(argv, &ext);
    directories = flag_value(argv, &dir);
    ignores = flag_value(argv, &ign);

    /* Convert path from arguments to absolute to avoid duplication */
    directories = convert_absolute(directories);
    ignores = convert_absolute(ignores);

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

        total_lines += count_directory(directories[i], extensions, ignores, recursive);
    }

    printf("\nTotal lines of code: %lld\n", total_lines);
    return (0);
}
