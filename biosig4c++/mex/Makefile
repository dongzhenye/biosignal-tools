####### Makefile for "BioSig for C/C++" #####################
###
###  Copyright (C) 2006-2013 Alois Schloegl <alois.schloegl@ist.ac.at>
###  This file is part of the "BioSig for C/C++" repository
###  (biosig4c++) at http://biosig.sf.net/
###
##############################################################


### modify directories according to your needs 
INCOCTAVE = -I /usr/include/octave 
MATLABDIR = /usr/local/MATLAB/R2011b/
# comment the following line if you use MATLAB on 32-bit operating system
MEX_OPTION += -largeArrayDims
# LIBEXT. Setting it to 'a' links statically, 'so' links dynamically
LIBEXT        = a
#LIBEXT	      = so
# 
# include directory for Win32-Matlab include
W32MAT_INC = $(HOME)/bin/win32/Matlab/R2010b/extern/include/ -I../win32
W64MAT_INC = $(HOME)/bin/win64/Matlab/R2010b/extern/include/ -I../win64
# path to GNUMEX libraries, available from here http://sourceforge.net/projects/gnumex/
GNUMEX   = $(HOME)/bin/win32/gnumex
GNUMEX64 = $(HOME)/bin/win64/gnumex
###############################


### User-specified options: its likely you want to change this
MEX_OPTION    = -largeArrayDims # turn on for 64 bit Matlab, otherwise empty
LIBS          = $(LDLIBS)
LFLAGS        = 
COPY          = cp

ifeq ($(LIBEXT),so)
  LIBS        += -L.. -lbiosig
else
  LIBS        += ../libbiosig.$(LIBEXT)  -lz -lcholmod
endif


win32 win mexw32: mexSLOAD.mexw32 mexSOPEN.mexw32 mexSSAVE.mexw32
win64 mexw64: mexSLOAD.mexw64 mexSOPEN.mexw64 mexSSAVE.mexw64
all: mex4m mex4o win32 win64
mexSOPEN.cpp : mexSLOAD.cpp
	echo "#define mexSOPEN" > mexSOPEN.cpp
	cat mexSLOAD.cpp >> mexSOPEN.cpp

clean:
	-$(RM) *.o *.obj *.o64 core octave-core *.oct *~ *.mex* 

##########################################################
## set Matlab and Octave variables
ifndef MATLABDIR
ifneq ($(shell which matlab), )
  MATLABDIR  := $(dir $(shell readlink -f $(shell which matlab) ) )
endif
endif

ifndef MATLABDIR
mex: mex4o
else
mex: mex4m mex4o
  MEX        = $(MATLABDIR)bin/mex
  MEX_EXT   := $(shell $(MATLABDIR)bin/mexext)
endif

# Octave - global install  (e.g. from debian package)
# - defined as variable by calling function 
# OCTAVE_VERSION 	= 

OCT           	= mkoctfile$(OCTAVE_VERSION)
##########################################################

#../libbiosig.$(LIBEXT):
#	$(MAKE) -C .. libbiosig.$(LIBEXT)

#../win32/libbiosig.a:
#	$(MAKE) -C .. win32/libbiosig.a

#../win64/libbiosig.a:
#	$(MAKE) -C .. win64/libbiosig.a

#############################################################
#	MEX-Interface for Octave and Matlab
#############################################################
ifdef MEX_EXT
mex4m: mexSLOAD.$(MEX_EXT) mexSOPEN.$(MEX_EXT) mexSSAVE.$(MEX_EXT)
mexSLOAD: mexSLOAD.$(MEX_EXT) 
mexSOPEN: mexSOPEN.$(MEX_EXT)
%.$(MEX_EXT): %.cpp Makefile ../libbiosig.$(LIBEXT) Makefile
	$(MEX) $(MEX_OPTION) $(DEFINES) "$<" $(LFLAGS) $(LIBS)
	-$(COPY) $@ ../../biosig4matlab/t200_FileAccess/
endif


### MEX-files for Octave
mex4o:  mexSLOAD.mex mexSOPEN.mex mexSSAVE.mex
%.mex:  %.cpp ../libbiosig.$(LIBEXT) Makefile
	$(OCT)  $(DEFINES) -v -g --mex "$<" $(LIBS)
	-$(COPY) $@ ../../biosig4matlab/t200_FileAccess/
#physicalunits.mex:  physicalunits.cpp libbiosig.$(LIBEXT)
#	$(OCT) -v -g --mex physicalunits.cpp $(LFLAGS) $(LIBS)

oct:  mexSLOAD.oct mexSOPEN.oct mexSSAVE.oct
%.oct:  %.cpp ../libbiosig.$(LIBEXT) Makefile
	$(OCT)  $(DEFINES) "$<" $(LIBS)
	-$(COPY) "$@" ../../biosig4matlab/t200_FileAccess/


#########################################################
#	MATLAB/WIN32
#########################################################
%.obj: %.cpp
	$(CROSS)-$(CXX) -c -DMATLAB_MEX_FILE $(DEFINES) -x c++ -o "$@" -I$(W32MAT_INC) -O2 -DMX_COMPAT_32 "$<" 
%.obj: %.c
	$(CROSS)-$(CXX) -c -DMATLAB_MEX_FILE $(DEFINES) -x c++ -o "$@" -I$(W32MAT_INC) -O2 -DMX_COMPAT_32 "$<" 

%.mexw32: %.obj
	$(CROSS)-$(CXX) -shared $(GNUMEX)/mex.def -o "$@" -L$(GNUMEX) -s "$<" -llibmx -llibmex -lbiosig -lssp $(LDLIBS) -lws2_32


#########################################################
#	MATLAB/WIN64
#########################################################
%.o64: %.cpp
	$(CROSS64)-$(CXX) -c -DMATLAB_MEX_FILE $(DEFINES) -x c++ -o "$@" -I$(W64MAT_INC) -O2 "$<" 
%.o64: %.c
	$(CROSS64)-$(CXX) -c -DMATLAB_MEX_FILE $(DEFINES) -x c++ -o "$@" -I$(W64MAT_INC) -O2 "$<" 

%.mexw64: %.o64
	$(CROSS64)-$(CXX) -shared $(GNUMEX64)/mex.def -o "$@" -L$(GNUMEX64) -s "$<" -llibmx -llibmex -lbiosig -lssp $(LDLIBS) -lws2_32
