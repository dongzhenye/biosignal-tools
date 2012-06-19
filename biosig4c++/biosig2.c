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


#include <biosig.h>
#include "biosig2.h"


struct hdrlist_t {
	HDRTYPE *hdr;
} *hdrlist = NULL;
int hdrlistlen=0; 

int biosig_lib_version(void) {
	return(BIOSIG_VERSION);
}


int biosig_open_file_readonly(const char *path, HDRTYPE *hdr, int read_annotations) {

	int k = 0;
	HDRTYPE *hdr = sopen(path,"r",hdr); 
	while (k < hdrlistlen && hdrlist[k].hdr != NULL) k++;
	if (k >=hdrlistlen) {
		void *ptr = realloc(hdrlist, (k+1)*sizeof(*hdrlist));
		if (ptr==NULL) return (-1); 
		hdrlist = (struct hdrlist_t*) ptr;
		hdrlistlen = k+1;
	}
	hdrlist[k].hdr  = hdr;
}


int biosig_close_file(int handle) {
	destructHDR(hdrlist[handle].hdr);
	hdrlist[handle].hdr = NULL;
	int k;
	for (k=0; k<hdrlistlen; k++)
		if (hdrlist[k]!=NULL) return(0); 
	free(hdrlist);
	return(0);
}

// TODO
int biosig_read_physical_samples(int handle, int biosig_signal, int n, double *buf);
int biosig_read_digital_samples(int handle, int biosig_signal, int n, int *buf);

long long biosig_seek(int handle, int biosig_signal, long long offset, int whence) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	sseek(hdr, offset, whence);
	return (hdr->FILE.POS);
}

long long biosig_tell(int handle) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	return(stell(hdrlist[handle].hdr));
}

void biosig_rewind(int handle, int biosig_signal) {
/* It is equivalent to: (void) biosig_seek(int handle, int biosig_signal, 0LL, biosig_SEEK_SET) */
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	srewind(hdrlist[handle].hdr);
	return(0);
}

int biosig_get_annotation(int handle, int n, struct biosig_annotation_struct *annot) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	if (n<0 || n>=hdr->EVENT.N) return (-1); 

	annot->POS = hdr->EVENT.POS[n];
	annot->duration = hdr->EVENT.DUR[n];
// TODO: biosig.h needs a function to export event description
//	annot->annotation = (hdr->EVENT.TYP < hdr->EVENT.LenCodeDesc) ? hdr->EVENT.CodeDesc[hdr->EVENT.TYP] : // TODO

	return(0);
}


int biosig_open_file_writeonly(const char *path, enum FileFormat filetype, int number_of_signals) {

	HDRTYPE *hdr = constructHDR(number_of_signals,0);
	if (hdr==NULL) return (-1); 

	hdr->FileName = strdup(path);
	hdr->TYPE = filetype;
	int k = 0;
	while (k < hdrlistlen && hdrlist[k].hdr != NULL) k++;
	if (k>=hdrlistlen) {
		void *ptr = realloc(hdrlist, (k+1)*sizeof(*hdrlist));
		if (ptr==NULL) return (-1); 
		hdrlist = (struct hdrlist_t*) ptr;
		hdrlistlen = k+1;
	}
	hdrlist[k].hdr  = hdr;
	return(0); 
}


int biosig_set_samplefrequency(int handle, int biosig_signal, double samplefrequency) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);
	
	if (samplerate != hdr->SampleRate) {
		double spr = samplefrequency*hdr->SPR/hdr->SampleRate;
		hdr->CHANNEL[biosig_signal].SPR = spr;
		hdr->SampleRate = samplefrequency;
		if (spr!=ceil(spr)) return (-1);
	}
	return 0;
}


int biosig_set_physical_maximum(int handle, int biosig_signal, double phys_max) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].PhysMax = phys_max;
}


int biosig_set_physical_minimum(int handle, int biosig_signal, double phys_min) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].PhysMin = phys_min;
}


int biosig_set_digital_maximum(int handle, int biosig_signal, int dig_max) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].DigMax = dig_max;
}


int biosig_set_digital_minimum(int handle, int biosig_signal, int dig_min) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].DigMin = dig_min;
}


int biosig_set_label(int handle, int biosig_signal, const char *label) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	strncpy(hdr->CHANNEL[biosig_signal].Label, label, MAX_LENGTH_LABEL);
}


//int biosig_set_prefilter(int handle, int biosig_signal, const char *prefilter);
int biosig_set_highpassfilter(int handle, int biosig_signal, double frequency) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].HighPass = frequency;

	return 0; 
}


int biosig_set_lowpassfilter(int handle, int biosig_signal, double frequency) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].LowPass = frequency;

	return 0; 
}


int biosig_set_notchfilter(int handle, int biosig_signal, double frequency) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].Notch = frequency;

	return 0; 
}


int biosig_set_transducer(int handle, int biosig_signal, const char *transducer) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	strncpy(hdr->CHANNEL[biosig_signal].Transducer, transducer, MAX_LENGTH_Transducer);

	return 0; 
}


int biosig_set_physical_dimension(int handle, int biosig_signal, const char *phys_dim) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].PhysDimCode = physdimcode(phys_dim);

	return 0; 
}


int biosig_set_startdatetime(int handle, int startdate_year, int startdate_month, int startdate_day, int starttime_hour, int starttime_minute, int starttime_second) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	struct tm T;
	T.tm_year = startdate_year;
	T.tm_month = startdate_month;
	T.tm_mday = startdate_day;
	T.tm_hour = starttime_hour;
	T.tm_minute = starttime_minute;
	T.tm_second = starttime_second;
	hdr->T0 = tm_time2gdf_time(&T);
}


