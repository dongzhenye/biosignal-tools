/*	This file provides support for Igor-related things, such as writing
	variable and wave records, for the Packed File demo program.
	
	See main.c for examples showing how these routines are called.
	
	See Igor Pro Tech Note PTN#003 for details.
*/

#include <string.h>
#include <stdio.h>
#include <stddef.h>					// For offsetof macro.

#include "PackedFile.h"
#include "Variables.h"
#include "IgorBin.h"
#include "CrossPlatformFileIO.h"
#include "IgorSupport.h"


// Support for writing packed file records.

int
WriteRecordHeader(CP_FILE_REF fr, int recordType, int recordVersion, long recordBytes)
{
	PackedFileRecordHeader h;
	unsigned long numBytesToWrite, numBytesWritten;
	int err;

	h.recordType = recordType;
	h.version = recordVersion;
	h.numDataBytes = recordBytes;
	numBytesToWrite = sizeof(PackedFileRecordHeader);
	err = CPWriteFile(fr, numBytesToWrite, &h, &numBytesWritten);
	return err;
}

/*	SetRecordHeaderNumDataBytesField(...)

	This is used to set the numDataBytes field of the PackedFileRecordHeader
	structure on disk, after the data section of a record has been written.
	This must be called after writing the data section so that the current
	file mark is just after the last data byte written.
*/
int
SetRecordHeaderNumDataBytesField(CP_FILE_REF fr, long recordStartPos)
{
	unsigned long currentFilePos;
	long numDataBytes;
	unsigned long numBytesToWrite, numBytesWritten;
	long filePos;
	int err;

	if (err = CPGetFilePosition(fr, &currentFilePos))
		return err;
	numDataBytes = currentFilePos - recordStartPos - sizeof(PackedFileRecordHeader);
	filePos = recordStartPos + offsetof(PackedFileRecordHeader, numDataBytes);
	if (err = CPSetFilePosition(fr, filePos, -1))
		return err;
	numBytesToWrite = sizeof(numDataBytes);
	if (err = CPWriteFile(fr, numBytesToWrite, &numDataBytes, &numBytesWritten))
		return err;
	if (err = CPSetFilePosition(fr, currentFilePos, -1))
		return err;
	return 0;
}


// Support for writing variables.

int
WriteNumericVar(CP_FILE_REF fr, const char* name, int isComplex, double realPart, double imagPart)
{
	UserNumVarRec nv;
	unsigned long numBytesToWrite, numBytesWritten;
	int err;
	
	strcpy(nv.name, name);
	nv.type = 1;								// Signifies numeric variable.
	if (isComplex)
		nv.num.numType = NT_FP64 | NT_CMPLX;	// Double-precision, complex.
	else
		nv.num.numType = NT_FP64;				// Double-precision, real.
	nv.num.realPart = realPart;
	nv.num.imagPart = imagPart;
	nv.num.reserved = 0;
	
	numBytesToWrite = sizeof(UserNumVarRec);
	err = CPWriteFile(fr, numBytesToWrite, &nv, &numBytesWritten);
	return err;
}

int
WriteStringVar(CP_FILE_REF fr, const char* name, const char* contents, long numBytes)
{
	UserStrVarRec2 sv;
	unsigned long numBytesToWrite, numBytesWritten;
	int err;
	
	strcpy(sv.name, name);
	sv.strLen = numBytes;
	
	numBytesToWrite = offsetof(UserStrVarRec2, data);
	if (err = CPWriteFile(fr, numBytesToWrite, &sv, &numBytesWritten))	// Write up to but not including the data field.
		return err;
	
	numBytesToWrite = numBytes;
	err = CPWriteFile(fr, numBytesToWrite, contents, &numBytesWritten);	// Write the string data.
	return err;
}

int
WriteVariablesHeader(CP_FILE_REF fr, int numSysVars, int numUserVars, int numUserStrs, int numDependentVars, int numDependentStrs)
{
	VarHeader2 vh;
	unsigned long numBytesToWrite, numBytesWritten;
	int err;
	
	// Write the specific header for variables data.
	vh.version = 2;										// Version 2 variables header.
	vh.numSysVars = numSysVars;
	vh.numUserVars = numUserVars;
	vh.numUserStrs = numUserStrs;
	vh.numDependentVars = numDependentVars;
	vh.numDependentStrs = numDependentStrs;
	numBytesToWrite = sizeof(vh);
	err = CPWriteFile(fr, numBytesToWrite, &vh, &numBytesWritten);
	return err;
	
	return 0;
}


