/*

    $Id$
    Copyright (C) 2010,2011,2012 Alois Schloegl <alois.schloegl@gmail.com>

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

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../biosig-dev.h"


EXTERN_C void sopen_cfs_read(HDRTYPE* hdr) {	
/*
	this function will be called by the function SOPEN in "biosig.c"

	Input: 
		char* Header	// contains the file content
		
	Output: 
		HDRTYPE *hdr	// defines the HDR structure accoring to "biosig.h"
*/


#define H1LEN (8+14+4+8+8+2+2+2+2+2+4+2+2+74+4+40)

		size_t count = hdr->HeadLen; 

#define CFS_NEW		// this flag allows to switch back to old version 

		while (!ifeof(hdr)) {
			hdr->AS.Header = (uint8_t*) realloc(hdr->AS.Header,count*2+1);
			count += ifread(hdr->AS.Header+count,1,count,hdr);
		}
		hdr->AS.Header[count] = 0;
		hdr->FLAG.OVERFLOWDETECTION = 0;

		/*
			Implementation is based on the following reference: 
		        CFS - The CED Filing System October 2006
		*/

		/* General Header */
		// uint32_t filesize = leu32p(hdr->AS.Header+22);	// unused
		hdr->FILE.size = leu32p(hdr->AS.Header+0x16);	//    file size
		hdr->NS    = leu16p(hdr->AS.Header+0x2a);	// 6  number of channels
		uint16_t n = leu16p(hdr->AS.Header+0x2c);	// 7  number of file variables
		uint16_t d = leu16p(hdr->AS.Header+0x2e);	// 8  number of data section variables
		uint16_t FileHeaderSize = leu16p(hdr->AS.Header+0x30);	// 9  byte size of file header
		uint16_t DataHeaderSize = leu16p(hdr->AS.Header+0x32);	// 10 byte size of data section header
		uint32_t LastDataSectionHeaderOffset = leu32p(hdr->AS.Header+0x34);	// 11 last data section header offset
		uint16_t NumberOfDataSections = leu16p(hdr->AS.Header+0x38);	// 12 last data section header offset

		// decode start time and date
		{
			struct tm t;
			char strtmp[9];
			memcpy(strtmp, hdr->AS.Header+0x1a, 8);
			strtmp[8] = 0; // terminating null character
			t.tm_hour = atoi(strtok(strtmp,":/"));
			t.tm_min  = atoi(strtok(NULL,":/"));
			t.tm_sec  = atoi(strtok(NULL,":/"));
			memcpy(strtmp, hdr->AS.Header+0x22, 8);
			t.tm_mday = atoi(strtok(strtmp,":/"));
			t.tm_mon  = atoi(strtok(NULL,":/"))-1;
			t.tm_year = atoi(strtok(NULL,":/"));
			if (t.tm_year<39) t.tm_year+=100;
			hdr->T0 = tm_time2gdf_time(&t);
		}

		if (NumberOfDataSections) {
			hdr->EVENT.TYP = (typeof(hdr->EVENT.TYP)) realloc(hdr->EVENT.TYP, (hdr->EVENT.N + NumberOfDataSections - 1) * sizeof(*hdr->EVENT.TYP));
			hdr->EVENT.POS = (typeof(hdr->EVENT.POS)) realloc(hdr->EVENT.POS, (hdr->EVENT.N + NumberOfDataSections - 1) * sizeof(*hdr->EVENT.POS));
		}

if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 131 - %d,%d,%d,0x%x,0x%x,0x%x,%d,0x%x\n",hdr->NS,n,d,FileHeaderSize,DataHeaderSize,LastDataSectionHeaderOffset,NumberOfDataSections,leu32p(hdr->AS.Header+0x86));

		/* channel information */
		hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
#define H2LEN (22+10+10+1+1+2+2)
 		char* H2 = (char*)(hdr->AS.Header + H1LEN);
		double xPhysDimScale[100];		// CFS is limited to 99 channels
		typeof(hdr->NS) NS = hdr->NS;
		uint8_t k;
		for (k = 0; k < hdr->NS; k++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL + k;
			/*
				1 offset because CFS uses pascal type strings (first byte contains string length)
				in addition, the strings are \0 terminated.
			*/
			hc->OnOff = 1;
			hc->LeadIdCode = 0; 
			hc->bi8 = 0; 

			uint8_t len = min(21, MAX_LENGTH_LABEL);
			strncpy(hc->Label, H2 + 1 + k*H2LEN, len);	// Channel name 
			len = strlen(hc->Label);
			while (isspace(hc->Label[len])) len--;		// remove trailing blanks
			hc->Label[len+1] = 0;

			hc->PhysDimCode  = PhysDimCode (H2 + 22 + 1 + k*H2LEN);			// Y axis units
			xPhysDimScale[k] = PhysDimScale(PhysDimCode(H2 + 32 + 1 + k*H2LEN));	// X axis units

			uint8_t  dataType  = H2[42 + k*H2LEN];
			//uint8_t  dataKind  = H2[43 + k*H2LEN];
			uint16_t byteSpace = leu16p(H2+44 + k*H2LEN);		// stride 
			//uint16_t next      = leu16p(H2+46 + k*H2LEN);
			hc->GDFTYP = dataType < 5 ? dataType+1 : dataType+11;
			if (H2[43 + k * H2LEN] == 1) {
				// Matrix data does not return an extra channel, but contains Markers and goes into the event table.
				hc->OnOff = 0;
				NS--;
			}
			hc->Impedance = NAN;
			hc->TOffset  = NAN;
			hc->LowPass  = NAN;
			hc->HighPass = NAN;
			hc->Notch    = NAN;
if (VERBOSE_LEVEL>7) fprintf(stdout,"Channel #%i: [%s](%i/%i) <%s>/<%s> ByteSpace%i,Next#%i\n",k+1, H2 + 1 + k*H2LEN, dataType, H2[43], H2 + 23 + k*H2LEN, H2 + 33 + k*H2LEN, leu16p(H2+44+k*H2LEN), leu16p(H2+46+k*H2LEN));
		}

		size_t datapos = H1LEN + H2LEN*hdr->NS;

		/* file variable information */
		// n*36 bytes
