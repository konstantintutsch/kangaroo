#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define MAX_ARRAY_LENGTH 16
#define MAX_FILEEXT_LENGTH 8
#define MAX_DIRNAME_LENGTH 256


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

  for (int arg = 1; arg < argc; arg++)
  {
    if (argv[arg][0] == '-')
    {
      /* argv + 2: remove the e. g. "-d"
       * memcpy (…) writes argv + 2 to argv (end) into langs
       * last byte becomes a '\0' to end the string correctly
       */
      if (argv[arg][1] == 'l')
      {
        langs[arg][0] = '.';
        memcpy (langs[arg] + 1, argv[arg] + 2, strlen(argv[arg]));
        langs[arg][strlen(langs[arg])] = 0;
      }
      else if (argv[arg][1] == 'd')
      {
        memcpy (dirs[arg], argv[arg] + 2, strlen(argv[arg]));
        dirs[arg][strlen(dirs[arg])] = 0;
      }
      else
      {
        printf ("Unknown parameter: -%c\n", argv[arg][1]);
        return (1);
      }
    }
    else
    {
      //printf ("Ignoring: %s\n", argv[arg]);
    }
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
