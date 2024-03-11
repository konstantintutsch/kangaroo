#ifndef FILES_H
#define FILES_H

unsigned long int file_size(char *path);
unsigned int count_lines(char *path);
unsigned long long int count_directory(char *path, char **extensions, int recursive);

#endif
