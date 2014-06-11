/*
    sandbox is used for development and under constraction work
    The functions here are either under construction or experimental.
    The functions will be either fixed, then they are moved to another place;
    or the functions are discarded. Do not rely on the interface in this function

    Copyright (C) 2008-2014 Alois Schloegl <alois.schloegl@gmail.com>
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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <iconv.h>


#include "../biosig-dev.h"

// these functios are stubs

#ifdef WITH_DCMTK
#undef WITH_DICOM
#undef WITH_GDCM


EXTERN_C int sopen_dicom_read(HDRTYPE* hdr) {
	fprintf(stdout,"DCMTK is used to read dicom files.\n");

}

#endif

#ifdef WITH_HDF
#include <hdf5.h>
#endif
#ifdef WITH_MATIO
#include <matio.h>
#endif


#ifdef WITH_GDCM
#undef WITH_DICOM

#include "gdcmReader.h"
//#include "gdcmImageReader.h"
//#include "gdcmWriter.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"

//#include "gdcmCommon.h"
#include "gdcmPreamble.h"
#include "gdcmFile.h"
#include "gdcmFileMetaInformation.h"
#include "gdcmReader.h"
#include "gdcmImageReader.h"
#include "gdcmWriter.h"
#include "gdcmDataSet.h"
#include <gdcmAttribute.h>
#include <gdcmWaveform.h>

EXTERN_C int sopen_dicom_read(HDRTYPE* hdr) {

	fprintf(stdout,"GDCM is used to read dicom files.\n");

	gdcm::Reader r;
        const gdcm::DataElement *de;
	r.SetFileName( hdr->FileName );
	if( !r.Read() )
		return 1;

	gdcm::File &file = r.GetFile();
	gdcm::FileMetaInformation &header = file.GetHeader();
	if ( header.FindDataElement( gdcm::Tag(0x0002, 0x0013 ) ) )
		const gdcm::DataElement &de = header.GetDataElement( gdcm::Tag(0x0002, 0x0013) );

	gdcm::DataSet &ds = file.GetDataSet();

	if ( header.FindDataElement( gdcm::Tag(0x0002, 0x0010 ) ) )
		de = &header.GetDataElement( gdcm::Tag(0x0002, 0x0010) );

fprintf(stdout,"attr <0x0002,0x0010> len=%i\n",de->GetByteValue() );

/*
	{
		gdcm::Attribute<0x28,0x100> at;
		at.SetFromDataElement( ds.GetDataElement( at.GetTag() ) );
		if( at.GetValue() != 8 )
			return 1;
		//at.SetValue( 32 );
		//ds.Replace( at.GetAsDataElement() );
	}
	{

fprintf(stdout,"attr <0x0008,0x002a>\n");
		gdcm::Attribute<0x0008,0x002a> at;
fprintf(stdout,"attr <0x0008,0x002a>\n");
 		ds.GetDataElement( at.GetTag() );
fprintf(stdout,"attr <0x0008,0x002a>\n");
		at.SetFromDataElement( ds.GetDataElement( at.GetTag() ) );

		fprintf(stdout,"DCM: [0008,002a]: %i %p\n",at.GetNumberOfValues(), at.GetValue());
	}
*/
	{

fprintf(stdout,"attr <0x0008,0x0023>\n");
		gdcm::Attribute<0x0008,0x0023> at;
fprintf(stdout,"attr <0x0008,0x0023>\n");
 		ds.GetDataElement( at.GetTag() );
fprintf(stdout,"attr <0x0008,0x0023>\n");
//		at.SetFromDataElement( ds.GetDataElement( at.GetTag() ) );

//		fprintf(stdout,"DCM: [0008,0023]: %i %p\n",at.GetNumberOfValues(), at.GetValue());
	}


/*
				{
				struct tm t0;
				hdr->AS.Header[pos+14]=0;
				t0.tm_sec = atoi((char*)hdr->AS.Header+pos+12);
				hdr->AS.Header[pos+12]=0;
				t0.tm_min = atoi((char*)hdr->AS.Header+pos+10);
				hdr->AS.Header[pos+10]=0;
				t0.tm_hour = atoi((char*)hdr->AS.Header+pos+8);
				hdr->AS.Header[pos+8]=0;
				t0.tm_mday = atoi((char*)hdr->AS.Header+pos+6);
				hdr->AS.Header[pos+6]=0;
				t0.tm_mon = atoi((char*)hdr->AS.Header+pos+4)-1;
				hdr->AS.Header[pos+4]=0;
				t0.tm_year = atoi((char*)hdr->AS.Header+pos)-1900;

				hdr->T0 = tm_time2gdf_time(&t0);
				break;
				}

*/
}
#endif



