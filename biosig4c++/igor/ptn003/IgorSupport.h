// Prototypes for IgorSupport.c.
int WriteRecordHeader(CP_FILE_REF fr, int recordType, int recordVersion, long recordBytes);
int SetRecordHeaderNumDataBytesField(CP_FILE_REF fr, long recordStartPos);
int WriteNumericVar(CP_FILE_REF fr, const char* name, int isComplex, double realPart, double imagPart);
int WriteStringVar(CP_FILE_REF fr, const char* name, const char* contents, long numBytes);
int WriteVariablesHeader(CP_FILE_REF fr, int numSysVars, int numUserVars, int numUserStrs, int numDependentVars, int numDependentStrs);
int WriteWaveBinHeader2(CP_FILE_REF fr, WavePtr2 wp, long waveDataSize, long noteSize);
void InitWaveHeader2(WavePtr2 wp, const char* name, unsigned long creationDate, long numPoints, int type);
int WriteWaveBinHeader5(CP_FILE_REF fr, WavePtr5 wp, long waveDataSize, long formulaSize, long noteSize, long extDataUnitsSize, long extDimUnitsSize[], long dimLabelsSize[], long sIndiciesSize);
void InitWaveHeader5(WavePtr5 wp, const char* name, unsigned long creationDate, long numPoints, int type);
int WriteDataFolderStartRecord(CP_FILE_REF fr, const char* name);
int WriteDataFolderEndRecord(CP_FILE_REF fr);
int WritePlainTextRecord(CP_FILE_REF fr, int recordType, const char* text, long numBytes);
