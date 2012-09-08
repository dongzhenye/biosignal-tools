/*

    $Id$
    Copyright (C) 2012 Alois Schloegl <alois.schloegl@gmail.com>
    This file is part of the "BioSig for C/C++" repository
    (biosig4c++) at http://biosig.sf.net/


    BioSig is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef __LIBBIOSIG2_H__
#define __LIBBIOSIG2_H__

#include "biosig.h"


#ifdef __cplusplus
extern "C" {
#endif



struct biosig_annotation_struct {       /* this structure is used for annotations */
        size_t onset;                   /* onset time of the event, expressed in units of 100 nanoSeconds and relative to the starttime in the header */
        size_t duration;                /* duration time, this is a null-terminated ASCII text-string */
        const char *annotation; 	/* description of the event in UTF-8, this is a null terminated string */
       };


int biosig_lib_version(void);

int biosig_open_file_readonly(const char *path, HDRTYPE *hdr, int read_annotations);

int biosig_close_file(int handle);
int biosig_read_samples(int handle, size_t channel, size_t n, double *buf, unsigned char UCAL);
int biosig_read_physical_samples(int handle, size_t channel, size_t n, double *buf);
int biosig_read_digital_samples(int handle, size_t channel, size_t n, double *buf);
//#define biosig_read_physical_samples(a,b,c,d) biosig_read_samples(a,b,c,d,0) 
//#define biosig_read_digital_samples(a,b,c,d)  biosig_read_samples(a,b,c,d,1) 
size_t biosig_seek(int handle, long long offset, int whence);
size_t biosig_tell(int handle);
void biosig_rewind(int handle, int biosig_signal);
int biosig_get_annotation(int handle, size_t n, struct biosig_annotation_struct *annot);
int biosig_open_file_writeonly(const char *path, enum FileFormat filetype, int number_of_signals);

double biosig_get_samplefrequency(int handle, int biosig_signal);
int biosig_set_samplefrequency(int handle, int biosig_signal,  double samplefrequency);

double biosig_get_physical_maximum(int handle, int biosig_signal);
int biosig_set_physical_maximum(int handle, int biosig_signal, double phys_max);

double biosig_get_physical_minimum(int handle, int biosig_signal);
int biosig_set_physical_minimum(int handle, int biosig_signal, double phys_min);

double biosig_get_digital_maximum(int handle, int biosig_signal);
int biosig_set_digital_maximum(int handle, int biosig_signal, double dig_max);

double biosig_get_digital_minimum(int handle, int biosig_signal);
int biosig_set_digital_minimum(int handle, int biosig_signal, double dig_min);

const char *biosig_get_label(int handle, int biosig_signal);
int biosig_set_label(int handle, int biosig_signal, const char *label);

//const char *biosig_get_prefilter(int handle, int biosig_signal);
int biosig_set_prefilter(int handle, int biosig_signal, const char *prefilter);
double biosig_get_highpassfilter(int handle, int biosig_signal);
int biosig_set_highpassfilter(int handle, int biosig_signal, double frequency);
double biosig_get_lowpassfilter(int handle, int biosig_signal);
int biosig_set_lowpassfilter(int handle, int biosig_signal, double frequency);
double biosig_get_notchfilter(int handle, int biosig_signal);
int biosig_set_notchfilter(int handle, int biosig_signal, double frequency);

const char *biosig_get_transducer(int handle, int biosig_signal);
int biosig_set_transducer(int handle, int biosig_signal, const char *transducer);

const char *biosig_get_physical_dimension(int handle, int biosig_signal);
int biosig_set_physical_dimension(int handle, int biosig_signal, const char *phys_dim);

int biosig_get_startdatetime(int handle, struct tm *T);
int biosig_set_startdatetime(int handle, const struct tm *T);

const char *biosig_get_patientname(int handle);
int biosig_set_patientname(int handle, const char *patientname);
const char *biosig_get_patientcode(int handle);
int biosig_set_patientcode(int handle, const char *patientcode);
int biosig_get_gender(int handle);
int biosig_set_gender(int handle, int gender);

int biosig_get_birthdate(int handle, struct tm *T);
int biosig_set_birthdate(int handle, const struct tm *T);

int biosig_set_patient_additional(int handle, const char *patient_additional);
int biosig_set_admincode(int handle, const char *admincode);
const char *biosig_get_technician(int handle);
int biosig_set_technician(int handle, const char *technician);
int biosig_set_equipment(int handle, const char *equipment);
int biosig_set_recording_additional(int handle, const char *recording_additional);

int biosig_write_physical_samples(int handle, double *buf);
int biosig_blockwrite_physical_samples(int handle, double *buf);
int biosig_write_digital_samples(int handle, int *buf);
int biosig_blockwrite_digital_samples(int handle, int *buf);
int biosig_write_annotation_utf8(int handle, size_t onset, size_t duration, const char *description);
int biosig_write_annotation_latin1(int handle, size_t onset, size_t duration, const char *description);
int biosig_set_datarecord_duration(int handle, double duration);


#if defined(MAKE_EDFLIB)

// definitions according to edflib v1.09
#define edflib_version()			(109)
#define EDFLIB_MAX_ANNOTATION_LEN 	512