#ifdef __cplusplus
extern "C" {
#endif


#ifdef WITH_HDF
int sopen_hdf5(HDRTYPE* hdr) {
        /*
                file hdr->FileName is already opened and hdr->HeadLen bytes are read
                These are available from hdr->AS.Header.

                ToDo: populate hdr
        */
	size_t count = hdr->HeadLen;
        fprintf(stdout,"Trying to read HDF data using \"%s\"\n",H5_VERS_INFO);

	ifclose(hdr);

        return(-1);
}
#endif

#ifdef WITH_MATIO
int sopen_matlab(HDRTYPE* hdr) {
        /*
                file hdr->FileName is already opened and hdr->HeadLen bytes are read
                These are available from hdr->AS.Header.

                ToDo: populate hdr
			sanity checks
			memory leaks
        */
	ifclose(hdr);
	//size_t count = hdr->HeadLen;

        fprintf(stdout, "Trying to read Matlab data using MATIO v%i.%i.%i\n", MATIO_MAJOR_VERSION, MATIO_MINOR_VERSION, MATIO_RELEASE_LEVEL);

	mat_t *matfile = Mat_Open(hdr->FileName, MAT_ACC_RDONLY);
	matvar_t *EEG=NULL, *pnts=NULL, *nbchan=NULL, *trials=NULL, *srate=NULL, *data=NULL, *chanlocs=NULL, *event=NULL;
	if (matfile != NULL) {
		EEG    = Mat_VarRead(matfile, "EEG" );
		if (EEG != NULL) {
			Mat_VarReadDataAll(matfile, EEG );
			pnts   = Mat_VarGetStructField(EEG, "pnts", BY_NAME, 0);
			nbchan = Mat_VarGetStructField(EEG, "nbchan", BY_NAME, 0);
			trials = Mat_VarGetStructField(EEG, "trials", BY_NAME, 0);
			srate  = Mat_VarGetStructField(EEG, "srate", BY_NAME, 0);
			data   = Mat_VarGetStructField(EEG, "data", BY_NAME, 0);
			chanlocs = Mat_VarGetStructField(EEG, "chanlocs", BY_NAME, 0);
			event    = Mat_VarGetStructField(EEG, "event", BY_NAME, 0);

			hdr->NS  = *(double*)(nbchan->data);
			hdr->SPR = *(double*)(pnts->data);
			hdr->NRec= *(double*)(trials->data);
			hdr->SampleRate = *(double*)(srate->data);

/* TODO CB
			hdr->NRec 	 = ;
			hdr->SPR  	 = ;
			hdr->T0 	 = 0;        // Unknown;
			uint16_t gdftyp  = ;	16: float; 17: double
*/
			hdr->CHANNEL = (CHANNEL_TYPE*) realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
			size_t k;
			for (k=0; k<hdr->NS; k++) {
				CHANNEL_TYPE *hc = hdr->CHANNEL+k;
				sprintf(hc->Label,"#%2d",k+1);
				hc->SPR = hdr->SPR;
/* TODO CB
				hc->GDFTYP = gdftyp;
				hc->Transducer[0] = '\0';
			    	hc->LowPass	= ;
			    	hc->HighPass = ;
			    	hc->Notch	= ;  // unknown
			    	hc->PhysMax	= ;
			    	hc->DigMax	= ;
			    	hc->PhysMin	= ;
			    	hc->DigMin	= ;
			    	hc->Cal	 	= 1.0;
			    	hc->Off	 	= 0.0;
				hc->OnOff    	= 1;
			    	hc->PhysDimCode = 4275; // uV
			    	hc->LeadIdCode  = 0;
			    	hc->bi      	= k*GDFTYP_BITS[gdftyp]>>3;	// TODO AS
*/
			}

			size_t sz = hdr->NS*hdr->SPR*hdr->NRec*GDFTYP_BITS[gdftyp]>>3;
			hdr->AS.rawdata = realloc(hdr->AS.rawdata, sz);
/* TODO CB
			memcpy(hdr->AS.rawdata,...,sz);
*/
			hdr->EVENT.N = 0; 	// TODO CB
			hdr->EVENT.POS = (uint32_t*) realloc(hdr->EVENT.POS, hdr->EVENT.N*sizeof(*hdr->EVENT.POS));
			hdr->EVENT.TYP = (uint16_t*) realloc(hdr->EVENT.TYP, hdr->EVENT.N*sizeof(*hdr->EVENT.TYP));
			hdr->EVENT.DUR = (uint32_t*) realloc(hdr->EVENT.DUR, hdr->EVENT.N*sizeof(*hdr->EVENT.DUR));
			hdr->EVENT.CHN = (uint16_t*) realloc(hdr->EVENT.CHN, hdr->EVENT.N*sizeof(*hdr->EVENT.CHN));
			for (k=0; k<hdr->EVENT.N; k++) {
/* TODO CB
				hdr->EVENT.TYP[k] =
				FreeTextEvent(hdr, k, annotation)
				hdr->EVENT.POS[k] =
				hdr->EVENT.CHN[k] = 0;
				hdr->EVENT.DUR[k] = 0;
*/
			}

		hdr->AS.bpb = hdr->NS*2;
		hdr->FLAG.OVERFLOWDETECTION = 0; 	// BKR does not support automated overflow and saturation detection


			Mat_VarPrint(pnts,   1);
			Mat_VarPrint(nbchan, 1);
			Mat_VarPrint(trials, 1);
			Mat_VarPrint(srate,  1);
			//Mat_VarPrint(data,   1);
			//Mat_VarPrint(chanlocs, 1);
			//Mat_VarPrint(event,  1);


			Mat_VarFree(EEG);
		}

		Mat_Close(matfile);
	}

        return (0);
}
#endif


void sopen_axg_read(HDRTYPE* hdr) {

		hdr->FILE.LittleEndian = 0;

		// read whole file into RAM
		size_t count = hdr->HeadLen;
		while (!ifeof(hdr)) {
			const int minsize = 1024;
			hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, 2*count+minsize);
			count += ifread(hdr->AS.Header+count, 1, count+minsize, hdr);
		}
		ifclose(hdr);

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %p %i\n", __FILE__, __LINE__, hdr->CHANNEL, hdr->NS );

		int32_t nCol;
		switch ((int) hdr->VERSION) {
		case 1:
		case 2:
			nCol         = bei16p(hdr->AS.Header+6);
			hdr->HeadLen = 8;
			break;
		case 3:
		case 4:
		case 5:
		case 6:
			nCol      = bei32p(hdr->AS.Header+8);
			hdr->HeadLen = 12;
			break;
		default:
			biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - unsupported version number");
			return;
		}

		/* hack: for now each trace (i.e. column) becomes a separate channel -
			later the traces of the channels will be reorganized
		 */
		CHANNEL_TYPE *TEMPCHANNEL = (CHANNEL_TYPE*)malloc(nCol*sizeof(CHANNEL_TYPE));
		char **ValLabel = (char**)malloc(nCol*sizeof(char*));
		uint32_t *keyLabel = (uint32_t*)malloc(nCol*sizeof(uint32_t));

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %p %i\n", __FILE__, __LINE__, hdr->CHANNEL, hdr->NS );

		/******* read all column/traces ******/
		int32_t k;
		uint8_t *pos = hdr->AS.Header+hdr->HeadLen;
		hdr->SPR    = beu32p(pos);
		switch ((int) hdr->VERSION) {
		case 1:
			for (k = 0; k < nCol; k++) {
				CHANNEL_TYPE *hc = TEMPCHANNEL+k;
				hc->GDFTYP = 16; 	//float
				hc->PhysDimCode = 0;
				hc->SPR    = beu32p(pos);

				int strlen = pos[4];   // string in Pascal format
				if (strlen > 79) {
					biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - invalid title length ");
					return;
				}

				/*  Organize channels  */
				uint32_t i;
				for (i = 0; i < hdr->NS; i++) {
					// check if channel with same title already exists
					if (!memcmp(ValLabel[hdr->NS], pos+4, strlen)) {
						keyLabel[k] = hdr->NS;
						break;
					}
				}
				if (i==hdr->NS) {
					// in case of new title, add another channel
					ValLabel[hdr->NS] = pos+4;
					keyLabel[k] = hdr->NS;
					hdr->NS++;
				}

				// start of data section
				hc->bufptr = pos+84;
				pos += 84 + hc->SPR * sizeof(float);

			}
			break;
		case 2:
			for (k = 0; k < nCol; k++) {
				CHANNEL_TYPE *hc = TEMPCHANNEL+k;
				hc->GDFTYP = 3;		// int16
				hc->PhysDimCode = 0;
				hc->SPR    = beu32p(pos);
				if (k==0) {
					hc->Off    = bef32p(pos+84);
					hc->Cal    = bef32p(pos+88);
					hc->bufptr = NULL;
					hdr->SampleRate = 1.0 / hc->Cal;
				}
				else {
					hc->Cal    = bef32p(pos+84);
					hc->Off    = 0.0;
					hc->bufptr = pos + 88;
				}
				int strlen = pos[4];   // string in Pascal format
				if (strlen > 79) {
					biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - invalid title length ");
					return;
				}

				biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - version 2 not supported yet ");
				return;

				// start of data sectioB
				pos += (k==0 ? 92 : 88 + hc->SPR * sizeof(int16_t) );

			}
			break;
		case 6:
			for (k=0; k < nCol; k++) {

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %p %i\n", __FILE__, __LINE__, hdr->CHANNEL, k );

				CHANNEL_TYPE *hc  = TEMPCHANNEL+k;
				hc->SPR           = beu32p(pos);
				uint32_t datatype = beu32p(pos+4);
				size_t titleLen   = beu32p(pos+8);
				char *inbuf       = pos + 12;
				hc->bufptr        = pos + 12 + titleLen;

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %i %i %i\n", __FILE__, __LINE__, (int)datatype, (int)titleLen, (int)hc->SPR);
				/*
				// The only types used for data file columns are...
				//   ShortArrayType = 4     IntArrayType = 5
				//   FloatArrayType = 6     DoubleArrayType = 7
				//   SeriesArrayType = 9    ScaledShortArrayType = 10
				*/
				hc->Cal = 1.0;
				hc->Off = 0.0;
				hc->GDFTYP = datatype;	//TEMPCHANNEL.GDFTYP uses a different encoding than standard GDFTYP
				hc->OnOff = 1;
				switch (datatype) {
				case 4: // int16
				case 5: // int32
				case 6: // float32
				case 7: // double
					break;
				case 9: hc->GDFTYP = 17;  // series
					double firstval  = bef64p(hc->bufptr);
					double increment = bef64p(hc->bufptr+8);
					hc->bufptr = NULL;
					hc->OnOff = 0;
					if (!memcmp(inbuf,"\0T\0i\0m\0e\0 \0(\0s\0)\0",8)) {
						hdr->SampleRate = 1.0/increment;
						hc->OnOff = 2;	// time axis
					}
					else {
						biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "AXG: series data not being a Time axis is not supported. ");
						return;
					}
					break;
				case 10: // scaled short
					hc->Cal = bef64p(hc->bufptr);
					hc->Off = bef64p(hc->bufptr+8);
					break;
				default:
					hc->OnOff = 0;
					biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "error reading AXG: unsupported data type");
					return;
				}

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %p %i\n", __FILE__, __LINE__, hdr->CHANNEL, k );
				if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i): %i %i %i %i\n", __FILE__, __LINE__, (int)hc->SPR, (int)datatype, (int)titleLen, (int)(pos-hdr->AS.Header) );

				/*  Organize channels
					find number of channels and
					setup data structure that assignes each column to a channel
					ValLabel contains the different Labels - one for each channel
					keyLabel contains the channel number for the corresponding column
				*/
				uint32_t i;
				for (i = 0; i < hdr->NS; i++) {
					// check if channel with same title already exists
					uint32_t prevTitleLen = beu32p((uint8_t*)(ValLabel[i])-4);
					if ((titleLen == prevTitleLen) && !memcmp(ValLabel[i], pos+12, titleLen)) {
						keyLabel[k] = i;
						break;
					}
				}
