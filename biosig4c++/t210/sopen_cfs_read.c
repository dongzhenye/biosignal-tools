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
		hdr->NS    = leu16p(hdr->AS.Header+0x2a);	// 6  number of channels
		uint16_t n = leu16p(hdr->AS.Header+0x2c);	// 7  number of file variables
		uint16_t d = leu16p(hdr->AS.Header+0x2e);	// 8  number of data section variables
		uint16_t FileHeaderSize = leu16p(hdr->AS.Header+0x30);	// 9  byte size of file header
		uint16_t DataHeaderSize = leu16p(hdr->AS.Header+0x32);	// 10 byte size of data section header
		uint32_t LastDataSectionHeaderOffset = leu32p(hdr->AS.Header+0x34);	// 11 last data section header offset
		uint16_t NumberOfDataSections = leu16p(hdr->AS.Header+0x38);	// 12 last data section header offset

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
			//uint16_t byteSpace = leu16p(H2+44 + k*H2LEN);		// stride 
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
if (VERBOSE_LEVEL>7) 	{
			if (typ<5) fprintf(stdout," *0x%x = [%d]",(int)p3,i);
			else if (typ<7) fprintf(stdout," *0x%x = [%g]",(int)p3,f);
			else if (typ==7) fprintf(stdout," *0x%x = <%s>",(int)p3,hdr->AS.Header+p3);
			}
		}

if (VERBOSE_LEVEL>7) fprintf(stdout,"\n******* DS variable information *********\n");
		datapos = LastDataSectionHeaderOffset; //H1LEN + H2LEN*hdr->NS + n*36;
		// reverse order of data sections
		uint32_t *DATAPOS = (uint32_t*)malloc(sizeof(uint32_t)*NumberOfDataSections);

		uint16_t m;
		for (m = NumberOfDataSections; 0 < m; ) {
			DATAPOS[--m] = datapos;
			datapos = leu32p(hdr->AS.Header + datapos);
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

			if (NumberOfDataSections > 1) {
				/* hack: copy data into a single block (rawdata)
				   this is used as a data cache, no I/O is needed in sread, at the cost that sopen is slower
				   sread_raw does not attempt to reload the data
				 */
				hdr->AS.flag_collapsed_rawdata = 0; 
				void *ptr = realloc(hdr->AS.rawdata, hdr->NS * (SPR + spr) * sizeof(double));
				if (!ptr) {
					biosigERROR(hdr,B4C_MEMORY_ALLOCATION_FAILED, "CFS: memory allocation failed in line __LINE__");
				}
				hdr->AS.rawdata = (uint8_t*)ptr;

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
			else {
				hdr->AS.rawdata = (uint8_t*)realloc(hdr->AS.rawdata,sz);
				memcpy(hdr->AS.rawdata, hdr->AS.Header + leu32p(hdr->AS.Header+datapos + 4), leu32p(hdr->AS.Header+datapos + 8));
				hdr->AS.bpb = sz;
			}

			SPR += spr;
			SZ  += sz;

#if 0
			// for (k = 0; k < d; k++) {
			for (k = 0; k < 0; k++) {
			// read data variables of each block - this currently broken.
				//size_t pos = leu16p(hdr->AS.Header + datapos + 30 + hdr->NS * 24 + k * 36 + 34);
				size_t pos = datapos + 30 + hdr->NS * 24;
				int i; double f;
				uint16_t typ = leu16p(hdr->AS.Header + pos + 22 + k*36) ;
				uint16_t off = leu16p(hdr->AS.Header + pos + 34 + k*36);
				uint32_t p3  = pos + off;

if (VERBOSE_LEVEL>7) fprintf(stdout,"\n[DS#%3i/%3i] @0x%6x+0x%3x: <%s>  %i  [%s] :", m, k, pos, off, hdr->AS.Header+pos+off+1, typ, hdr->AS.Header+pos+off+25);

				switch (typ) {
				case 0:
				case 1: i = hdr->AS.Header[p3];        break;
				case 2: i = lei16p(hdr->AS.Header+p3); break;
				case 3: i = leu16p(hdr->AS.Header+p3); break;
				case 4: i = lei32p(hdr->AS.Header+p3); break;
				case 5: f = lef32p(hdr->AS.Header+p3); break;
				case 6: f = lef64p(hdr->AS.Header+p3); break;
				}
if (VERBOSE_LEVEL>7) {
				if (typ<5) fprintf(stdout," *0x%x = %d",p3,i);
				else if (typ<7) fprintf(stdout," *0x%x = %g", p3,f);
				else if (typ==7) fprintf(stdout," *0x%x = <%s>",p3,hdr->AS.Header+p3);
}
			}
#endif
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
		else if (NumberOfDataSections == 1) {
			// hack: copy data into a single block, only if more than one section
			hdr->FLAG.UCAL = 0;
			hdr->SPR  = SPR;
			hdr->NRec = 1;
			hdr->AS.length = 1;
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

/*
#include "../src/libson/Son.h"
#include "../src/libson/Sonintl.h"
*/

EXTERN_C void sopen_smr_read(HDRTYPE* hdr) {	
        /*TODO: implemnt SON/SMR format */
        fprintf(stdout,"SOPEN: Support for CED's SMR/SON format is under construction \n");

        hdr->VERSION = leu16p(hdr->AS.Header); 
        size_t off= (size_t)&TFileHead - (size_t)&TFileHead.channels;       
        hdr->NS = leu16p(hdr->AS.Header+off); 

        biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"Support for CED's SMR/SON format is under construction.");
        
}
