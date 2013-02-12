/*	The code in this file writes a sample Igor Pro packed experiment file.
	
	See Igor Pro Tech Note PTN#003 for details.
*/

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

// Constants
#define NOMEM 1						// Error code for out of memory.
#define DEMO_WAVE_LENGTH 10			// Number of points in the demo wave.

static void
PrintResult(const char* message, int error)
{
	char buffer[256];
	
	if (error)
		sprintf(buffer, "Error %d: %s", error, message);
	else
		strcpy(buffer, message);
	printf("%s\n", buffer);
}

static void
ReorderBytes(void *p, int bytesPerPoint, long numValues)	// Reverses byte order.
{
	unsigned char ch, *p1, *p2, *pEnd;
	
	pEnd = (unsigned char *)p + numValues*bytesPerPoint;
	while (p < (void *)pEnd) {
		p1 = p;
		p2 = (unsigned char *)p + bytesPerPoint-1;
		while (p1 < p2) {
			ch = *p1;
			*p1++ = *p2;
			*p2-- = ch;
		}
		p = (unsigned char *)p + bytesPerPoint;
	}
}

static void
ReorderShort(void* sp)
{
	ReorderBytes(sp, 2, 1);
}

static void
ReorderLong(void* lp)
{
	ReorderBytes(lp, 4, 1);
}

static void
ReorderDouble(void* dp)
{
	ReorderBytes(dp, 8, 1);
}

static void
ReorderPackedFileRecordHeader(PackedFileRecordHeader* php)
{
	ReorderShort(&php->recordType);
	ReorderShort(&php->version);
	ReorderLong(&php->numDataBytes);
}

static void
ReorderVarHeader2(VarHeader2* vhp)
{
	ReorderBytes(vhp, 2, 6);					// 6 shorts
}

static void
ReorderUserNumVarRec(UserNumVarRec* nvp)
{
	// The name field does not need to be reordered.
	ReorderShort(&nvp->type);
	ReorderShort(&nvp->num.numType);
	ReorderDouble(&nvp->num.realPart);
	ReorderDouble(&nvp->num.imagPart);
}

static void
ReorderUserDependentVarRec(UserDependentVarRec* dvp)
{
	// The name field does not need to be reordered.
	ReorderShort(&dvp->type);
	ReorderShort(&dvp->num.numType);
	ReorderDouble(&dvp->num.realPart);
	ReorderDouble(&dvp->num.imagPart);
	ReorderShort(&dvp->formulaLen);
	// The formula field does not need to be reordered.
}

static void
DisplayTextLines(const char* text, unsigned long textLength)
{
	const char* p1;
	const char* p;
	long bytesInLine, bytesToDisplay, count;
	char line[512];
	int i;

	p1 = text;
	for(i=0; i <= 3; i++) {
		if (i == 3) {
			printf("  Skipping the rest of the text.\n");
			break;
		}
		
		p = strchr(p1, 13);							// Look for next CR.
		if (p == NULL) {
			bytesInLine = text + textLength - p1;
			bytesToDisplay = bytesInLine;
		}	
		else {
			bytesInLine = p+1 - p1;
			bytesToDisplay = bytesInLine - 1;		// Exclude CR.
		}
		count = bytesToDisplay;
		if (count > sizeof(line)-1)
			count = sizeof(line)-1;
		strncpy(line, p1, count);
		line[count] = 0;
		printf("  Line %d: %s\n", i, line);
		p1 += bytesInLine;
		if (p1 < text + textLength) {
			if (*p1 == 10)
				p1 += 1;							// Skip linefeed following carriage return.
		}
		if (p1 >= text + textLength)
			break;									// All done.
	}
}


static int
ReadPackedHeader(CP_FILE_REF fr, int needToReorderBytes, PackedFileRecordHeader* php)
{
	unsigned long numBytesToRead, numBytesRead;
	int err;
	
	numBytesToRead = sizeof(PackedFileRecordHeader);
	if (err = CPReadFile(fr, numBytesToRead, php, &numBytesRead))
		return err;
	
	if (needToReorderBytes)
		ReorderPackedFileRecordHeader(php);

	return 0;
}