if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %p %i\n", __FILE__, __LINE__, hdr->CHANNEL, k );
				if (i==hdr->NS) {
					// in case of new title, add another channel
					ValLabel[hdr->NS] = pos+12; 	// pointer to title of channel 'nLabel', length of title is stored in beu32p(pos+8)
					keyLabel[k] = hdr->NS;
					hdr->NS++;
				}

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %p %i\n", __FILE__, __LINE__, hdr->CHANNEL, k );
				// pointer to data sections
				hc->bufptr = pos + 12 + titleLen;


				// move pos to the starting position of the next column
				pos += 12 + titleLen;
				// position of next column
				switch (datatype) {
				case 4:
					pos += hc->SPR * sizeof(int16_t);
					break;
				case 5: //int32
				case 6: //float
					pos += hc->SPR * 4;
					break;
				case 7:
					pos += hc->SPR * sizeof(double);
					break;
				case 9:
					pos += 2 * sizeof(double);
					break;
				case 10:
					pos += 2 * sizeof(double) + hc->SPR * sizeof(int16_t);
					break;
				default:
					biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"error reading AXG: unsupported data type");
				}
				if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i): %i %i %i %i\n", __FILE__, __LINE__, (int)hc->SPR, (int)datatype, (int)titleLen, (int)(pos-hdr->AS.Header) );

			}
			break;
		default:
			biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG version is not supported");
		}

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %p %p %i %i\n", __FILE__, __LINE__, TEMPCHANNEL, hdr->CHANNEL, (int)hdr->NS , sizeof(CHANNEL_TYPE));

		/* convert columns/traces into channels */
		hdr->CHANNEL = (CHANNEL_TYPE*)realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
		uint32_t ns;
		for (ns=0; ns < hdr->NS; ns++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL + ns;
			hc->SPR = 0;
			hc->GDFTYP = 0;
		}
		size_t EventN = 0;

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) NS=%i nCol=%i\n", __FILE__, __LINE__, hdr->NS, nCol );

		int flag_traces_of_first_sweep_done=0;
		for (k=0; k < nCol; k++) {
			/*
				copy essential parameters ´(GDFTYP, OnOff, Cal, Off) from TEMPCHANNEL
				and keep track of the number of samples SPR for each channel
			*/

			// define GDFTYP, Cal, Off
			ns = keyLabel[k];	// channel number for current column
			CHANNEL_TYPE *hc = hdr->CHANNEL + ns;

			hc->SPR   += TEMPCHANNEL[k].SPR;

			switch (TEMPCHANNEL[k].GDFTYP) {
			case 4: // int16
				if (hc->GDFTYP < 3) hc->GDFTYP = 3;
				break;
			case 5: // int32
				if (hc->GDFTYP < 5) hc->GDFTYP = 5;
				break;
			case 6: // float32
				if (hc->GDFTYP < 16) hc->GDFTYP = 16;
				break;
			case 7: // double
				if (hc->GDFTYP < 17) hc->GDFTYP = 17;
				break;
			case 10: hc->GDFTYP = 3;  // scaled short
				if (hc->GDFTYP < 16) hc->GDFTYP = 16;
			}

			hc->OnOff  = TEMPCHANNEL[k].OnOff;

			if (!flag_traces_of_first_sweep_done) {
				hc->Cal    = TEMPCHANNEL[k].Cal;
				hc->Off    = TEMPCHANNEL[k].Off;
			}
			else {
				if (hc->Cal != TEMPCHANNEL[k].Cal || hc->Off != TEMPCHANNEL[k].Off) {
					hc->GDFTYP = max(16, hc->GDFTYP);
				}
			}


if (VERBOSE_LEVEL > 8) fprintf(stdout,"%s (line %i) %i %i %f %f\n", __FILE__, __LINE__, (int)k, (int)ns, TEMPCHANNEL[k].Cal, hc->Cal );

			if (ns+1 == hdr->NS) {
				flag_traces_of_first_sweep_done = 1;
				// if current column corresponds to last channel, ...
				uint32_t spr = 0;
				// check if all traces of the same sweep have the same length, and ...
				for (ns=0; ns < hdr->NS; ns++) {
					CHANNEL_TYPE *hc = hdr->CHANNEL + ns;
					if (hc->OnOff != 1) continue;
					if (spr == 0 ) spr = hc->SPR;
					else if (spr != hc->SPR) {
						biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - SPR differs between channel");
						return;
					}
				}

				// add segment break in event table.
				if ( hdr->EVENT.N + 1 >= EventN ) {
					EventN += max(EventN, 16);
					hdr->EVENT.POS = (uint32_t*)realloc(hdr->EVENT.POS, EventN * sizeof(*hdr->EVENT.POS));
					hdr->EVENT.TYP = (uint16_t*)realloc(hdr->EVENT.TYP, EventN * sizeof(*hdr->EVENT.TYP));
				}
				hdr->EVENT.TYP[hdr->EVENT.N] = 0x7ffe;
				hdr->EVENT.POS[hdr->EVENT.N] = hdr->CHANNEL[0].SPR;
				hdr->EVENT.N++;
			}
		}

		hdr->NRec = hdr->CHANNEL[0].SPR;
		hdr->SPR = 1;
		uint32_t bi = 0, bi8=0;
		for (ns=0; ns < hdr->NS; ns++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL+ns;
			hc->SPR = hdr->SPR;
			hc->bi8 = bi8;
			hc->bi  = bi8/8;
			if (hc->OnOff != 1) continue;
			bi8    += GDFTYP_BITS[hc->GDFTYP];
		}
		hdr->AS.bpb = bi;

		for (ns=0; ns < hdr->NS; ns++) {

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) #%i/%i\n", __FILE__, __LINE__, (int)ns,(int)hdr->NS );

			CHANNEL_TYPE *hc = hdr->CHANNEL+ns;

			// define hdr->channel[.].Label, hdr->channel[.].PhysDim
			if (hdr->Version <= 2) {
				// PascalToCString(ValLabel[ns]); 	// shift by 1 byte and terminate 0 char
				int strlen = min(ValLabel[ns][0],MAX_LENGTH_LABEL);
				strncpy(hc->Label, (ValLabel[ns])+1, strlen);

				char *u1 = strrchr(ValLabel[ns],'(');
				char *u2 = strrchr(ValLabel[ns],')');
				if (u1 != NULL && u2 != NULL && u1 < u2) {
					*u1 = 0;
					*u2 = 0;
					hc->PhysDimCode = PhysDimCode(u1+1);
				}
			}
			else if (hdr->Version <= 6) {
				char *inbuf       = ValLabel[ns];
				size_t inlen      = beu32p((uint8_t*)(ValLabel[ns])-4);
				char *outbuf      = hc->Label;
				size_t outlen     = MAX_LENGTH_LABEL+1;
#if  defined(_ICONV_H) || defined (_LIBICONV_H)
				iconv_t ICONV = iconv_open("UTF-8","UCS-2BE");
				size_t reticonv = iconv(ICONV, &inbuf, &inlen, &outbuf, &outlen);
				iconv_close(ICONV);

				if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i): %i %i %i %li\n", __FILE__, __LINE__, (int)hc->SPR, (int)inlen, (int)(pos-hdr->AS.Header), reticonv );

				if (reticonv == (size_t)(-1) ) {
					perror("AXG - conversion of title failed!!!");
					biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - conversion of title failed");
					return;
				}
				*outbuf=0;
