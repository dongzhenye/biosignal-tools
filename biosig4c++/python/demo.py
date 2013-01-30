####### Demo for Python interface to BioSig" #####################
###
###  $Id$
###  Copyright (C) 2009 Alois Schloegl <a.schloegl@ieee.org>
###  This file is part of the "BioSig for C/C++" repository 
###  (biosig4c++) at http://biosig.sf.net/ 
###
##############################################################

# download and extract 
#   http://www.biosemi.com/download/BDFtestfiles.zip 
# into /tmp/
# then run this demo 
#
# on linux you can run instead  
#   make test 

import biosig
import numpy as S
import ctypes
import datetime

filename = '/scratch/schloegl/R/data/test/CFS/example_6channels.dat'
filename = '/home/as/data/test/CFS/example_6channels.dat'

filename = '/home/as/data/test/BDF/sample_bdf_plus_file.bdf'
filename = '/fs/home/schloegl/data/test/gdf/test.clemens.20121220.gdf'

print 'open file ',filename

HDR = biosig.constructHDR(0, 0)
HDR = biosig.sopen(filename , 'r', HDR)

HDR.Patient.Id
HDR.Patient.Sex
HDR.Patient.Handedness
HDR.Patient.DrugAbuse
HDR.Patient.Smoking

### convert from gdftime/matlab/octave datetime format to python datetime format
###   for some unknown reason, the starting point is 366 days off.
d = datetime.timedelta(days=HDR.T0*(2**-32)-366)
m, s = divmod(d.seconds, 60)
h, m = divmod(m, 60)
starttime = datetime.datetime.combine(datetime.date.fromordinal(d.days), datetime.time(h,m,s))
print "starting time:",starttime


status = biosig.serror2(HDR)	# save and reset error status
if status:
    print 'Can not open file ',filename
else: 
    ### show header information 
    #biosig.hdr2ascii(HDR,3)  

    ### Extract event table 
    if HDR.EVENT.TYP: TYP = ctypes.cast( HDR.EVENT.TYP.__long__(), ctypes.POINTER( ctypes.c_uint16 ) )
    if HDR.EVENT.CHN: CHN = ctypes.cast( HDR.EVENT.CHN.__long__(), ctypes.POINTER( ctypes.c_uint16 ) )
    if HDR.EVENT.POS: POS = ctypes.cast( HDR.EVENT.POS.__long__(), ctypes.POINTER( ctypes.c_uint32 ) )
    if HDR.EVENT.DUR: DUR = ctypes.cast( HDR.EVENT.DUR.__long__(), ctypes.POINTER( ctypes.c_uint32 ) )

    ### show extracted events
    #for k in range(HDR.EVENT.N): print k,TYP[k],POS[k],POS[k]/HDR.EVENT.SampleRate #,HDR.EVENT.CodeDesc[TYP[k]]

    for k in range(HDR.NS):	
	# convert C to Python string: get rid of everything after \x00, then remove leading and trailing whitespace
	str = HDR.CHANNEL[k].Label
        HDR.CHANNEL[k].Label      = str[0:str.find(chr(0))].strip()
	str = HDR.CHANNEL[k].Transducer
    	HDR.CHANNEL[k].Transducer = str[0:str.find(chr(0))].strip()
	print k,HDR.CHANNEL[k].OnOff==0,'<',HDR.CHANNEL[k].Label,'>,<',HDR.CHANNEL[k].Transducer,'>'
    #	turn off all channels 
    #    for i in range(HDR.NS):
    #        HDR.CHANNEL[i].OnOff = 0
    #
    #	turn on specific channels 
    #    HDR.CHANNEL[0].OnOff = 1
    #    HDR.CHANNEL[1].OnOff = 1
    #    HDR.CHANNEL[HDR.NS-1].OnOff = 1
    #
    # read data 
    data = biosig.sread(0, HDR.NRec, HDR)
    print data.shape
    #
    # close file
    biosig.sclose(HDR)
    #
    # release allocated memory
    biosig.destructHDR(HDR)
    #    
    #return data