// Support for writing waves.

static long
Checksum(
	short *data,
	long oldcksum,
	long numBytes)
{
	numBytes >>= 1;				// 2 bytes to a short -- ignore trailing odd byte.
	while(numBytes-- > 0)
		oldcksum += *data++;
	return oldcksum&0xffff;
}


/*	WriteWaveBinHeader2(...)
	
	This writes a version 2 BinHeader to disk.
	
	The wave BinHeader contains a checksum over the BinHeader itself plus
	the following wave WaveHeader2 structure. The wData field of the WaveHeader2
	structure is included in the checksum. Therefore, the wData field must
	contain the same data as the first 16 bytes of the actual wave data.
	
	The wfmSize parameter is the size of the WaveHeader2 structure up to
	but not including the wData field plus the size of the wave's main data
	bytes plus 16 bytes of padding. Main data bytes means the numeric data
	for numeric waves.
	
	The noteSize parameters can be 0.
*/
int
WriteWaveBinHeader2(
	CP_FILE_REF fr,
	WavePtr2 wp,				// Pointer to wave structure for wave to be written
	long wfmSize,				// Size of wave structure plus wave data proper plus 16 bytes of padding
	long noteSize)
{
	BinHeader2 bh;
	long checksum;
	unsigned long numBytesToWrite, numBytesWritten;
	int err;
	
	memset(&bh, 0, sizeof(BinHeader2));
	
	bh.version = 2;						// This is a version 2 wave record.
	bh.wfmSize = wfmSize;
	bh.noteSize = noteSize;
	
	// Do checksum over wave structure and then add checksum over wave header.
	checksum = Checksum((short *)wp, 0, sizeof(WaveHeader2));						// Includes wData field which must contain the same data as the first 16 bytes of the main wave data.
	checksum = -Checksum((short *)&bh, checksum, sizeof(struct BinHeader2));
	bh.checksum = (short)checksum;

	numBytesToWrite = sizeof(BinHeader2);
	err = CPWriteFile(fr, numBytesToWrite, &bh, &numBytesWritten);
	return err;
}

/*	InitWaveHeader2(...)

	Initializes the wave structure pointed to by wp to valid values
	for a 1D numeric wave.
	
	Since this sets all fields to safe values, you should call this first
	for all waves and then change the structure as needed for multi-dimensional
	or text waves. See PTN003 for details.
	
	If you don't care about setting the creation and modification dates,
	pass 0 for creationDate.
	
	numPoints is the number of data points in all dimensions of the wave.
*/
void
InitWaveHeader2(
	WavePtr2 wp,							// Pointer to wave structure to be inited.
	const char* name,						// C string. Up to 31 characters.
	unsigned long creationDate,				// Number of seconds since midnight January 1, 1904.
	long numPoints,							// Number of points in the wave.
	int type)								// Valid Igor number type. See IgorBin.h.
{
	memset(wp, 0, sizeof(WaveHeader2));
	
	wp->creationDate = creationDate;
	wp->modDate = creationDate;
	wp->npnts = numPoints;
	wp->type = type;
	wp->whVersion = 0;
	strcpy(wp->bname, name);
	wp->hsA = 1.0;
	wp->hsB = 0.0;
}