#else
				++inbuf;
				int i = min(MAX_LENGTH_LABEL, titleLen/2);
				for (; i>0 ; i-- ) {
					*outbuf= *inbuf;
					inbuf += 2;
					outbuf++;
				}
				outbuf = 0;
#endif
				char *u1 = strrchr(hc->Label,'(');
				char *u2 = strrchr(hc->Label,')');
				if (u1 != NULL && u2 != NULL && u1 < u2) {
					*u1 = 0;
					*u2 = 0;
					hc->PhysDimCode = PhysDimCode(u1+1);
				}
			}

			// these might be reorganized below
			hc->DigMax  =  1e9;
			hc->DigMin  = -1e9;
			hc->PhysMax = hc->DigMax;
			hc->PhysMin = hc->DigMin;

			hc->LeadIdCode = 0;
			hc->Transducer[0] = 0;

			hc->Cal     =  1.0;
			hc->Off     =  0.0;

			hc->TOffset   = 0;
			hc->HighPass  = NAN;
			hc->LowPass   = NAN;
			hc->Notch     = NAN;
			hc->Impedance = INFINITY;
			hc->fZ        = NAN;
			hc->XYZ[0] = 0.0;
			hc->XYZ[1] = 0.0;
			hc->XYZ[2] = 0.0;
		}

		hdr->AS.rawdata = (uint8_t*)realloc( hdr->AS.rawdata, hdr->AS.bpb*hdr->SPR*hdr->NRec);

		for (ns=0; ns < hdr->NS; ns++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL + ns;
			hc->SPR = 0;
		}
		for (k=0; k < nCol; k++) {

			ns = keyLabel[k];
			CHANNEL_TYPE *hc = hdr->CHANNEL + ns;

			if (hc->OnOff != 1) continue;

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i) %i %i\n", __FILE__, __LINE__, hc->GDFTYP, TEMPCHANNEL[k].GDFTYP );

			uint32_t i;
			switch (hc->GDFTYP) {
			case 3:
				assert(TEMPCHANNEL[k].GDFTYP==4);
				for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
					*(int16_t*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = bei16p(TEMPCHANNEL[k].bufptr + i*2);
				}
				break;
			case 5:
				switch (TEMPCHANNEL[k].GDFTYP) {
				case 4:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(int32_t*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = bei16p(TEMPCHANNEL[k].bufptr + i*2);
					}
					break;
				case 5:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(int32_t*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = bei32p(TEMPCHANNEL[k].bufptr + i*4);
					}
					break;
				default: 
					biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - data conversion not supported ");
					return;
				}
				break;
			case 16:
				switch (TEMPCHANNEL[k].GDFTYP) {
				case 4:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(float*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = (float)bei16p(TEMPCHANNEL[k].bufptr + i*2);
					}
					break;
				case 5:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(float*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = (float)bei32p(TEMPCHANNEL[k].bufptr + i*4);
					}
					break;
				case 6:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(float*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = bef32p(TEMPCHANNEL[k].bufptr + i*4);
					}
					break;
				case 10:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(float*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = bei16p(TEMPCHANNEL[k].bufptr + i*2) * TEMPCHANNEL[k].Cal + TEMPCHANNEL[k].Off;
					}
					break;
				default: 
					biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - data conversion not supported ");
					return;
				}
				break;
			case 17:
				switch (TEMPCHANNEL[k].GDFTYP) {
				case 4:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(double*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = (double)bei16p(TEMPCHANNEL[k].bufptr + i*2);
					}
					break;
				case 5:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(double*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = (double)bei32p(TEMPCHANNEL[k].bufptr + i*4);
					}
					break;
				case 6:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(double*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = (double)bef32p(TEMPCHANNEL[k].bufptr + i*4);
					}
					break;
				case 7:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(double*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = bef64p(TEMPCHANNEL[k].bufptr + i*8);
					}
					break;
				case 10:
					for (i=0; i < TEMPCHANNEL[k].SPR; i++) {
						*(double*)(hdr->AS.rawdata + hc->bi + (hc->SPR + i) * hdr->AS.bpb) = bei16p(TEMPCHANNEL[k].bufptr + i*2) * TEMPCHANNEL[k].Cal + TEMPCHANNEL[k].Off;
					}
					break;
				default: 
					biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - data conversion not supported ");
					return;
				}
				break;
			default: 
				biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - unsupported target data type");
				return;
			}
			hc->SPR += TEMPCHANNEL[k].SPR;
		}

		for (ns=0; ns < hdr->NS; ns++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL + ns;
			hc->SPR = hdr->SPR;
		}
		// free intermediate data structure to reorganized column/trace to channels
		if(TEMPCHANNEL) free(TEMPCHANNEL);
		if(keyLabel) free(keyLabel);
		if(ValLabel) free(ValLabel);

		// data is stored on hdr->AS.rawdata in such a way that swapping must not be applied
		hdr->FILE.LittleEndian = (__BYTE_ORDER == __LITTLE_ENDIAN);
		hdr->AS.first  = 0;
		hdr->AS.length = (size_t)hdr->NRec;


