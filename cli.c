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

#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

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
 * arg_type - Count how many arguments of a given type exist
 *
 * @arg1: arguments
 * @arg2: type, e. g. d,e
 *
 * @return: Count of arguments
 */

int arg_type(char **arguments,
             char   type)
{
    int amount = 0;

    for (int i = 0; i < array_elements(arguments); i++)
    {
        if (arguments[i][1] == type)
            amount++;
    }

    return amount;
}
