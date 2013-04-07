/*

    $Id: sopen_abf_read.c,v 1.2 2009-02-12 16:15:17 schloegl Exp $
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


#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../biosig-dev.h"

#define ABFLONG uint32_t
#include "axon_structs.h"	// ABF2
#include "abfheadr.h"	// ABF1


/*
	read data block of ATF file into the cache
	also HDR.NRec is defined here
*/

EXTERN_C void sread_atf(HDRTYPE* hdr) {

	if (VERBOSE_LEVEL>6) fprintf(stdout,"SREAD ATF [%i,%i]\n",(unsigned)hdr->NRec, (unsigned)hdr->SPR);

	if (hdr->AS.rawdata != NULL) return;
	if (hdr->NRec * hdr->SPR > 0)
		hdr->AS.rawdata = malloc(hdr->NRec * hdr->SPR * hdr->AS.bpb);

	ifseek(hdr, hdr->HeadLen, SEEK_SET);

	size_t ll  = 512;
	char *line = malloc(ll+1);

	if (VERBOSE_LEVEL>6) fprintf(stdout,"SREAD ATF\n");

	size_t ln = 0;
	while (~ifeof(hdr)) {
		ssize_t nc = getline(&line, &ll, hdr->FILE.FID);
		if (nc < 0) break;

		if (VERBOSE_LEVEL>8) fprintf(stdout,"SREAD ATF  %i\t<%s>\n",(unsigned)ln,line );

		if (hdr->NRec * hdr->SPR <= (ln+1) ) {
			hdr->NRec = max(1024, ln*2);
			hdr->AS.rawdata = realloc(hdr->AS.rawdata, hdr->NRec * hdr->SPR * hdr->AS.bpb);
		}

		char *str = strtok(line,"\t");
		typeof(hdr->NS) k;
		for (k = 0; k < hdr->NS; k++) {
			*(double*)(hdr->AS.rawdata + ln*hdr->AS.bpb + hdr->CHANNEL[k].bi) = strtod(str, &str);
			// extract next value
			str = strtok(NULL,"\t");
		}
		ln++;
	}
	free(line);

	hdr->NRec = ln;
	hdr->AS.first  = 0;
	hdr->AS.length = hdr->NRec;

}

EXTERN_C void sopen_abf_read(HDRTYPE* hdr) {	
/*
	this function will be called by the function SOPEN in "biosig.c"

	Input: 
		char* Header	// contains the file content
		
	Output: 
		HDRTYPE *hdr	// defines the HDR structure accoring to "biosig.h"
*/	

	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 101\n");

	if (!memcmp(hdr->AS.Header, "ABF ", 4)) {	// ABF v1.x
		size_t count = hdr->HeadLen; 	
		hdr->VERSION = lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fFileVersionNumber));
		hdr->HeadLen = (hdr->VERSION < 1.6) ? ABF_OLDHEADERSIZE : ABF_HEADERSIZE;
		if (count < hdr->HeadLen) {
			hdr->AS.Header = (uint8_t*) realloc(hdr->AS.Header, hdr->HeadLen);
			count         += ifread(hdr->AS.Header+count, 1, hdr->HeadLen-count, hdr);
		}
		hdr->HeadLen = count;
	} else {	// ABF 2.0+
		if (hdr->HeadLen < 512) {
		    	hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, 512);
			hdr->HeadLen  += ifread(hdr->AS.Header+hdr->HeadLen, 1, 512-hdr->HeadLen, hdr);
		}
	}

	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115\n");

//   unsigned int  uFileSignature;	// +0
//   unsigned int  uFileVersionNumber;	// +4

   // After this point there is no need to be the same as the ABF 1 equivalent.
//   unsigned int  uFileInfoSize;	// +8

//   unsigned int  uActualEpisodes;	// +12
	uint32_t actual_episodes = leu32p(hdr->AS.Header+12); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: actual:episodes %u\n", actual_episodes);

//   unsigned int  uFileStartDate;	// +16
	uint32_t start_date = leu32p(hdr->AS.Header+16); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: start_date %u %x\n", start_date, start_date);

//   unsigned int  uFileStartTimeMS;	// +20 
	uint32_t start_time = leu32p(hdr->AS.Header+20); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: start time %u %x\n", start_time, start_time);

//   unsigned int  uStopwatchTime;	// +24
	uint32_t stopwatchtime  = leu32p(hdr->AS.Header+24); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: stopwatchtime %u %x\n", stopwatchtime, stopwatchtime);

//   short nFileType;			// +28
	uint16_t filetype = leu16p(hdr->AS.Header+28); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: filetype %u 0x%x\n", filetype, filetype);

//   short nDataFormat;			// +30
	uint16_t dataformat = leu16p(hdr->AS.Header+30); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: dataformat %u\n", dataformat);

//   short nSimultaneousScan;		// +32
	uint16_t simScan = leu16p(hdr->AS.Header+32); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: simScan %u\n", simScan);