if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i)\n", __FILE__, __LINE__ );


		// read Comments
		size_t szComments = beu32p(pos);
		char  *inbuf       = pos+4;
		char  *Comments    = malloc(szComments+1);
		char  *outbuf      = Comments;
		size_t outlen     = szComments+1;
		size_t inlen      = szComments;

		iconv_t ICONV = iconv_open("UTF-8","UCS-2BE");
		size_t reticonv = iconv(ICONV, &inbuf, &inlen, &outbuf, &outlen);
		iconv_close(ICONV);
		if (reticonv == (size_t)(-1) ) {
			perror("AXG - conversion of comments failed!!!");
			biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - conversion of comments failed");
			return;
		}
		Comments[outlen]=0;

		if (VERBOSE_LEVEL >7)
			fprintf(stdout,"\n=== COMMENT === \n %s\n",Comments);
		pos += 4+szComments;


		// read Notes
		size_t szNotes  = beu32p(pos);
		inbuf           = pos+4;
		hdr->AS.fpulse  = malloc(szNotes+1);
		outbuf = hdr->AS.fpulse;
		outlen = szNotes+1;
		inlen  = szNotes;

		ICONV    = iconv_open("UTF-8","UCS-2BE");
		reticonv = iconv(ICONV, &inbuf, &inlen, &outbuf, &outlen);
		iconv_close(ICONV);
		if ( reticonv == (size_t)(-1) ) {
			biosigERROR(hdr,B4C_FORMAT_UNSUPPORTED,"AXG - conversion of Notes failed");
			return;
		}
		hdr->AS.fpulse[outlen]=0;

		if (VERBOSE_LEVEL >7)
			fprintf(stdout,"=== NOTES === \n %s\n",hdr->AS.fpulse);
		pos += 4+szNotes;

		free(Comments);

