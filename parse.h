#ifndef PARSE_H
#define PARSE_H

int array_elements(char **array);
int cmp_extensions(char *file_name, char **extensions);
char *arg_value(char *argument);
int arg_type(char **arguments, char type);

#endif
