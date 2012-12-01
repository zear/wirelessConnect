#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

char *homeDir = NULL;

// Reads one line from fp
int fgetLine(FILE *fp, char line[], int max)
{
	int nch = 0;
	int c;
	max = max - 1;		/* leave room for '\0' */

	int ignoreLine = 0;

	while((c = getc(fp)) != EOF)
	{
		if(c == '\n')
		{
			ignoreLine = 0;
			break;
		}

		/* We're treating anything after '#' as comments and ignoring the rest of the line */
		if(c == '#')
		{
			ignoreLine = 1;
		}

		if(nch < max)
		{
			if(!ignoreLine)
			{
				line[nch] = c;
				nch = nch + 1;
			}
		}
	}

	if(c == EOF && nch == 0)
	{
		return EOF;
	}

	line[nch] = '\0';
	return nch;
}
//breaks a line into words
int getWords(char *line, char *words[], int maxWords)
{
	char *p = line;
	char lastChar;
	int nWords = 0;
	int doLoop = 1;
	int isWord = 0;
	int ignoreSpace = 0;

	while(doLoop)
	{
		if(*p == '\0' || nWords >= maxWords)
		{
			break;
		}

		if(*p == '"' && lastChar != '\\')
		{
			if(ignoreSpace)
			{
				isWord = 0;
				*p = '\0';
				ignoreSpace = 0;
			}
			else
			{
				ignoreSpace = 1;
			}
			lastChar = *p;
			p++;
		}
		else if(isspace(*p) && !ignoreSpace)
		{
			if(isWord)
			{
				isWord = 0;
				*p = '\0';
			}
			lastChar = *p;
			p++;
		}
		else
		{
			if(!isWord)
			{
				if(p != line) // don't do if it's the first character
				{
					nWords++;
				}
				words[nWords] = p;
				isWord = 1;
			}
			lastChar = *p;
			p++;
		}
	}

	return nWords;
}