static int
ReadVariablesRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	VarHeader2 vh;
	short version;
	unsigned long numBytesToRead, numBytesRead;
	int i;
	int err;
	
	memset(&vh, 0, sizeof(vh));
	
	// First determine if we have a version 1 or version 2 VarHeader structure.
	numBytesToRead = 2;
	if (err = CPReadFile(fr, numBytesToRead, &version, &numBytesRead)) {
		PrintResult("ReadVariablesRecord error", err);
		return err;
	}
	if (needToReorderBytes)
		ReorderShort(&version);
	CPSetFilePosition(fr, -2, 0);				// Back up to the starting point.

	switch(version) {
		case 1:
			numBytesToRead = sizeof(VarHeader1);
			err = CPReadFile(fr, numBytesToRead, &vh, &numBytesRead);
			break;
	
		case 2:
			numBytesToRead = sizeof(VarHeader2);
			err = CPReadFile(fr, numBytesToRead, &vh, &numBytesRead);
			break;
	
		default:
			printf("Unknown VarHeader version: %d\n", version);
			err = -1;
			break;	
	}
	if (err != 0) {
		PrintResult("Error reading VarHeader record", err);
		return err;
	}
	if (needToReorderBytes)
		ReorderVarHeader2(&vh);
	
	printf("--Variables record--\n");
	printf("  System: %d, numeric: %d, string: %d, dependent numeric: %d, dependent string: %d\n", vh.numSysVars, vh.numUserVars, vh.numUserStrs, vh.numDependentVars, vh.numDependentStrs);
	
	// Read system variables.
	for(i = 0; i < vh.numSysVars; i++) {
		float f;
		numBytesToRead = sizeof(float);
		if (err = CPReadFile(fr, numBytesToRead, &f, &numBytesRead)) {
			PrintResult("Error reading system variable", err);
			return err;
		}
		if (needToReorderBytes)
			ReorderLong(&f);
	}
	
	// Read user numeric variables.
	for(i = 0; i < vh.numUserVars; i++) {
		UserNumVarRec nv;
		numBytesToRead = sizeof(nv);
		if (err = CPReadFile(fr, numBytesToRead, &nv, &numBytesRead)) {
			PrintResult("Error reading user numeric variable", err);
			return err;
		}
		if (needToReorderBytes)
			ReorderUserNumVarRec(&nv);
		if (i <= 3) {
			if (i == 3) {
				printf("  Skipping remaining numeric variables.\n");
			}
			else {
				if (nv.num.numType & NT_CMPLX)
					printf("  Numeric variable %s, value=(%g,%g)\n", nv.name, nv.num.realPart, nv.num.imagPart);
				else
					printf("  Numeric variable %s, value=%g\n", nv.name, nv.num.realPart);
			}
		}
	}

	// Read user string variables.
	for(i = 0; i < vh.numUserStrs; i++) {
		UserStrVarRec1 sv1;
		UserStrVarRec2 sv2;
		char buffer[32];
		char ellipsis[16];
		unsigned long pos;
		
		if (version == 1) {
			numBytesToRead = offsetof(UserStrVarRec1, data);
			err = CPReadFile(fr, numBytesToRead, &sv1, &numBytesRead);
			if (err == 0) {
				if (needToReorderBytes)
					ReorderShort(&sv1.strLen);
				// Copy sv1 record to sv2. Only sv2 is used below.
				strcpy(sv2.name, sv1.name);
				sv2.strLen = sv1.strLen;
			}
		}
		else {
			numBytesToRead = offsetof(UserStrVarRec2, data);
			err = CPReadFile(fr, numBytesToRead, &sv2, &numBytesRead);
			if (err == 0) {
				if (needToReorderBytes)
					ReorderLong(&sv2.strLen);
			}
		}
		if (err != 0) {
			PrintResult("Error reading user string variable header", err);
			return err;
		}
		
		CPGetFilePosition(fr, &pos);
		numBytesToRead = sv2.strLen;
		if (numBytesToRead > sizeof(buffer)-1)
			numBytesToRead = sizeof(buffer)-1;
		if (err = CPReadFile(fr, numBytesToRead, buffer, &numBytesRead)) {
			PrintResult("Error reading user string variable", err);
			return err;
		}
		buffer[numBytesRead] = 0;
		if (numBytesRead < (unsigned long)sv2.strLen)			// Read only part of the string?
			strcpy(ellipsis, "...");							// This is used below in printout.
		else
			*ellipsis = 0;		
		
		CPSetFilePosition(fr, pos + sv2.strLen, -1);			// Move to the end of the string.
				
		if (i <= 3) {
			if (i == 3)
				printf("  Skipping remaining string variables.\n");
			else
				printf("  String variable %s, value=\"%s%s\"\n", sv2.name, buffer, ellipsis);
		}
	}
	
	// Read user dependent numeric variables.
	for(i = 0; i < vh.numDependentVars; i++) {
		UserDependentVarRec dv;
		numBytesToRead = sizeof(dv);
		if (err = CPReadFile(fr, numBytesToRead, &dv, &numBytesRead)) {
			PrintResult("Error reading user dependent numeric variable", err);
			return err;
		}
		if (needToReorderBytes)
			ReorderUserDependentVarRec(&dv);
		if (i <= 3) {
			if (i == 3) {
				printf("  Skipping remaining numeric dependent variables.\n");
			}
			else {
				if (dv.num.numType & NT_CMPLX)
					printf("  Numeric dependent variable %s, value=(%g,%g)\n", dv.name, dv.num.realPart, dv.num.imagPart);
				else
					printf("  Numeric dependent variable %s, value=%g\n", dv.name, dv.num.realPart);
			}
		}
	}
	
	// Read user dependent string variables.
	for(i = 0; i < vh.numDependentStrs; i++) {
		UserDependentVarRec dv;
		numBytesToRead = sizeof(dv);
		if (err = CPReadFile(fr, numBytesToRead, &dv, &numBytesRead)) {
			PrintResult("Error reading user dependent string variable", err);
			return err;
		}
		if (needToReorderBytes)
			ReorderUserDependentVarRec(&dv);
		if (i <= 3) {
			if (i == 3) {
				printf("  Skipping remaining string dependent variables.\n");
			}
			else {
				printf("  String dependent variable %s\n", dv.name);
			}
		}
	}
	
	return 0;
}