if (VERBOSE_LEVEL>7) fprintf(stdout,"\n******* file variable information *********\n");
		for (k = 0; k < n; k++) {
			int i=-1; double f=NAN;
			size_t pos = datapos + k*36;
			uint16_t typ = leu16p(hdr->AS.Header+pos+22);
			char   *unit = (char*)(hdr->AS.Header+pos+25);
			uint16_t off = leu16p(hdr->AS.Header+pos+34);

			size_t p3 = H1LEN + H2LEN*hdr->NS + (n+d)*36 + off + 42;

			switch (typ) {
			case 0:
			case 1: i = hdr->AS.Header[p3]; break;
			case 2: i = lei16p(hdr->AS.Header+p3); break;
			case 3: i = leu16p(hdr->AS.Header+p3); break;
			case 4: i = lei32p(hdr->AS.Header+p3); break;
			case 5: f = lef32p(hdr->AS.Header+p3); break;
			case 6: f = lef64p(hdr->AS.Header+p3); break;
			}
if (VERBOSE_LEVEL>8) 	{
			fprintf(stdout,"[%i:] ",typ);
			if (typ<1) ;
			else if (typ<5) fprintf(stdout,"%d",i);
			else if (typ<7) fprintf(stdout,"%g",f);
			else if (typ==7) fprintf(stdout,"%s",hdr->AS.Header+p3+1);
			fprintf(stdout,"%s\n",unit+1);
			}
else if (VERBOSE_LEVEL>7)
			{
			if (typ==7) fprintf(stdout,"[%i:] %s %s",typ, hdr->AS.Header+p3+1,unit);
			}
		}

if (VERBOSE_LEVEL>7) fprintf(stdout,"\n******* Data Section variable information *********\n");
		datapos = LastDataSectionHeaderOffset; //H1LEN + H2LEN*hdr->NS + n*36;
		// reverse order of data sections
		uint32_t *DATAPOS = (uint32_t*)malloc(sizeof(uint32_t)*NumberOfDataSections);

		uint16_t m;
		for (m = NumberOfDataSections; 0 < m; ) {
			DATAPOS[--m] = datapos;
			datapos = leu32p(hdr->AS.Header + datapos);
		}

		for (m = 0; m < NumberOfDataSections; m++ ) {
			int i=-1; double f=NAN;
			size_t pos = DATAPOS[m];
			uint16_t typ = leu16p(hdr->AS.Header+pos+22);
			char   *unit = (char*)(hdr->AS.Header+pos+25);
			uint16_t off = leu16p(hdr->AS.Header+pos+34);

			size_t p3 = H1LEN + H2LEN*hdr->NS + (n+d)*36 + off + 42;

			switch (typ) {
			case 0:
			case 1: i = hdr->AS.Header[p3]; break;
			case 2: i = lei16p(hdr->AS.Header+p3); break;
			case 3: i = leu16p(hdr->AS.Header+p3); break;
			case 4: i = lei32p(hdr->AS.Header+p3); break;
			case 5: f = lef32p(hdr->AS.Header+p3); break;
			case 6: f = lef64p(hdr->AS.Header+p3); break;
			}
if (VERBOSE_LEVEL>8) 	{
			fprintf(stdout,"[%i:] ",typ);
			if (typ<5) fprintf(stdout,"%d",i);
			else if (typ<7) fprintf(stdout,"%g",f);
			else if (typ==7) fprintf(stdout,"%s",hdr->AS.Header+p3+1);
			fprintf(stdout,"%s\n",unit+1);
			}
else if (VERBOSE_LEVEL>7)
			{
			if (typ==7) fprintf(stdout,"[%i:] %s %s",typ, hdr->AS.Header+p3+1,unit);
			}
		}

		if (hdr->AS.SegSel[0] > NumberOfDataSections) {
			fprintf(stderr,"Warning loading CFS file: selected sweep number is larger than number of sweeps [%d,%d] - no data is loaded\n",hdr->AS.SegSel[0], NumberOfDataSections);
			NumberOfDataSections = 0;	
		}
		else if (0 < hdr->AS.SegSel[0]) {
			// hack: if sweep is selected, use same method than for data with a single sweep 	
			DATAPOS[0] = DATAPOS[hdr->AS.SegSel[0]-1];
			NumberOfDataSections = 1; 
		}