int loadConfig(char *filePath, char *fileName)
{
	FILE *ifp;
	char *newFileName = NULL;
	char line[MAX_LINE];
	char *words[MAX_WORDS];
	int n;
	int i;
	int j;
	int wordsSize;

	newFileName = (char *)malloc(strlen(filePath) + strlen(fileName) + 1);
	if(newFileName == NULL)
	{
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	strcpy(newFileName, filePath);
	strcat(newFileName, fileName);

	printf("Loading config from: %s\n", newFileName);
	ifp = fopen(newFileName, "r");
	if(ifp == NULL)
	{
		fprintf(stderr, "ERROR (loadConfig): Cannot open config file: %s\n", newFileName);
		return 1;
	}

	while(fgetLine(ifp, line, MAX_LINE) != EOF)
	{
		n = getWords(line, words, MAX_WORDS);

		if(!strcmp(words[0], "INTERFACE:"))
		{
			if(n < 1)
			{
				fprintf(stderr, "WARNING (loadConfig): No INTERFACE specified!\n");
			}
			else
			{
				memset(&CurNetwork.interface, 0, sizeof(CurNetwork.interface)); // clean the default interface
				strcpy(CurNetwork.interface, words[1]);
			}
		}
		if(!strcmp(words[0], "MODE:"))
		{
			if(n < 1)
			{
				fprintf(stderr, "WARNING (loadConfig): No MODE specified!\n");
			}
			else
			{
				CurNetwork.mode = atoi(words[1]);
			}
		}
		if(!strcmp(words[0], "ESSID:"))
		{
			if(n < 1)
			{
				fprintf(stderr, "WARNING (loadConfig): No ESSID specified!\n");
			}
			else
			{
				strcat(CurNetwork.essid, words[1]);
			}
		}
		if(!strcmp(words[0], "ENCRYPTION:"))
		{
			if(n < 1)
			{
				fprintf(stderr, "WARNING (loadConfig): No ENCRYPTION specified!\n");
			}
			else
			{
				CurNetwork.encryption = atoi(words[1]);
			}
		}
		if(!strcmp(words[0], "KEY:"))
		{
			if(n < 1)
			{
				fprintf(stderr, "WARNING (loadConfig): No password specified!\n");
			}
			else
			{
				strcat(CurNetwork.key, words[1]);
			}
		}
		if(!strcmp(words[0], "DHCP:"))
		{
		}
		if(!strcmp(words[0], "IP:"))
		{
		}
		if(!strcmp(words[0], "NETMASK:"))
		{
		}
	}

	fclose(ifp);

	return 0;
}

int loadTemp(char *filePath, char *fileName)
{
	FILE *ifp;
	char *newFileName = NULL;
	char line[MAX_LINE];
	char *words[MAX_WORDS];
	int n;
	int i;
	int j;
	int wordsSize;

	newFileName = (char *)malloc(strlen(filePath) + strlen(fileName) + 1);
	if(newFileName == NULL)
	{
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	strcpy(newFileName, filePath);
	strcat(newFileName, fileName);

	printf("Loading config from: %s\n", newFileName);
	ifp = fopen(newFileName, "r");
	if(ifp == NULL)
	{
		fprintf(stderr, "ERROR (loadTemp): Cannot open temp file: %s\n", newFileName);
		return 1;
	}

	while(fgetLine(ifp, line, MAX_LINE) != EOF)
	{
		n = getWords(line, words, MAX_WORDS);

		if (n > 0)
		if(!strcmp(words[0], "STATUS:"))
		{
			if(n < 1)
			{
				fprintf(stderr, "WARNING (loadTemp): No STATUS specified!\n");
				CurNetwork.status = STATUS_OFF;
			}
			else
			{
				CurNetwork.status = atoi(words[1]);
			}
		}
	}

	fclose(ifp);

	return 0;
}

int saveConfig(char *filePath, char *fileName)
{
	FILE *ofp;
	char *newFileName = NULL;

	newFileName = (char *)malloc(strlen(filePath) + strlen(fileName) + 1);
	if(newFileName == NULL)
	{
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	strcpy(newFileName, filePath);
	strcat(newFileName, fileName);

	ofp = fopen(newFileName, "w");
	if(ofp == NULL)
	{
		fprintf(stderr, "ERROR: (saveConfig) Can't open %s\n", newFileName);
		free(newFileName);
		return 1;
	}

	printf("Saving config\n");
	fprintf(ofp, "INTERFACE: \"%s\"\n", CurNetwork.interface);
	fprintf(ofp, "MODE: %d\n", CurNetwork.mode);
	fprintf(ofp, "ENCRYPTION: %d\n", CurNetwork.encryption);
	fprintf(ofp, "ESSID: \"%s\"\n", CurNetwork.essid);
	fprintf(ofp, "KEY: \"%s\"\n", CurNetwork.key);
	fclose(ofp);
	free(newFileName);

	return 0;
}

int saveTemp(char *filePath, char *fileName)
{
	FILE *ofp;
	char *newFileName = NULL;

	newFileName = (char *)malloc(strlen(filePath) + strlen(fileName) + 1);
	if(newFileName == NULL)
	{
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	strcpy(newFileName, filePath);
	strcat(newFileName, fileName);

	ofp = fopen(newFileName, "w");
	if(ofp == NULL)
	{
		fprintf(stderr, "ERROR: (saveTemp) Can't open %s\n", newFileName);
		free(newFileName);
		return 1;
	}

	fprintf(ofp, "#Temporary file used by wirelessConnect\n");
	fprintf(ofp, "STATUS: %d\n", CurNetwork.status);
	fclose(ofp);
	free(newFileName);

	return 0;
}

char *getHomeDir(char *home)
{
	if(home != NULL)
	{
		free(home);
	}

	home = (char *)malloc(strlen(getenv("HOME")) + strlen("/.wiCon") + 1);
	strcpy(home, getenv("HOME"));
	strcat(home, "/.wiCon");
	mkdir(home, 0755); // create $HOME/.wiCon if it doesn't exist
	printf("home: %s\n", home);

	return home;
}