/*	WriteWaveBinHeader5(...)
	
	This writes a version 5 BinHeader to disk.
	
	Because the wave BinHeader contains a checksum over the header itself
	plus the following wave WaveHeader5 structure, this routine requires a
	pointer to the WaveHeader5 structure to be written. The structure must
	contain the values to be written so that the checksum will be correct.
	
	The wfmSize parameter is the size of the WaveHeader5 structure up to
	but not including the wData field plus the size of the wave's main data
	bytes. Main data bytes means the numeric data for numeric waves or the
	text data for text waves but does not include any optional data or the
	string indices.
	
	All of the size parameters, except for wfmSize, can be 0.
	For text waves, the sIndicesSize must be as described in TN003 (not PTN003).
*/
int
WriteWaveBinHeader5(
	CP_FILE_REF fr,
	WavePtr5 wp,				// Pointer to wave structure for wave to be written
	long wfmSize,				// size of wave structure plus wave data proper
	long formulaSize,
	long noteSize,
	long dataEUnitsSize,
	long dimEUnitsSize[],		// Array of 4 longs or NULL if no extended dimension units
	long dimLabelsSize[],		// Array of 4 longs or NULL if no dimension labels
	long sIndicesSize)			// Size of string indices if this is a text wave
{
	BinHeader5 bh;
	long checksum;
	unsigned long numBytesToWrite, numBytesWritten;
	int err;
	
	memset(&bh, 0, sizeof(BinHeader5));
	
	bh.version = 5;						// This is a version 5 wave record.
	bh.wfmSize = wfmSize;
	bh.formulaSize = formulaSize;
	bh.noteSize = noteSize;
	bh.dataEUnitsSize = dataEUnitsSize;
	if (dimEUnitsSize != NULL)
		memcpy(bh.dimEUnitsSize, dimEUnitsSize, MAXDIMS*sizeof(long));
	if (dimLabelsSize != NULL)
		memcpy(bh.dimLabelsSize, dimLabelsSize, MAXDIMS*sizeof(long));
	bh.sIndicesSize = sIndicesSize;
	
	// Do checksum over wave structure and then add checksum over wave header.
	checksum = Checksum((short *)wp, 0, offsetof(WaveHeader5, wData));
	checksum = -Checksum((short *)&bh, checksum, sizeof(struct BinHeader5));
	bh.checksum = (short)checksum;

	numBytesToWrite = sizeof(BinHeader5);
	err = CPWriteFile(fr, numBytesToWrite, &bh, &numBytesWritten);
	return err;
}

/*	InitWaveHeader5(...)

	Initializes the wave structure pointed to by wp to valid values
	for a 1D numeric wave.
	
	Since this sets all fields to safe values, you should call this first
	for all waves and then change the structure as needed for multi-dimensional
	or text waves. See PTN003 for details.
	
	If you don't care about setting the creation and modification dates,
	pass 0 for creationDate.
	
	numPoints is the number of data points in all dimensions of the wave.
*/
void
InitWaveHeader5(
	WavePtr5 wp,							// Pointer to wave structure to be inited.
	const char* name,						// C string. Up to 31 characters.
	unsigned long creationDate,				// Number of seconds since midnight January 1, 1904.
	long numPoints,							// Number of points in the wave.
	int type)								// Valid Igor number type. See IgorBin.h.
{
	int i;
	
	memset(wp, 0, sizeof(WaveHeader5));
	
	wp->creationDate = creationDate;
	wp->modDate = creationDate;
	wp->npnts = numPoints;
	wp->type = type;
	wp->whVersion = 1;
	strcpy(wp->bname, name);
	wp->nDim[0] = numPoints;				// This assumes a 1D wave.

	for(i = 0; i < MAXDIMS; i++) {
		wp->sfA[i] = 1.0;					// Dimension scaling delta value.
		wp->sfB[i] = 0.0;					// Dimension scaling offset value.
	}
}


// Support for data folders.

int
WriteDataFolderStartRecord(CP_FILE_REF fr, const char* name)
{
	char temp[32];
	unsigned long numBytesToWrite, numBytesWritten;
	int err;
	
	if (err = WriteRecordHeader(fr, kDataFolderStartRecord, 0, 32))
		return err;

	memset(temp, 0, sizeof(temp));
	strncpy(temp, name, 31);
	numBytesToWrite = 32;
	err = CPWriteFile(fr, numBytesToWrite, temp, &numBytesWritten);
	return err;
}

int
WriteDataFolderEndRecord(CP_FILE_REF fr)
{
	return WriteRecordHeader(fr, kDataFolderEndRecord, 0, 0);
}


// Support for plain text records (kRecreationRecord, kHistoryRecord, kProcedureRecord).

int
WritePlainTextRecord(CP_FILE_REF fr, int recordType, const char* text, long numBytes)
{
	unsigned long recordStartPos;
	unsigned long numBytesToWrite, numBytesWritten;
	int err;
	
	if (err = CPGetFilePosition(fr, &recordStartPos))		// Remember where record starts in the file.
		return err;
	
	// Write the generic packed file record header. numDataBytes field will be updated later.
	if (err = WriteRecordHeader(fr, recordType, 0, 0))
		return err;

	// Write the text.
	numBytesToWrite = numBytes;
	if (err = CPWriteFile(fr, numBytesToWrite, text, &numBytesWritten))
		return err;
	
	// Update the numDataBytes field to record the actual length of the data.
	if (err = SetRecordHeaderNumDataBytesField(fr, recordStartPos))
		return err;
	
	return 0;
}
