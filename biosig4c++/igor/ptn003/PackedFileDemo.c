/*	This is the main file for the Packed File Demo program
	which writes an Igor Pro 3.x packed experiment file.
	
	See Igor Pro Tech Note PTN#003 for details.
*/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>					// For offsetof macro.

#include "PackedFile.h"
#include "Variables.h"
#include "IgorBin.h"
#include "CrossPlatformFileIO.h"
#include "IgorSupport.h"

#include "PackedFileDemo.h"

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
		strcpy(fullPathSample, "C:\\FolderA\\Test Packed File.pxp");
		strcpy(partialPathSample, ".\\Test Packed File.pxp");
	#else
		strcpy(fullPathSample, "hd:FolderA:Test Packed File");
		strcpy(partialPathSample, ":SamplesMac:Test Packed File");
	#endif
	
	printf("This program illustrates reading and writing Igor packed experiment files.\n\n");
	printf("To read a file, enter the following: Read <file path>\n\n");
	printf("To write a file, enter the following: Write <file path>\n\n");
	printf("<file path> can be either a full path (%s)\n", fullPathSample);
	printf("or a partial path relative to the folder containing this program (%s)\n", partialPathSample);
	printf("\n");
	printf("Here are some sample commands:\n");
	#ifdef WIN32
		printf("  Read Test Packed File.pxp\n");
		printf("  Read .\\Test Packed File.pxp\n");
		printf("  Write Test Packed File.pxp\n");
	#else
		printf("  Read Test Packed File\n");
		printf("  Read :Test Packed File\n");
		printf("  Write Test Packed File\n");
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
			err = ReadPackedFile(filePath);
		if (command == 2)
			err = WritePackedFile(filePath);
		if (command == 3) {
			#ifndef WIN32
				printf("Now you must choose Quit from the File menu to quit.\n");
			#endif
			break;
		}
	}
	
	return 0;	
}
