#ifndef _FILEIO_H_
#define _FILEIO_H_

#include <stdio.h>

#define MAX_LINE	1000
#define MAX_WORDS	500

int fgetLine(FILE *fp, char line[], int max);
int getWords(char *line, char *words[], int maxwords);
int loadConfig(char *filePath, char *fileName);
int loadTemp(char *filePath, char *fileName);
int saveConfig(char *filePath, char *fileName);
int saveTemp(char *filePath, char *fileName);
char *getHomeDir(char *home);

extern char *homeDir;

#endif /* _FILEIO_H_ */