static int
ReadHistoryRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	char* text;
	unsigned long numBytesToRead, numBytesRead;
	int err;
	
	printf("--History record--\n");
	
	if (numDataBytes > 0) {
		text = malloc(numDataBytes);
		if (text == NULL) {
			printf("ReadHistoryRecord unable to allocate %ld bytes\n", numDataBytes);
			return -1;
		}
		numBytesToRead = numDataBytes;
		if (err = CPReadFile(fr, numBytesToRead, text, &numBytesRead)) {
			PrintResult("Error reading history", err);
			free(text);
			return err;
		}

		DisplayTextLines(text, numDataBytes);
		
		free(text);
	}
	
	return 0;
}

static int
ReadWaveRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	int type;
	long npnts;
	void* waveDataPtr;
	int err;
	
	printf("--Wave record--\n");

	err = ReadWave(fr, &type, &npnts, &waveDataPtr);
	
	// If no error occurred, the main wave data is now available via waveDataPtr.
	
	if (waveDataPtr != NULL)
		free(waveDataPtr);
	
	if (err) {
		PrintResult("Error reading wave", err);
		return err;
	}

	return 0;
}

static int
ReadRecreationRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	char* text;
	unsigned long numBytesToRead, numBytesRead;
	int err;

	printf("--Recreation macros record--\n");
	
	if (numDataBytes > 0) {
		text = malloc(numDataBytes);
		if (text == NULL) {
			printf("ReadRecreationRecord unable to allocate %ld bytes\n", numDataBytes);
			return -1;
		}
		numBytesToRead = numDataBytes;
		if (err = CPReadFile(fr, numBytesToRead, text, &numBytesRead)) {
			PrintResult("Error reading recreation procedures", err);
			free(text);
			return err;
		}

		DisplayTextLines(text, numDataBytes);
		
		free(text);
	}
	
	return 0;
}


static int
ReadProcedureRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	char* text;
	unsigned long numBytesToRead, numBytesRead;
	int err;

	printf("--Procedures record--\n");
	
	if (numDataBytes > 0) {
		text = malloc(numDataBytes);
		if (text == NULL) {
			printf("ReadProcedureRecord unable to allocate %ld bytes\n", numDataBytes);
			return -1;
		}
		numBytesToRead = numDataBytes;
		if (err = CPReadFile(fr, numBytesToRead, text, &numBytesRead)) {
			PrintResult("Error reading procedures", err);
			free(text);
			return err;
		}

		DisplayTextLines(text, numDataBytes);
		
		free(text);
	}
	
	return 0;
}

static int
ReadGetHistoryRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	printf("--Get history record--\n");

	return 0;
}

static int
ReadPackedFileRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	printf("--Packed file record--\n");
	printf("  This type of record is used to store packed notebooks and procedure windows.\n");
	printf("  The format of the record is quite complex and is not documented in PTN003.\n");

	return 0;
}

static int
ReadDataFolderStartRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	unsigned long numBytesToRead, numBytesRead;
	char name[32];
	int err;
	
	printf("--Data folder start record--\n");
	
	numBytesToRead = 32;
	if (err = CPReadFile(fr, numBytesToRead, name, &numBytesRead)) {
		PrintResult("Error reading data folder name", err);
		return err;
	}
	
	printf("  Entering data folder \'%s\'.\n", name);

	return 0;
}

