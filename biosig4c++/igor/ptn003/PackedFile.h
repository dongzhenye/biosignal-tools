/*	Data structure and equates relating to Igor Pro packed files.
	See Igor Pro Technical Note PTN003 for details.
*/

#ifdef __cplusplus
extern "C" {
#endif

/* All structures written to disk are 2 byte-aligned */
#if GENERATINGPOWERPC
	#pragma options align=mac68k
#endif
#ifdef _WINDOWS_
	#pragma pack(2)
#endif

#define CR_STR "\015"						// CR is the line terminator used by Igor.

#define IGOR_CREATOR_CODE 'IGR0'			// Mac file creator code (zero at the end).
#define IGOR_PACKED_FILE_TYPE 'IGsU'		// Mac file type code for packed experiment file.
#define IGOR_PACKED_STATIONERY_TYPE 'IGsS'	// Mac file type code for packed experiment stationery file.


/* *** Packed File Structures *** */

/*	The Igor Pro packed file consists of a series of records.
	Each record starts with a record header followed by the record data.
	The record header identifies the type and size of the record data.
	
	New types may be added in the future. When reading a packed file,
	you should skip over records whose types you do not know.
*/

enum PackedFileRecordType {
	kUnusedRecord = 0,
	kVariablesRecord,		//  1: Contains system numeric variables (e.g., K0) and user numeric and string variables.
	kHistoryRecord,			//  2: Contains the experiment's history as plain text.
	kWaveRecord,			//  3: Contains the data for a wave
	kRecreationRecord,		//  4: Contains the experiment's recreation procedures as plain text.
	kProcedureRecord,		//  5: Contains the experiment's main procedure window text as plain text.
	kUnused2Record,
	kGetHistoryRecord,		//  6: Not a real record but rather, a message to go back and read the history text.
	kPackedFileRecord,		//  7: Contains the data for a procedure file or notebook in a packed form.
	kDataFolderStartRecord,	//  8: Marks the start of a new data folder.
	kDataFolderEndRecord	// 10: Marks the end of a data folder.
	
	/*	Igor writes other kinds of records in a packed experiment file, for storing
		things like pictures, page setup records, and miscellaneous settings. The
		format for these records is quite complex and is not described in PTN003.
		If you are writing a program to read packed files, you must skip any record
		with a record type that is not listed above.
	*/
};

typedef struct PackedFileRecordHeader {
	unsigned short recordType; 	/* Record type plus superceded flag. */
	short version;				/* Version information depends on the type of record. */
	long numDataBytes;			/* Number of data bytes in the record following this record header. */
} PackedFileRecordHeader, *PackedFileRecordHeaderPtr;

#define PACKEDRECTYPE_MASK 0x7FFF	/* Record type = (recordType & PACKEDREC_TYPE_MASK) */
#define SUPERCEDED_MASK 0x8000		/* Bit is set if the record is superceded by a later record in the packed file. */



#if GENERATINGPOWERPC
	#pragma options align=reset
#endif
#ifdef _WINDOWS_
	#pragma pack()
#endif
/* All structures written to disk are 2 byte-aligned */

#ifdef __cplusplus
}
#endif
