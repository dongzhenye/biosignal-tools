/*

    Copyright (C) 2012,2013 Alois Schloegl <alois.schloegl@gmail.com>
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


// swig.i

%module biosig2
%{
#define SWIG_FILE_WITH_INIT
#include "../biosig2.h"
#include <numpy/arrayobject.h>
%}


%include <inttypes.i>

// import_array() call initialises Numpy
%init
%{
        import_array();
%}

/* disable deprecated functions - deprecated functions are curently not provided by the swig interface */
#define ATT_DEPREC //

HDRTYPE* constructHDR(const unsigned NS, const unsigned N_EVENT);
/* 	allocates memory initializes header HDR of type HDRTYPE
	with NS channels an N_EVENT event elements
 --------------------------------------------------------------- */
void 	 destructHDR(HDRTYPE* hdr);
/* 	destroys the header *hdr and frees allocated memory
 --------------------------------------------------------------- */

/* =============================================================
	setter and getter functions for accessing fields of HDRTYPE
	these functions are currently experimential and are likely to change
   ============================================================= */

/* get, set and check function of filetype */
enum FileFormat biosig_get_filetype(HDRTYPE *hdr);
int biosig_set_filetype(HDRTYPE *hdr, enum FileFormat format);
#define biosig_check_filetype(a,b) (biosig_get_filetype(a)==b)

ATT_DEPREC int biosig_set_flags(HDRTYPE *hdr, char compression, char ucal, char overflowdetection);
int biosig_get_flag(HDRTYPE *hdr, unsigned flags);
int biosig_set_flag(HDRTYPE *hdr, unsigned flags);
int biosig_reset_flag(HDRTYPE *hdr, unsigned flags);

int biosig_set_targetsegment(HDRTYPE *hdr, unsigned targetsegment);
int biosig_get_targetsegment(HDRTYPE *hdr);

const char* biosig_get_filename(HDRTYPE *hdr);
float biosig_get_version(HDRTYPE *hdr);

int biosig_set_segment_selection(HDRTYPE *hdr, int k, uint32_t argSweepSel);
uint32_t* biosig_get_segment_selection(HDRTYPE *hdr);


// returns error message in memory allocated with strdup
int biosig_check_error(HDRTYPE *hdr);
char *biosig_get_errormsg(HDRTYPE *hdr);

long biosig_get_number_of_channels(HDRTYPE *hdr);
size_t biosig_get_number_of_samples(HDRTYPE *hdr);
ATT_DEPREC size_t biosig_get_number_of_samples_per_record(HDRTYPE *hdr);
size_t biosig_get_number_of_records(HDRTYPE *hdr);
size_t biosig_get_number_of_segments(HDRTYPE *hdr);

int biosig_set_number_of_channels(HDRTYPE *hdr, int ns);
int biosig_set_number_of_samples(HDRTYPE *hdr, ssize_t nrec, ssize_t spr);
#define biosig_set_number_of_samples_per_record(h,n)  biosig_set_number_of_samples(h,-1,n)
#define biosig_set_number_of_records(h,n)             biosig_set_number_of_samples(h,n,-1)
// ATT_DEPREC int biosig_set_number_of_segments(HDRTYPE *hdr, );

int biosig_get_datablock(HDRTYPE *hdr, biosig_data_type **data, size_t *rows, size_t *columns);
biosig_data_type* biosig_get_data(HDRTYPE *hdr, char flag);

double biosig_get_samplerate(HDRTYPE *hdr);
int biosig_set_samplerate(HDRTYPE *hdr, double fs);

size_t biosig_get_number_of_events(HDRTYPE *hdr);
size_t biosig_set_number_of_events(HDRTYPE *hdr, size_t N);

// get n-th event, variables pointing to NULL are ignored
int biosig_get_nth_event(HDRTYPE *hdr, size_t n, uint16_t *typ, uint32_t *pos, uint16_t *chn, uint32_t *dur, gdf_time *timestamp, char **desc);
/* set n-th event, variables pointing to NULL are ignored
   typ or  Desc can be used to determine the type of the event.
   if both, typ and Desc, are not NULL, the result is undefined */
int biosig_set_nth_event(HDRTYPE *hdr, size_t n, uint16_t* typ, uint32_t *pos, uint16_t *chn, uint32_t *dur, gdf_time *timestamp, char *Desc);

double biosig_get_eventtable_samplerate(HDRTYPE *hdr);
int    biosig_set_eventtable_samplerate(HDRTYPE *hdr, double fs);
int    biosig_change_eventtable_samplerate(HDRTYPE *hdr, double fs);


int biosig_get_startdatetime(HDRTYPE *hdr, struct tm *T);
int biosig_set_startdatetime(HDRTYPE *hdr, struct tm *T);

int biosig_get_birthdate(HDRTYPE *hdr, struct tm *T);
int biosig_set_birthdate(HDRTYPE *hdr, struct tm *T);

const char* biosig_get_patient_name(HDRTYPE *hdr);
const char* biosig_get_patient_id(HDRTYPE *hdr);

const char* biosig_get_recording_id(HDRTYPE *hdr);
const char* biosig_get_technician(HDRTYPE *hdr);
const char* biosig_get_manufacturer_name(HDRTYPE *hdr);
const char* biosig_get_manufacturer_model(HDRTYPE *hdr);
const char* biosig_get_manufacturer_version(HDRTYPE *hdr);
const char* biosig_get_manufacturer_serial_number(HDRTYPE *hdr);

