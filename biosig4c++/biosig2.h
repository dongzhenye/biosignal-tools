#ifdef __cplusplus
extern "C" {
#endif

struct biosig_annotation_struct{                           /* this structure is used for annotations */
        size_t onset;                                /* onset time of the event, expressed in units of 100 nanoSeconds and relative to the starttime in the header */
        size_t duration;                              /* duration time, this is a null-terminated ASCII text-string */
        const char *annotation; /* description of the event in UTF-8, this is a null terminated string */
       };



int biosig_lib_version(void);
int biosig_open_file_readonly(const char *path, HDRTYPE *hdr, int read_annotations);
int biosig_close_file(int handle);
int biosig_read_physical_samples(int handle, int biosig_signal, int n, double *buf);
int biosig_read_digital_samples(int handle, int biosig_signal, int n, int *buf);
long long biosig_seek(int handle, int biosig_signal, long long offset, int whence);
long long biosig_tell(int handle, int biosig_signal);
void biosig_rewind(int handle, int biosig_signal);
int biosig_get_annotation(int handle, int n, struct biosig_annotation_struct *annot);
int biosig_open_file_writeonly(const char *path, int filetype, int number_of_signals);
int biosig_set_samplefrequency(int handle, int biosig_signal, int samplefrequency);
int biosig_set_physical_maximum(int handle, int biosig_signal, double phys_max);
int biosig_set_physical_minimum(int handle, int biosig_signal, double phys_min);
int biosig_set_digital_maximum(int handle, int biosig_signal, int dig_max);
int biosig_set_digital_minimum(int handle, int biosig_signal, int dig_min);
int biosig_set_label(int handle, int biosig_signal, const char *label);
int biosig_set_prefilter(int handle, int biosig_signal, const char *prefilter);
int biosig_set_transducer(int handle, int biosig_signal, const char *transducer);
int biosig_set_physical_dimension(int handle, int biosig_signal, const char *phys_dim);
int biosig_set_startdatetime(int handle, int startdate_year, int startdate_month, int startdate_day,
int biosig_set_patientname(int handle, const char *patientname);
int biosig_set_patientcode(int handle, const char *patientcode);
int biosig_set_gender(int handle, int gender);
int biosig_set_birthdate(int handle, int birthdate_year, int birthdate_month, int birthdate_day);
int biosig_set_patient_additional(int handle, const char *patient_additional);
int biosig_set_admincode(int handle, const char *admincode);
int biosig_set_technician(int handle, const char *technician);
int biosig_set_equipment(int handle, const char *equipment);
int biosig_set_recording_additional(int handle, const char *recording_additional);
int biosig_write_physical_samples(int handle, double *buf);
int biosig_blockwrite_physical_samples(int handle, double *buf);
int biosig_write_digital_samples(int handle, int *buf);
int biosig_blockwrite_digital_samples(int handle, int *buf);
int biosig_write_annotation_utf8(int handle, long long onset, long long duration, const char *description);
int biosig_write_annotation_latin1(int handle, long long onset, long long duration, const char *description);
int biosig_set_datarecord_duration(int handle, double duration);


#if defined(EDFLIB_INCLUDED)

struct edf_annotation_struct{                           /* this structure is used for annotations */
        long long onset;                                /* onset time of the event, expressed in units of 100 nanoSeconds and relative to the starttime in the header */
        char duration[16];                              /* duration time, this is a null-terminated ASCII text-string */
        char annotation[EDFLIB_MAX_ANNOTATION_LEN + 1]; /* description of the event in UTF-8, this is a null terminated string */
       };

int edfopen_file_writeonly(const char *path, int filetype, int number_of_signals);
int edf_set_gender(int handle, int gender);
#define edflib_version()			(109)
#define edfopen_file_readonly(a,b,c) 		biosig_open_file_readonly(a,b,c) 
#define edfclose_file(handle) 			biosig_close(handle)
int edfread_physical_samples(int handle, int edfsignal, int n, double *buf);
int edfread_digital_samples(int handle, int edfsignal, int n, int *buf);
long long edfseek(int handle, int biosig_signal, long long offset, int whence);
long long edftell(int handle, int biosig_signal);
int edfrewind(int handle, int edfsignal);
//#define edf_get_annotation(int handle, int n, struct edf_annotation_struct *annot);
int edf_get_annotation(int handle, int n, struct edf_annotation_struct *annot);
#define edfopen_file_writeonly(a,b,c)		biosig_open_file_writeonly(a,b,c)		
#define edf_set_samplefrequency(a,b,c)		biosig_set_samplefrequency(a,b,c)
#define edf_set_physical_maximum(a,b,c) 	biosig_set_physical_maximum(a,b,c)
#define edf_set_physical_minimum(a,b,c) 	biosig_set_physical_minimum(a,b,c)
#define edf_set_digital_maximum(a,b,c) 		biosig_set_digital_maximum(a,b,c)
#define edf_set_digital_minimum(a,b,c) 		biosig_set_digital_minimum(a,b,c)
#define edf_set_label(a,b,c) 			biosig_set_label(a,b,c)
#define edf_set_prefilter(a,b,c) 		biosig_set_prefilter(a,b,c)
#define edf_set_transducer(a,b,c) 		biosig_set_transducer(a,b,c)
#define edf_set_physical_dimension(a,b,c) 	biosig_set_physical_maximum(a,b,c)
#define edf_set_startdatetime(a,b,c,d,e,f)	biosig_set_startdatetime(a,b,c,d,e,f)
#define edf_set_patientname(a,b)		biosig_set_patientname(a,b)
#define edf_set_patientcode(a,b)		biosig_set_patientcode(a,b)
#define edf_set_gender(a,b)			biosig_set_gender(a,b)
#define edf_set_birthdate(a,b,c)		biosig_set_birthdate(a,b,c)
#define edf_set_patient_additional(a,b)		biosig_set_patient_additional(a,b)
#define edf_set_admincode(a,b)			biosig_set_admincode(a,b)
#define edf_set_technician(a,b)			biosig_set_technician(a,b)
#define edf_set_equipment(a,b)			biosig_set_equipment(a,b)
//#define edf_set_recording_additional(a,b)	biosig_set_recording_additional(a,b)
//#define edfwrite_physical_samples(a,b)	biosig_write_physical_samples(a,b)
//#define edf_blockwrite_physical_samples(a,b)	biosig_blockwrite_physical_samples(a,b)
//#define edfwrite_digital_samples(a,b)		biosig_write_digital_samples(a,b)
//#define edf_blockwrite_digital_samples(a,b)	biosig_blockwrite_digital_samples(a,b)
//#define edfwrite_annotation_utf8(int handle, long long onset, long long duration, const char *description);
//#define edfwrite_annotation_latin1(int handle, long long onset, long long duration, const char *description);
/*
int edf_set_recording_additional(int handle, const char *recording_additional);
int edfwrite_physical_samples(int handle, double *buf);
int edf_blockwrite_physical_samples(int handle, double *buf);
int edfwrite_digital_samples(int handle, int *buf);
int edf_blockwrite_digital_samples(int handle, int *buf);
int edfwrite_annotation_utf8(int handle, long long onset, long long duration, const char *description);
int edfwrite_annotation_latin1(int handle, long long onset, long long duration, const char *description);
*/
#define edf_set_datarecord_duration(a,b)	biosig_set_datarecord_duration(a,b)

#endif 


#ifdef __cplusplus
} /* extern "C" */
#endif

