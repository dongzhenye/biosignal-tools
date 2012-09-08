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

filename = '/scratch/schloegl/R/data/test/CFS/example_6channels.dat'
filename = '/home/as/data/test/CFS/example_6channels.dat'
filename = '/home/as/data/test/BDF/sample_bdf_plus_file.bdf'

print 'open file ',filename

HDR = biosig.constructHDR(0, 0)
HDR = biosig.sopen(filename , 'r', HDR)

status = biosig.serror()	# save and reset error status
if status:
    print 'Can not open file ',filename
else: 
    ### show header information 
    biosig.hdr2ascii(HDR,3)  

    ### Extract event table 
    if HDR.EVENT.TYP: TYP = ctypes.cast( HDR.EVENT.TYP.__long__(), ctypes.POINTER( ctypes.c_uint16 ) )
    if HDR.EVENT.CHN: CHN = ctypes.cast( HDR.EVENT.CHN.__long__(), ctypes.POINTER( ctypes.c_uint16 ) )
    if HDR.EVENT.POS: POS = ctypes.cast( HDR.EVENT.POS.__long__(), ctypes.POINTER( ctypes.c_uint32 ) )
    if HDR.EVENT.DUR: DUR = ctypes.cast( HDR.EVENT.DUR.__long__(), ctypes.POINTER( ctypes.c_uint32 ) )
     	
    ### show extracted events
    for k in range(HDR.EVENT.N): print k,TYP[k],POS[k],POS[k]/HDR.EVENT.SampleRate #,HDR.EVENT.CodeDesc[TYP[k]]

    for k in range(HDR.NS):	
	# convert C to Python string: get rid of everything after \x00, then remove leading and trailing whitespace
	str = HDR.CHANNEL[k].Label
        HDR.CHANNEL[k].Label      = str[0:str.find(chr(0))].strip()
	str = HDR.CHANNEL[k].Transducer
    	HDR.CHANNEL[k].Transducer = str[0:str.find(chr(0))].strip()
	print k,'<',HDR.CHANNEL[k].Label,'>,<',HDR.CHANNEL[k].Transducer,'>'
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
    #
    # close file
    biosig.sclose(HDR)
    #
    # release allocated memory
    biosig.destructHDR(HDR)
    #    
    #return data