int biosig_set_patient_name(HDRTYPE *hdr, const char* rid);
int biosig_set_patient_id(HDRTYPE *hdr, const char* rid);
int biosig_set_recording_id(HDRTYPE *hdr, const char* rid);
int biosig_set_technician(HDRTYPE *hdr, const char* rid);
int biosig_set_manufacturer_name(HDRTYPE *hdr, const char* rid);
int biosig_set_manufacturer_model(HDRTYPE *hdr, const char* rid);
int biosig_set_manufacturer_version(HDRTYPE *hdr, const char* rid);
int biosig_set_manufacturer_serial_number(HDRTYPE *hdr, const char* rid);

double biosig_get_channel_samplerate(HDRTYPE *hdr, int chan);
int biosig_set_channel_samplerate_and_samples_per_record(HDRTYPE *hdr, int chan, ssize_t spr, double fs);


/* =============================================================
	setter and getter functions for accessing fields of CHANNEL_TYPE
	these functions are currently experimential and are likely to change
   ============================================================= */

// returns M-th channel, M is zero-based
CHANNEL_TYPE* biosig_get_channel(HDRTYPE *hdr, int M);

const char* biosig_channel_get_label(CHANNEL_TYPE *chan);
int         biosig_channel_set_label(CHANNEL_TYPE *chan, const char* label);

uint16_t    biosig_channel_get_physdimcode(CHANNEL_TYPE *chan);
const char* biosig_channel_get_physdim(CHANNEL_TYPE *chan);
#define     biosig_channel_get_unit(h) biosig_channel_get_physdim(h)

int         biosig_channel_set_physdimcode(CHANNEL_TYPE *chan, uint16_t physdimcode);
#define     biosig_channel_set_physdim(a,b) biosig_channel_set_physdimcode(a, PhysDimCode(b))
#define     biosig_channel_set_unit(a,b) biosig_channel_set_physdimcode(a, PhysDimCode(b))

// this will affect result of next SREAD when flag.ucal==0
int     biosig_channel_change_scale_to_physdimcode(CHANNEL_TYPE *chan, uint16_t physdimcode);
#define biosig_channel_change_scale_to_unitcode(a,b) biosig_channel_set_scale_to_physdimcode(a, b)
#define biosig_channel_change_scale_to_physdim(a,b) biosig_channel_set_scale_to_physdimcode(a, PhysDimCode(b))
#define biosig_channel_change_scale_to_unit(a,b) biosig_channel_set_scale_to_physdimcode(a, PhysDimCode(b))

int biosig_channel_get_scaling(CHANNEL_TYPE *chan, double *PhysMax, double *PhysMin, double *DigMax, double *DigMin);
int biosig_channel_set_scaling(CHANNEL_TYPE *chan, double PhysMax, double PhysMin, double DigMax, double DigMin);
double biosig_channel_get_cal(CHANNEL_TYPE *chan);
double biosig_channel_get_off(CHANNEL_TYPE *chan);
ATT_DEPREC int biosig_channel_set_cal(CHANNEL_TYPE *chan, double cal);
ATT_DEPREC int biosig_channel_set_off(CHANNEL_TYPE *chan, double off);

int    biosig_channel_get_filter(CHANNEL_TYPE *chan, double *LowPass, double *HighPass, double *Notch);
int    biosig_channel_set_filter(CHANNEL_TYPE *chan, double LowPass, double HighPass, double Notch);

double biosig_channel_get_timing_offset(CHANNEL_TYPE *hc);
int    biosig_channel_set_timing_offset(CHANNEL_TYPE *hc, double off);

double biosig_channel_get_impedance(CHANNEL_TYPE *hc);
int    biosig_channel_set_impedance(CHANNEL_TYPE *hc, double val);

/*
double biosig_channel_get_samplerate(CHANNEL_TYPE *hc);
int    biosig_channel_set_samplerate_and_samples_per_record(CHANNEL_TYPE *hc, size_t spr, double val);
*/

size_t biosig_channel_get_samples_per_record(CHANNEL_TYPE *hc);
int    biosig_channel_set_samples_per_record(CHANNEL_TYPE *hc, size_t spr);

uint16_t biosig_channel_get_datatype(CHANNEL_TYPE *hc);
int  biosig_channel_set_datatype(CHANNEL_TYPE *hc, uint16_t gdftyp);
#define biosig_channel_set_datatype_to_int8(h)		biosig_channel_set_datatype(h,1)
#define biosig_channel_set_datatype_to_uint8(h)		biosig_channel_set_datatype(h,2)
#define biosig_channel_set_datatype_to_int16(h)		biosig_channel_set_datatype(h,3)
#define biosig_channel_set_datatype_to_uint16(h)	biosig_channel_set_datatype(h,4)
#define biosig_channel_set_datatype_to_int32(h)		biosig_channel_set_datatype(h,5)
#define biosig_channel_set_datatype_to_uint32(h)	biosig_channel_set_datatype(h,6)
#define biosig_channel_set_datatype_to_int64(h)		biosig_channel_set_datatype(h,7)
#define biosig_channel_set_datatype_to_uint64(h)	biosig_channel_set_datatype(h,8)
#define biosig_channel_set_datatype_to_float(h)		biosig_channel_set_datatype(h,16)
#define biosig_channel_set_datatype_to_single(h)	biosig_channel_set_datatype(h,16)
#define biosig_channel_set_datatype_to_double(h)	biosig_channel_set_datatype(h,17)

const char *biosig_channel_get_transducer(CHANNEL_TYPE *hc);
int biosig_channel_set_transducer(CHANNEL_TYPE *hc, const char *transducer);



