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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"


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
    for (int i = 0; i < array_elements(extensions); i++)
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
 * deduplicate - Check if an array contains an item more than once
 *
 * @arg1: Array
 * @arg2: String to check for
 *
 * @return:  -1 No duplication
 *          <=0 First position of duplicate item
 */

int deduplicate(char **array,
                char  *match)
{
    int first = -1;
    int count = 0;

    for (int i = 0; i < array_elements(array); i++)
    {
        if (strcmp(array[i], match) != 0)
            continue;

        if (first == -1)
            first = i;

        count++;
        if (count > 1)
            return first;
    }

    return (-1);
}