//   short nCRCEnable;			// +34
	uint16_t crc_enabled = leu16p(hdr->AS.Header+34); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: crc_enabled %u\n", crc_enabled);

//   unsigned int  uFileCRC;		// +36
	uint32_t fileCRC = leu32p(hdr->AS.Header+36); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: fileCRC %u\n", fileCRC);

//   MYGUID  FileGUID;			// +40

//   unsigned int  uCreatorVersion;	// +56
	uint32_t creatorVersion = leu32p(hdr->AS.Header+56); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: creatorVersion %u\n", creatorVersion);

//   unsigned int  uCreatorNameIndex;	// +60 
	uint32_t creatorNameIndex = leu32p(hdr->AS.Header+60); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: creatorNameIndex %u\n", creatorNameIndex);

//   unsigned int  uModifierVersion;	// +64
	uint32_t modifierVersion = leu32p(hdr->AS.Header+64); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: modifierVersion %u\n", modifierVersion);

//   unsigned int  uModifierNameIndex;	// +68
	uint32_t modifierNameIndex = leu32p(hdr->AS.Header+68); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: modifierNameIndex %u\n", modifierNameIndex);

//   unsigned int  uProtocolPathIndex;	// +72
	uint32_t ProtocolPathIndex = leu32p(hdr->AS.Header+72); 
	if (VERBOSE_LEVEL>7) fprintf(stdout,"sopen_abf_read 115: ProtocolPathIndex %u\n", ProtocolPathIndex);


	
			//uint16_t gdftyp = 3;
			float fADCRange;
			float fDACRange;
			long  lADCResolution;
			long  lDACResolution;
			uint8_t* b = NULL;
			int k1,k;
			for (k1=0; k1<18; ++k1) {
				uint32_t BlockIndex = leu32p(hdr->AS.Header + k1*16 + 19*4);
				uint32_t BlockSize  = leu32p(hdr->AS.Header + k1*16 + 19*4+4);
				uint64_t numBlocks  = leu64p(hdr->AS.Header + k1*16 + 19*4+8);

				if (VERBOSE_LEVEL>7)
					fprintf(stdout,"ABF %02i: %04u %04u %08u\n",k1,(uint32_t)BlockIndex,(uint32_t)BlockSize,(uint32_t)numBlocks);

				ifseek(hdr, BlockIndex*512, SEEK_SET);
				b  = (uint8_t*)realloc(b,numBlocks*BlockSize);

continue; // FIXME
				if (numBlocks==0 || BlockSize==0) continue; 
				ifread(b,numBlocks,BlockSize,hdr);

				if 	(BlockIndex==1) {
					k = 1; 
					hdr->SampleRate = 1.0 / lef32p(b+k*BlockSize+2);
					hdr->NRec       = leu32p(b+k*BlockSize+36);
   					fADCRange 	= lef32p(b+k*BlockSize+108);
					fDACRange 	= lef32p(b+k*BlockSize+112);
					lADCResolution	= leu32p(b+k*BlockSize+116);
					lDACResolution	= leu32p(b+k*BlockSize+120);
				}
				else if (BlockIndex==2) {
					hdr->NS = numBlocks;
					hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));

					hdr->AS.bpb = 0;
					for (k=0;k<hdr->NS;k++)	{
						CHANNEL_TYPE *hc = hdr->CHANNEL+k;
						// initialize fields
					      	hc->Label[0]  = 0;
					      	strcpy(hc->Transducer, "EEG: Ag-AgCl electrodes");
					      	hc->PhysDimCode = 19+4256; // uV
					      	hc->PhysMax   = +100;
					      	hc->PhysMin   = -100;
					      	hc->DigMax    = +2047;
					      	hc->DigMin    = -2048;
					      	hc->GDFTYP    = 3;	// int16
					      	hc->SPR       = leu32p(b+k*BlockSize+20);
					      	hc->OnOff     = 1;
					      	hc->Notch     = 50;
					      	hc->Impedance = INFINITY;
					      	hc->fZ        = NAN;
					      	hc->bi 	  = hdr->AS.bpb;
					      	hdr->AS.bpb += (GDFTYP_BITS[hc->GDFTYP]*hc->SPR)>>3;
					}

					uint16_t ch;
					for (k=0;k<hdr->NS;k++)	{
						ch = leu16p(b+k*BlockSize);
					      	hdr->CHANNEL[ch].LeadIdCode= 0;
					      	hdr->CHANNEL[ch].OnOff     = 1;
						hdr->CHANNEL[ch].Cal 	 = lef32p(b+k*BlockSize+48);
						hdr->CHANNEL[ch].Off 	 = lef32p(b+k*BlockSize+52);
						hdr->CHANNEL[ch].LowPass = lef32p(b+k*BlockSize+56);
						hdr->CHANNEL[ch].HighPass= lef32p(b+k*BlockSize+60);
					}
				}
				else if (BlockIndex==11) {
//					fprintf(stdout,"%i: %s\n",c,buf);
				}
			}
			if (b) free(b);
}

