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

#include "files.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

#include "compare.h"


/**
 * file_size - Get file size of file
 *
 * @arg1: file path
 *
 * @return: size of the given file
 */

unsigned long int file_size(char *path)
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

unsigned int count_lines(char *path)
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
        return (0);
    }

    while ((lread = getline(&buffer, &llen, file)) != -1)
        lines++;

    fclose(file);
    free(buffer);
    return (lines);
}

/**
 * count_directory - Loop through the elements of a directory and count all lines of files
 *
 * @arg1: Path of directory
 * @arg2: Array of extensions for files that should match
 * @arg3: Recursive: 0 No
 *                   1 Yes
 *
 * @return: Total lines in directory
 */

unsigned long long int count_directory(char  *path,
                                       char **extensions,
                                       int    recursive)
{
    unsigned long long int total_lines = 0;

    DIR *directory;
    struct dirent *element;

    directory = opendir(path);

    if (directory == NULL)
    {
        printf("Error opening directory %s: %s\n", path, strerror(errno));
        return (0);
    }

    unsigned long int directory_lines = 0;

    char line_buffer[PATH_MAX + 1024] = {'\0'};
    char list_buffer[sizeof(line_buffer) * 10] = {'\0'}; /* MAX: 1024 files per directory */

    while ((element = readdir(directory)) != NULL)
    {
        if (recursive == 1 &&
            element->d_type == DT_DIR &&
            strcmp(element->d_name, "." ) != 0 &&
            strcmp(element->d_name, "..") != 0)
        {
            char access_path[PATH_MAX];
            sprintf(access_path, "%s/%s", path, element->d_name);
            total_lines += count_directory(access_path, extensions, recursive);
            continue;
        }
        
        if (cmp_extensions(element->d_name, extensions) != 0) /* file's extension doesn't match */
            continue;


        unsigned int lines = 0;
        char file_path[PATH_MAX];
        sprintf(file_path, "%s/%s", path, element->d_name); /* merge directory path and whole file name */
        lines = count_lines(file_path);

        directory_lines += lines;

        /**
         * Using buffer instead of directly printing -> order directories with their files
         * Append new line to list_buffer
         * -> Not using a single sprintf line to prevent buffer overflows
         */
        snprintf(line_buffer, sizeof(line_buffer), "- %s: %d lines\n", element->d_name, lines);
        strncat(list_buffer, line_buffer, sizeof(list_buffer) - 1);
    }

    closedir(directory);

    if (directory_lines > 0)
    {
        total_lines += directory_lines;
        printf("\n%s(All files @ %s: %ld lines)\n", list_buffer, path, directory_lines);
    }

    return (total_lines);
}
