/*

    Copyright (C) 2013 Alois Schloegl <alois.schloegl@gmail.com>

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

/*
#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
*/
#include <endian.h>
#include <string.h>
#include "../biosig-dev.h"
#include "../igor/IgorBin.h"


#ifdef __cplusplus
extern "C" {
#endif


#define ITX_MAXLINELENGTH 400

char *IgorChanLabel(char *inLabel, HDRTYPE *hdr, size_t *ngroup, size_t *nseries, size_t *nsweep, size_t *ns) {
	/*
		extract Channel Label of IGOR ITX data format
	*/

	*ns = 0;
	// static char Label[ITX_MAXLINELENGTH+1];
	int k, s = 0, pos4=0, pos1=0;
	for (k = strlen(inLabel); inLabel[k] < ' '; k--);
	inLabel[k+1] = 0;

	while (inLabel[k] >= ' ') {
		while ( inLabel[k] >= '0' && inLabel[k] <= '9' )
			k--;
		if (inLabel[k]=='_') {
			s++;
			if (s==1) pos4 = k;
			if (s==4) pos1 = k;
			k--;
		}
		if ( inLabel[k] < '0' || inLabel[k] > '9' )
			break;
	}

	if (3 < s) {
		char nvar = 0;
		for (k = strlen(inLabel); 0 < k && nvar < 4; k--) {
			if (inLabel[k] == '_') {
				inLabel[k] = 0;
				char  *v = inLabel+k+1;
				size_t n = atol(v);

				switch (nvar) {
				case 0: *ns = n;
					nvar++;
					break;
				case 1: *nsweep = n;
					nvar++;
					break;
				case 2: *nseries = n;
					nvar++;
					break;
				case 3: *ngroup = n;
					nvar++;
					break;
				}
				inLabel[k] = 0;
			}
		}
		for (k=1; inLabel[pos4+k-1]; k++) {
			inLabel[pos1+k] = inLabel[pos4+k];
		}
	}

	for (k=0; k<hdr->NS; k++) {

	}

	if ((*ns)+1 > hdr->NS) {	// another channel
		hdr->NS = (*ns)+1;
		hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
	}

	return(inLabel);
}


/* compute igor checksum */
int ibwChecksum(int16_t *data, int flag_swap, int oldcksum, int numbytes) {
	numbytes >>= 1;				// 2 bytes to a short -- ignore trailing odd byte.
	if (flag_swap) {
		while(numbytes-- > 0) 
			oldcksum += bswap_16(*data++);
	} else {
		while(numbytes-- > 0)
			oldcksum += *data++;
	}
	return oldcksum&0xffff;
}



void ReorderBytes(void *p, int bytesPerPoint, long numValues)	// Reverses byte order.
{
	unsigned char ch, *p1, *p2, *pEnd;
	
	pEnd = (unsigned char *)p + numValues*bytesPerPoint;
	while (p < (void *)pEnd) {
		p1 = p;
		p2 = (unsigned char *)p + bytesPerPoint-1;
		while (p1 < p2) {
			ch = *p1;
			*p1++ = *p2;
			*p2-- = ch;
		}
		p = (unsigned char *)p + bytesPerPoint;
	}
}

inline void ReorderShort(void* sp)
{
	*(uint16_t*)sp = bswap_16(*(uint16_t*)sp);
}

inline void ReorderLong(void* lp)
{
	*(uint32_t*)lp = bswap_32(*(uint32_t*)lp);
}

inline void ReorderDouble(void* dp)
{
	*(uint64_t*)dp = bswap_64(*(uint64_t*)dp);
}

void ReorderBinHeader1(BinHeader1* p)
{
	ReorderShort(&p->version);
	ReorderLong(&p->wfmSize);
	ReorderShort(&p->checksum);
}

void ReorderBinHeader2(BinHeader2* p)
{
	ReorderShort(&p->version);
	ReorderLong(&p->wfmSize);
	ReorderLong(&p->noteSize);
	ReorderLong(&p->pictSize);
	ReorderShort(&p->checksum);
}

void ReorderBinHeader3(BinHeader3* p)
{
	ReorderShort(&p->version);
	ReorderLong(&p->wfmSize);
	ReorderLong(&p->noteSize);
	ReorderLong(&p->formulaSize);
	ReorderLong(&p->pictSize);
	ReorderShort(&p->checksum);
}

void ReorderBinHeader5(BinHeader5* p)
{
	ReorderShort(&p->version);
	ReorderShort(&p->checksum);
	ReorderLong(&p->wfmSize);
	ReorderLong(&p->formulaSize);
	ReorderLong(&p->noteSize);
	ReorderLong(&p->dataEUnitsSize);
	ReorderBytes(&p->dimEUnitsSize, 4, 4);
	ReorderBytes(&p->dimLabelsSize, 4, 4);
	ReorderLong(&p->sIndicesSize);
	ReorderLong(&p->optionsSize1);
	ReorderLong(&p->optionsSize2);
}

void ReorderWaveHeader2(WaveHeader2* p)
{
	ReorderShort(&p->type);
	ReorderLong(&p->next);
	// char bname does not need to be reordered.
	ReorderShort(&p->whVersion);
	ReorderShort(&p->srcFldr);
	ReorderLong(&p->fileName);
	// char dataUnits does not need to be reordered.
	// char xUnits does not need to be reordered.
	ReorderLong(&p->npnts);
	ReorderShort(&p->aModified);
	ReorderDouble(&p->hsA);
	ReorderDouble(&p->hsB);
	ReorderShort(&p->wModified);
	ReorderShort(&p->swModified);
	ReorderShort(&p->fsValid);
	ReorderDouble(&p->topFullScale);
	ReorderDouble(&p->botFullScale);
	// char useBits does not need to be reordered.
	// char kindBits does not need to be reordered.
	ReorderLong(&p->formula);
	ReorderLong(&p->depID);
	ReorderLong(&p->creationDate);
	// char wUnused does not need to be reordered.
	ReorderLong(&p->modDate);
	ReorderLong(&p->waveNoteH);
	// The wData field marks the start of the wave data which will be reordered separately.
}

void ReorderWaveHeader5(WaveHeader5* p)
{
	ReorderLong(&p->next);
	ReorderLong(&p->creationDate);
	ReorderLong(&p->modDate);
	ReorderLong(&p->npnts);
	ReorderShort(&p->type);
	ReorderShort(&p->dLock);
	// char whpad1 does not need to be reordered.
	ReorderShort(&p->whVersion);
	// char bname does not need to be reordered.
	ReorderLong(&p->whpad2);
	ReorderLong(&p->dFolder);
	ReorderBytes(&p->nDim, 4, 4);
	ReorderBytes(&p->sfA, 8, 4);
	ReorderBytes(&p->sfB, 8, 4);
	// char dataUnits does not need to be reordered.
	// char dimUnits does not need to be reordered.
	ReorderShort(&p->fsValid);
	ReorderShort(&p->whpad3);
	ReorderDouble(&p->topFullScale);
	ReorderDouble(&p->botFullScale);
	ReorderLong(&p->dataEUnits);
	ReorderBytes(&p->dimEUnits, 4, 4);
	ReorderBytes(&p->dimLabels, 4, 4);
	ReorderLong(&p->waveNoteH);
	ReorderBytes(&p->whUnused, 4, 16);
	ReorderShort(&p->aModified);
	ReorderShort(&p->wModified);
	ReorderShort(&p->swModified);
	// char useBits does not need to be reordered.
	// char kindBits does not need to be reordered.
	ReorderLong(&p->formula);
	ReorderLong(&p->depID);
	ReorderShort(&p->whpad4);
	ReorderShort(&p->srcFldr);
	ReorderLong(&p->fileName);
	ReorderLong(&p->sIndices);
	// The wData field marks the start of the wave data which will be reordered separately.
}


void sopen_ibw_read (HDRTYPE* hdr) {
/*
	this function will be called by the function SOPEN in "biosig.c"

	Input: 
		char* Header	// contains the file content
		
	Output: 
		HDRTYPE *hdr	// defines the HDR structure accoring to "biosig.h"
*/


	fprintf(stdout,"Warning: support for IBW is very experimental\n");
	
	uint16_t version = *(uint16_t*)hdr->AS.Header;
	char flag_swap = (version & 0xFF) == 0;
	if (flag_swap) 
		version = bswap_16(version); 
	
	unsigned count=0; 
	int binHeaderSize; 
	int waveHeaderSize; 

	switch (version) {
	case 1: binHeaderSize=sizeof(BinHeader1); waveHeaderSize=sizeof(WaveHeader2); break; 
	case 2: binHeaderSize=sizeof(BinHeader2); waveHeaderSize=sizeof(WaveHeader2); break; 
	case 3: binHeaderSize=sizeof(BinHeader3); waveHeaderSize=sizeof(WaveHeader2); break; 
	case 5: binHeaderSize=sizeof(BinHeader5); waveHeaderSize=sizeof(WaveHeader5); break; 
	default:
		if (VERBOSE_LEVEL>7) fprintf(stderr,"ver=%x \n",version);
		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Igor/IBW: unsupported version number");
		return; 
	}
	count = binHeaderSize+waveHeaderSize; 

	if (hdr->HeadLen < count) {
		hdr->AS.Header = realloc(hdr->AS.Header, count+1);
		hdr->HeadLen  += ifread(hdr->AS.Header + hdr->HeadLen, 1, count-hdr->HeadLen, hdr);
	}

	// compute check sum 
	if ((int)hdr->VERSION == 5) count -= 4; 

	void *buffer = hdr->AS.Header;
	// Check the checksum.	
	int crc;
	if ((ibwChecksum((int16_t*)buffer, flag_swap, 0, count))) {
		if (VERBOSE_LEVEL>7) fprintf(stderr,"ver=%x crc = %x  %i %i \n",version, crc, binHeaderSize, waveHeaderSize);
		biosigERROR(hdr, B4C_CRC_ERROR, "Igor/IBW: checksum error");
		return;
	}
	
	// Do byte reordering if the file is from another platform.	
	if (flag_swap) {
		version = bswap_16(version); 
		switch(version) {
			case 1:
				ReorderBinHeader1((BinHeader1*)hdr->AS.Header);
				break;
			case 2:
				ReorderBinHeader2((BinHeader2*)hdr->AS.Header);
				break;
			case 3:
				ReorderBinHeader3((BinHeader3*)hdr->AS.Header);
				break;
			case 5:
				ReorderBinHeader5((BinHeader5*)hdr->AS.Header);
				break;
		}
		switch(version) {
			case 1:				// Version 1 and 2 files use WaveHeader2.
			case 2:
			case 3:
				ReorderWaveHeader2((WaveHeader2*)(hdr->AS.Header+binHeaderSize));
				break;
			case 5:
				ReorderWaveHeader5((WaveHeader5*)(hdr->AS.Header+binHeaderSize));
				break;
		}
	}
	
	// Read some of the BinHeader fields.
	int32_t wfmSize;
	uint32_t modDate;
	int16_t type = 0;			// See types (e.g. NT_FP64) above. Zero for text waves.

	hdr->NS = 1; 
	hdr->CHANNEL = (CHANNEL_TYPE*) realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));

		
	switch(version) {
		case 1:
			{
				BinHeader1* b1;
				b1 = (BinHeader1*)buffer;
				wfmSize = b1->wfmSize;
			}
			break;
			
		case 2:
			{
				BinHeader2* b2;
				b2 = (BinHeader2*)buffer;
				wfmSize = b2->wfmSize;
			}
			break;
			
		case 3:
			{
				BinHeader3* b3;
				b3 = (BinHeader3*)buffer;
				wfmSize = b3->wfmSize;
			}
			break;
			
		case 5:
			{
				BinHeader5* b5;
				b5 = (BinHeader5*)buffer;
				wfmSize = b5->wfmSize;
			}
			break;
	}

	// Read some of the WaveHeader fields.
	switch(version) {
		case 1:
		case 2:
		case 3:
			{
				WaveHeader2* w2;
				w2 = (WaveHeader2*)(buffer+binHeaderSize);
				modDate = w2->modDate;
				type = w2->type;
				strncpy(hdr->CHANNEL[0].Label, w2->bname, MAX_LENGTH_LABEL);
				hdr->CHANNEL[0].PhysDimCode = PhysDimCode(w2->dataUnits);
				hdr->CHANNEL[0].SPR = hdr->SPR = 1;
				hdr->NRec = w2->npnts;
				hdr->CHANNEL[0].Cal = w2->hsA;
				hdr->CHANNEL[0].Off = w2->hsB;
/*
				hdr->CHANNEL[0].PhysMax = w2->topFullScale;
				hdr->CHANNEL[0].PhysMin = w2->botFullScale;
				hdr->CHANNEL[0].DigMax = (w2->topFullScale-w2->hsB)/w2->hsA;
				hdr->CHANNEL[0].DigMin = (w2->botFullScale-w2->hsB)/w2->hsA;
*/
				hdr->FLAG.OVERFLOWDETECTION = !w2->fsValid;	
			}
			break;
			
		case 5:
			{
				WaveHeader5* w5;
				w5 = (WaveHeader5*)(buffer+binHeaderSize);
				modDate = w5->modDate;
				type = w5->type;
				strncpy(hdr->CHANNEL[0].Label, w5->bname, MAX_LENGTH_LABEL);
				hdr->CHANNEL[0].PhysDimCode = PhysDimCode(w5->dataUnits);
				hdr->CHANNEL[0].SPR = hdr->SPR = 1;
				hdr->NRec = w5->npnts;
/*
				hdr->CHANNEL[0].Cal = w5->hsA;
				hdr->CHANNEL[0].Off = w5->hsB;
*/
/*				hdr->CHANNEL[0].PhysMax = w5->topFullScale;
				hdr->CHANNEL[0].PhysMin = w5->botFullScale;
*/
				hdr->FLAG.OVERFLOWDETECTION = !w5->fsValid;	
			}
			break;
	}

	if (VERBOSE_LEVEL > 7) fprintf(stdout, "Wave name=%s, npnts=%d, type=0x%x, wfmSize=%d.\n", hdr->CHANNEL[0].Label, hdr->SPR, type, wfmSize);
	
	uint16_t gdftyp; 
	double digmin,digmax; 
	// Consider the number type, not including the complex bit or the unsigned bit.
	switch(type & ~(NT_CMPLX | NT_UNSIGNED)) {
		case NT_I8:
			gdftyp = 0; 
			hdr->AS.bpb = 1;		// char
			break;
		case NT_I16:
			gdftyp = 3; 
			hdr->AS.bpb = 2;		// short
			if (type & NT_UNSIGNED) {
				gdftyp++;
				digmin = ldexp(-1,15);
				digmax = ldexp( 1,15)-1;
			} else {
				digmin = 0;
				digmax = ldexp( 1,16)-1;
			}	
			break;
		case NT_I32:
			gdftyp = 5; 
			hdr->AS.bpb = 4;		// long
			if (type & NT_UNSIGNED) {
				gdftyp++;
				digmin = ldexp(-1,31);
				digmax = ldexp( 1,31)-1;
			} else {
				digmin = 0;
				digmax = ldexp( 1,32)-1;
			}	
			break;
		case NT_FP32:
			gdftyp = 16; 
			hdr->AS.bpb = 4;		// float
			digmin = -__FLT_MAX__;
			digmax = __FLT_MAX__;
			break;
		case NT_FP64:
			gdftyp = 17;
			hdr->AS.bpb = 8;		// double
			digmin = -__DBL_MAX__;
			digmax = __DBL_MAX__;
			break;
		default:
			if (VERBOSE_LEVEL>7) fprintf(stderr,"type=%x \n",version);
			biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "Igor/IBW: unknonw data type");
			return;
			break;
	}

	if (type & NT_CMPLX) {
		hdr->AS.bpb *= 2;			// Complex wave - twice as many points.
		hdr->NS     *= 2; 	
		hdr->CHANNEL = (CHANNEL_TYPE*) realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
		strcpy(hdr->CHANNEL[2].Label,"imag part");
	}
	
	typeof (hdr->NS) k; 
	for (k = 0; k < hdr->NS; k++) {
		CHANNEL_TYPE *hc = hdr->CHANNEL+k; 
		hc->GDFTYP = gdftyp; 
		hc->OnOff  = 1;
		hc->DigMin = digmin; 
		hc->DigMax = digmax; 
		hc->PhysMax = digmax*hc->Cal+hc->Off; 
		hc->PhysMin = digmin*hc->Cal+hc->Off; 
	}

/*
	// Return information to the calling routine.
	*typePtr = type;
	*npntsPtr = npnts;

	// Determine the number of bytes of wave data in the file.
	switch(version) {
		case 1:
		case 2:
		case 3:
			waveDataSize = wfmSize - offsetof(WaveHeader2, wData) - 16;
			hdr->HeadLen = binHeaderSize+waveHeaderSize-16;    // 16 = size of wData field in WaveHeader2 structure.
			break;
		case 5:
			waveDataSize = wfmSize - offsetof(WaveHeader5, wData);
			hdr->HeadLen = binHeaderSize+waveHeaderSize-4;    // 4 = size of wData field in WaveHeader5 structure.
			break;
	}
*/
	
	ifseek(hdr,hdr->HeadLen,'SEEK_SET');
	hdr->FILE.POS = 0; 
	hdr->AS.first = 0; 
	hdr->AS.length= hdr->NRec; 
	hdr->data.block = NULL; 
	hdr->AS.rawdata = NULL;
	
	if (type == 0) {
		// For simplicity, we don't load text wave data in this example program.
		printf("This is a text wave.\n");
		return;
	}


	
}


#ifdef __cplusplus
}
#endif


