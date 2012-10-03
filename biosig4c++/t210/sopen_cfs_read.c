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

		uint8_t k;
		typeof (hdr->NS) NS = 0; 

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
			hdr->EVENT.CHN = (typeof(hdr->EVENT.CHN)) realloc(hdr->EVENT.CHN, (hdr->EVENT.N + NumberOfDataSections - 1) * sizeof(*hdr->EVENT.CHN));
			hdr->EVENT.DUR = (typeof(hdr->EVENT.DUR)) realloc(hdr->EVENT.DUR, (hdr->EVENT.N + NumberOfDataSections - 1) * sizeof(*hdr->EVENT.DUR));
		}


if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 131 - %d,%d,%d,0x%x,0x%x,0x%x,%d,0x%x\n",hdr->NS,n,d,FileHeaderSize,DataHeaderSize,LastDataSectionHeaderOffset,NumberOfDataSections,leu32p(hdr->AS.Header+0x86));

		/* channel information */
		hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
#define H2LEN (22+10+10+1+1+2+2)
 		char* H2 = (char*)(hdr->AS.Header + H1LEN);
		double xPhysDimScale[100];		// CFS is limited to 99 channels
		assert(hdr->NS < 100);
		uint16_t stride = 0; 	
		uint32_t SZ = 0;
		char flag_sequential = 1; 
		for (k = 0; k < hdr->NS; k++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL + k;
			/*
				1 offset because CFS uses pascal type strings (first byte contains string length)
				in addition, the strings are \0 terminated.
			*/
			hc->LeadIdCode = 0; 

			uint8_t len = min(21, MAX_LENGTH_LABEL);
			strncpy(hc->Label, H2 + 1 + k*H2LEN, len);	// Channel name 
			len = strlen(hc->Label);
			while (isspace(hc->Label[len])) len--;		// remove trailing blanks
			hc->Label[len+1] = 0;

			hc->PhysDimCode  = PhysDimCode (H2 + 22 + 1 + k*H2LEN);			// Y axis units
			xPhysDimScale[k] = PhysDimScale(PhysDimCode(H2 + 32 + 1 + k*H2LEN));	// X axis units

			uint8_t  dataType  = H2[42 + k*H2LEN];
			uint8_t  dataKind  = H2[43 + k*H2LEN];
			uint16_t byteSpace = leu16p(H2+44 + k*H2LEN);
			uint16_t next      = leu16p(H2+46 + k*H2LEN);
			hc->GDFTYP = dataType < 5 ? dataType+1 : dataType+11;

			uint16_t sz = GDFTYP_BITS[hc->GDFTYP]>>3;	
			
			flag_sequential &= (sz == byteSpace); 

if (VERBOSE_LEVEL > 7) fprintf(stdout, "#%i:\t%i %i %i %i %i %i\n", k, hc->GDFTYP, dataType, dataKind, byteSpace, next, sz);

#ifdef CFS_NEW
/*
			#define TCFSKind (H2[43 + k * H2LEN])
			#define SUBSIDIARY 2
			#define MATRIX 1
			#define EQUALSPACED 0 
*/
			// Matrix and Subsidiary data not considered yet, ignore.
			// selected channels must have same stride
			if (dataKind > 0) {
				hc->OnOff = 0; 
			}
			else {
				hc->OnOff = 1;
				NS++;
				if ( stride == 0 ) {
					stride = byteSpace; 
					hc->bi = 0;
					hdr->AS.bpb = stride;
				}
				else if ( 0 < stride ) {	
					if (stride == byteSpace) {
						if (stride == sz) {
							/*
							   In this case, the header information can not be fully defined 
 							   because SPR is needed to compute bpb, bi, etc. 	
							   This need to be defined further below. 

							   hc->bi = k*sz*; 
							   hdr->AS.bpb
							*/
						}
						else {
							hc->bi = SZ;
							hdr->AS.bpb = stride;
						}
					}
					else {
						B4C_ERRNUM = B4C_FORMAT_UNSUPPORTED;
						B4C_ERRMSG = 'CFS: either the file is corrupt or the format is not supported';	
					}
				}
				SZ += sz;
			}	
			
#endif
			hc->PhysMax  = NAN;
			hc->PhysMin  = NAN;
			hc->DigMax   = NAN;
			hc->DigMin   = NAN;
			hc->LowPass  = NAN;
			hc->HighPass = NAN;
			hc->Notch    = NAN;
			hc->TOffset  = 0;
			hc->XYZ[0]   = 0;
			hc->XYZ[1]   = 0;
			hc->XYZ[2]   = 0;
			hc->bi8      = 0;

if (VERBOSE_LEVEL>7) fprintf(stdout,"Channel #%i: [%s](dataType %i, dataKind=%i) <%s>/<%s> spacing %i,Next#%i\n",k+1, H2 + 1 + k*H2LEN, dataType, dataKind, H2 + 23 + k*H2LEN, H2 + 33 + k*H2LEN, byteSpace, next);
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

//fprintf(stdout,"\n%3i @0x%6x: <%s>  %i  [%s] %i ",k, pos, hdr->AS.Header+pos+1,typ,hdr->AS.Header+pos+25,off);
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
		size_t SPR = 0; 	// cumulative number of samples
//		size_t SZ = 0;
		for (m = 0; m < NumberOfDataSections; m++) {
			datapos = DATAPOS[m];
			if (!leu32p(hdr->AS.Header+datapos+8)) continue; 	// empty segment

if (VERBOSE_LEVEL>7) fprintf(stdout,"\n******* DATA SECTION --%03i-- %i *********\n",m,flag_ChanInfoChanged);
if (VERBOSE_LEVEL>7) fprintf(stdout,"\n[DS#%3i] 0x%x 0x%x [0x%x 0x%x szChanData=%i] 0x02%x\n", m, FileHeaderSize, (int)datapos, leu32p(hdr->AS.Header+datapos), leu32p(hdr->AS.Header+datapos+4), leu32p(hdr->AS.Header+datapos+8), leu16p(hdr->AS.Header+datapos+12));

			uint32_t sz    = 0;
			uint32_t bpb   = 0, spb = 0, spr = 0;
			hdr->AS.first  = 0;
			hdr->AS.length = 0;
			for (k = 0; k < hdr->NS; k++) {
				uint8_t *pos = hdr->AS.Header + datapos + 30 + 24 * k;

				char flag_interleaved = 0; 
				CHANNEL_TYPE *hc = hdr->CHANNEL + k;

				uint32_t p  = leu32p(pos);	// Offset in data section to first byte 
				hc->SPR     = leu32p(pos+4);	// data points (not bytes)
				hc->Cal     = lef32p(pos+8);	// Y Scale
				hc->Off     = lef32p(pos+12);	// Y Offset
				double Xcal = lef32p(pos+16);	// X increment (equalSpaced and subsidiary data)
				//double Xoff = lef32p(pos+20); // X offset (equalSpaced and subsidiary data) - not supported yet
				uint16_t SZ = GDFTYP_BITS[hc->GDFTYP]>>3;	// per single sample
				
					
#ifdef CFS_NEW
				/* TODO 
					this part must be correctly defined for equalSpaced and subsidiary data
					hc->bi, hdr->AS.bpb, hc->SPR, 
					eventually hc->bufptr
					such that sread can access the data in the proper way. 
				*/ 
				uint16_t byteSpace = leu16p(H2+44 + k*H2LEN);
				uint16_t next      = leu16p(H2+46 + k*H2LEN);
				if (hc->OnOff) {
					if (byteSpace = SZ) {
						hc->bi = sz;
					}
					else {
					}
				}
					
				if ((hc->OnOff) ^ (hc->SPR > 0)) {
					B4C_ERRNUM = B4C_DATATYPE_UNSUPPORTED; 
					B4C_ERRMSG = 'CFS: Matrix/Subsidiary data section with None-zero SPR found - this case has not yet been considered.'; 
				}
				hc->bi      = bpb;
#else
				hc->OnOff   = 1;
				hc->bi      = sz;
#endif

if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 409: %i #%i: SPR=%i=%i=%i  x%f+-%f %i %i\n",m,k,spr,(int)SPR,hc->SPR,hc->Cal,hc->Off,p,hc->bi);

#ifdef CFS_NEW
				if (!hc->OnOff) continue; 
#endif

				double Fs = 1.0 / (xPhysDimScale[k] * Xcal);
				if (!m && !k) {
					hdr->SampleRate = Fs;
				}
				else if (fabs(hdr->SampleRate - Fs) > 1e-3) {
					B4C_ERRNUM = B4C_FORMAT_UNSUPPORTED;
					B4C_ERRMSG = "CED/CFS: different sampling rates are not supported";
				}

				if (spr < hc->SPR) spr  = hc->SPR;
				spb += hc->SPR;		// cumulative samples per block 
				sz  += hc->SPR * GDFTYP_BITS[hc->GDFTYP] >> 3;
				bpb += GDFTYP_BITS[hc->GDFTYP]>>3;	// per single sample
//				hdr->AS.length += hc->SPR;	// ???
			}

			if (NumberOfDataSections > 1) { 
				/*
					hack: copy data into a single block, only if more than one section
					the data will be converted into a multiplexed format ch1s1,ch2s1, ... chNs1, ch1s2, ch2s2, ..., chNs2, 
				 */

#ifdef CFS_NEW
				hdr->AS.rawdata = (uint8_t*)realloc(hdr->AS.rawdata, hdr->NS * (SPR + spr) * sizeof(double));
#else
				hdr->AS.rawdata = (uint8_t*)realloc(hdr->AS.rawdata, (hdr->NS * SPR + spb) * sizeof(double));
#endif
				if (VERBOSE_LEVEL>7)
				 	fprintf(stdout,"CFS 411: @%p: %i * (%i + %i) = %i \n",hdr->AS.rawdata, hdr->NS, SPR, spr, NS * (SPR + spr));

/*
				if (VERBOSE_LEVEL>7)
				 	fprintf(stdout,"CFS 411: @%p %i @%p\n",hdr->AS.rawdata, (hdr->NS * SPR + spb), srcaddr);
*/

				hdr->AS.first = 0;
				for (k = 0; k < hdr->NS; k++) {
					CHANNEL_TYPE *hc = hdr->CHANNEL + k;
					if (!hc->OnOff) continue; 

					uint32_t memoffset = leu32p(hdr->AS.Header + datapos + 30 + 24 * k);
					uint8_t *srcaddr = hdr->AS.Header+leu32p(hdr->AS.Header+datapos + 4) + memoffset;

				if (VERBOSE_LEVEL>7)
				 	fprintf(stdout,"CFS 412 #%i %i: @%p %i\n", k, hc->SPR, srcaddr, leu32p(hdr->AS.Header+datapos + 4) + leu32p(hdr->AS.Header + datapos + 30 + 24 * k));

					int16_t szz = (GDFTYP_BITS[hc->GDFTYP]>>3);
					size_t k2;
					for (k2 = 0; k2 < hc->SPR; k2++) {
						uint8_t *ptr = srcaddr + k2*szz;
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
							B4C_ERRNUM = B4C_FORMAT_UNSUPPORTED;
							B4C_ERRMSG = "CED/CFS: invalid data type";
						}

if (VERBOSE_LEVEL>8)
 	fprintf(stdout,"CFS read: %2i #%2i:%5i [%i,%i]: %f -> %f  @%p\n",m,k,(int)k2,bpb,(int)SPR,val,val*hc->Cal + hc->Off, hdr->AS.rawdata + ((SPR + k2) * hdr->NS + k) * sizeof(double));

#if 1 //def CFS_NEW
						*(double*) (hdr->AS.rawdata + SPR * hdr->NS * sizeof(double) + k * spr * sizeof(double) + k2) = val * hc->Cal + hc->Off;
#else 
						*(double*) (hdr->AS.rawdata + k * sizeof(double) + (SPR + k2) * NS * sizeof(double)) = val * hc->Cal + hc->Off;
#endif
					}
//					srcaddr += hdr->CHANNEL[k].SPR * GDFTYP_BITS[hdr->CHANNEL[k].GDFTYP] >> 3;
				}
			}
			else {
				/* TODO: 
					copy single data block into a dynamically allocated space, 
					otherwise, the 
				 */
				hdr->AS.rawdata = (uint8_t*)realloc(hdr->AS.rawdata,sz);

if (VERBOSE_LEVEL>7)
 	fprintf(stdout,"CFS read: %i,#%i: %i,%i,%i\n",m,k,leu32p(hdr->AS.Header+datapos + 8),sz, stride);

				
				assert(leu32p(hdr->AS.Header+datapos + 8)==sz);	
//				memcpy(hdr->AS.rawdata, hdr->AS.Header + leu32p(hdr->AS.Header+datapos + 4), leu32p(hdr->AS.Header+datapos + 8));
				memcpy(hdr->AS.rawdata, hdr->AS.Header + leu32p(hdr->AS.Header+datapos + 4), sz);
				hdr->AS.bpb = sz;

if (VERBOSE_LEVEL>8)
 	fprintf(stdout,"CFS read: %04x,%04x,%04x,%04x,%04x,%04x\n",leu16p(hdr->AS.rawdata),leu16p(hdr->AS.rawdata+2),leu16p(hdr->AS.rawdata+4),leu16p(hdr->AS.rawdata+6),leu16p(hdr->AS.rawdata+8),leu16p(hdr->AS.rawdata+10));
 	fprintf(stdout,"CFS read: %04x,%04x,%04x,%04x,%04x,%04x\n",leu16p(hdr->AS.rawdata+12),leu16p(hdr->AS.rawdata+14),leu16p(hdr->AS.rawdata+16),leu16p(hdr->AS.rawdata+18),leu16p(hdr->AS.rawdata+20),leu16p(hdr->AS.rawdata+22));


			}

			if (m>0) {
				hdr->EVENT.TYP[hdr->EVENT.N] = 0x7ffe;
				hdr->EVENT.POS[hdr->EVENT.N] = SPR;
				hdr->EVENT.CHN[hdr->EVENT.N] = 0;
				hdr->EVENT.DUR[hdr->EVENT.N] = 0;
				hdr->EVENT.N++;
			}

			SPR += spr;
			SZ  += sz;
if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 414: SPR=%i,%i,%i NRec=%i, @%p\n",spr,(int)SPR,hdr->SPR,(int)hdr->NRec, hdr->AS.rawdata);

#if 0
			// for (k = 0; k < d; k++) {
			for (k = 0; k < 0; k++) {
			// read data variables of each block - this is broken.
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

if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 419: SPR=%i=%i NRec=%i  @%p #dataSections: %d\n",(int)SPR,hdr->SPR,(int)hdr->NRec, hdr->AS.rawdata,NumberOfDataSections);

		hdr->AS.first = 0;
		hdr->EVENT.SampleRate = hdr->SampleRate;
		if (NumberOfDataSections < 1) {
			hdr->SPR = 0; 	
		}
		else if (NumberOfDataSections == 1) {
			// hack: copy data into a single block, only if more than one section
			hdr->FLAG.UCAL = 0;
			hdr->AS.bpb = stride;
#if 0//def CFS_NEW
			hdr->SPR  = 1;
			hdr->NRec = SPR;
			for (k = 0; k < hdr->NS; k++) {
				CHANNEL_TYPE *hc = hdr->CHANNEL + k;
				hc->SPR    = hc->OnOff ? 1 : 0;
			}
#else
			hdr->SPR  = SPR;
			hdr->NRec = 1;
#endif
		}
		else  {
			hdr->FLAG.UCAL = 1;
			hdr->SPR       = 1;
			hdr->NRec      = SPR;
			hdr->AS.bpb    = hdr->NS * sizeof(double);
			hdr->AS.length = SPR;
			for (k = 0; k < hdr->NS; k++) {
				CHANNEL_TYPE *hc = hdr->CHANNEL + k;
				hc->GDFTYP = 17;	// double
				hc->bi     = sizeof(double)*k;
				hc->SPR    = hc->OnOff ? 1 : 0;
				hc->Cal    = 1.0;
				hc->Off    = 0.0;
			}
		}

if (VERBOSE_LEVEL>7) fprintf(stdout,"CFS 429: SPR=%i=%i NRec=%i\n",(int)SPR,hdr->SPR,(int)hdr->NRec);
		datapos   = FileHeaderSize;  //+DataHeaderSize;

		if (flag_ChanInfoChanged) {
			B4C_ERRNUM = B4C_FORMAT_UNSUPPORTED;
			B4C_ERRMSG = "CED/CFS: varying channel information not supported";
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

if (VERBOSE_LEVEL>7) hdr2ascii(hdr,stdout,4);

#undef H1LEN

}