if (VERBOSE_LEVEL > 7) fprintf(stdout,"%s (line %i)\n", __FILE__, __LINE__ );


}


int sopen_fiff_read(HDRTYPE* hdr) {
	/* TODO: implement FIFF support
	        define all fields in hdr->....
		currently only the first hdr->HeadLen bytes are stored in
		hdr->AS.Header, all other fields still need to be defined.
	*/
	size_t k;

	/* define basic header */
	hdr->NS = 0;
	// hdr->.... fill in all

	/* define channel headers */
	hdr->CHANNEL = (CHANNEL_TYPE*) realloc(hdr->CHANNEL, hdr->NS * sizeof(CHANNEL_TYPE));
	for (k = 0; k < hdr->NS; k++) {
		CHANNEL_TYPE *hc = hdr->CHANNEL + k;
	}

	/* define event table */
	hdr->EVENT.N = reallocEventTable(hdr, 0);

	/* report status header and return */
	hdr2ascii(hdr,stdout,4);
	biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "FIFF support not completed");
	return 0;
}


int sopen_unipro_read(HDRTYPE* hdr) {
		hdr->FILE.LittleEndian = (__BYTE_ORDER == __LITTLE_ENDIAN);
		char *Header1 = (char*)hdr->AS.Header;
		struct tm t0;
		char tmp[5];
		memset(tmp,0,5);
		strncpy(tmp,Header1+0x9c,2);
		t0.tm_mon = atoi(tmp)-1;
		strncpy(tmp,Header1+0x9e,2);
		t0.tm_mday = atoi(tmp);
		strncpy(tmp,Header1+0xa1,2);
		t0.tm_hour = atoi(tmp);
		strncpy(tmp,Header1+0xa3,2);
		t0.tm_min = atoi(tmp);
		strncpy(tmp,Header1+0xa5,2);
		t0.tm_sec = atoi(tmp);
		strncpy(tmp,Header1+0x98,4);
		t0.tm_year = atoi(tmp)-1900;
		hdr->T0 = tm_time2gdf_time(&t0);

		memset(tmp,0,5);
		strncpy(tmp,Header1+0x85,2);
		t0.tm_mday = atoi(tmp);
		strncpy(tmp,Header1+0x83,2);
		t0.tm_mon = atoi(tmp)-1;
		strncpy(tmp,Header1+0x7f,4);
		t0.tm_year = atoi(tmp)-1900;
		hdr->Patient.Birthday = tm_time2gdf_time(&t0);

		// filesize = leu32p(hdr->AS.Header + 0x24);
		biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "UNIPRO not supported");
		return(0);
}