#define EDFLIB_FILETYPE_EDF                  0
#define EDFLIB_FILETYPE_EDFPLUS              1
#define EDFLIB_FILETYPE_BDF                  2
#define EDFLIB_FILETYPE_BDFPLUS              3
#define EDFLIB_MALLOC_ERROR                 -1
#define EDFLIB_NO_SUCH_FILE_OR_DIRECTORY    -2
#define EDFLIB_FILE_CONTAINS_FORMAT_ERRORS  -3
#define EDFLIB_MAXFILES_REACHED             -4
#define EDFLIB_FILE_READ_ERROR              -5
#define EDFLIB_FILE_ALREADY_OPENED          -6
#define EDFLIB_FILETYPE_ERROR               -7
#define EDFLIB_FILE_WRITE_ERROR             -8
#define EDFLIB_NUMBER_OF_SIGNALS_INVALID    -9
#define EDFLIB_FILE_IS_DISCONTINUOUS       -10
#define EDFLIB_INVALID_READ_ANNOTS_VALUE   -11

/* values for annotations */
#define EDFLIB_DO_NOT_READ_ANNOTATIONS 0
#define EDFLIB_READ_ANNOTATIONS        1
#define EDFLIB_READ_ALL_ANNOTATIONS    2

/* the following defines are possible errors returned by edfopen_file_writeonly() */
#define EDFLIB_NO_SIGNALS                  -20
#define EDFLIB_TOO_MANY_SIGNALS            -21
#define EDFLIB_NO_SAMPLES_IN_RECORD        -22
#define EDFLIB_DIGMIN_IS_DIGMAX            -23
#define EDFLIB_DIGMAX_LOWER_THAN_DIGMIN    -24
#define EDFLIB_PHYSMIN_IS_PHYSMAX          -25

#define EDFLIB_TIME_DIMENSION (10000000LL)
#define EDFLIB_MAXSIGNALS 256
#define EDFLIB_MAX_ANNOTATION_LEN 512

#define EDFSEEK_SET 0
#define EDFSEEK_CUR 1
#define EDFSEEK_END 2


struct edf_annotation_struct {                       /* this structure is used for annotations */
        size_t onset;                                /* onset time of the event, expressed in units of 100 nanoSeconds and relative to the starttime in the header */
        size_t duration;                              /* duration time, this is a null-terminated ASCII text-string */
        char annotation[EDFLIB_MAX_ANNOTATION_LEN + 1]; /* description of the event in UTF-8, this is a null terminated string */
       };

int edfopen_file_writeonly(const char *path, int filetype, int number_of_signals);
#define edfopen_file_readonly(a,b,c) 		biosig_open_file_readonly(a,b,c) 
#define edfclose_file(handle) 			biosig_close_file(handle)
int edfread_physical_samples(int handle, int edfsignal, int n, double *buf);
int edfread_digital_samples(int handle, int edfsignal, int n, int *buf);
long long edfseek(int handle, int biosig_signal, long long offset, int whence);
long long edftell(int handle, int biosig_signal);
int edfrewind(int handle, int edfsignal);
//#define edf_get_annotation(a,b,c)               biosig_get_annotation(a,b,c) 
int edf_get_annotation(int handle, int n, struct edf_annotation_struct *annot);
//#define edfopen_file_writeonly(a,b,c)		biosig_open_file_writeonly(a,b,c)		
int biosig_open_file_writeonly(const char *path, enum FileFormat filetype, int number_of_signals);
#define edf_set_samplefrequency(a,b,c)		biosig_set_samplefrequency(a,b,c)
#define edf_set_physical_maximum(a,b,c) 	biosig_set_physical_maximum(a,b,c)
#define edf_set_physical_minimum(a,b,c) 	biosig_set_physical_minimum(a,b,c)
#define edf_set_digital_maximum(a,b,c)		biosig_set_digital_maximum(a,b,(double)(c))
#define edf_set_digital_minimum(a,b,c)		biosig_set_digital_minimum(a,b,(double)(c))
#define edf_set_label(a,b,c) 			biosig_set_label(a,b,c)
#define edf_set_prefilter(a,b,c) 		biosig_set_prefilter(a,b,c)
#define edf_set_transducer(a,b,c) 		biosig_set_transducer(a,b,c)
#define edf_set_physical_dimension(a,b,c) 	biosig_set_physical_dimension(a,b,c)
int edf_set_startdatetime(int handle, int startdate_year, int startdate_month, int startdate_day, int starttime_hour, int starttime_minute, int starttime_second);
#define edf_set_patientname(a,b)		biosig_set_patientname(a,b)
#define edf_set_patientcode(a,b)		biosig_set_patientcode(a,b)
//#define edf_set_gender(a,b)			biosig_set_gender(a,b)
int edf_set_gender(int handle, int gender);
int edf_set_birthdate(int handle, int birthdate_year, int birthdate_month, int birthdate_day);
#define edf_set_patient_additional(a,b)		biosig_set_patient_additional(a,b)
#define edf_set_admincode(a,b)			biosig_set_admincode(a,b)
#define edf_set_technician(a,b)			biosig_set_technician(a,b)
#define edf_set_equipment(a,b)			biosig_set_equipment(a,b)

int edf_set_recording_additional(int handle, const char *recording_additional);
int edfwrite_physical_samples(int handle, double *buf);
int edf_blockwrite_physical_samples(int handle, double *buf);
int edfwrite_digital_samples(int handle, int *buf);
int edf_blockwrite_digital_samples(int handle, int *buf);

#define edfwrite_annotation_utf8(a,b,c,d) 	biosig_write_annotation_utf8(a,b,c,d) 
#define edfwrite_annotation_latin1(a,b,c,d) 	biosig_write_annotation_latin1(a,b,c,d) 
#define edf_set_datarecord_duration(a,b)	biosig_set_datarecord_duration(a,b)

#endif 


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif
