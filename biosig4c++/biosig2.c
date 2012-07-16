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


#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "biosig.h"
#include "biosig2.h"


#define hdrlistlen 64
struct hdrlist_t {
	HDRTYPE *hdr;		// header information as defined in level 1 interface
	//const char *filename; // name of file, is always hdr->FileName
	uint16_t NS; 	// number of effective channels, CHANNEL[].OnOff are ignored
	size_t *chanpos; 	// position of file handle for each channel
} ; 

struct hdrlist_t hdrlist[hdrlistlen];

CHANNEL_TYPE *getChannelHeader(HDRTYPE *hdr, uint16_t channel) {
	// returns channel header - skip Off-channels
	CHANNEL_TYPE *hc = hdr->CHANNEL;
	typeof(hdr->NS) chan = 0; 
	while (1) {
		if (hc->OnOff) {
			if (chan==channel) return hc;
			chan++;
		}
		hc++;
	}
	return NULL;
}


int biosig_lib_version(void) {
	return (BIOSIG_VERSION);
}

int biosig_open_file_readonly(const char *path, HDRTYPE *hdr, int read_annotations) {
/* 

	on success returns handle. 
*/
	int k = 0;
	while (k < hdrlistlen && hdrlist[k].hdr != NULL) k++;
	if (k >= hdrlistlen) return(-1);
	hdr = sopen(path,"r",hdr);
	hdrlist[k].hdr = hdr;
	//hdrlist[k].filename = hdr->FileName;
	hdrlist[k].NS  = 0; 
	for (k=0; k<hdr->NS; k++) 
		hdrlist[k].NS++;
	hdrlist[k].chanpos  = calloc(hdrlist[k].NS,sizeof(size_t)); 

	return(k);
}

int biosig_close_file(int handle) {
	destructHDR(hdrlist[handle].hdr);
	hdrlist[handle].hdr = NULL;
	if (hdrlist[handle].chanpos) free(hdrlist[handle].chanpos);
	hdrlist[handle].NS  = 0; 
	//hdrlist[handle].filename = NULL;
	
#if 0
	int k;
	for (k=0; k<hdrlistlen; k++)
		if (hdrlist[k].hdr!=NULL) return(0); 
	free(hdrlist);
#endif 
	return(0);
}

int biosig_read_samples(int handle, size_t channel, size_t n, double *buf, unsigned char UCAL) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL || hdrlist[handle].NS<=channel ) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;

	CHANNEL_TYPE *hc = getChannelHeader(hdr,channel);

	size_t stride = 1; // stride between consecutive samples of same channel, depends on data orientation hdr->FLAG.ROW_BASED_CHANNELS
	size_t div = hdr->SPR/hc->SPR; 	// stride if sample rate of channel is smaller than the overall sampling rate

	size_t POS = hdrlist[handle].chanpos[channel]*div;	// 
	size_t LEN = n*div;
	size_t startpos = POS/hdr->SPR;  // round towards 0
	size_t endpos = (POS+LEN)/hdr->SPR + ((POS+LEN)%hdr->SPR != 0);  // round towards infinity

	if (hdr->AS.first > startpos || (endpos-startpos) > hdr->AS.length || hdr->FLAG.UCAL!=UCAL) {
		// read data when not in data buffer hdr->data.block
		hdr->FLAG.UCAL = UCAL; 
		sread(NULL, startpos, endpos - startpos, hdr);
	}	

	// when starting position is not aligned with start of data
	size_t offset = hdr->AS.first * hdr->SPR - POS; 

	// find starting position and stride of data 
	double *data = hdr->data.block;
	if (hdr->FLAG.ROW_BASED_CHANNELS) {
		stride = hdr->data.size[0];
		data = hdr->data.block + channel + offset * stride;
	} 
	else {
		data = hdr->data.block + offset + channel * hdr->data.size[0];
	}
	size_t k;
	for (k = 0; k < n; k++) {
		buf[k] = data[k*div*stride];	// copy data into output buffer
	}
	hdrlist[handle].chanpos[channel] += n; // update position pointer of channel chan
	return (0);
}

int biosig_read_physical_samples(int handle, size_t biosig_signal, size_t n, double *buf) {
	return biosig_read_samples(handle, biosig_signal, n, buf, (unsigned char)(0));
}

int biosig_read_digital_samples(int handle, size_t biosig_signal, size_t n, double *buf) {
	return biosig_read_samples(handle, biosig_signal, n, buf, (unsigned char)(1));
}

size_t biosig_seek(int handle, long long offset, int whence) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	sseek(hdr, offset, whence);
	return (hdr->FILE.POS);
}

size_t biosig_tell(int handle) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	return(stell(hdrlist[handle].hdr));
}

void biosig_rewind(int handle, int biosig_signal) {
/* It is equivalent to: (void) biosig_seek(int handle, int biosig_signal, 0LL, biosig_SEEK_SET) */
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return;
	srewind(hdrlist[handle].hdr);
}

int biosig_get_annotation(int handle, size_t n, struct biosig_annotation_struct *annot) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	if (n>=hdr->EVENT.N) return (-1); 

	annot->onset = hdr->EVENT.POS[n];
	annot->duration = hdr->EVENT.DUR[n];
	annot->annotation = GetEventDescription(hdr, n);

	return(0);
}

int biosig_open_file_writeonly(const char *path, enum FileFormat filetype, int number_of_signals) {
#if 1
	int k = 0;
	while (k < hdrlistlen && hdrlist[k].hdr != NULL) k++;
	if (k>=hdrlistlen) return -1;

	HDRTYPE *hdr = constructHDR(number_of_signals,0);
#else
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
#endif
	hdrlist[k].hdr  = hdr;
	return(0); 
}

int biosig_set_samplefrequency(int handle, int biosig_signal, double samplefrequency) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);
	
	if (samplefrequency != hdr->SampleRate) {
		double spr = samplefrequency*hdr->SPR/hdr->SampleRate;
		hdr->CHANNEL[biosig_signal].SPR = spr;
		hdr->SampleRate = samplefrequency;
		if (spr!=ceil(spr)) return (-1);
	}
	return 0;
}

int biosig_set_physical_maximum(int handle, int biosig_signal, double phys_max) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].PhysMax = phys_max;
	return (0);
}

int biosig_set_physical_minimum(int handle, int biosig_signal, double phys_min) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].PhysMin = phys_min;
	return (0);
}

int biosig_set_digital_maximum(int handle, int biosig_signal, int dig_max) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].DigMax = dig_max;
	return (0);
}

int biosig_set_digital_minimum(int handle, int biosig_signal, int dig_min) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].DigMin = dig_min;
	return (0);
}

int biosig_set_label(int handle, int biosig_signal, const char *label) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	strncpy(hdr->CHANNEL[biosig_signal].Label, label, MAX_LENGTH_LABEL);
	return (0);
}

//int biosig_set_prefilter(int handle, int biosig_signal, const char *prefilter);
int biosig_set_highpassfilter(int handle, int biosig_signal, double frequency) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].HighPass = frequency;

	return 0; 
}

int biosig_set_lowpassfilter(int handle, int biosig_signal, double frequency) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].LowPass = frequency;

	return 0; 
}

int biosig_set_notchfilter(int handle, int biosig_signal, double frequency) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].Notch = frequency;

	return (0);
}


int biosig_set_transducer(int handle, int biosig_signal, const char *transducer) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	strncpy(hdr->CHANNEL[biosig_signal].Transducer, transducer, MAX_LENGTH_TRANSDUCER+1);

	return (0);
}


int biosig_set_physical_dimension(int handle, int biosig_signal, const char *phys_dim) {

	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	typeof(hdr->NS) ns = hdr->NS;
	if (biosig_signal >= ns) return(-1);

	hdr->CHANNEL[biosig_signal].PhysDimCode = PhysDimCode(phys_dim);

	return (0);
}


int biosig_set_startdatetime(int handle, int startdate_year, int startdate_month, int startdate_day, int starttime_hour, int starttime_minute, int starttime_second) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	struct tm T;
	T.tm_year = startdate_year;
	T.tm_mon  = startdate_month;
	T.tm_mday = startdate_day;
	T.tm_hour = starttime_hour;
	T.tm_min  = starttime_minute;
	T.tm_sec  = starttime_second;
	hdr->T0   = tm_time2gdf_time(&T);
	return (0);
}


int biosig_set_patientname(int handle, const char *patientname) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	strncpy(hdrlist[handle].hdr->Patient.Name, patientname, MAX_LENGTH_NAME+1);
	return (0);
}


int biosig_set_patientcode(int handle, const char *patientcode) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	strncpy(hdrlist[handle].hdr->Patient.Id, patientcode, MAX_LENGTH_PID+1);
	return(0);
}


int biosig_set_gender(int handle, int gender) {
	if (gender<0 || gender>9) return (-1); 
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	hdrlist[handle].hdr->Patient.Sex = gender;
	return(0);
}


int biosig_set_birthdate(int handle, int birthdate_year, int birthdate_month, int birthdate_day) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	struct tm T;
	T.tm_year = birthdate_year;
	T.tm_mon  = birthdate_month;
	T.tm_mday = birthdate_day;
	T.tm_hour = 12;
	T.tm_min  = 0;
	T.tm_sec  = 0;
	hdr->Patient.Birthday = tm_time2gdf_time(&T);
	return (0);
}


int biosig_set_patient_additional(int handle, const char *patient_additional) {
	fprintf(stderr,"Warning: biosig_set_patient_additional() not supported.\n");
	return (-1);
}


int biosig_set_admincode(int handle, const char *admincode) {
	fprintf(stderr,"Warning: biosig_set_admincode() not supported.\n");
	return (-1);
}


int biosig_set_technician(int handle, const char *technician) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	strncpy(hdrlist[handle].hdr->Patient.Id, technician, MAX_LENGTH_TECHNICIAN+1);
	return(0);
}

// TODO: implement the following functions
int biosig_set_equipment(int handle, const char *equipment) {
	return (-1);
}
int biosig_set_recording_additional(int handle, const char *recording_additional) {
	return (-1);
}
int biosig_write_physical_samples(int handle, double *buf) {
	return (-1);
}
int biosig_blockwrite_physical_samples(int handle, double *buf) {
	return (-1);
}
int biosig_write_digital_samples(int handle, int *buf) {
	return (-1);
}
int biosig_blockwrite_digital_samples(int handle, int *buf) {
	return (-1);
}

int biosig_write_annotation(int handle, size_t onset, size_t duration, const char *description) {
	/* onset and duration are in samples */
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;

	size_t N = hdr->EVENT.N++;
	hdr->EVENT.POS = (uint32_t*) realloc(hdr->EVENT.POS, hdr->EVENT.N*sizeof(*(hdr->EVENT.POS)) );
	hdr->EVENT.TYP = (uint16_t*) realloc(hdr->EVENT.TYP, hdr->EVENT.N*sizeof(*(hdr->EVENT.TYP)) );
	hdr->EVENT.DUR = (uint32_t*) realloc(hdr->EVENT.DUR, hdr->EVENT.N*sizeof(*(hdr->EVENT.DUR)) );
	hdr->EVENT.CHN = (uint16_t*) realloc(hdr->EVENT.CHN, hdr->EVENT.N*sizeof(*(hdr->EVENT.CHN)) );

	hdr->EVENT.POS[N] = onset;	
	hdr->EVENT.DUR[N] = duration;
	hdr->EVENT.CHN[N] = 0;
	FreeTextEvent(hdr, N, description);
	return (serror());
}

int biosig_write_annotation_utf8(int handle, size_t onset, size_t duration, const char *description) {
	fprintf(stdout,"biosig_write_annotation_latin1(): It's recommended to use biosig_write_annotation() instead.\n");
	return ( biosig_write_annotation(handle, onset, duration, description) );
}
int biosig_write_annotation_latin1(int handle, size_t onset, size_t duration, const char *description) {
	fprintf(stdout,"biosig_write_annotation_latin1(): It's recommended to use biosig_write_annotation() instead.\n");
	return ( biosig_write_annotation(handle, onset, duration, description) );
}

