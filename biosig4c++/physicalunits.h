/*
% $Id: biosig.h,v 1.140 2009/04/09 15:06:55 schloegl Exp $
% Copyright (C) 2005,2006,2007,2008,2009,2010,2011,2012 Alois Schloegl <alois.schloegl@gmail.com>
% This file is part of the "BioSig for C/C++" repository
% (biosig4c++) at http://biosig.sf.net/


    This program is free software; you can redistribute it and/or
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


/* test whether HDR.CHANNEL[].{bi,bi8} can be replaced, reduction of header size by about 3%
   currently this is not working, because FAMOS seems to need it.
//#define NO_BI
*/


/* External API definitions */

/****************************************************************************/
/**                                                                        **/
/**                 DEFINITIONS, TYPEDEFS AND MACROS                       **/
/**                                                                        **/
/****************************************************************************/
#ifndef __PHYSICALUNITS_H__
#define __PHYSICALUNITS_H__

#ifdef _VCPP_DEF
typedef unsigned short		uint16_t;
#else
    #include <stdint.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

uint16_t PhysDimCode(const char* PhysDim);
/* Encodes  Physical Dimension as 16bit integer according to
   ISO/IEEE 11073-10101:2004 Vital Signs Units of Measurement
 --------------------------------------------------------------- */

const char* PhysDim3(uint16_t PhysDimCode);
/* converts HDR.CHANNEL[k].PhysDimCode into a readable Physical Dimension
 --------------------------------------------------------------- */

double PhysDimScale(uint16_t PhysDimCode);
/* returns scaling factor of physical dimension
	e.g. 0.001 for milli, 1000 for kilo etc.
 --------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif


#endif	/* __PHYSICALUNITS_H__ */
