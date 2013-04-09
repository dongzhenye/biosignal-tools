/*

    $Id: sopen_abf_read.c,v 1.2 2009-02-12 16:15:17 schloegl Exp $
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


#include <assert.h>
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

		if ((size_t)(hdr->NRec * hdr->SPR) <= (ln+1) ) {
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

		fprintf(stdout,"Warning ABF v%4.2f: not well tested!\n",hdr->VERSION);

		size_t count = hdr->HeadLen; 	
		hdr->VERSION = lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fFileVersionNumber));

		hdr->HeadLen = (hdr->VERSION < 1.6) ? ABF_OLDHEADERSIZE : ABF_HEADERSIZE;
		if (count < hdr->HeadLen) {
			hdr->AS.Header = (uint8_t*) realloc(hdr->AS.Header, hdr->HeadLen);
			count         += ifread(hdr->AS.Header+count, 1, hdr->HeadLen-count, hdr);
		}
		hdr->HeadLen = count;
		assert(hdr->HeadLen == leu32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lHeaderSize)));

		{
			struct tm t;
			uint32_t u = leu32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lFileStartDate));
				t.tm_year = u / 10000 - 1900;
			t.tm_mon  = (u % 10000)/100 - 1;
			t.tm_mday = (u % 100);
			u = leu32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lFileStartTime));
			t.tm_hour = u / 3600;
			t.tm_min = (u % 3600)/60;
			t.tm_sec = (u % 60);
			//u = leu16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nFileStartMillisecs));
			hdr->T0 = tm_time2gdf_time(&t);
		}

		uint16_t gdftyp = 3;
		switch (lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nDataFormat))) {
		case 0: gdftyp = 3; break;
		case 1: gdftyp = 16; break;
		}

		size_t slen;
		slen = min(MAX_LENGTH_MANUF, sizeof(((struct ABFFileHeader*)(hdr->AS.Header))->sCreatorInfo));
		slen = min(MAX_LENGTH_MANUF, ABF_CREATORINFOLEN);
		strncpy(hdr->ID.Manufacturer._field, (char*)hdr->AS.Header + offsetof(struct ABFFileHeader, sCreatorInfo), slen);
		hdr->ID.Manufacturer._field[slen] = 0;
		hdr->ID.Manufacturer.Name = hdr->ID.Manufacturer._field;
		slen = min(MAX_LENGTH_RID, sizeof(((struct ABFFileHeader*)(hdr->AS.Header))->sFileComment));
		strncpy(hdr->ID.Recording,(char*)hdr->AS.Header + offsetof(struct ABFFileHeader, sFileComment), slen);
		hdr->ID.Recording[slen] = 0;

		if (VERBOSE_LEVEL>7) {
			fprintf(stdout,"sCreatorInfo:\t%s\n",hdr->AS.Header + offsetof(struct ABFFileHeader, sCreatorInfo));
			fprintf(stdout,"_sFileComment:\t%s\n",hdr->AS.Header + offsetof(struct ABFFileHeader, _sFileComment));
			fprintf(stdout,"sFileComment:\t%s\n",hdr->AS.Header + offsetof(struct ABFFileHeader, sFileComment));
			fprintf(stdout,"sFileComment:\t%s\n",hdr->AS.Header + offsetof(struct ABFFileHeader, sFileComment));


			fprintf(stdout,"\nlHeaderSize:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lHeaderSize)));
			fprintf(stdout,"lTagSectionPtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lTagSectionPtr)));
			fprintf(stdout,"lNumTagEntries:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lNumTagEntries)));
			fprintf(stdout,"lVoiceTagPtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lVoiceTagPtr)));
			fprintf(stdout,"lVoiceTagEntries:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lVoiceTagEntries)));
			fprintf(stdout,"lSynchArrayPtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lSynchArrayPtr)));
			fprintf(stdout,"lSynchArraySize:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lSynchArraySize)));

			fprintf(stdout,"\nlDataSectionPtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lDataSectionPtr)));
			fprintf(stdout,"lScopeConfigPtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lScopeConfigPtr)));
			fprintf(stdout,"lNumScopes:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lNumScopes)));
			fprintf(stdout,"_lDACFilePtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, _lDACFilePtr)));
			fprintf(stdout,"_lDACFileNumEpisodes:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, _lDACFileNumEpisodes)));
			fprintf(stdout,"lDeltaArrayPtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lDeltaArrayPtr)));
			fprintf(stdout,"lNumDeltas:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lNumDeltas)));
			fprintf(stdout,"nDataFormat:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nDataFormat)));
			fprintf(stdout,"nSimultaneousScan:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nSimultaneousScan)));
			fprintf(stdout,"lStatisticsConfigPtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lStatisticsConfigPtr)));
			fprintf(stdout,"lAnnotationSectionPtr:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lAnnotationSectionPtr)));
			fprintf(stdout,"lNumAnnotations:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lNumAnnotations)));

			fprintf(stdout,"\nlNumSamplesPerEpisode:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lNumSamplesPerEpisode)));
			fprintf(stdout,"lPreTriggerSamples:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lPreTriggerSamples)));
			fprintf(stdout,"lEpisodesPerRun:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lEpisodesPerRun)));
			fprintf(stdout,"lActualAcqLength:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lActualAcqLength)));
			fprintf(stdout,"lActualEpisodes:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lActualEpisodes)));
			fprintf(stdout,"lRunsPerTrial:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lRunsPerTrial)));
			fprintf(stdout,"lNumberOfTrials:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lNumberOfTrials)));
			fprintf(stdout,"fADCRange:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCRange)));
			fprintf(stdout,"fDACRange:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fDACRange)));
			fprintf(stdout,"lADCResolution:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lADCResolution)));
			fprintf(stdout,"lDACResolution:\t%i\n",lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lDACResolution)));

			fprintf(stdout,"\nchannel_count_acquired:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, channel_count_acquired)));
			fprintf(stdout,"nADCNumChannels:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nADCNumChannels)));
			fprintf(stdout,"nDigitalDACChannel:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nDigitalDACChannel)));
			fprintf(stdout,"nOperationMode:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nOperationMode)));
			fprintf(stdout,"nDigitalEnable:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nDigitalEnable)));

			fprintf(stdout,"\nfFileVersionNumber:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fFileVersionNumber)));
			fprintf(stdout,"fHeaderVersionNumber:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fHeaderVersionNumber)));
			fprintf(stdout,"fADCSampleInterval:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCSampleInterval)));
			fprintf(stdout,"fADCSecondSampleInterval:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCSecondSampleInterval)));
			fprintf(stdout,"fSynchTimeUnit:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSynchTimeUnit)));
			fprintf(stdout,"fSecondsPerRun:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSecondsPerRun)));
			fprintf(stdout,"fTriggerThreshold:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fTriggerThreshold)));

			fprintf(stdout,"\nfScopeOutputInterval:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fScopeOutputInterval)));
			fprintf(stdout,"fEpisodeStartToStart:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fEpisodeStartToStart)));
			fprintf(stdout,"fRunStartToStart:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fRunStartToStart)));
			fprintf(stdout,"fTrialStartToStart:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fTrialStartToStart)));
			fprintf(stdout,"fStatisticsPeriod:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fStatisticsPeriod)));
			fprintf(stdout,"fADCRange:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCRange)));
			fprintf(stdout,"fDACRange:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fTriggerThreshold)));
			fprintf(stdout,"fTriggerThreshold:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fDACRange)));
			fprintf(stdout,"dFileDuration:\t%g\n",lef64p(hdr->AS.Header + offsetof(struct ABFFileHeader, dFileDuration)));
			fprintf(stdout,"fTriggerThreshold:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fTriggerThreshold)));

		}

		hdr->NS = lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nADCNumChannels));
		if (lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nDigitalEnable)))
			hdr->NS += lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nDigitalDACChannel));

		hdr->SampleRate = 1e6 / (hdr->NS * lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCSampleInterval)));

		hdr->NRec = lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lActualAcqLength)) / lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nADCNumChannels));
		hdr->SPR  = 1;
		hdr->AS.bpb = hdr->NS*GDFTYP_BITS[gdftyp]/8;

		hdr->CHANNEL = realloc(hdr->CHANNEL, hdr->NS*sizeof(CHANNEL_TYPE));
		uint32_t k1=0,k;	// ABF is 32 bits only, no need for more
		for (k=0; k < ABF_ADCCOUNT + ABF_DACCOUNT; k++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL+k1;
			hc->bufptr = NULL;
			hc->LeadIdCode = 0;
			if ((k < ABF_ADCCOUNT) && (lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nADCSamplingSeq) + 2 * k) >= 0)) {
				hc->OnOff = 1;
				strncpy(hc->Label, (char*)hdr->AS.Header + offsetof(struct ABFFileHeader, sADCChannelName) + k*ABF_ADCNAMELEN, min(ABF_ADCNAMELEN,MAX_LENGTH_LABEL));
				hc->Label[ABF_ADCNAMELEN] = 0;

				char units[ABF_ADCUNITLEN+1]; {
					memcpy(units, (char*)hdr->AS.Header + offsetof(struct ABFFileHeader, sADCUnits) + k*ABF_ADCUNITLEN, ABF_ADCUNITLEN);
					units[ABF_ADCUNITLEN] = 0;
					int p=ABF_ADCUNITLEN; 	while ( (0<p) && isspace(units[--p])) units[p]=0;  // remove trailing white space
					hc->PhysDimCode = PhysDimCode(units);
				}
				hc->LowPass  = lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalLowpassFilter) + 4 * k);
				hc->HighPass = lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalHighpassFilter) + 4 * k);
				hc->GDFTYP   = gdftyp;
				hc->SPR      = hdr->SPR;
				hc->bi       = k1*GDFTYP_BITS[gdftyp]/8;

				double PhysMax = lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCRange));
				double DigMax  = (double)lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lADCResolution));
				double fInstrumentScaleFactor = lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fInstrumentScaleFactor) + 4 * k);
				hc->Cal      = fInstrumentScaleFactor * PhysMax/DigMax;
				hc->Off      = 0.0;
				hc->DigMax   = DigMax-1.0;
				hc->DigMin   = -hc->DigMax;
				hc->PhysMax  = hc->DigMax * hc->Cal;
				hc->PhysMin  = hc->DigMin * hc->Cal;

				if (VERBOSE_LEVEL>7) {
					fprintf(stdout,"==== CHANNEL %i [%s] ====\n",k,units);
					fprintf(stdout,"nADCPtoLChannelMap:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nADCPtoLChannelMap) + 2 * k));
					fprintf(stdout,"nADCSamplingSeq:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nADCSamplingSeq) + 2 * k));
					fprintf(stdout,"fADCProgrammableGain:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCProgrammableGain) + 4 * k));

					fprintf(stdout,"fADCDisplayAmplification:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCDisplayAmplification) + 4 * k));
					fprintf(stdout,"fADCDisplayOffset:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCDisplayOffset) + 4 * k));
					fprintf(stdout,"fInstrumentScaleFactor:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fInstrumentScaleFactor) + 4 * k));
					fprintf(stdout,"fInstrumentOffset:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fInstrumentOffset) + 4 * k));
					fprintf(stdout,"fSignalGain:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalGain) + 4 * k));
					fprintf(stdout,"fSignalOffset:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalOffset) + 4 * k));
					fprintf(stdout,"fSignalLowpassFilter:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalLowpassFilter) + 4 * k));
					fprintf(stdout,"fSignalHighpassFilter:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalHighpassFilter) + 4 * k));
				}
				k1++;
			}
			else if (k < ABF_ADCCOUNT) {
				hc->OnOff = 0;
				// do not increase k1;
				if (VERBOSE_LEVEL>7) {
					fprintf(stdout,"==== CHANNEL %i ====\n",k);

					fprintf(stdout,"nADCPtoLChannelMap:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nADCPtoLChannelMap) + 2 * k));
					fprintf(stdout,"nADCSamplingSeq:\t%i\n",lei16p(hdr->AS.Header + offsetof(struct ABFFileHeader, nADCSamplingSeq) + 2 * k));

					fprintf(stdout,"fADCProgrammableGain:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCProgrammableGain) + 4 * k));

					fprintf(stdout,"fADCDisplayAmplification:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCDisplayAmplification) + 4 * k));
					fprintf(stdout,"fADCDisplayOffset:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fADCDisplayOffset) + 4 * k));
					fprintf(stdout,"fInstrumentScaleFactor:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fInstrumentScaleFactor) + 4 * k));
					fprintf(stdout,"fInstrumentOffset:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fInstrumentOffset) + 4 * k));
					fprintf(stdout,"fSignalGain:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalGain) + 4 * k));
					fprintf(stdout,"fSignalOffset:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalOffset) + 4 * k));
					fprintf(stdout,"fSignalLowpassFilter:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalLowpassFilter) + 4 * k));
					fprintf(stdout,"fSignalHighpassFilter:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fSignalHighpassFilter) + 4 * k));
				}
			}
			else if ( (k1 < ABF_ADCCOUNT+ABF_DACCOUNT) && (k < hdr->NS) ) {
				hc->OnOff = 1;
				strncpy(hc->Label, (char*)hdr->AS.Header + offsetof(struct ABFFileHeader, sDACChannelName) + (k-ABF_ADCCOUNT)*ABF_DACNAMELEN,
					min(ABF_DACNAMELEN,MAX_LENGTH_LABEL));
				hc->Label[ABF_DACNAMELEN] = 0;

				char units[ABF_DACUNITLEN+1]; {
					memcpy(units, hdr->AS.Header + offsetof(struct ABFFileHeader, sDACChannelUnits) + (k-ABF_ADCCOUNT)*ABF_DACUNITLEN, ABF_DACUNITLEN);
					units[ABF_DACUNITLEN] = 0;
					int p=ABF_ADCUNITLEN; 	while ( (0<p) && isspace(units[--p])) units[p]=0;  // remove trailing white space
					hc->PhysDimCode = PhysDimCode(units);
				}
				hc->GDFTYP   = gdftyp;
				hc->SPR = hdr->SPR;

				double PhysMax = lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fDACRange));
				double DigMax  = (double)lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lDACResolution));
				hc->Cal = PhysMax/DigMax;
				hc->Off = 0.0;
				hc->PhysMax  = PhysMax * (DigMax-1.0) / DigMax;
				hc->PhysMin  = -hc->PhysMax;
				hc->DigMax  = DigMax-1;
				hc->DigMin  = -hc->DigMax;

				if (VERBOSE_LEVEL>7) {
					fprintf(stdout,"==== CHANNEL %i [%s] ====\nfDACScaleFactor:\t%f\n",k,units,
						lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fDACScaleFactor) + 4 * (k-ABF_ADCCOUNT)));
					fprintf(stdout,"fDACHoldingLevel:\t%f\n",lef32p(hdr->AS.Header + offsetof(struct ABFFileHeader, fDACHoldingLevel) + 4 * (k-ABF_ADCCOUNT)));
				}
				k1++;
			}
		}

		/* ===== EVENT TABLE ===== */
		uint32_t n1,n2;
		n1 = lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lActualEpisodes)) - 1;
		n2 = lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lNumTagEntries));
		hdr->EVENT.N = n1+n2;

		/* add breaks between sweeps */
		size_t spr = lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lNumSamplesPerEpisode))/hdr->NS;
		hdr->EVENT.SampleRate = hdr->SampleRate;
		hdr->EVENT.POS = (uint32_t*) realloc(hdr->EVENT.POS, hdr->EVENT.N * sizeof(*hdr->EVENT.POS));
		hdr->EVENT.TYP = (uint16_t*) realloc(hdr->EVENT.TYP, hdr->EVENT.N * sizeof(*hdr->EVENT.TYP));
		for (k=0; k < n1; k++) {
			hdr->EVENT.TYP[k] = 0x7ffe;
			hdr->EVENT.POS[k] = (k+1)*spr;
		}
		/* add tags */
		hdr->AS.auxBUF = realloc(hdr->AS.auxBUF, n2 * sizeof(struct ABFTag));
		ifseek(hdr, lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lTagSectionPtr))*ABF_BLOCKSIZE, SEEK_SET);
		count = ifread(hdr->AS.auxBUF, sizeof(struct ABFTag), n2, hdr);
		if (count>255) {
			count = 255;
			fprintf(stderr,"Warning ABF: more than 255 tags cannot be read");
		}
		hdr->EVENT.N = n1+count;
		for (k=0; k < count; k++) {
			uint8_t *abftag = hdr->AS.auxBUF + k * sizeof(struct ABFTag);
			hdr->EVENT.POS[k+n1] = leu32p(abftag)/hdr->NS;
			abftag[ABF_TAGCOMMENTLEN+4-1]=0;
			FreeTextEvent(hdr, k+n1, (char*)(abftag+4));
		}

		// set HeadLen to begin of data block
		hdr->HeadLen = ABF_BLOCKSIZE * lei32p(hdr->AS.Header + offsetof(struct ABFFileHeader, lDataSectionPtr));
		ifseek(hdr, hdr->HeadLen, SEEK_SET);

	} else {	// ABF 2.0+

		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "ABF2 format currently not supported");
		return;

		fprintf(stdout,"Warning ABF v%4.2f: implementation is not complete!\n",hdr->VERSION);

		if (hdr->HeadLen < 512) {
		    	hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, 512);
			hdr->HeadLen  += ifread(hdr->AS.Header+hdr->HeadLen, 1, 512-hdr->HeadLen, hdr);
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
}

