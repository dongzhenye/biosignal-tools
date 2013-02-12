
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

extern int DoReadTest(const char* filePath);
extern int DoWriteTest(const char* filePath);

static int
GetNextCommand(char* filePath)
{
	char input[1024];
	int command;
	int i;

	*filePath = 0;
	
	while(1) {
		printf("Command: ");
		if (fgets(input, sizeof(input)-1, stdin) != NULL) {
			char* p = strchr(input, '\n');			// Find new line character.
			if (p != NULL)
				*p = 0;								// Remove new line character.
			for(i=0; i<5; i++)
				input[i] = toupper(input[i]);		// Convert command to upper case.
			if (strncmp(input, "READ ", 5) == 0) {
				command = 1;
				strcpy(filePath, input+5);
				break;
			}
			if (strncmp(input, "WRITE ", 6) == 0) {
				command = 2;
				strcpy(filePath, input+6);
				break;
			}
			if (strncmp(input, "QUIT", 4) == 0) {
				command = 3;
				break;
			}
		}
		printf("\n\n");
	}
	
	return command;
}

int
main(void)
{
	int command;
	char filePath[1024];
	char fullPathSample[32];
	char partialPathSample[32];
	int err;
	
	#ifdef WIN32
		strcpy(fullPathSample, "C:\\FolderA\\MyWave.ibw");
		strcpy(partialPathSample, "..\\SamplesWin\\version2.ibw");
	#else
		strcpy(fullPathSample, "hd:FolderA:MyWave.ibw");
		strcpy(partialPathSample, "::SamplesMac:version2.ibw");
	#endif
	
	printf("This program illustrates reading and writing Igor binary wave files.\n\n");
	printf("To read a file, enter the following: Read <file path>\n\n");
	printf("To write a file, enter the following: Write <file path>\n\n");
	printf("<file path> can be either a full path (%s)\n", fullPathSample);
	printf("or a partial path relative to the folder containing this program (%s)\n", partialPathSample);
	printf("\n");
	printf("Here are some sample commands:\n");
	#ifdef WIN32
		printf("  Read ..\\SamplesMac\\version2.ibw\n");
		printf("  Read ..\\SamplesWin\\version5.ibw\n");
		printf("  Write TestWave.ibw\n");
	#else
		printf("  Read ::SamplesMac:version2.ibw\n");
		printf("  Read ::SamplesWin:version5.ibw\n");
		printf("  Write TestWave.ibw\n");
	#endif

	#ifdef WIN32
		printf("To quit the program, type Quit\n");
	#else
		printf("To quit the program, choose Quit from the file menu.\n");
	#endif
	
	while(1) {
		printf("\n");
		command = GetNextCommand(filePath);
		if (command == 1)
			err = DoReadTest(filePath);
		if (command == 2)
			err = DoWriteTest(filePath);
		if (command == 3) {
			#ifndef WIN32
				printf("Now you must choose Quit from the File menu to quit.\n");
			#endif
			break;
		}
	}
	
	return 0;	
}