#ifdef WITH_DICOM
int sopen_dicom_read(HDRTYPE* hdr) {

		fprintf(stdout,"home-made parser is used to read dicom files.\n");

		char FLAG_implicite_VR = 0;
		int EndOfGroup2=-1;

		if (hdr->HeadLen<132) {
			hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, 132);
		    	hdr->HeadLen += ifread(hdr->AS.Header+hdr->HeadLen, 1, 132-hdr->HeadLen, hdr);
		}
		size_t count = hdr->HeadLen;
		size_t pos = 128;
		while (!hdr->AS.Header[pos] && (pos<128)) pos++;
		if ((pos==128) && !memcmp(hdr->AS.Header+128,"DICM",4)) {
//			FLAG_implicite_VR = 0;
			pos = 132;
		}
		else
			pos = 0;

		size_t bufsiz = 16384;
		while (!ifeof(hdr)) {
			hdr->AS.Header = (uint8_t*)realloc(hdr->AS.Header, count+bufsiz+1);
		    	count += ifread(hdr->AS.Header+count, 1, bufsiz, hdr);
		    	bufsiz *= 2;
		}
	    	ifclose(hdr);
	    	hdr->AS.Header[count] = 0;

		uint16_t nextTag[2];

		struct tm T0;
		char flag_t0=0;
		char flag_ignored;
		uint32_t Tag;
		uint32_t Len;
		nextTag[0] = *(uint16_t*)(hdr->AS.Header+pos);
		nextTag[1] = *(uint16_t*)(hdr->AS.Header+pos+2);
		while (pos < count) {

			if ((__BYTE_ORDER == __BIG_ENDIAN) ^ !hdr->FILE.LittleEndian) {
				// swapping required
				Tag  = (((uint32_t)bswap_16(nextTag[0])) << 16) + bswap_16(nextTag[1]);
				pos += 4;
				if (FLAG_implicite_VR) {
					Len = bswap_32(*(uint32_t*)(hdr->AS.Header+pos));
					pos += 4;
				}
				else {
					// explicite_VR
					if (pos+4 > count) break;

					if (memcmp(hdr->AS.Header+pos,"OB",2)
					 && memcmp(hdr->AS.Header+pos,"OW",2)
					 && memcmp(hdr->AS.Header+pos,"OF",2)
					 && memcmp(hdr->AS.Header+pos,"SQ",2)
					 && memcmp(hdr->AS.Header+pos,"UT",2)
					 && memcmp(hdr->AS.Header+pos,"UN",2) ) {
						Len = bswap_16(*(uint16_t*)(hdr->AS.Header+pos+2));
						pos += 4;
					}
					else {
						Len = bswap_32(*(uint32_t*)(hdr->AS.Header+pos+4));
						pos += 8;
					}
				}
			}
			else {
				// no swapping
				Tag  = (((uint32_t)nextTag[0]) << 16) + nextTag[1];
				pos += 4;
				if (FLAG_implicite_VR) {
					Len = *(uint32_t*)(hdr->AS.Header+pos);
					pos += 4;
				}
				else {
					// explicite_VR
					if (pos+4 > count) break;

					if (memcmp(hdr->AS.Header+pos,"OB",2)
					 && memcmp(hdr->AS.Header+pos,"OW",2)
					 && memcmp(hdr->AS.Header+pos,"OF",2)
					 && memcmp(hdr->AS.Header+pos,"SQ",2)
					 && memcmp(hdr->AS.Header+pos,"UT",2)
					 && memcmp(hdr->AS.Header+pos,"UN",2) ) {
						Len = *(uint16_t*)(hdr->AS.Header+pos+2);
						pos += 4;
					}
					else {
						Len = *(uint32_t*)(hdr->AS.Header+pos+4);
						pos += 8;
					}
				}
			}

			/*
			    backup next tag, this allows setting of terminating 0
			*/
			if (pos+Len < count) {
				nextTag[0] = *(uint16_t*)(hdr->AS.Header+pos+Len);
				nextTag[1] = *(uint16_t*)(hdr->AS.Header+pos+Len+2);
				hdr->AS.Header[pos+Len] = 0;
	    		}


			flag_ignored = 0;
			if (VERBOSE_LEVEL>8)
				fprintf(stdout,"        %6x:   (%04x,%04x) %8d\t%s\n",pos,Tag>>16,Tag&0x0ffff,Len,(char*)hdr->AS.Header+pos);

			switch (Tag) {


			/* elements of group 0x0002 use always
				Explicite VR little Endian encoding
			*/
			case 0x00020000: {
				int c = 0;
				if (!memcmp(hdr->AS.Header+pos-8,"UL",2))
					c = leu32p(hdr->AS.Header+pos);
				else if (!memcmp(hdr->AS.Header+pos-8,"SL",2))
					c = lei32p(hdr->AS.Header+pos);
				else if (!memcmp(hdr->AS.Header+pos-8,"US",2))
					c = leu16p(hdr->AS.Header+pos);
				else if (!memcmp(hdr->AS.Header+pos-8,"SS",2))
					c = lei16p(hdr->AS.Header+pos);
				else  {
					biosigERROR(hdr, B4C_FORMAT_UNSUPPORTED, "DICOM (0002,0000): unsupported");
				}
				EndOfGroup2 = c + pos;
				break;
				}
			case 0x00020001:
				break;

			case 0x00020002: {
				hdr->NS = 1;
				char *t = (char*)hdr->AS.Header+pos;
				while (isspace(*t)) t++;	// deblank
				char *ct[] = {	"1.2.840.10008.5.1.4.1.1.9.1.1",
						"1.2.840.10008.5.1.4.1.1.9.1.2",
						"1.2.840.10008.5.1.4.1.1.9.1.3",
						"1.2.840.10008.5.1.4.1.1.9.2.1",
						"1.2.840.10008.5.1.4.1.1.9.3.1",
						"1.2.840.10008.5.1.4.1.1.9.4.1"
						};
				if (!strcmp(t,*ct)) hdr->NS = 12;
				break;
				}

			case 0x00020003:
				break;

			case 0x00020010: {
				char *t = (char*)hdr->AS.Header+pos;
				while (isspace(*t)) t++;	// deblank
				char *ct[] = {	"1.2.840.10008.1.2",
						"1.2.840.10008.1.2.1",
						"1.2.840.10008.1.2.1.99",
						"1.2.840.10008.1.2.2"
						};

				if      (!strcmp(t,*ct))   FLAG_implicite_VR = 1;
				else if (!strcmp(t,*ct+1)) FLAG_implicite_VR = 0;
				else if (!strcmp(t,*ct+3)) FLAG_implicite_VR = 1;
				break;
				}

			case 0x00080020:  // StudyDate
			case 0x00080023:  // ContentDate
				{
				hdr->AS.Header[pos+8]=0;
				T0.tm_mday = atoi((char*)hdr->AS.Header+pos+6);
				hdr->AS.Header[pos+6]=0;
				T0.tm_mon = atoi((char*)hdr->AS.Header+pos+4)-1;
				hdr->AS.Header[pos+4]=0;
				T0.tm_year = atoi((char*)hdr->AS.Header+pos)-1900;
				flag_t0 |= 1;
				break;
				}
			case 0x0008002a:  // AcquisitionDateTime
				{
				struct tm t0;
				hdr->AS.Header[pos+14]=0;
				t0.tm_sec = atoi((char*)hdr->AS.Header+pos+12);
				hdr->AS.Header[pos+12]=0;
				t0.tm_min = atoi((char*)hdr->AS.Header+pos+10);
				hdr->AS.Header[pos+10]=0;
				t0.tm_hour = atoi((char*)hdr->AS.Header+pos+8);
				hdr->AS.Header[pos+8]=0;
				t0.tm_mday = atoi((char*)hdr->AS.Header+pos+6);
				hdr->AS.Header[pos+6]=0;
				t0.tm_mon = atoi((char*)hdr->AS.Header+pos+4)-1;
				hdr->AS.Header[pos+4]=0;
				t0.tm_year = atoi((char*)hdr->AS.Header+pos)-1900;

				hdr->T0 = tm_time2gdf_time(&t0);
				break;
				}
			case 0x00080030:  // StudyTime
			case 0x00080033:  // ContentTime
				{
				hdr->AS.Header[pos+6]=0;
				T0.tm_sec = atoi((char*)hdr->AS.Header+pos+4);
				hdr->AS.Header[pos+4]=0;
				T0.tm_min = atoi((char*)hdr->AS.Header+pos+2);
				hdr->AS.Header[pos+2]=0;
				T0.tm_hour = atoi((char*)hdr->AS.Header+pos);
				flag_t0 |= 2;
				break;
				}
			case 0x00080070:  // Manufacturer
				{
				strncpy(hdr->ID.Manufacturer._field,(char*)hdr->AS.Header+pos,MAX_LENGTH_MANUF);
				hdr->ID.Manufacturer.Name = hdr->ID.Manufacturer._field;
				break;
				}
			case 0x00081050:  // Performing Physician
				{
				strncpy(hdr->ID.Technician,(char*)hdr->AS.Header+pos,MAX_LENGTH_TECHNICIAN);
				break;
				}
			case 0x00081090: // Manufacturer Model
				{
				const size_t nn = strlen(hdr->ID.Manufacturer.Name)+1;
				strncpy(hdr->ID.Manufacturer._field+nn,(char*)hdr->AS.Header+pos,MAX_LENGTH_MANUF-nn-1);
				hdr->ID.Manufacturer.Model = hdr->ID.Manufacturer._field+nn;
				break;
				}

			case 0x00100010: // Name
				if (!hdr->FLAG.ANONYMOUS) {
					strncpy(hdr->Patient.Name,(char*)hdr->AS.Header+pos,MAX_LENGTH_NAME);
					hdr->Patient.Name[MAX_LENGTH_NAME]=0;
				}
				break;
			case 0x00100020: // ID
				strncpy(hdr->Patient.Id,(char*)hdr->AS.Header+pos,MAX_LENGTH_NAME);
				hdr->Patient.Id[MAX_LENGTH_PID]=0;
				break;

			case 0x00100030: // Birthday
				{
				struct tm t0;
				t0.tm_sec = 0;
				t0.tm_min = 0;
				t0.tm_hour = 12;

				hdr->AS.Header[pos+8]=0;
				t0.tm_mday = atoi((char*)hdr->AS.Header+pos+6);
				hdr->AS.Header[pos+6]=0;
				t0.tm_mon = atoi((char*)hdr->AS.Header+pos+4)-1;
				hdr->AS.Header[pos+4]=0;
				t0.tm_year = atoi((char*)hdr->AS.Header+pos)-1900;

				hdr->Patient.Birthday = tm_time2gdf_time(&t0);
				break;
				}
			case 0x00100040:
				hdr->Patient.Sex = (toupper(hdr->AS.Header[pos])=='M') + 2*(toupper(hdr->AS.Header[pos])=='F');
				break;

			case 0x00101010: //Age
				break;
			case 0x00101020:
				hdr->Patient.Height = (uint8_t)(atof((char*)hdr->AS.Header+pos)*100.0);
				break;
			case 0x00101030:
				hdr->Patient.Weight = (uint8_t)atoi((char*)hdr->AS.Header+pos);
				break;

			default:
				flag_ignored = 1;
				if (VERBOSE_LEVEL<7)
					fprintf(stdout,"ignored %6x:   (%04x,%04x) %8d\t%s\n",pos,Tag>>16,Tag&0x0ffff,Len,(char*)hdr->AS.Header+pos);

			}

			if (VERBOSE_LEVEL>6) {
			if (!FLAG_implicite_VR || (Tag < 0x00030000))
				fprintf(stdout,"%s %6x:   (%04x,%04x) %8d %c%c \t%s\n",(flag_ignored?"ignored":"       "),pos,Tag>>16,Tag&0x0ffff,Len,hdr->AS.Header[pos-8],hdr->AS.Header[pos-7],(char*)hdr->AS.Header+pos);
			else
				fprintf(stdout,"%s %6x:   (%04x,%04x) %8d\t%s\n",(flag_ignored?"ignored":"       "),pos,Tag>>16,Tag&0x0ffff,Len,(char*)hdr->AS.Header+pos);
			}
			pos += Len + (Len & 0x01 ? 1 : 0); // even number of bytes

		}
		if (flag_t0 == 3) hdr->T0 = tm_time2gdf_time(&T0);
	return(0);
}
#endif


#ifdef WITH_PDP
#include "../NONFREE/sopen_pdp_read.c"
#endif

#ifdef WITH_TRC
#include "../NONFREE/sopen_trc_read.c"
#endif

#ifdef __cplusplus
}
#endif

