#ifndef FILES_H
#define FILES_H

long int file_size(char *path);
int count_lines(char *path);
long int count_directory(char *path, char **extensions, int recursive);

#endif