static int
ReadDataFolderEndRecord(CP_FILE_REF fr, long numDataBytes, int needToReorderBytes)
{
	printf("--Data folder end record--\n");

	return 0;
}

static int
ReadPackedRecords(CP_FILE_REF fr)
{
	unsigned long numberOfBytesInFile;
	unsigned long recordStartPos;
	unsigned long numBytesToRead, numBytesRead;
	PackedFileRecordHeader ph;
	unsigned short stemp;
	int needToReorderBytes;
	int err;
	
	/*	First we must determine if the file uses the wrong byte ordering, in which
		case we will need to do reordering. We do this by reading the recordType
		field of the first record. All record types have 0 in the high byte,
		except for a possibly set SUPERCEDED_MASK.
	*/
	numBytesToRead = 2;
	if (err = CPReadFile(fr, numBytesToRead, &stemp, &numBytesRead)) {
		PrintResult("Read file error", err);
		return err;
	}
	needToReorderBytes = 0;
	stemp &= PACKEDRECTYPE_MASK;				// Clear SUPERCEDED_MASK bit.
	if ((stemp & 0xFF00) != 0)					// High byte is non-zero.
		needToReorderBytes = 1;					// The file uses the wrong byte ordering.	
	CPSetFilePosition(fr, -2, 0);				// Back up to the starting point.

	if (needToReorderBytes) {
		#ifdef WIN32
			printf("This file uses big-endian byte ordering.\n");
		#else
			printf("This file uses little-endian byte ordering.\n");
		#endif
	}
	
	// Now read the records.
	CPNumberOfBytesInFile(fr, &numberOfBytesInFile);
	while(1) {
		CPGetFilePosition(fr, &recordStartPos);
		
		if (recordStartPos >= numberOfBytesInFile)
			break;										// No more data in the file.
		
		if (err = ReadPackedHeader(fr, needToReorderBytes, &ph)) {
			PrintResult("ReadPackedHeader error", err);
			return err;
		}
		
		switch(ph.recordType) {
			case kVariablesRecord:						//  1: Contains system numeric variables (e.g., K0) and user numeric and string variables.
				if (err = ReadVariablesRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
				
			case kHistoryRecord:						//  2: Contains the experiment's history as plain text.
				if (err = ReadHistoryRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
				
			case kWaveRecord:							//  3: Contains the data for a wave
				if (err = ReadWaveRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
				
			case kRecreationRecord:						//  4: Contains the experiment's recreation procedures as plain text.
				if (err = ReadRecreationRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
				
			case kProcedureRecord:						//  5: Contains the experiment's main procedure window text as plain text.
				if (err = ReadProcedureRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
				
			case kGetHistoryRecord:						//  6: Not a real record but rather a message to go back and read the history text.
				if (err = ReadGetHistoryRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
				
			case kPackedFileRecord:						//  7: Contains the data for a procedure file or notebook in a packed form.
				if (err = ReadPackedFileRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
				
			case kDataFolderStartRecord:				//  8: Marks the start of a new data folder.
				if (err = ReadDataFolderStartRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
				
			case kDataFolderEndRecord:					// 10: Marks the end of a data folder.
				if (err = ReadDataFolderEndRecord(fr, ph.numDataBytes, needToReorderBytes))
					return err;
				break;
			
			default:
				//	Here if we have an unknown record type or a superceded record.
				break;
		}
	
		// Skip to next record.
		CPSetFilePosition(fr, recordStartPos + sizeof(ph) + ph.numDataBytes, -1);
	}

	return 0;
}

int
ReadPackedFile(const char* packedFilePath)
{
	CP_FILE_REF fr;			// Ref number for the file we are writing to.
	unsigned long numberOfBytesInFile;
	char temp[256];
	int err;
	
	if (err = CPOpenFile(packedFilePath, 0, &fr)) {
		PrintResult("Open file error", err);
		return err;
	}
	
	CPNumberOfBytesInFile(fr, &numberOfBytesInFile);
	printf("The file consists of %ld bytes.\n", numberOfBytesInFile);
	
	// Read the records.
	if (err = ReadPackedRecords(fr))
		return err;
	
	if (err = CPCloseFile(fr)) {
		PrintResult("Close file error", err);
		return err;
	}
	
	sprintf(temp, "\"%s\" read successfully.", packedFilePath);
	PrintResult(temp, 0);
	
	return 0;
}