int biosig_set_datarecord_duration(int handle, double duration) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	double spr   = hdr->SampleRate * duration;
	size_t rspr  = round(spr);
	if (fabs(spr - rspr) > 1e-8*spr) {
		fprintf(stderr,"Warning biosig_set_datarecord_duration(): number of samples is not integer (%g) - rounded to integers (%i)\n",spr,(int)rspr);
	}
	hdr->SPR = (size_t)rspr;
	return 0;
}


#if defined(MAKE_EDFLIB)

int edfopen_file_writeonly(const char *path, int filetype, int number_of_signals) {
	enum FileFormat fmt=unknown; 
 	switch (filetype) {
	case EDFLIB_FILETYPE_EDF:
	case EDFLIB_FILETYPE_EDFPLUS:
		fmt = EDF;
		break;
	case EDFLIB_FILETYPE_BDF:
	case EDFLIB_FILETYPE_BDFPLUS:
		fmt = EDF;
		break;
	default:
		return(-1); 
	}
	return(biosig_open_file_writeonly(path, fmt, number_of_signals));
}

int edf_set_gender(int handle, int gender) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
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

long long edfseek(int handle, int channel, long long offset, int whence) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL || hdrlist[handle].NS<=channel ) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;

	switch (whence) {
	case SEEK_SET:
		hdrlist[handle].chanpos[channel] = offset; // update position pointer of channel chan
		break;
	case SEEK_CUR:
		hdrlist[handle].chanpos[channel] += offset; // update position pointer of channel chan
		break;
	case SEEK_END: {
		CHANNEL_TYPE *hc = getChannelHeader(hdr,channel);
		hdrlist[handle].chanpos[channel] = hdr->NRec*hc->SPR + offset; // update position pointer of channel chan
		break;
		}
	}	
	return (hdrlist[handle].chanpos[channel]);
}

long long edftell(int handle, int channel) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL || hdrlist[handle].NS<=channel ) return(-1);
	return ( hdrlist[handle].chanpos[channel] );
}

int edfrewind(int handle, int channel) {
/* It is equivalent to: (void) edf_seek(int handle, int biosig_signal, 0LL, SEEK_SET) */
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL || hdrlist[handle].NS<=channel ) return(-1);
	hdrlist[handle].chanpos[channel] = 0;
	return(0);
}

int edf_get_annotation(int handle, int n, struct edf_annotation_struct *annot) {
	if (handle<0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;

	annot->onset = hdr->EVENT.POS[n]*1e4/hdr->EVENT.SampleRate;
	annot->duration = hdr->EVENT.DUR[n]*1e4/hdr->EVENT.SampleRate;
	strncpy(annot->annotation,GetEventDescription(hdr, n),sizeof(annot->annotation));

	return(0);
}

int edfwrite_annotation(int handle, size_t onset, size_t duration, const char *description) {
	/* onset and duration are multiples of 100 microseconds */
	if (handle < 0 || handle >= hdrlistlen || hdrlist[handle].hdr==NULL) return(-1);
	HDRTYPE *hdr = hdrlist[handle].hdr;
	return (biosig_write_annotation(handle, onset*1e-4*hdr->EVENT.SampleRate, duration*1e-4*hdr->EVENT.SampleRate, description));
}

/*
   TODO: the following functions neeed to be implemented 	
*/
int edf_set_recording_additional(int handle, const char *recording_additional) {
	return fprintf(stderr,"this function is not implemented, yet.\n");
}

int edfwrite_physical_samples(int handle, double *buf) {
	return fprintf(stderr,"this function is not implemented, yet.\n");
}

int edf_blockwrite_physical_samples(int handle, double *buf) {
	return fprintf(stderr,"this function is not implemented, yet.\n");
}

int edfwrite_digital_samples(int handle, int *buf) {
	return fprintf(stderr,"this function is not implemented, yet.\n");
}

int edf_blockwrite_digital_samples(int handle, int *buf) {
	return fprintf(stderr,"this function is not implemented, yet.\n");
}

#endif 


