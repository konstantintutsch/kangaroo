#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define MAX_ARRAY_LENGTH 16
#define MAX_FILEEXT_LENGTH 8
#define MAX_DIRNAME_LENGTH 256

/**
 * endswith - Checks if string ends with extension from array `exts`
 *
 * Return - 0 No
 *          1 Yes
 */

int endswith (char *str,
              char exts[MAX_ARRAY_LENGTH][MAX_FILEEXT_LENGTH])
{
  int withext = 0;

  for (int ext = 0; ext < MAX_ARRAY_LENGTH; ext++)
  {
    if (exts[ext][0] != 0)
    {
      if (strncmp (str + strlen (str) - strlen (exts[ext]), exts[ext], strlen (exts[ext])) == 0)
        withext = 1;
    }
  }

  return withext;
}

/**
 * arg_value - Filter out the value from @arg1 
 *             e. g. `-d./src` -> `./src`
 *
 * Return - String
 */

char *arg_value(char *argument)
{
    /* Arguments should always start with a dash */
    if (argument[0] != '-')
    {
        printf("Missing dash. Not an argument: %s\n", argument);
        return NULL;
    }

    char *value = malloc(strlen(argument));

    /* Write string to value, skip first two characters */
    memcpy (value, argument + 2, strlen(argument));
    /* End string */
    value[strlen(argument)] = 0;

    return value;
}

/**
 * main - Entry point of code
 *
 * Return - 0 Success
 */

int main (int   argc,
          char *argv[])
{
  unsigned long total_lines = 0;
  char langs[MAX_ARRAY_LENGTH][MAX_FILEEXT_LENGTH];
  char dirs[MAX_ARRAY_LENGTH][MAX_DIRNAME_LENGTH];

  DIR *directory;
  struct dirent *ep;
  
  FILE *file;
  char file_path[MAX_DIRNAME_LENGTH + 32 + MAX_FILEEXT_LENGTH];
  unsigned long file_lines = 0;
  char *buffer = NULL;
  size_t llen = 0;
  ssize_t lread;

  for (int i = 0; i < MAX_ARRAY_LENGTH; i++)
  {
    langs[i][0] = 0;
    dirs[i][0] = 0;
  }

    for (int argument_index = 1; argument_index < argc; argument_index++)
    {
        char *value_buffer;
        char type_buffer;

        int counter_langs = 0;
        int counter_dirs = 0;

        value_buffer = arg_value(argv[argument_index]);
        if (value_buffer == NULL)
            continue;

        switch (argv[argument_index][1])
        {
            case 'l':
                strcpy(langs[counter_langs], value_buffer);
                counter_langs++;
                break;
            case 'd':
                strcpy(dirs[counter_dirs], value_buffer);
                counter_dirs++;
                break;
            default:
                printf("Unknown argument type '%c', skipping.\n", argv[argument_index][1]);
                type_buffer = 0;
        }

        free(value_buffer);
    }

  printf ("Languages:");
  for (int lang = 0; lang < MAX_ARRAY_LENGTH; lang++)
  {
    if (langs[lang][0] != 0)
    {
      printf (" %s", langs[lang]);
    }
  }
  printf ("\n");
  for (int dir = 0; dir < MAX_ARRAY_LENGTH; dir++)
  {
    if (dirs[dir][0] != 0)
    {
      printf ("Directory %s", dirs[dir]);
      
      directory = opendir (dirs[dir]);

      if (directory != NULL)
      {
        printf ("\n");

        while ((ep = readdir (directory)) != NULL)
        {
          if (endswith (ep->d_name, langs))
          {
            sprintf (file_path, "%s/%s", dirs[dir], ep->d_name);
            printf ("- %s: ", ep->d_name);
            file = fopen (file_path, "r");
            if (file == NULL)
            {
              printf ("%s\n", strerror (errno));
              file_lines = 0;
            }
            else
            {
              while ((lread = getline (&buffer, &llen, file)) != -1)
                file_lines++;
              fclose (file);

              printf ("%ld\n", file_lines);
            }

            total_lines += file_lines;
            file_lines = 0;
          }
        }
      }
      else
      {
        printf (": %s\n", strerror (errno));
      }
    }
  }
  printf ("Total lines of code: %ld\n", total_lines);

  return (0);
}
