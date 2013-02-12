int ReadPackedFile(const char* packedFilePath);
int WritePackedFile(const char* packedFilePath);
int WriteVersion2NumericWave(CP_FILE_REF fr, WaveHeader2* whp, const void* data, const char* waveNote, long noteSize);
int WriteVersion5NumericWave(CP_FILE_REF fr, WaveHeader5* whp, const void* data, const char* waveNote, long noteSize);
int ReadWave(CP_FILE_REF fr, int* typePtr, long* npntsPtr, void** waveDataPtrPtr);
