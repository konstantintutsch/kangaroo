/**
 * Copyright (c) 2024 Konstantin Tutsch.
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

#include "parse.h"

#include <stdlib.h>

/**
 * array_elements - Counts how many elements exist in an array of pointers to strings
 *
 * @arg1: array
 *
 * @return: counted elements
 */

int array_elements(char **array)
{
    int count = 0;

    while (array[count] != NULL)
        count++;

    return (count);
}
