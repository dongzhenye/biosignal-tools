/*

    $Id$
    Copyright (C) 2005-2012 Alois Schloegl <alois.schloegl@gmail.com>
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
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "physicalunits.h"

/* physical units are defined in
 prEN ISO 11073-10101 (Nov 2003)
 Health Informatics - Point-of-care medical device communications - Part 10101:Nomenclature
 (ISO/DIS 11073-10101:2003)
 Table A.6.1: Table of Decimal Factors

 CEN/TC251/PT40 2001
 File Exchange Format for Vital Signs - Annex A
 Table A.4.1: Table of Decimal Factors	const double scale[32] =
*/

const struct PhysDimIdx
	{
		const uint16_t	idx;
		const char*	PhysDimDesc;
	} _physdim[] = {
#include "units.i"
	{0xffff,  "end-of-table" },
} ;

/*
	compare strings, accept bit7=1
 */
int strcmp8(const char* str1, const char* str2)
{
	unsigned int k=0;
	int r;
	r = str1[k] - str2[k];
	while (r==0 && str1[k]!='\0' && str2[k]!='\0') {
		k++;
		r = str1[k] - str2[k];
	}
	return(r);
}

const char* PhysDimFactor[] = {
	"","da","h","k","M","G","T","P",	//  0..7
	"E","Z","Y","#","#","#","#","#",	//  8..15
	"d","c","m","u","n","p","f","a",	// 16..23
	"z","y","#","#","#","#","#","#",	// 24..31
	"\xB5"	//hack for "µ" = "u"		// 32
	};

double PhysDimScale(uint16_t PhysDimCode)
{
// converting PhysDimCode -> scaling factor

	const double scale[] =
	{ 1e+0, 1e+1, 1e+2, 1e+3, 1e+6, 1e+9,  1e+12, 1e+15,	//  0..7
	  1e+18,1e+21,1e+24,NAN,  NAN,  NAN,   NAN,   NAN, 	//  8..15
	  1e-1, 1e-2, 1e-3, 1e-6, 1e-9, 1e-12, 1e-15, 1e-18, 	// 16..23
	  1e-21,1e-24,NAN,  NAN,  NAN,  NAN,   NAN,   NAN,	// 24..31
	  1e-6	// hack for "µ" = "u" 				// 32
	  };

	return (scale[PhysDimCode & 0x001f]);
}

char* PhysDim2(uint16_t PhysDimCode)
{
	// converting PhysDimCode -> PhysDim
	uint16_t k  = 0;
	uint16_t l2 = strlen(PhysDimFactor[PhysDimCode & 0x001F]);	

	for (k = 0; _physdim[k].idx < 0xffff; k++)
	if  ( (PhysDimCode & ~0x001F) == _physdim[k].idx) {
		char *PhysDim = (char*)malloc(l2 + 1 + strlen(_physdim[k].PhysDimDesc));
		memcpy(PhysDim, PhysDimFactor[PhysDimCode & 0x001F], l2);
		strcpy(PhysDim+l2, _physdim[k].PhysDimDesc);
		return(PhysDim);
	}
	return(NULL);
}

uint16_t PhysDimCode(const char* PhysDim0)
{
// converting PhysDim -> PhysDimCode
	/* converts Physical dimension into 16 bit code */
	uint16_t k1, k2;
	char s[80];
	char *s1;

	if (PhysDim0==NULL) return(0);
	if (strlen(PhysDim0)==0) return(0);

	// greedy search - check all codes 0..65535
	for (k1=0; k1<33; k1++)
	if (strncmp(PhysDimFactor[k1],PhysDim0,strlen(PhysDimFactor[k1]))==0 && (PhysDimScale(k1)>0.0))
	{ 	// exclude if beginning of PhysDim0 differs from PhysDimFactor and if NAN
		strncpy(s, PhysDimFactor[k1],3);
		s1 = s+strlen(s);
		for (k2=0; _physdim[k2].idx < 0xffff; k2++) {
			strncpy(s1, _physdim[k2].PhysDimDesc, 77);
			if (strcmp8(PhysDim0, s)==0) {
		 		if (k1==32) k1 = 19;		// hack for "Âµ" = "u"
				return(_physdim[k2].idx+k1);
			}
		}
	}
	return(0);
}

/*------------------------------------------------------------------------
 *	Table of Physical Units
 * 
 * This part can be better optimized with a more sophisticated hash table 
 *
 * These functions are thread safe except for the call to PhysDim2 which updates
   the table (it does a malloc). Everything else is just read operation, and 
   the content is defined only by PhysDimFactor and PhysDimIdx, which are constant. 
   Thus the 	
 *------------------------------------------------------------------------*/

static char *PhysDimTable[0x10000];
static char FlagInit_PhysDimTable = 0; 

void ClearPhysDimTable() {
	unsigned k = 0;
	while (k < 0x10000) {
		char *o = PhysDimTable[k++];
		if (o != NULL) free(o); 
	}
	FlagInit_PhysDimTable = 0; 
}

const char* PhysDim(uint16_t PhysDimCode) {
	if (!FlagInit_PhysDimTable) {
		memset(PhysDimTable, 0, 0x10000 * sizeof(char*));
		atexit(&ClearPhysDimTable);
		FlagInit_PhysDimTable = 1; 
	}

	// mutex_lock();
	char **o = PhysDimTable+PhysDimCode; 
	if (*o==NULL) *o = PhysDim2(PhysDimCode);
	// mutex_unlock();
	return( (const char*) *o);
}



/****************************************************************************/
/**                                                                        **/
/**                               EOF                                      **/
/**                                                                        **/
/****************************************************************************/