//		void *VarChanInfoPos = hdr->AS.Header + datapos + 30;  // unused
		char flag_ChanInfoChanged = 0;
		hdr->NRec = NumberOfDataSections;
		size_t SPR = 0, SZ = 0;
		for (m = 0; m < NumberOfDataSections; m++) {
			if (m>0) {
				hdr->EVENT.TYP[hdr->EVENT.N] = 0x7ffe;
				hdr->EVENT.POS[hdr->EVENT.N] = SPR;
				hdr->EVENT.N++;
			}

			datapos = DATAPOS[m];
			if (!leu32p(hdr->AS.Header+datapos+8)) continue; 	// empty segment

if (VERBOSE_LEVEL>7) fprintf(stdout,"\n******* DATA SECTION --%03i-- %i *********\n",m,flag_ChanInfoChanged);
if (VERBOSE_LEVEL>7) fprintf(stdout,"\n[DS#%3i] 0x%x 0x%x [0x%x 0x%x szChanData=%i] 0x02%x\n", m, FileHeaderSize, (int)datapos, leu32p(hdr->AS.Header+datapos), leu32p(hdr->AS.Header+datapos+4), leu32p(hdr->AS.Header+datapos+8), leu16p(hdr->AS.Header+datapos+12));

			uint32_t sz    = 0;
			uint32_t bpb   = 0, spr = 0;
			hdr->AS.first  = 0;
			hdr->AS.length = 0;
			char flag_firstchan = 1; 
			for (k = 0; k < hdr->NS; k++) {
				uint8_t *pos = hdr->AS.Header + datapos + 30 + 24 * k;

				CHANNEL_TYPE *hc = hdr->CHANNEL + k;

				hc->bi      = leu32p(pos);
				hc->SPR     = leu32p(pos+4);
				hc->Cal     = lef32p(pos+8);
				hc->Off     = lef32p(pos+12);
				double Xcal = lef32p(pos+16);
				double Xoff = lef32p(pos+20);// unused

if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 409: %i #%i: SPR=%i=%i=%i  x%f+-%f %i x%g %g %g\n",m,k,spr,(int)SPR,hc->SPR,hc->Cal,hc->Off,hc->bi,xPhysDimScale[k], Xcal, Xoff);

				double Fs = 1.0 / (xPhysDimScale[k] * Xcal);
				if ( (hc->OnOff == 0) || (Xcal == 0.0) ) {
					; // do nothing:  
				}
				else if (flag_firstchan) {
					hdr->SampleRate = Fs;
					flag_firstchan = 0;
				}
				else if (fabs(hdr->SampleRate - Fs) > 1e-3) {
					biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "CED/CFS: different sampling rates are not supported\n");
				}

				if (hc->OnOff && (spr < hc->SPR)) 
					spr = hc->SPR;
				sz  += hc->SPR * GDFTYP_BITS[hc->GDFTYP] >> 3;
				bpb += GDFTYP_BITS[hc->GDFTYP]>>3;	// per single sample
				hdr->AS.length += hc->SPR;
				
			}

			SPR += spr;
			SZ  += sz;
		}

		/*
		The previous and the next loop have been split, in oder to avoid
		multiple reallocations. Multiple reallocations seem to be a major
		issue for performance problems of Windows (a 50 MB file with 480 sweeps took 50 s to load).
		However, the two loops might contain some redundant operations.
		*/
		hdr->AS.flag_collapsed_rawdata = 0;
		void *ptr = realloc(hdr->AS.rawdata, hdr->NS * SPR * sizeof(double));
		if (!ptr) {
			biosigERROR(hdr,B4C_MEMORY_ALLOCATION_FAILED, "CFS: memory allocation failed in line __LINE__");
		}
		hdr->AS.rawdata = (uint8_t*)ptr;

		SPR = 0, SZ = 0;
		for (m = 0; m < NumberOfDataSections; m++) {

			datapos = DATAPOS[m];
			if (!leu32p(hdr->AS.Header+datapos+8)) continue; 	// empty segment

			uint32_t sz    = 0;
			uint32_t bpb   = 0, spr = 0;
			hdr->AS.first  = 0;
			hdr->AS.length = 0;
			char flag_firstchan = 1;
			for (k = 0; k < hdr->NS; k++) {
				uint8_t *pos = hdr->AS.Header + datapos + 30 + 24 * k;

				CHANNEL_TYPE *hc = hdr->CHANNEL + k;

				hc->bi      = leu32p(pos);
				hc->SPR     = leu32p(pos+4);
				hc->Cal     = lef32p(pos+8);
				hc->Off     = lef32p(pos+12);
				double Xcal = lef32p(pos+16);
				double Xoff = lef32p(pos+20);// unused

if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 409: %i #%i: SPR=%i=%i=%i  x%f+-%f %i x%g %g %g\n",m,k,spr,(int)SPR,hc->SPR,hc->Cal,hc->Off,hc->bi,xPhysDimScale[k], Xcal, Xoff);

				double Fs = 1.0 / (xPhysDimScale[k] * Xcal);
				if ( (hc->OnOff == 0) || (Xcal == 0.0) ) {
					; // do nothing:
				}
				else if (flag_firstchan) {
					hdr->SampleRate = Fs;
					flag_firstchan = 0;
				}
				else if (fabs(hdr->SampleRate - Fs) > 1e-3) {
					biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "CED/CFS: different sampling rates are not supported\n");
				}

				if (hc->OnOff && (spr < hc->SPR))
					spr = hc->SPR;
				sz  += hc->SPR * GDFTYP_BITS[hc->GDFTYP] >> 3;
				bpb += GDFTYP_BITS[hc->GDFTYP]>>3;	// per single sample
				hdr->AS.length += hc->SPR;

			}

			if (1) {
				/* hack: copy data into a single block (rawdata)
				   this is used as a data cache, no I/O is needed in sread, at the cost that sopen is slower
				   sread_raw does not attempt to reload the data
				 */

				hdr->AS.first = 0;
				uint8_t ns = 0; 
				for (k = 0; k < hdr->NS; k++) {
					CHANNEL_TYPE *hc = hdr->CHANNEL + k;
					

					uint32_t memoffset = + hc->bi;
					uint8_t *srcaddr = hdr->AS.Header + leu32p(hdr->AS.Header+datapos + 4) ;
					//uint16_t byteSpace = leu16p(H2+44 + k*H2LEN);
					int16_t stride = leu16p(H2+44 + k*H2LEN);

					uint8_t  dataType  = H2[42 + k*H2LEN];
					//uint8_t  dataKind  = H2[43 + k*H2LEN];		// equidistant, Subsidiary or Matrix data 
					//uint16_t stride = leu16p(H2+44 + k*H2LEN);		// byteSpace
					uint16_t next      = leu16p(H2+46 + k*H2LEN);
					hc->GDFTYP = dataType < 5 ? dataType+1 : dataType+11;
		
if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 412 #%i %i %i %i: %i @%p %i\n", k, hc->SPR, hc->GDFTYP, stride, memoffset, srcaddr, leu32p(hdr->AS.Header+datapos + 4) + leu32p(hdr->AS.Header + datapos + 30 + 24 * k));

					size_t k2;
					for (k2 = 0; k2 < hc->SPR; k2++) {
						uint8_t *ptr = srcaddr + hc->bi + k2*stride;
						double val;
						
						switch (hc->GDFTYP) {
						// reorder for performance reasons - more frequent gdftyp's come first
						case 3:  val = lei16p(ptr); break;
						case 4:  val = leu16p(ptr); break;
						case 16: val = lef32p(ptr); break;
						case 17: val = lef64p(ptr); break;
						case 0:  val = *(   char*) ptr; break;
						case 1:  val = *( int8_t*) ptr; break;
						case 2:  val = *(uint8_t*) ptr; break;
						case 5:  val = lei32p(ptr); break;
						case 6:  val = leu32p(ptr); break;
						case 7:  val = lei64p(ptr); break;
						case 8:  val = leu64p(ptr); break;
						default:
							val = NAN;
							biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "CED/CFS: invalid data type");
						}

					   	if (hc->OnOff) {
							/* TODO: channels with less samples are currently ignored - resampling or ignoring the channel ? */
							*(double*) (hdr->AS.rawdata + k * sizeof(double) + (SPR + k2) * hdr->NS * sizeof(double)) = val * hc->Cal + hc->Off;
							continue;
					   	}
						
						if (!strncmp(hc->Label,"Marker",6) && hc->PhysDimCode==2176 && hc->GDFTYP==5 && next != 0) { 
							// matrix data might contain time markers. 						

							// memory allocation for additional events - more efficient implementation would be nice
							hdr->EVENT.TYP = (typeof(hdr->EVENT.TYP)) realloc(hdr->EVENT.TYP, (hdr->EVENT.N + NumberOfDataSections) * sizeof(*hdr->EVENT.TYP));
							hdr->EVENT.POS = (typeof(hdr->EVENT.POS)) realloc(hdr->EVENT.POS, (hdr->EVENT.N + NumberOfDataSections) * sizeof(*hdr->EVENT.POS));

							/*
							char Desc[2]; Desc[0] = srcaddr[hdr->CHANNEL[next].bi + k2*stride]; Desc[1] = 0; 
								this does currently not work because FreeTextEvent expects that 
								the string constant is available as long as hdr, which is not the case here. 
							*/

						 	// typically a single character within a 4 byte integer, this should be sufficient to ensure \0 termination
							char *Desc = srcaddr + hdr->CHANNEL[next].bi + k2*stride;
							Desc[1] = 0; 
							FreeTextEvent(hdr, hdr->EVENT.N, Desc);
							hdr->EVENT.POS[hdr->EVENT.N] = lround( (val * hc->Cal + hc->Off) * hdr->SampleRate) + SPR;  
							hdr->EVENT.N++;
						}
					}
					ns += hc->OnOff;
				}
			}

			SPR += spr;
			SZ  += sz;

			datapos = leu32p(hdr->AS.Header + datapos);
		}
		free(DATAPOS);

