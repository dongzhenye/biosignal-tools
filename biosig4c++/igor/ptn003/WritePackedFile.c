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

static int
WriteDemoVariablesRecord(CP_FILE_REF fr)
{
	unsigned long recordStartPos;
	char temp[256];
	int err;
	
	if (err = CPGetFilePosition(fr, &recordStartPos))		// Remember where record starts in the file.
		return err;
	
	// Write the generic packed file record header. numDataBytes field will be updated later.
	if (err = WriteRecordHeader(fr, kVariablesRecord, 0, 0))
		return err;

	// Write the specific header for variables data.
	if (err = WriteVariablesHeader(fr, 0, 1, 1, 0, 0))	// There will be one numeric and one string variable.
		return err;
	
	// Write the numeric variable.
	if (err = WriteNumericVar(fr, "userNumVar", 0, 1234.0, 0.0))
		return err;
	
	// Write the string variable.
	strcpy(temp, "This is a string variable.");
	if (err = WriteStringVar(fr, "userStrVar", temp, strlen(temp)))
		return err;
	
	// Update the numDataBytes field to record the actual length of the data.
	if (err = SetRecordHeaderNumDataBytesField(fr, recordStartPos))
		return err;
	
	return 0;
}

static void
SetDemoWaveDataContents(float* fp, long numPoints)
{
	float val;
	
	val = 0.0;
	while(numPoints > 0) {
		*fp++ = val;
		val += 1.0;
		numPoints -= 1;
	}
}

static int
WriteDemoWave1DNumericWaveRecord(CP_FILE_REF fr)
{
	WaveHeader2 wh;
	float* data;
	unsigned long recordStartPos;
	unsigned long creationDate;
	long waveMainDataSize, noteSize;
	char note[256];
	int err;
	
	if (err = CPGetFilePosition(fr, &recordStartPos))		// Remember where record starts in the file.
		return err;
	
	// Write the generic packed file record header. numDataBytes field will be updated later.
	if (err = WriteRecordHeader(fr, kWaveRecord, 0, 0))
		return err;
	
	// It is OK to pass 0 for creationDate if you don't care about it.
	#ifdef WIN32
		creationDate = 0;						// GetDateTime can be simulated on Windows but it is fairly complicated.
	#else
		GetDateTime(&creationDate);				// Seconds since midnight, January 1, 1904.
	#endif

	// Create the demo data.
	waveMainDataSize = DEMO_WAVE_LENGTH * sizeof(float);		// Size of just the numeric data.
	data = malloc(waveMainDataSize);
	if (data == NULL)
		return NOMEM;
	SetDemoWaveDataContents(data, DEMO_WAVE_LENGTH);
	
	// This sets structure to be valid for a 1D numeric wave.
	InitWaveHeader2(&wh, "wave0", creationDate, DEMO_WAVE_LENGTH, NT_FP32);
	
	// Set X scaling. x = hsA*p + hsB.
	wh.hsA = 0.2;
	wh.hsB = 0.0;
	
	/*	For the checksum calculated by WriteVersion2NumericWave, the wData field of the
		WaveHeader2 structure must contain the same data as the first 16 bytes of the main
		wave data. The wData field was cleared by InitWaveHeader2.
	*/
	{
		int tmp = 16;
		if (waveMainDataSize < tmp)
			tmp = waveMainDataSize;
		memcpy(&wh.wData, data, tmp);
	}
	
	// We need to know the size of the wave note.
	strcpy(note, "This is wave0.");
	noteSize = strlen(note);

	// Write the wave.
	err = WriteVersion2NumericWave(fr, &wh, data, note, strlen(note));
	free(data);
	if (err)
		return err;
	
	// Update the numDataBytes field to record the actual length of the data.
	if (err = SetRecordHeaderNumDataBytesField(fr, recordStartPos))
		return err;
	
	return 0;
}

static int
WriteDemoRecreationProcedures(CP_FILE_REF fr)
{
	char text[1024];

	// Generate text to call the Table0() macro defined below.
	strcpy(text, "Table0()"CR_STR);
	strcat(text, CR_STR);

	// Generate text for a table recreation macro to display wave0.
	strcat(text, "Window Table0() : Table"CR_STR);
	strcat(text, "\tPauseUpdate; Silent 1		| building window..."CR_STR);
	strcat(text, "\tEdit wave0"CR_STR);
	strcat(text, "End"CR_STR);
	
	return WritePlainTextRecord(fr, kRecreationRecord, text, strlen(text));
}

static int
WriteDemoProcedureWindowProcedures(CP_FILE_REF fr)
{
	char text[1024];
	
	// Generate procedure window text.
	strcpy(text, "Macro Demo()"CR_STR);
	strcat(text, "\tDoAlert 0, \"Greetings!\""CR_STR);
	strcat(text, "End"CR_STR);
	
	return WritePlainTextRecord(fr, kProcedureRecord, text, strlen(text));
}

static int
WriteDemoHistory(CP_FILE_REF fr)
{
	char text[1024];
	int err;
	
	// Generate history text.
	strcpy(text, "// This experiment was created using Igor Pro Tech Note PTN003"CR_STR);
	strcat(text, "// This tech note shows how to write a packed Igor Pro 3.0 file from your own program."CR_STR);
	
	if (err = WritePlainTextRecord(fr, kHistoryRecord, text, strlen(text)))
		return err;
	return WriteRecordHeader(fr, kGetHistoryRecord, 0, 0);
}

static int
WriteDemoPackedFile(CP_FILE_REF fr)
{
	int err;
	
	if (err = WriteDemoVariablesRecord(fr))
		return err;
	if (err = WriteDemoWave1DNumericWaveRecord(fr))
		return err;
	if (err = WriteDemoRecreationProcedures(fr))
		return err;
	if (err = WriteDemoProcedureWindowProcedures(fr))
		return err;
	if (err = WriteDemoHistory(fr))
		return err;
	return 0;
}

int
WritePackedFile(const char* packedFilePath)
{
	CP_FILE_REF fr;			// Ref number for the file we are writing to.
	char temp[256];
	int err;

	// Delete test file if it already exists and create a new file.
	if (err = CPCreateFile(packedFilePath, 1, IGOR_CREATOR_CODE, IGOR_PACKED_FILE_TYPE)) {
		PrintResult("Create file error", err);
		return err;
	}
	
	if (err = CPOpenFile(packedFilePath, 1, &fr)) {
		PrintResult("Open file error", err);
		return err;
	}
	
	// Write all of the records to the file.
	if (err = WriteDemoPackedFile(fr)) {
		PrintResult("WritePackedFile error", err);
		return err;
	}
	
	if (err = CPCloseFile(fr)) {
		PrintResult("Close file error", err);
		return err;
	}
	
	sprintf(temp, "\"%s\" written successfully.", packedFilePath);
	PrintResult(temp, 0);
	
	return 0;
}