int biosig_set_patientname(int handle, const char *patientname) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	strncpy(hdrlist[handle].hdr->Patient.Name, patientname, MAX_LENGTH_NAME+1);
	return(0);
}


int biosig_set_patientcode(int handle, const char *patientcode) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	strncpy(hdrlist[handle].hdr->Patient.Id, patientcode, MAX_LENGTH_PID+1);
	return(0);
}


int biosig_set_gender(int handle, int gender) {
	if (gender<0 || gender>9) return (-1); 
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	hdrlist[handle].hdr->Patient.Sex = gender;
	return(0);
}


int biosig_set_birthdate(int handle, int birthdate_year, int birthdate_month, int birthdate_day) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	struct tm T;
	T.tm_year = birthdate_year;
	T.tm_month = birthdate_month;
	T.tm_mday = birthdate_day;
	T.tm_hour = 12;
	T.tm_minute = 0;
	T.tm_second = 0;
	hdr->Patient.Birthday = tm_time2gdf_time(&T);
}


int biosig_set_patient_additional(int handle, const char *patient_additional) {
	fprintf(stderr,"Warning: biosig_set_patient_additional() not supported.\n");
}


int biosig_set_admincode(int handle, const char *admincode) {
	fprintf(stderr,"Warning: biosig_set_admincode() not supported.\n");
}


int biosig_set_technician(int handle, const char *technician) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	strncpy(hdrlist[handle].hdr->Patient.Id, technician, MAX_LENGTH_TECHNICIAN+1);
	return(0);
}

// TODO: implement the following functions
int biosig_set_equipment(int handle, const char *equipment);
int biosig_set_recording_additional(int handle, const char *recording_additional);
int biosig_write_physical_samples(int handle, double *buf);
int biosig_blockwrite_physical_samples(int handle, double *buf);
int biosig_write_digital_samples(int handle, int *buf);
int biosig_blockwrite_digital_samples(int handle, int *buf);
int biosig_write_annotation(int handle, long long onset, long long duration, const char *description) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;

	size_t N = hdr->EVENT.N++;
	hdr->EVENT.POS = (uint32_t*) realloc(hdr->EVENT.POS, hdr->EVENT.N*sizeof(*hdr->EVENT.POS) );
	hdr->EVENT.TYP = (uint16_t*) realloc(hdr->EVENT.TYP, hdr->EVENT.N*sizeof(*hdr->EVENT.TYP) );
	hdr->EVENT.DUR = (uint32_t*) realloc(hdr->EVENT.DUR, hdr->EVENT.N*sizeof(*hdr->EVENT.DUR) );
	hdr->EVENT.CHN = (uint16_t*) realloc(hdr->EVENT.CHN, hdr->EVENT.N*sizeof(*hdr->EVENT.CHN) );

	FreeTextEvent(hdr, N, description);
	hdr->EVENT.POS[N] = onset;
	hdr->EVENT.DUR[N] = duration;
	hdr->EVENT.CHN[N] = 0;
	return (0);
}
int biosig_write_annotation_utf8(int handle, long long onset, long long duration, const char *description);
int biosig_write_annotation_latin1(int handle, long long onset, long long duration, const char *description) {


int biosig_set_datarecord_duration(int handle, double duration) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
// TODO: 
	hdr->SPR = hdr->SampleRate * duration;
	return 0;
}


#if defined(EDFLIB_INCLUDED)

int edfopen_file_writeonly(const char *path, int filetype, int number_of_signals) {
	enum FileFormat fmt=unknown; 
 	switch (filetype) {
	case EDFLIB_FILETYPE_EDF:
	case EDFLIB_FILETYPE_EDFPLUS:
		fmt = EDF;
		break
	case EDFLIB_FILETYPE_BDF:
	case EDFLIB_FILETYPE_BDFPLUS:
		fmt = EDF;
		break
	default:
		return(-1); 

	return(biosig_open_file_writeonly(path, fmt, number_of_signals));
}

int edf_set_gender(int handle, int gender) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	hdr->Patient.Sex = (gender==1) + (gender==0)*2 ;
}

int edfread_physical_samples(int handle, int edfsignal, int n, double *buf) {
	fprintf(stderr,"error: edfread_physical_samples - use biosig_read_physical_samples instead.\n");
	return(-1);
}

int edfread_digital_samples(int handle, int edfsignal, int n, int *buf) {
	fprintf(stderr,"error: edfread_digital_samples - use biosig_read_digital_samples instead.\n");
	return(-1);
}

long long edfseek(int handle, int biosig_signal, long long offset, int whence) {
	fprintf(stderr,"Warning edfseek: argument 'edfsignal' ignored - use biosig_seek instead.\n");
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	sseek(hdr, offset, whence);
	return (hdr->FILE.POS);
}

long long edftell(int handle, int biosig_signal) {
	fprintf(stderr,"Warning edftell: argument 'edfsignal' ignored - use biosig_tell instead.\n");
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	return(stell(hdrlist[handle].hdr));
}

int edfrewind(int handle, int edfsignal) {
/* It is equivalent to: (void) edf_seek(int handle, int biosig_signal, 0LL, biosig_SEEK_SET) */
	fprintf(stderr,"Warning edfrewind: argument 'edfsignal' ignored - use biosig_rewind instead.\n");
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle]==NULL) return(-1);
	srewind(hdrlist[handle].hdr);
	return(0);
}

//#define edf_get_annotation(int handle, int n, struct edf_annotation_struct *annot);
int edf_get_annotation(int handle, int n, struct edf_annotation_struct *annot){
	fprintf(stderr,"error: edf_get_annotation - use biosig_get_annotation instead.\n");
	return(-1);
}

#endif 