if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 419: SPR=%i=%i NRec=%i  @%p\n",(int)SPR,hdr->SPR,(int)hdr->NRec, hdr->AS.rawdata);

		// set variables such that sread_raw does not attempt to reload the data
		hdr->AS.first = 0;
		hdr->EVENT.SampleRate = hdr->SampleRate;
		if (NumberOfDataSections < 1) {
			hdr->SPR = 0; 	
		}
		else  {
			hdr->FLAG.UCAL = 1;
			hdr->SPR       = 1;
			hdr->NRec      = SPR;
			hdr->AS.bpb    = hdr->NS * sizeof(double);
			hdr->AS.length = SPR;

			for (k = 0; k < hdr->NS; k++) {
				CHANNEL_TYPE *hc = hdr->CHANNEL + k;
				hc->GDFTYP  = 17;	// double
				hc->bi      = sizeof(double)*k;
				hc->SPR     = hdr->SPR;
				hc->Cal     = 1.0;
				hc->Off     = 0.0;
			}
		}

if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 429: SPR=%i=%i NRec=%i\n",(int)SPR,hdr->SPR,(int)hdr->NRec);
		datapos   = FileHeaderSize;  //+DataHeaderSize;

		if (flag_ChanInfoChanged) {
			biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "CED/CFS: varying channel information not supported");
		}

		for (k = 0; k < hdr->NS; k++) {
			switch (hdr->CHANNEL[k].GDFTYP) {
			case 0:
			case 1:
				hdr->CHANNEL[k].DigMax  =  127;
				hdr->CHANNEL[k].DigMin  = -128;
				break;
			case 2:
				hdr->CHANNEL[k].DigMax  =  255;
				hdr->CHANNEL[k].DigMin  =  0;
				break;
			case 3:
				hdr->CHANNEL[k].DigMax  = (int16_t)0x7fff;
				hdr->CHANNEL[k].DigMin  = (int16_t)0x8000;
				break;
			case 4:
				hdr->CHANNEL[k].DigMax  = 0xffff;
				hdr->CHANNEL[k].DigMin  = 0;
				break;
			case 16:
			case 17:
				hdr->CHANNEL[k].DigMax  =  1e9;
				hdr->CHANNEL[k].DigMin  = -1e9;
				break;
			}
			hdr->CHANNEL[k].PhysMax = hdr->CHANNEL[k].DigMax * hdr->CHANNEL[k].Cal + hdr->CHANNEL[k].Off;
			hdr->CHANNEL[k].PhysMin = hdr->CHANNEL[k].DigMin * hdr->CHANNEL[k].Cal + hdr->CHANNEL[k].Off;
		}
		hdr->FLAG.UCAL = 0;

#undef H1LEN
}



/*****************************************************************************
	
	SOPEN_SMR_READ for reading SON/SMR files


	son.h, sonintl.h are used just for understanding the file format. 
	and are not needed for the core functionality. 
	

        TODO:
	Currently waveform data is supported, 
        events and marker information is ignored	


 *****************************************************************************/

#if !defined(NDEBUG) && defined(WITH_SON)
  #ifdef __MINGW32__
    #include <windows.h>
  #endif 
  #include "sonintl.h"
  // defines __SONINTL__	
#endif 


EXTERN_C void sopen_smr_read(HDRTYPE* hdr) {	
        /*TODO: implemnt SON/SMR format */
	fprintf(stdout,"SOPEN: Support for CED's SMR/SON format is under construction \n");

	size_t count = hdr->HeadLen;		
	if (count < 512) {
			hdr->HeadLen = 512; 
			// make sure fixed header (first 512 bytes) are read
			hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header,hdr->HeadLen+1);
			count += ifread(hdr->AS.Header+count, 1, hdr->HeadLen-count, hdr);
			hdr->AS.Header[count]=0;
	}	
		
	// get Endianity, Version and Header size
	hdr->FILE.LittleEndian = (*(uint16_t*)(hdr->AS.Header+38) == 0);   // 0x0000: little endian, 0x0101: big endian
	if (hdr->FILE.LittleEndian) {
		hdr->VERSION = leu16p(hdr->AS.Header); 
		hdr->HeadLen = leu32p(hdr->AS.Header + 26 ); // first data 
	} else {
		hdr->VERSION = beu16p(hdr->AS.Header); 
		hdr->HeadLen = beu32p(hdr->AS.Header + 26); // first data
		// TODO: relax this restriction 
		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "SON/SMR: big-endian file not supported,yet");
	}
	
	size_t HeadLen = hdr->HeadLen;	
	size_t size_factor = 1; 
	if (hdr->VERSION >= 9) {
		size_factor = 512;
		HeadLen *= size_factor;
		if (sizeof(size_t) <= 4) {
			// TODO: relax that restriction 
			biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "SON/SMR: version 9 (bigfile) not supported,yet");
			return;	
		}
	}
		
	if (count < HeadLen) {
		// read channel header and extra data (i.e. everything up to firstData)
		hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header,HeadLen+1);
		count += ifread(hdr->AS.Header+count, 1, HeadLen-count, hdr);
		hdr->AS.Header[count]=0;
	}


#ifdef __SONINTL__	
	if (VERBOSE_LEVEL > 6) {

		TFileHead *tfh = (void*)(hdr->AS.Header); 
		TChannel  *tc  = (void*)(hdr->AS.Header+512); 

		// debugging information 
		fprintf(stdout,"\tsizeof TFileHead %i\n\tsizeof TChannel %i\n\tsizeof TDataBlock %i\n",
			(int)sizeof(TFileHead),(int)sizeof(TChannel),(int)sizeof(TDataBlock));

		fprintf(stdout,"%i NS=%i %i %i %i\n",
			(int)sizeof(*tfh),hdr->NS,(int)offsetof(TFileHead,channels),(int)sizeof(TChannel),(int)sizeof(TDataBlock));

		fprintf(stdout,"systemID\t%i %i %i\n",(int)offsetof(TFileHead,systemID),
			sizeof(tfh->systemID),lei16p(hdr->AS.Header+offsetof(TFileHead,systemID)));
		fprintf(stdout,"copyright\t%i %i\n",(int)offsetof(TFileHead,copyright),sizeof(tfh->copyright));
		fprintf(stdout,"creator  \t%i %i <%8s>\n",(int)offsetof(TFileHead,creator),
			sizeof(tfh->creator),hdr->AS.Header+offsetof(TFileHead,creator));
		fprintf(stdout,"usPerTime\t%i %i %i\n",(int)offsetof(TFileHead,usPerTime),
			sizeof(tfh->usPerTime),lei16p(hdr->AS.Header+offsetof(TFileHead,usPerTime)));
		fprintf(stdout,"timePerADC\t%i %i %i\n",(int)offsetof(TFileHead,timePerADC),
			sizeof(tfh->timePerADC),lei16p(hdr->AS.Header+offsetof(TFileHead,timePerADC)));
		fprintf(stdout,"fileState\t%i %i %i\n",(int)offsetof(TFileHead,fileState),
			sizeof(tfh->fileState),lei16p(hdr->AS.Header+offsetof(TFileHead,fileState)));
		fprintf(stdout,"firstData\t%i %i %i\n",(int)offsetof(TFileHead,firstData),
			sizeof(tfh->firstData),lei32p(hdr->AS.Header+offsetof(TFileHead,firstData)));
		fprintf(stdout,"channels\t%i %i 0x%04x\n",(int)offsetof(TFileHead,channels),
			sizeof(tfh->channels),lei16p(hdr->AS.Header+offsetof(TFileHead,channels)));
		fprintf(stdout,"chanSize\t%i %i 0x%04x\n",(int)offsetof(TFileHead,chanSize),
			sizeof(tfh->chanSize),lei16p(hdr->AS.Header+offsetof(TFileHead,chanSize)));
		fprintf(stdout,"extraData\t%i %i %i\n",(int)offsetof(TFileHead,extraData),
			sizeof(tfh->extraData),lei16p(hdr->AS.Header+offsetof(TFileHead,extraData)));
		fprintf(stdout,"bufferSz\t%i %i %i\n",(int)offsetof(TFileHead,bufferSz),
			sizeof(tfh->bufferSz),lei16p(hdr->AS.Header+offsetof(TFileHead,bufferSz)));
		fprintf(stdout,"osFormat\t%i %i %i\n",(int)offsetof(TFileHead,osFormat),
			sizeof(tfh->osFormat),lei16p(hdr->AS.Header+offsetof(TFileHead,osFormat)));
		fprintf(stdout,"maxFTime\t%i %i %i\n",(int)offsetof(TFileHead,maxFTime),
			sizeof(tfh->maxFTime),lei32p(hdr->AS.Header+offsetof(TFileHead,maxFTime)));
		fprintf(stdout,"dTimeBase\t%i %i %g\n",(int)offsetof(TFileHead,dTimeBase),
			sizeof(tfh->dTimeBase),lef64p(hdr->AS.Header+offsetof(TFileHead,dTimeBase)));
		fprintf(stdout,"timeDate\t%i %i\n",(int)offsetof(TFileHead,timeDate),sizeof(tfh->timeDate));
		fprintf(stdout,"cAlignFlag\t%i %i\n",(int)offsetof(TFileHead,cAlignFlag),sizeof(tfh->cAlignFlag));
		fprintf(stdout,"pad0     \t%i %i\n",(int)offsetof(TFileHead,pad0),sizeof(tfh->pad0));
		fprintf(stdout,"LUTable  \t%i %i\n",(int)offsetof(TFileHead,LUTable),sizeof(tfh->LUTable));
		fprintf(stdout,"pad      \t%i %i\n",(int)offsetof(TFileHead,pad),sizeof(tfh->pad));
		fprintf(stdout,"fileComment\t%i %i\n",(int)offsetof(TFileHead,fileComment),sizeof(tfh->fileComment));
        
		fprintf(stdout,"==CHANNEL==\n");
		fprintf(stdout,"delSize  \t%i %i\n",(int)offsetof(TChannel,delSize),sizeof(tc->delSize));
		fprintf(stdout,"nextDelBlock\t%i %i\n",(int)offsetof(TChannel,nextDelBlock),sizeof(tc->nextDelBlock));
		fprintf(stdout,"firstBlock\t%i %i\n",(int)offsetof(TChannel,firstBlock),sizeof(tc->firstBlock));
		fprintf(stdout,"lastBlock\t%i %i\n",(int)offsetof(TChannel,lastBlock),sizeof(tc->lastBlock));
		fprintf(stdout,"blocks   \t%i %i\n",(int)offsetof(TChannel,blocks),sizeof(tc->blocks));
		fprintf(stdout,"nExtra   \t%i %i\n",(int)offsetof(TChannel,nExtra),sizeof(tc->nExtra));
		fprintf(stdout,"preTrig  \t%i %i\n",(int)offsetof(TChannel,preTrig),sizeof(tc->preTrig));
		fprintf(stdout,"blocksMSW\t%i %i\n",(int)offsetof(TChannel,blocksMSW),sizeof(tc->blocksMSW));
		fprintf(stdout,"phySz    \t%i %i\n",(int)offsetof(TChannel,phySz),sizeof(tc->phySz));
		fprintf(stdout,"maxData  \t%i %i\n",(int)offsetof(TChannel,maxData),sizeof(tc->maxData));
		fprintf(stdout,"comment  \t%i %i\n",(int)offsetof(TChannel,comment),sizeof(tc->comment));

		fprintf(stdout,"maxChanTime  \t%i %i\n",(int)offsetof(TChannel,maxChanTime),sizeof(tc->maxChanTime));

		fprintf(stdout,"lChanDvd\t%i %i\n",(int)offsetof(TChannel,lChanDvd),sizeof(tc->lChanDvd));
		fprintf(stdout,"phyChan  \t%i %i\n",(int)offsetof(TChannel,phyChan),sizeof(tc->phyChan));
		fprintf(stdout,"title    \t%i %i %s\n",(int)offsetof(TChannel,title),sizeof(tc->title),((char*)&(tc->title))+1);
		fprintf(stdout,"idealRate\t%i %i\n",(int)offsetof(TChannel,idealRate),sizeof(tc->idealRate));
		fprintf(stdout,"kind     \t%i %i\n",(int)offsetof(TChannel,kind),sizeof(tc->kind));
		fprintf(stdout,"delSizeMSB\t%i %i\n",(int)offsetof(TChannel,delSizeMSB),sizeof(tc->delSizeMSB));
        
		fprintf(stdout,"v.adc.scale\t%i %i\n",(int)offsetof(TChannel,v.adc.scale),sizeof(tc->v.adc.scale));
		fprintf(stdout,"v.adc.offset\t%i %i\n",(int)offsetof(TChannel,v.adc.offset),sizeof(tc->v.adc.offset));
		fprintf(stdout,"v.adc.units\t%i %i\n",(int)offsetof(TChannel,v.adc.units),sizeof(tc->v.adc.units));
		fprintf(stdout,"v.adc.divide\t%i %i\n",(int)offsetof(TChannel,v.adc.divide),sizeof(tc->v.adc.divide));
        
		fprintf(stdout,"v.event.initLow\t%i %i\n",(int)offsetof(TChannel,v.event.initLow),sizeof(tc->v.event.initLow));
		fprintf(stdout,"v.event.nextLow\t%i %i\n",(int)offsetof(TChannel,v.event.nextLow),sizeof(tc->v.event.nextLow));

		fprintf(stdout,"v.real.min\t%i %i\n",(int)offsetof(TChannel,v.real.min),sizeof(tc->v.real.min));
		fprintf(stdout,"v.real.max\t%i %i\n",(int)offsetof(TChannel,v.real.max),sizeof(tc->v.real.max));
		fprintf(stdout,"v.real.units\t%i %i\n",(int)offsetof(TChannel,v.real.units),sizeof(tc->v.real.units));

		assert(sizeof(TFileHead)==512);         
		assert(sizeof(TChannel)==140);         
		assert(sizeof(TDataBlock)==64020);         
	}
