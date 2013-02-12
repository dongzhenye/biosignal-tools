/*	Structures and equates used for version 1 and 2 Igor Pro variable records.
	See Igor Pro Technical Note PTN003 for details.
*/

#ifdef __cplusplus
extern "C" {
#endif

// All structures written to disk are 2-byte-aligned.
#if GENERATINGPOWERPC
	#pragma options align=mac68k
#endif
#ifdef WIN32
	#pragma pack(2)
#endif

/*	VarHeader1
	This is the header written at the start of the version 1 variables record,
	before the actual variable data.
	
	The entire variables record on disk consists of:
		PackedFileRecordHeader of type kVariablesRecord

		numSysVars system variables written as floats in order (K0, K1 . . . K19).
		
		numUserVars user numeric variables, each written as a UserNumVarRec record.
		
		numUserStrs user string variables, each consisting of a UserStrVarRec record.
*/
typedef struct VarHeader1 {
	short version;						// Version number is 1 for this header.
	short numSysVars;					// Number of system variables (K0, K1 . . .).
	short numUserVars;					// Number of user numeric variables -- may be zero./
	short numUserStrs;					// Number of user string variables -- may be zero.
} VarHeader1;

/*	UserStrVarRec1
	This header precedes each user string variable in a version 2 variables record.
*/
typedef struct UserStrVarRec1 {
	char name[31+1];					// Name of the string variable.
	short strLen;						// The real size of the following array.
	char data[1];
} UserStrVarRec1;


/*	VarHeader2
	This is the header written at the start of the version 2 variables record,
	before the actual variable data.
	
	The entire variables record on disk consists of:
		PackedFileRecordHeader of type kVariablesRecord

		numSysVars system variables written as floats in order (K0, K1 . . . K19).
		
		numUserVars user numeric variables, each written as a UserNumVarRec record.
		
		numUserStrs user string variables, each consisting of a UserStrVarRec record.
		
		numDependentVars user dependent numeric variables, each consisting of a UserDependentVarRec record.
		
		numDependentStrs user dependent string variables, each consisting of a UserDependentVarRec record.
*/
typedef struct VarHeader2 {
	short version;						// Version number is 2 for this header.
	short numSysVars;					// Number of system variables (K0, K1 . . .).
	short numUserVars;					// Number of user numeric variables -- may be zero.
	short numUserStrs;					// Number of user string variables -- may be zero.
	short numDependentVars;				// Number of dependent numeric variables -- may be zero.
	short numDependentStrs;				// Number of dependent string variables -- may be zero.
} VarHeader2;


/*	UserStrVarRec2
	This header precedes each user string variable in a version 2 variables record.
*/
typedef struct UserStrVarRec2 {
	char name[31+1];					// Name of the string variable.
	long strLen;						// Number of bytes in the string.
	char data[1];						// Start of string data. This is not a C string - no null terminator.
} UserStrVarRec2;

struct VarNumRec {
	short numType;						// Type from IgorMath.h.
	double realPart;					// The real part of the number.
	double imagPart;					// The imag part if the number is complex.
	long reserved;						// Reserved - set to zero.
};
typedef struct VarNumRec VarNumRec;


/*	UserNumVarRec
	This header precedes each user numeric variable.
*/
typedef struct UserNumVarRec {
	char name[31+1];					// Name of the variable as a C string including null terminator.
	short type;							// Must be 1, signifying numeric.
	VarNumRec num;						// Type and value of the variable if it is numeric.
} UserNumVarRec;

/*	UserDependentVarRec
	This header precedes each user dependent numeric or string variable.
	A dependent variable is one controlled by a dependency formula,
	such as:
		numVar0 := 2*numVar1
*/
typedef struct UserDependentVarRec {
	char name[31+1];					// Name of the variable.
	short type;							// 0 = string, 1 = numeric.
	VarNumRec num;						// Type and value of the variable if it is numeric. Not used for string.
	short formulaLen;					// The length of the dependency formula.
	char formula[1];					// Start of the dependency formula. A C string including null terminator.
} UserDependentVarRec;


#if GENERATINGPOWERPC
	#pragma options align=reset
#endif
#ifdef WIN32
	#pragma pack()
#endif
// All structures written to disk are 2-byte-aligned.

#ifdef __cplusplus
}
#endif
