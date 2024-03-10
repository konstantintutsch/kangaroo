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
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

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