#endif

	uint16_t timePerADC; 
	uint32_t maxFTime;
	double timebase,dTimeBase;
	memcpy(&hdr->ID.Equipment, hdr->AS.Header+12, 8);
	if (hdr->FILE.LittleEndian) {
		timebase = hdr->VERSION < 6 ? 1e-6 : lef64p(hdr->AS.Header + 44);    
		hdr->SampleRate = 1.0 / (timebase * leu16p(hdr->AS.Header + 20) ); 
		hdr->NS         = leu16p(hdr->AS.Header + 30 ); 
		timePerADC      = lei16p(hdr->AS.Header + 22);
		maxFTime 	= leu32p(hdr->AS.Header + 40);
		dTimeBase	= lef64p(hdr->AS.Header + 44);
	} else {
		timebase = hdr->VERSION < 6 ? 1e-6 : bef64p(hdr->AS.Header + 44);     
		hdr->SampleRate = 1.0 / (timebase * beu16p(hdr->AS.Header + 20)); 
		hdr->NS         = beu16p(hdr->AS.Header + 30); 
		timePerADC      = bei16p(hdr->AS.Header + 22);
		maxFTime 	= beu32p(hdr->AS.Header + 40);
		dTimeBase	= bef64p(hdr->AS.Header + 44);
	}        
	hdr->SPR = 1; 

	assert( timebase == 1e-6 || hdr->VERSION >= 6 );
	if (VERBOSE_LEVEL > 6) {
		fprintf(stdout,"SMR:\tmaxFTime=%g\n\t timebase=%g\n\t dTimeBase=%g timePerADC=%i\n",maxFTime,timebase,dTimeBase,timePerADC);
	}



	/*********************************************
	  read channel header 
 	 *********************************************/
	hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));

	typeof(hdr->NS) k,ns=0; 

	size_t bpb = 0; 
	for (k = 0; k < hdr->NS; k++) {
		uint32_t off = 512 + k*140;
		CHANNEL_TYPE *hc    = hdr->CHANNEL+k;
	
		hc->Cal   = lef32p(hdr->AS.Header + off + 124);	// v.adc.scale
		hc->Off   = lef32p(hdr->AS.Header + off + 128);	// v.adc.off
		hc->OnOff = *(int16_t*)(hdr->AS.Header + off + 106) != 0;
		hc->GDFTYP = 3; 
		hc->DigMax = (double)(int16_t)0x7fff;
		hc->DigMin = (double)(int16_t)0x8000;
		hc->PhysMax = hc->DigMax * hc->Cal + hc->Off;
		hc->PhysMin = hc->DigMin * hc->Cal + hc->Off;

		strncpy(hc->Label, hdr->AS.Header+off+108+1, min(9,MAX_LENGTH_LABEL));
		if ( !strcmp((char*)(hdr->AS.Header+off+132),"\x5 Volt")
		  || !strcmp((char*)(hdr->AS.Header+off+132),"\x4Volt")  )
			hc->PhysDimCode = 4256; 	// Volt
		else 	
			hc->PhysDimCode = 0;

		hc->bi = bpb;
		uint8_t kind = hdr->AS.Header[off+122];	
		switch (kind) {
		case 0: // 
			//    ChanOff=0,          /* the channel is OFF - */
			hc->OnOff = 0; 
			break; 
		case 1: 
			//    Adc,                /* a 16-bit waveform channel */
			hc->OnOff = 1; 
			hc->GDFTYP = 3; 	
			bpb += 2;
			break; 
		case 2: 
			//    EventFall,          /* Event times (falling edges) */
			hc->OnOff = 0; 
			break; 
		case 3: 
			//    EventRise,          /* Event times (rising edges) */
			hc->OnOff = 0; 
			break; 
		case 4: 
			//    EventBoth,          /* Event times (both edges) */
			hc->OnOff = 0; 
			break; 
		case 5: 
			//    Marker,             /* Event time plus 4 8-bit codes */
			hc->OnOff = 0; 
			break; 
		case 6: 
			//    AdcMark,            /* Marker plus Adc waveform data */
			hc->OnOff = 0; 
			break; 
		case 7: 
			//    RealMark,           /* Marker plus float numbers */
			hc->OnOff = 0; 
			break; 
		case 8: 
			//    TextMark,           /* Marker plus text string */
			hc->OnOff = 0; 
			break; 
		case 9: 
			//    RealWave            /* waveform of float numbers */
			hc->OnOff = 0; 
			hc->GDFTYP = 16; 	
			bpb += 4;
			break; 
		default: 		
			//    unknown
			hc->OnOff = 0; 
			fprintf(stderr,"SMR/SON: channel %i ignored - unknown type %i\n",k,kind); 
		}

		if (hc->OnOff) {
			hc->SPR = 1; 
			ns++;
		} else {
			hc->SPR = 0; 
		}
			
			
		if (VERBOSE_LEVEL > 6) {
			fprintf(stdout,"[%i].delSize\t%i\n",k,lei16p(hdr->AS.Header+off));
		
			fprintf(stdout,"[%i].nextDelBlock\t%i\n",k,leu32p(hdr->AS.Header+off+2));
			fprintf(stdout,"[%i].firstBlock\t%i\n",k,leu32p(hdr->AS.Header+off+6));
			fprintf(stdout,"[%i].lastBlock\t%i\n",k,leu32p(hdr->AS.Header+off+10));

			fprintf(stdout,"[%i].blocks\t%i\n",k,leu16p(hdr->AS.Header+off+14));
			fprintf(stdout,"[%i].nExtra\t%i\n",k,leu16p(hdr->AS.Header+off+16));
			fprintf(stdout,"[%i].preTrig\t%i\n",k,lei16p(hdr->AS.Header+off+18));
			fprintf(stdout,"[%i].blocksMSW\t%i\n",k,lei16p(hdr->AS.Header+off+20));
			fprintf(stdout,"[%i].phySz\t%i\n",k,lei16p(hdr->AS.Header+off+22));
			fprintf(stdout,"[%i].maxData\t%i\n",k,lei16p(hdr->AS.Header+off+24));

			fprintf(stdout,"[%i].comment\t<%s>\n",k,hdr->AS.Header+off+26+1);

			fprintf(stdout,"[%i].maxChanTime\t%i\t%i\t%i\n",k,lei32p(hdr->AS.Header+off+98),*(int32_t*)(hdr->AS.Header+off+98));
			fprintf(stdout,"[%i].lChanDvd\t%i\n",k,lei32p(hdr->AS.Header+off+102));
			fprintf(stdout,"[%i].phyChan\t%i\n",k,lei16p(hdr->AS.Header+off+106));
			fprintf(stdout,"[%i].title\t<%s>\n",k,hdr->AS.Header+off+108+1);
			fprintf(stdout,"[%i].idealRate\t%f\n",k,lef32p(hdr->AS.Header+off+118));
			fprintf(stdout,"[%i].kind\t%i\n",k,*(hdr->AS.Header+off+122));
			fprintf(stdout,"[%i].delSizeMSB\t%i\n",k,*(hdr->AS.Header+off+123));


			fprintf(stdout,"[%i].v.adc.scale\t%f\n",k,lef32p(hdr->AS.Header+off+124));
			fprintf(stdout,"[%i].v.adc.offset\t%f\n",k,lef32p(hdr->AS.Header+off+128));
			char tmp[10]; tmp[6] = 0;	
			memcpy(tmp, hdr->AS.Header+off+132, 6);
			fprintf(stdout,"[%i].v.adc.units\t%s\n",  k, tmp+1);
			fprintf(stdout,"[%i].v.adc.divide\t%i\n", k, lei16p(hdr->AS.Header+off+138));

			fprintf(stdout,"[%i].v.real.min\t%f\n", k, lef32p(hdr->AS.Header+off+124));
			fprintf(stdout,"[%i].v.real.max\t%f\n", k, lef32p(hdr->AS.Header+off+128));
	
			fprintf(stdout,"[%i].v.event\t%0x\n", k, lef32p(hdr->AS.Header+off+124));
		}
        }         

	/*********************************************
	  read blocks 
 	 *********************************************/

	size_t H0Len = hdr->HeadLen; 
	while (!ifeof(hdr)) {
		// read channel header and extra data
		hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header,hdr->HeadLen*2);
		hdr->HeadLen *= 2; 	
		count += ifread(hdr->AS.Header+count, 1, hdr->HeadLen-count, hdr);
	}
	hdr->HeadLen = count; 
		
	size_t noblocks = (hdr->HeadLen - H0Len) / 64020;

	hdr->SPR        = maxFTime; // TODO: do we really need +10 ? 
	hdr->NRec       = 1;

	hdr->AS.rawdata = realloc(hdr->AS.rawdata, hdr->NRec * hdr->SPR * bpb);
	memset(hdr->AS.rawdata,-1,hdr->NRec * hdr->NRec * bpb);

	// Initialize with NaN, 0x8000 in case of INT16 
	typeof(hdr->NS) m;
	for (m = 0; m < hdr->NS; m++) {
		CHANNEL_TYPE *hc = hdr->CHANNEL + m;
		hc->bi          *= hdr->SPR*hdr->NRec; 
		
		if (hc->OnOff)	{
			if (hc->GDFTYP==3) {	
				size_t k;
				for (k = 0; k < hdr->SPR * hdr->NRec; k++) 
					*(int16_t*)(hdr->AS.rawdata + hc->bi + k*sizeof(int16_t) ) = 0x8000;
			}
			else if (hc->GDFTYP==16) {	
				size_t k;
				for (k = 0; k < hdr->SPR * hdr->NRec; k++) 
					*(float*)(hdr->AS.rawdata + hc->bi + k*sizeof(float) ) = (float)NAN;
			}
		}
	}


	hdr->FILE.LittleEndian = 1; 
	hdr->AS.first = 0; 	
	hdr->AS.length = hdr->NRec; 	
	hdr->AS.flag_collapsed_rawdata = 1;

	size_t maxTime = 0; 	
	ns = 0;
	uint32_t i,j;
	for (m = 0; m < hdr->NS; m++) {
		CHANNEL_TYPE *hc = hdr->CHANNEL + m;

		uint8_t kind = hdr->AS.Header[512 + m*140 + 122];	
		// TODO: supporting event and marker channels
		switch (kind) {
		case 0: 	//    channel is off, ignore it and get next channel 
			continue;

		case 1: 	//    Adc,                /* a 16-bit waveform channel */
		
		//case 6: 	//    AdcMark,            /* Marker plus Adc waveform data */
		
		case 9: 	//    RealWave            /* waveform of float numbers */
			break; 

		default: 		
			fprintf(stderr,"SON/SMR: channel %i ignored, kind %i is not supported\n",m+1,kind);
			continue; 	// ignore this channel and get next one
		}

		hc->SPR = hdr->SPR;
		uint32_t firstblock = leu32p(hdr->AS.Header + 512 + m * 140 + 6); 	// first block of channel k
		uint32_t lastblock  = leu32p(hdr->AS.Header + 512 + m * 140 + 10); 	// last block of channel k
		
		uint32_t pos = firstblock;
		k = 0; 
		do {
			k++;

	 		if (VERBOSE_LEVEL>8) 
 				fprintf(stdout,"#%i\t%i\t%8i\t%8i\t%8i\t%8i\t%8i\t%8i\t%i\n",m,k,
					leu32p(hdr->AS.Header+pos)   , leu32p(hdr->AS.Header+pos+4),
					leu32p(hdr->AS.Header+pos+8) , leu32p(hdr->AS.Header+pos+12),
					leu16p(hdr->AS.Header+pos+16), leu16p(hdr->AS.Header+pos+18), 
					lei32p(hdr->AS.Header+pos+98-6) 
					);

			size_t predBlock     = leu32p(hdr->AS.Header+pos) * size_factor;
			size_t succBlock     = leu32p(hdr->AS.Header+pos+4) * size_factor;
			uint32_t startTime   = leu32p(hdr->AS.Header+pos+8);
			
			 int32_t lChanDvD    = lei32p(hdr->AS.Header + 512 + m * 140 + 102); 
			uint32_t endTime     = leu32p(hdr->AS.Header + pos + 12) + lChanDvD;
			
			uint16_t chanNumber  = leu16p(hdr->AS.Header+pos+16);
			uint16_t item        = leu16p(hdr->AS.Header+pos+18);
			 int32_t maxChanTime = lei32p(hdr->AS.Header+pos+98-6);

			assert (chanNumber == m+1);
			
		if (VERBOSE_LEVEL>7) fprintf(stdout,"SMR 333:\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n",m,k,hc->GDFTYP,(int)predBlock,(int)succBlock,startTime,endTime,chanNumber,item,lChanDvD,startTime+item*lChanDvD-endTime);

			for (i = 0; i < item; i++) {
				// convert data to little endian 
				int16_t val = lei16p(hdr->AS.Header + pos + 20 + i*2);

		if (VERBOSE_LEVEL>8) fprintf(stdout,"%i\t%i\t%i\t%i\n",m,k,i,val);

				for (j = 0; j < lChanDvD; j++) 
					*(int16_t*)(hdr->AS.rawdata + hc->bi + (startTime + i * lChanDvD + j) * (GDFTYP_BITS[hc->GDFTYP]>>3) ) = val;

			}

			//fprintf(stdout,"#%i == %i ==\n",k,leu32p(hdr->AS.Header+pos+4)-pos);
			pos = leu32p(hdr->AS.Header + pos + 4); 	
	
		} while (pos != 0xffffffff);
	}
//	hdr->NS = ns;
        
}

