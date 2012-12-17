####### Makefile for "BioSig for C/C++" #####################
###
###  $Id$
###  Copyright (C) 2006,2007,2008,2009,2010,2011,2012 Alois Schloegl <alois.schloegl@ist.ac.at>
###  This file is part of the "BioSig for C/C++" repository
###  (biosig4c++) at http://biosig.sf.net/
###
##############################################################

##### Target: GNU/Linux

## make save2gdf   - makes converter
## make mex4o      - makes mexSLOAD, mexSOPEN for Octave (requires Octave-headers)
## make mex4m      - makes mexSLOAD, mexSOPEN for Matlab (requires Matlab, mex -setup must be configured)
## make mexw32     - makes mexSLOAD.mexw32, mexSOPEN.mexw32 (requires that mingw32, gnumex libraries from Matlab/Win32)
## make mexw64     - makes mexSLOAD.mexw64, mexSOPEN.mexw64 (requires that mce-w32, gnumex libraries from Matlab/Win64)
## make mex        - mex4o and mex4m combined
## make libbiosig  - makes libbiosig
## make biosig4python - makes python interface (requires Python)
## make biosig4java - makes Java interface (experimental)
## make biosig4php - makes PHP interface (experimental)
## make biosig4perl - makes perl interface (experimental)
## make biosig4ruby - makes ruby interface (experimental)
## make biosig4tcl - makes tcl/tk interface (experimental)
## make win32      - makes save2gdf.exe, libbiosig.a, libbiosig.dll, for MSWindows, requires MinGW
## make win64      - makes save2gdf.exe, libbiosig.a, libbiosig.dll for 64bit Windows, requires mce-w64 
## make sigviewer  - makes sigviewer

##### Target: Win32
## make win32      - makes save2gdf.exe,libbiosig.lib,libbiosig.dll, for MSWindows, requires MinGW
## make mexw32 mex/mexSLOAD.mexw32   - requires MinGW32 and GNUMEX libraries from Matlab/Win32
## make win32/sigviewer.exe 	- requires sources of SigViewer, and MinGW32 (mex: make suitesparse zlib qt )

##### Target: Win64
## make win64      - makes save2gdf.exe,libbiosig.lib,libbiosig.dll, for MSWindows, requires MinGW
## make mexw64 mex/mexSLOAD.mexw64   - requires MCE-W64 and GNUMEX libraries from Matlab/Win64
## make win64/sigviewer.exe 	- requires sources of SigViewer and MCE-W64, make suitesparse zlib qt 

###############################
# whether dynamic or static linking is used, can be controlled with
# LIBEXT. Setting it to 'a' links statically, 'so' links dynamically
#
LIBEXT        = a
#LIBEXT	      = so
###############################

### User-specified options: its likely you want to change this
# settings for cross compiler: tested with MXE and mce-w64 with make suitesparse zlib qt ; 
CROSS   = $(HOME)/src/mce-w64/usr/bin/i686-static-mingw32
## local MXE
ifeq "$(wildcard $(CROSS)-gcc)" ""
CROSS   = $(HOME)/src/mxe/usr/bin/i686-pc-mingw32
endif
ifeq "$(wildcard $(CROSS)-gcc)" ""
CROSS         = $(shell pwd)/../mxe/usr/bin/i686-pc-mingw32
endif
ifeq "$(wildcard $(CROSS)-gcc)" ""
CROSS         = /usr/local/src/mxe/usr/bin/i686-pc-mingw32
endif

CROSS64       = $(HOME)/src/mce-w64/usr/bin/x86_64-static-mingw32
ifeq "$(wildcard $(CROSS64)-gcc)" ""
CROSS64       = $(shell pwd)/../mce-w64/usr/bin/x86_64-static-mingw32
endif
ifeq "$(wildcard $(CROSS64)-gcc)" ""
CROSS64       = $(shell pwd)/../mce-w64/usr/bin/i686-pc-mingw32
endif
ifeq "$(wildcard $(CROSS64)-gcc)" ""
CROSS64       = /usr/local/src/mce-w64/usr/bin/i686-pc-mingw32
endif
IO_H_FILE   := $(subst /usr/bin/,/usr/,$(CROSS))/include/io.h
IO_H_FILE64 := $(subst /usr/bin/,/usr/,$(CROSS64))/include/io.h


# settings for mex files
MEX_OPTION    = -largeArrayDims # turn on for 64 bit Matlab, otherwise empty
# directory for sources of sigviewer
PathToSigViewer = ../sigviewer
PathToSigViewerWIN32 = ../sigviewer4win32
PathToSigViewerWIN64 = ../sigviewer4win64

## default options
CC            = gcc
CXX           = g++

DEFINES_ALL   = -D=NDEBUG 

DEFINES      += $(DEFINES_ALL)
#DEFINES      += -D=WITH_SON 
#DEFINES      += -D=WITHOUT_SCP_DECODE
#DEFINES      += -D=WITH_TIMESTAMPEVENT
#DEFINES      += -D=WITH_TIMESTAMPCHANNEL
DEFINES      += -D=WITH_ZLIB 
DEFINES      += -D=WITH_PTHREAD 
#DEFINES      += -D=WITH_CURL
DEFINES      += -D=__4HAERTEL__
DEFINES      += -D=WITH_FAMOS
#DEFINES      += -D=WITH_CHOLMOD
DEFINES      += -D=WITHOUT_NETWORK
#DEFINES      += -D=WITH_HDF
#DEFINES      += -D=WITH_MATIO
#DEFINES      += -D=WITH_LIBXML2 
DEFINES      += -D=WITH_FEF 
#DEFINES      += -D=WITH_PDP
#DEFINES      += -D=WITH_DICOM
#DEFINES      += -D=WITH_GDCM
#DEFINES      += -D=WITH_GSL
#DEFINES      += -D=WITH_EEPROBE
DEFINES      += -D=MAKE_EDFLIB

CFLAGS       += -fstack-protector -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Wl,-z,relro -Wl,-z,now -pipe -fPIC -Wall -fno-builtin-memcmp -O2 -Wextra
#CXXFLAGS      += -pipe -fPIC -Wall -fno-builtin-memcmp -O2 $(DEFINES) -Wextra # -I cntopenlib/include/
CXXFLAGS      = $(CFLAGS)
AR            = ar crs
INCPATH       = -I/opt/local/include/
#INCPATH       = -I /usr/local/include/gdcm-2.0/
#		../gdcm-2.0.10/Source/DataStructureAndEncodingDefinition \
#		../gdcm-2.0.10/Source/InformationObjectDefinition  	\
#		../gdcm-2.0.10/Source/MediaStorageAndFileFormat 	\
#		../gdcm-2.0.10/Source/DataDictionary 			\
#		../gdcm-2.0.10/Source/Common
LINK          = $(CXX)
LFLAGS_dynamic = -L. -L/opt/local/lib
LIBS_dynamic  = -lbiosig
LFLAGS_static = -L/opt/local/lib
LIBS_static   = libbiosig.a
ifeq ($(LIBEXT),so)
  LFLAGS      += $(LFLAGS_dynamic)
  LIBS        = $(LIBS_dynamic)
else
  LFLAGS      += $(LFLAGS_static)
  LIBS        = $(LIBS_static)
endif
ifneq (,$(findstring WITH_ZLIB, $(DEFINES)))
  LDLIBS     += -lz
endif
ifneq (,$(findstring WITH_PTHREAD, $(DEFINES)))
  LDLIBS     += -lpthread
endif
ifneq (,$(findstring WITH_CHOLMOD, $(DEFINES)))
  LDLIBS     += -lcholmod
endif
ifneq (,$(findstring WITH_LIBXML2, $(DEFINES)))
  LDLIBS     += -lxml2
endif
ifneq (,$(findstring WITH_CURL, $(DEFINES)))
  LDLIBS     += -lcurl
endif
ifneq (,$(findstring WITH_HDF, $(DEFINES)))
  LDLIBS     += -lhdf5
endif
ifneq (,$(findstring WITH_MATIO, $(DEFINES)))
  LDLIBS     += -lmatio
endif
LIBS         += $(LDLIBS)

DELETE        = rm -f
COPY          = cp -f
DATA_DIR      = data/
DATA_DIR_CFS  = $(HOME)/Q/data/test/CFS/
TEMP_DIR      = test/
SED           = sed
VERBOSE	     := -V0

##########################################################
## set Matlab and Octave variables
ifneq ($(shell which matlab), )
  MATLABDIR  := $(dir $(shell readlink -f $(shell which matlab) ) )
  MEX         = $(MATLABDIR)mex
  MEX_EXT    := $(shell $(MATLABDIR)mexext)
endif

# use environment variable to define OCTAVE_VERSION
#   e.g. export OCTAVE_VERSION=-3.6.2
# Octave - global install  (e.g. from debian package)
#OCTAVE_VERSION = 
# Octave - local install (e.g. compiled from sources) 
#OCTAVE_VERSION = -3.6.1

OCT           := mkoctfile$(OCTAVE_VERSION)
##########################################################

##########################################################
## set variables for Python
SWIG          := swig
PYTHON        := python
PYTHONVER     := $(shell $(PYTHON) -c "import sys; print sys.version[:3]")
NUMPY_INC     := $(shell $(PYTHON) -c 'import numpy.distutils.misc_util as mu; print " -I".join(mu.get_numpy_include_dirs())')

##########################################################
 
##########################################################
## set variables for MinGW Crosscompiler: compile on linux binaries for windows
##
PathToMinGW   = $(dir $(CROSS))..$(nondir $(CROSS))
PathToMinGW64 = $(dir $(CROSS64))..$(nondir $(CROSS64))

MinGWCC      = $(CROSS)-gcc
MinGWCXX     = $(CROSS)-g++
MinGWDEF     = $(DEFINES) 
MinGWCFLAGS  = -pipe -fPIC  -D_REENTRANT -D=BUILD_DLL $(MinGWDEF) -Iwin32 -I$(PathToMinGW)/include/
MinGWLIBS    = win32/libbiosig.a -L$(PathToMinGW)/lib/ -lws2_32 $(LDLIBS) # static
MinGWLINK    = $(MinGWCXX)

MinGW64CC      = $(CROSS64)-gcc
MinGW64CXX     = $(CROSS64)-g++
MinGW64CFLAGS  = -pipe -fPIC -O2 -D_REENTRANT -D=BUILD_DLL $(MinGWDEF) -Iwin64 -I$(PathToMinGW64)/include/
MinGW64LIBS    = win64/libbiosig.a -L$(PathToMinGW64)/lib/ -lws2_32 $(LDLIBS) # static
MinGW64LINK    = $(MinGW64CXX)
##########################################################

# Versioning
MAJOR_VERSION  := $(word 3, $(shell grep '\#define BIOSIG_VERSION_MAJOR' biosig.h))
MINOR_VERSION  := $(word 3, $(shell grep '\#define BIOSIG_VERSION_MINOR' biosig.h))
STEPPING       := $(word 3, $(shell grep '\#define BIOSIG_PATCHLEVEL' biosig.h))
BIOSIG_VERSION := ${MAJOR_VERSION}.${MINOR_VERSION}.${STEPPING}
TODAY          := $(shell date +%Y%m%d)

####### Output directory

OBJ	      = ./obj
BIN	      = $(DESTDIR)/usr/local/bin
LIB	      = $(DESTDIR)/usr/local/lib


####### Files
SOURCES       = biosig.c \
		t210/sopen_cfs_read.c \
		t210/sopen_heka_read.c \
		t210/sopen_scp_read.c \
		t210/sopen_famos_read.c \
		t210/sopen_abf_read.c \
		t210/sopen_alpha_read.c \
		t210/scp-decode.cpp \
		t220/sopen_scp_write.c \
		t220/crc4scp.c \
		t230/sopen_hl7aecg.cpp \
		t240/sopen_fef_read.c \
		test0/sandbox.c \
		xgethostname.c \
		physicalunits.c \
		biosig-network.c \
		save2gdf.c \
		biosig_client.c \
		biosig_server.c

ifeq (,$(findstring WITH_LIBXML2, $(DEFINES)))
  ## TinyXML is used when built without libxml2 	
  SOURCES    +=	XMLParser/tinyxml.cpp \
		XMLParser/tinyxmlparser.cpp \
		XMLParser/tinyxmlerror.cpp \
		XMLParser/tinystr.cpp 
endif

OBJECTS       = \
		crc4scp.o \
		biosig.o \
		sopen_cfs_read.o \
		sopen_heka_read.o \
		sopen_scp_read.o \
		sopen_alpha_read.o \
		sopen_abf_read.o \
		sopen_scp_write.o \
		sopen_hl7aecg.o \
		biosig-network.o \
		physicalunits.o \
		sandbox.o \
		xgethostname.o

MinGWOBJECTS  = \
		win32/crc4scp.obj \
		win32/biosig.obj \
		win32/getlogin.obj \
		win32/strndup.obj \
		win32/strnlen.obj \
		win32/sopen_cfs_read.obj \
		win32/sopen_heka_read.obj \
		win32/sopen_scp_read.obj \
		win32/sopen_abf_read.obj \
		win32/sopen_alpha_read.obj \
		win32/sopen_scp_write.obj \
		win32/sopen_hl7aecg.obj \
		win32/biosig-network.obj \
		win32/physicalunits.obj \
		win32/sandbox.obj \
		win32/xgethostname.obj

ifneq (,$(findstring WITH_FAMOS, $(DEFINES)))
  OBJECTS      += sopen_famos_read.o
endif
ifneq (,$(findstring WITH_FAMOS, $(MinGWDEF)))
  MinGWOBJECTS += win32/sopen_famos_read.obj
endif

ifneq (,$(findstring WITH_FEF, $(DEFINES)))
  OBJECTS      += sopen_fef_read.o
endif
ifneq (,$(findstring WITH_FEF, $(MinGWDEF)))
  MinGWOBJECTS += win32/sopen_fef_read.obj
endif

ifeq (,$(findstring WITHOUT_SCP_DECODE, $(DEFINES)))
  OBJECTS      += scp-decode.o
endif
ifeq (,$(findstring WITHOUT_SCP_DECODE, $(MinGWDEF)))
  MinGWOBJECTS += win32/scp-decode.obj
endif

ifeq (,$(findstring WITH_LIBXML2, $(DEFINES)))
  ## TinyXML is used when built without libxml2 	
  OBJECTS    += tinyxml.o tinyxmlparser.o tinyxmlerror.o tinystr.o 
endif
ifeq (,$(findstring WITH_LIBXML2, $(DEFINES)))
  ## TinyXML is used when built without libxml2 	
  MinGWOBJECTS += win32/tinyxml.obj win32/tinyxmlparser.obj win32/tinyxmlerror.obj win32/tinystr.obj 
endif


MinGW64OBJECTS  = $(patsubst win32/%.obj, win64/%.obj, $(MinGWOBJECTS))

TARGET = save2gdf libbiosig.a

first: $(TARGET)
all:   $(TARGET) mex4o biosig4python sigviewer win32 win64 win32/sigviewer.exe win64/sigviewer.exe #biosig_client biosig_server mma java tcl perl php ruby #sigviewer 

#############################################################
#	Compilation: Implicit, default rules
#############################################################

vpath %.c ./:./t210:./t220:./test0:./src:./mma

vpath %.cpp ./:./t210:./t230:./XMLParser:./mex

.SUFFIXES: .o .c .cpp .cc .cxx .C


%.o: t240/%.c biosig.h biosig-dev.h
	$(CC) -c $(DEFINES) $(CFLAGS) -I t240 $(INCPATH) -o "$@" "$<"

%.c: %.h biosig.h biosig-dev.h

%.cpp: %.h  biosig.h biosig-dev.h

%.o: %.c 
	$(CC) -c $(DEFINES) $(CFLAGS)  $(INCPATH) -o "$@" "$<"

%.o: %.cpp
	$(CXX) -c $(DEFINES) $(CXXFLAGS) $(INCPATH) -o "$@" "$<"


### exclude declaration of sopen in <io.h>
win32/io.h: $(IO_H_FILE)
	$(SED) '/ sopen/ s#^#//#' $(IO_H_FILE) > "$@"
win64/io.h: $(IO_H_FILE64)
	$(SED) '/ sopen/ s#^#//#' $(IO_H_FILE64) > "$@"


win32/%.obj: t240/%.c  biosig-dev.h biosig.h 
	$(MinGWCC) -c $(DEFINES) $(MinGWCFLAGS) -I t240 -o "$@" "$<"
win32/%.obj: %.c win32/io.h 
	$(MinGWCC) -c $(DEFINES) $(MinGWCFLAGS) -o "$@" "$<"
win32/%.obj: %.cpp
	$(MinGWCXX) -c $(DEFINES) $(MinGWCFLAGS) -o "$@" "$<"

win32/%.obj: win32/%.c 
	$(MinGWCC) -c $(DEFINES) $(MinGWCFLAGS) -I win32 -o "$@" "$<"


win64/%.obj: t240/%.c  biosig-dev.h biosig.h 
	$(MinGW64CC) -c $(DEFINES) $(MinGW64CFLAGS) -I t240 -o "$@" "$<"
win64/%.obj: %.c win64/io.h 
	$(MinGW64CC) -c $(DEFINES) $(MinGW64CFLAGS) -o "$@" "$<"
win64/%.obj: %.cpp
	$(MinGW64CXX) -c $(DEFINES) $(MinGW64CFLAGS) -o "$@" "$<"

win64/%.obj: win32/%.c 
	$(MinGW64CC) -c $(DEFINES) $(MinGW64CFLAGS) -I win32 -o "$@" "$<"



#############################################################
#	eventcodes and units: conversion from ascii to C code
#############################################################

biosig.o win32/biosig.obj win64/biosig.obj: eventcodes.i eventcodegroups.i biosig.c biosig.h biosig-dev.h

physicalunits.o win32/physicalunits.obj win64/physicalunits.obj: units.i physicalunits.h 

eventcodes.i eventcodegroups.i : extern/eventcodes.txt
	gawk -f eventcodes.awk "$<"

units.i : extern/units.csv
	awk -f units.awk "$<" > "$@"


#############################################################
#	Compilation: exceptions, explicit rules
#############################################################

gdf.o: biosig.c biosig-dev.h biosig.h
	$(CC) -c -D=ONLYGDF $(DEFINES_ALL) $(CFLAGS) $(INCPATH) -o "$@" "$<"

#getlogin_r.o: win32/getlogin_r.c
#	$(CC) -c $(DEFINES) $(CFLAGS) $(INCPATH) -o "$@" "$<"

########### WIN32 ##################

win32/gdf.obj: biosig.c biosig-dev.h biosig.h
	$(MinGWCC) -c -D=ONLYGDF $(DEFINES_ALL) $(MinGWCFLAGS) -o "$@" "$<"

$(PathToSigViewerWIN32): 
	svn co https://sigviewer.svn.sourceforge.net/svnroot/sigviewer/trunk ../sigviewer4win32

$(PathToSigViewerWIN32)/src/src.pro: $(PathToSigViewerWIN32)
	svn up -r 557 ../sigviewer4win32

win32/sigviewer.exe: win32/libbiosig.a win32/libbiosig.dll
	#-$(COPY) ../biosig/doc/eventcodes.txt $(PathToSigViewerWIN32)/src/
	-$(DELETE) $(PathToSigViewerWIN32)/extern/include/*.h
	-$(COPY) biosig.h physicalunits.h $(PathToSigViewerWIN32)/extern/include
	-$(DELETE) $(PathToSigViewerWIN32)/extern/lib/lib*
	-$(COPY) win32/libbiosig.a $(PathToSigViewerWIN32)/extern/lib
	$(SED) -i 's|\([[:space:]]*-lbiosig\)\([ #\\]*\)$$|\1 -lcholmod -lz -lcurl \2|' $(PathToSigViewerWIN32)/src/src.pro
	echo 0.5.2-v${BIOSIG_VERSION} > $(PathToSigViewerWIN32)/src/version.txt
	-(cd $(PathToSigViewerWIN32)/src; $(CROSS)-qmake; make)
	#-(cd $(PathToSigViewerWIN32); svn revert -R .; svn up -r 557; patch -p0 <../biosig4c++/patches/patch_sigviewer_0.5.2.diff; cd src; $(CROSS)-qmake; $(MAKE);)
	-$(COPY) $(PathToSigViewerWIN32)/bin/release/sigviewer.exe win32/sigviewer.exe

########### WIN64 ##################

win64/gdf.obj: biosig.c biosig-dev.h biosig.h
	$(MinGW64CC) -c -D=ONLYGDF $(DEFINES_ALL) $(MinGW64CFLAGS) -o "$@" "$<"

$(PathToSigViewerWIN64): 
	svn co https://sigviewer.svn.sourceforge.net/svnroot/sigviewer/trunk ../sigviewer4win64

$(PathToSigViewerWIN64)/src/src.pro: $(PathToSigViewerWIN64) 
	svn up -r 557 ../sigviewer4win64

win64/sigviewer.exe: win64/libbiosig.a win64/libbiosig.dll 
	#-$(COPY) ../biosig/doc/eventcodes.txt $(PathToSigViewerWIN64)/src/
	-$(DELETE) $(PathToSigViewerWIN64)/extern/include/*.h
	-$(COPY) biosig.h physicalunits.h $(PathToSigViewerWIN64)/extern/include
	-$(DELETE) $(PathToSigViewerWIN64)/extern/lib/lib*
	-$(COPY) win64/libbiosig.a $(PathToSigViewerWIN64)/extern/lib
	$(SED) -i 's|\([[:space:]]*-lbiosig\)\([ #\\]*\)$$|\1 -lcholmod -lz -lcurl\2|' $(PathToSigViewerWIN64)/src/src.pro
	echo 0.5.2-v${MAJOR_VERSION}.${MINOR_VERSION}.${STEPPING} > $(PathToSigViewerWIN64)/src/version.txt
	-(cd $(PathToSigViewerWIN64)/src; $(CROSS64)-qmake; make)
	#-(cd $(PathToSigViewerWIN64); svn revert -R .; svn up -r 557; patch -p0 <../biosig4c++/patches/patch_sigviewer_0.5.2.diff; cd src; $(CROSS64)-qmake; $(MAKE);)
	-$(COPY) $(PathToSigViewerWIN64)/bin/release/sigviewer.exe win64/sigviewer.exe


#############################################################
#	other language bindings (on Linux)
#############################################################

biosig4python : python/_biosig.so python/biosig.py

python/biosig.py python/swig_wrap.cxx: python/swig.i
	$(SWIG) -python  -I/usr/include/python$(PYTHONVER)/ -I$(NUMPY_INC) -o python/swig_wrap.cxx python/swig.i
python/_biosig.so : python/swig_wrap.cxx  libbiosig.$(LIBEXT)
	$(CXX) -c $(DEFINES) $(CXXFLAGS) -I/usr/include/python$(PYTHONVER)/ -I$(NUMPY_INC)  python/swig_wrap.cxx -o python/swig_wrap.o
	$(CXX) -shared python/swig_wrap.o $(LFLAGS) $(LIBS) -lpython$(PYTHONVER) -o python/_biosig.so 	 
python: libbiosig.$(LIBEXT) python/swig.i
	$(MAKE) -C python 
	 

java: libbiosig.$(LIBEXT) java/biosig.i
	$(MAKE) -C java 
perl: libbiosig.$(LIBEXT) perl/biosig.i
	$(MAKE) -C perl 
php: libbiosig.$(LIBEXT) php/biosig.i
	$(MAKE) -C php 


ruby/biosig_wrap.c: ruby/biosig.i
	(cd ruby && swig -ruby biosig.i)
ruby/Makefile: ruby/extconf.rb
	ruby -C ruby extconf.rb
biosig4ruby ruby: libbiosig.$(LIBEXT) ruby/Makefile ruby/biosig_wrap.c
	$(MAKE) -C ruby 


tcl: libbiosig.$(LIBEXT) tcl/biosig.i
	$(MAKE) -C tcl 


#############################################################
#	WIN32 - BUILD
#############################################################

win32/libphysicalunits.a: win32/physicalunits.obj
	-$(DELETE) "$@"
	$(CROSS)-$(AR) "$@" "$<"

win32/libphysicalunits.dll: win32/physicalunits.obj
	$(MinGWLINK) -shared -fPIC  "$<" -o "$@" -Wl,--output-def,win32/physicalunits.def,--out-implib,win32/physicalunits.dll.a


win32/libgdf.a: win32/gdf.obj
	-$(DELETE) "$@"
	$(CROSS)-$(AR) "$@" "$<"

win32/libgdf.dll: win32/gdf.obj
	$(MinGWLINK) -shared -fPIC "$<" -o "$@" -Wl,--output-def,win32/libgdf.def,--out-implib,win32/libgdf.dll.a

win32/libgdf.lib: win32/libgdf.a 
	$(COPY) "$<" "$@"

win32/libbiosig.lib: win32/libbiosig.a 
	$(COPY) "$<" "$@"

win32/libbiosig.a: $(MinGWOBJECTS) win32/libcnsfef.a
	-$(DELETE) "$@"
	$(CROSS)-$(AR) "$@" $(MinGWOBJECTS) t240/*.obj
	-touch ../sigviewer4win32/src/file_context.cpp	# trigger to rebuild sigviewer

win32/libcnsfef.a:
	$(MAKE) CROSS=$(CROSS) -C t240 ../win32/libcnsfef.a 

win32/libbiosig.dll: $(MinGWOBJECTS)
	$(MinGWLINK) -shared -fPIC  $(MinGWOBJECTS) $(MinGWLIBS) -o "$@" -Wl,--output-def,win32/libbiosig.def,--out-implib,win32/libbiosig.dll.a
	-$(COPY) "$@" $(PathToSigViewerWIN32)/extern/lib/

win32/libbiosig2.a: $(MinGWOBJECTS) win32/biosig2.obj win32/libcnsfef.a
	-$(DELETE) "$@"
	$(CROSS)-$(AR) "$@" $(MinGWOBJECTS) win32/biosig2.obj t240/*.obj
win32/libbiosig2.dll: $(MinGWOBJECTS) win32/biosig2.obj biosig.h biosig-dev.h
	$(MinGWLINK) -shared -fPIC  $(MinGWOBJECTS) win32/biosig2.obj $(MinGWLIBS) -o "$@" -Wl,--output-def,win32/libbiosig2.def,--out-implib,win32/libbiosig2.dll.a

win32/save2gdf.exe: save2gdf.c win32/libbiosig.a
	$(MinGWLINK) $(MinGWCFLAGS) "$<" $(MinGWLIBS) -o "$@"

win32/pdp2gdf.exe: pdp2gdf.c win32/libbiosig.a
	$(MinGWLINK) $(MinGWCFLAGS) "$<" $(MinGWLIBS) -o "$@"

win32 exe: win32/save2gdf.exe win32/libbiosig.dll win32/libbiosig.a #mexw32 win32mma 


#############################################################
#	WIN64 - BUILD
#############################################################

win64/libphysicalunits.a: win64/physicalunits.obj
	-$(DELETE) "$@"
	$(CROSS)-$(AR) "$@" "$<"

win64/libphysicalunits.dll: win64/physicalunits.obj
	$(MinGWLINK) -shared -fPIC  "$<" -o "$@" -Wl,--output-def,win64/physicalunits.def,--out-implib,win64/physicalunits.dll.a


win64/libgdf.a: win64/gdf.obj
	-$(DELETE) "$@"
	$(CROSS64)-$(AR) "$@" "$<"

win64/libgdf.dll: win64/gdf.obj
	$(MinGW64LINK) -shared -fPIC  "$<" -o "$@" -Wl,--output-def,win64/libgdf.def,--out-implib,win64/libgdf.dll.a

win64/libgdf.lib: win64/libgdf.a 
	$(COPY) "$<" "$@"

win64/libbiosig.lib: win64/libbiosig.a 
	$(COPY) "$<" "$@"

win64/libbiosig.a: $(MinGW64OBJECTS) win64/libcnsfef.a
	-$(DELETE) "$@"
	$(CROSS64)-$(AR) "$@" $(MinGW64OBJECTS) t240/*.o64
	-touch ../sigviewer4win64/src/file_context.cpp	# trigger to rebuild sigviewer

win64/libcnsfef.a:
	$(MAKE) CROSS64=$(CROSS64) -C t240 ../win64/libcnsfef.a 

win64/libbiosig.dll: $(MinGW64OBJECTS)
	$(MinGW64LINK) -shared -fPIC  $(MinGW64OBJECTS) $(MinGW64LIBS) -o "$@" -Wl,--output-def,win64/libbiosig.def,--out-implib,win64/libbiosig.dll.a
	-$(COPY) "$@" $(PathToSigViewerWIN64)/extern/lib/

win64/libbiosig2.a: $(MinGW64OBJECTS) win64/libcnsfef.a win64/biosig2.obj
	-$(DELETE) "$@"
	$(CROSS64)-$(AR) "$@" $(MinGW64OBJECTS) win64/biosig2.obj t240/*.o64
win64/libbiosig2.dll: $(MinGW64OBJECTS) win64/biosig2.obj
	$(MinGW64LINK) -shared -fPIC  $(MinGW64OBJECTS) win64/biosig2.obj $(MinGW64LIBS) -o "$@" -Wl,--output-def,win64/libbiosig2.def,--out-implib,win64/libbiosig2.dll.a

win64/save2gdf.exe: save2gdf.c win64/libbiosig.a
	$(MinGW64LINK) $(MinGW64CFLAGS) "$<" $(MinGW64LIBS) -o "$@"

win64/pdp2gdf.exe: pdp2gdf.c win64/libbiosig.a
	$(MinGW64LINK) $(MinGW64CFLAGS) "$<" $(MinGW64LIBS) -o "$@"

win64: win64/save2gdf.exe win64/libbiosig.dll win64/libbiosig.a #mexw64 win64mma 


#############################################################
#	GNU/Linux - BUILD
#############################################################

.phony libbiosig lib: libbiosig.a libbiosig.so libbiosig2.a libbiosig2.so libgdf.a libgdf.so libphysicalunits.a libphysicalunits.so

t240/libcnsfef.a:
#	$(MAKE) -C t240 regen		#
	$(MAKE) -C t240 libcnsfef.a	# generate t240/*.o files

libphysicalunits.a: physicalunits.o
	-$(DELETE) "$@"
	$(AR) "$@" "$<"
libphysicalunits.so: physicalunits.o
	$(CC) -shared -fPIC -Wl,-rpath,. -Wl,-soname,"$@" $(LDLIBS) -o "$@" "$<"

libgdf.a: gdf.o
	-$(DELETE) "$@"
	$(AR) "$@" "$<"

libgdf.so: gdf.o
	$(CC) -shared -fPIC -Wl,-rpath,. -Wl,-soname,"$@".${MAJOR_VERSION}.${MINOR_VERSION} $(LDLIBS) \
		-o "$@".${MAJOR_VERSION}.${MINOR_VERSION}.${STEPPING} "$<"
	/sbin/ldconfig -n .
	#ln -sf "$@".${MAJOR_VERSION}.${MINOR_VERSION}.${STEPPING} "$@".${MAJOR_VERSION}.${MINOR_VERSION}
	ln -sf "$@".${MAJOR_VERSION}.${MINOR_VERSION} "$@"

libbiosig.a: $(OBJECTS) t240/libcnsfef.a
	-$(DELETE) libbiosig.a
	$(AR) libbiosig.a $(OBJECTS) t240/*.o
	-touch ../sigviewer/src/file_context.cpp	# trigger to rebuild sigviewer

libbiosig.so: $(OBJECTS) t240/libcnsfef.a
	$(CXX) -shared -fPIC -Wl,-rpath,. -Wl,-soname,"$@".${MAJOR_VERSION}.${MINOR_VERSION} $(LDLIBS) \
		-o "$@".${MAJOR_VERSION}.${MINOR_VERSION}.${STEPPING} $(OBJECTS) t240/*.o
	/sbin/ldconfig -n .
	ln -sf "$@".${MAJOR_VERSION}.${MINOR_VERSION} "$@"

libbiosig2.a: $(OBJECTS) biosig2.o t240/libcnsfef.a
	-$(DELETE) libbiosig2.a
	$(AR) libbiosig2.a $(OBJECTS) biosig2.o t240/*.o

libbiosig2.so: $(OBJECTS) biosig2.o t240/libcnsfef.a
	$(CXX) -shared -fPIC -Wl,-rpath,. -Wl,-soname,libbiosig2.so.${MAJOR_VERSION} $(LDLIBS) \
		-o libbiosig2.so.${MAJOR_VERSION}.${MINOR_VERSION}.${STEPPING} $(OBJECTS) biosig2.o t240/*.o
	/sbin/ldconfig -n .
	ln -sf libbiosig2.so.${MAJOR_VERSION} libbiosig2.so

save2gdf: save2gdf.o libbiosig.$(LIBEXT)
	$(LINK) $(CXXFLAGS) save2gdf.o $(LFLAGS) $(LIBS) -o save2gdf

pdp2gdf: pdp2gdf.o libbiosig.$(LIBEXT)
	$(LINK) $(CXXFLAGS) pdp2gdf.o $(LFLAGS) $(LIBS) -o pdp2gdf

bscs: biosig_client biosig_server sandbox.o biosig.o
biosig_client: biosig_client.c libbiosig.$(LIBEXT) biosig-network.o
	$(LINK) $(CXXFLAGS) biosig_client.c  biosig-network.o $(LFLAGS) $(LIBS) -o biosig_client

biosig_server: biosig_server.c libbiosig.$(LIBEXT) biosig-network.o
	$(LINK) $(CXXFLAGS) biosig_server.c  biosig-network.o $(LFLAGS) $(LIBS) -o biosig_server


#############################################################
#	MathLink interface to Mathematica
#############################################################
.phony mma: mma/sload
mma/sload: libbiosig.$(LIBEXT) mma/sload.tm mma/sload.c
	$(MAKE) -C mma CROSS=$(CROSS) CROSS64=$(CROSS64) LDLIBS="$(LDLIBS)" sload

win32mma: win32/libbiosig.a mma/sload.tm mma/sload.c
	$(MAKE) -C mma CROSS=$(CROSS) CROSS64=$(CROSS64) LDLIBS="$(LDLIBS)" sload.exe

win64mma: win64/libbiosig.a  mma/sload.tm mma/sload.c
	$(MAKE) -C mma CROSS=$(CROSS) CROSS64=$(CROSS64) LDLIBS="$(LDLIBS)" sload.w64.exe


#############################################################
#	MEX-files for Octave and Matlab
#############################################################
mex mex4m mex4o oct: libbiosig.a
	$(MAKE) -C mex DEFINES='$(DEFINES)' CROSS=$(CROSS) CROSS64=$(CROSS64) LDLIBS="$(LDLIBS)" OCTAVE_VERSION=$(OCTAVE_VERSION) $@
	-cp mex/*.mex* ../biosig4matlab/t200_FileAccess/

mexw32: win32/libbiosig.a
	$(MAKE) -C mex DEFINES='$(DEFINES)' CROSS=$(CROSS) CROSS64=$(CROSS64) LDLIBS="$(LDLIBS)" $(@F)
	-cp mex/*.mexw32 ../biosig4matlab/t200_FileAccess/
	
mexw64: win64/libbiosig.a
	$(MAKE) -C mex DEFINES='$(DEFINES)' CROSS=$(CROSS) CROSS64=$(CROSS64) LDLIBS="$(LDLIBS)" $(@F)
	-cp mex/*.mexw64 ../biosig4matlab/t200_FileAccess/

#############################################################
#	SigViewer
#############################################################

sigviewer: $(PathToSigViewer)/bin/sigviewer 
	$(COPY) $(PathToSigViewer)/bin/release/sigviewer bin/sigviewer-$(TODAY)
	ln -sf sigviewer-$(TODAY) bin/sigviewer 

$(PathToSigViewer): 
	svn co https://sigviewer.svn.sourceforge.net/svnroot/sigviewer/trunk ../sigviewer

$(PathToSigViewer)/src/src.pro: $(PathToSigViewer) 
	svn up -r 557 ../sigviewer

$(PathToSigViewer)/bin/sigviewer: libbiosig.$(LIBEXT) biosig.h
	-$(DELETE) $(PathToSigViewer)/extern/include/*
	-$(COPY) biosig.h physicalunits.h $(PathToSigViewer)/extern/include
	-$(DELETE) $(PathToSigViewer)/extern/lib/lib*
	-$(COPY) libbiosig.a $(PathToSigViewer)/extern/lib
	$(SED) -i 's|\([[:space:]]*-lbiosig\)\([ #\\]*\)$$|\1 $(LDLIBS) \2|' $(PathToSigViewer)/src/src.pro
	echo 0.5.2-v${BIOSIG_VERSION} > $(PathToSigViewer)/src/version.txt
	(cd $(PathToSigViewer)/src; qmake; $(MAKE);)
	#-(cd $(PathToSigViewer); svn revert -R .; svn up -r 557; patch -p0 <../biosig4c++/patches/patch_sigviewer_0.5.2.diff; cd src; qmake; $(MAKE);)
	-$(COPY) $(PathToSigViewer)/bin/release/sigviewer bin/sigviewer

docs: 	docs/save2gdf.txt  docs/mexSLOAD.txt
	asciidoc -d manpage docs/save2gdf.txt
	asciidoc -d manpage docs/mexSLOAD.txt


# for backward compatibility
save2scp: save2gdf
save2aecg: save2gdf


#############################################################
#	APPLICATIONS: TTL2TRIG, FLOWMON
#############################################################

gdfwrite.o:  biosig.c biosig-dev.h biosig.h 
	$(CC) -c -D=ONLYGDF $(DEFINE_ALL) $(CFLAGS) -o "$@" biosig.c

###	TTL2TRIG
bin/ttl2trig : src/ttl2trig.c gdfwrite.o physicalunits.o
	$(CXX) -D=WITH_BIOSIG $(DEFINE_ALL) "$<" gdfwrite.o physicalunits.o -Wall -Wextra -lasound -o "$@"

###	FLOWMON
flowmon.o: src/flowmon2.c 
	$(CC)  -c $(DEFINES_ALL) $(CFLAGS) -o "$@" src/flowmon2.c 
        
bin/flowmon: flowmon.o gdfwrite.o physicalunits.o
	$(LINK) flowmon.o gdfwrite.o physicalunits.o -lpthread -o "$@"


#############################################################
#	INSTALL and DE-INSTALL
#############################################################

.PHONY: clean distclean install remove install_sigviewer install_octave asc bin testscp testhl7 testbin test test6 zip 

distclean:
	-$(DELETE) *.a
	-$(DELETE) *.i
	-$(DELETE) *.o
	-$(DELETE) *.lib
	-$(DELETE) *.so
	-$(DELETE) *.so.*
	-$(DELETE) *.mex*
	-$(DELETE) *.oct
	-$(DELETE) $(IO_H_FILE64) $(IO_H_FILE)
	-$(DELETE) t5.scp t6.scp save2gdf gztest test_scp_decode biosig_server biosig_client
	-$(DELETE) t?.[bge]df* t?.hl7* t?.scp* t?.cfw* t?.gd1* t?.*.gz *.fil $(TEMP_DIR)t1.* $(DATA_DIR)t1.*
	-$(DELETE) python/swig_wrap.* python/biosig.py* python/_biosig.so
	-$(DELETE) QMakefile
	-$(DELETE) igor/libIgor.a
	-$(DELETE) win32/*.a win32/*.lib win32/libbiosig.* win32/*.obj win32/*.exe
	-$(DELETE) win64/*.a win64/*.lib win64/libbiosig.* win64/*.obj win64/*.exe
	-$(DELETE) -rf win32/zlib
	-$(MAKE) -C java clean
	-$(MAKE) -C matlab clean
	-$(MAKE) -C mma clean
	-$(MAKE) -C php clean
	-$(MAKE) -C perl clean
	-$(MAKE) -C ruby clean
	-$(MAKE) -C tcl clean
	-$(MAKE) -C mex clean

clean:
	-$(DELETE) *~
	-$(DELETE) *.a
	-$(DELETE) *.i
	-$(DELETE) *.o
	-$(DELETE) *.so
	-$(DELETE) *.so.*
	-$(DELETE) *.mex*
	-$(DELETE) *.oct
	-$(DELETE) $(TEMP_DIR)t1.*
	-$(DELETE) python/biosig.py* _biosig.so
	-$(DELETE) python/swig_wrap.*
	-$(DELETE) win32/*.exe win32/*.o* win32/*.lib win32/*.a
	-$(DELETE) win64/*.exe win64/*.o* win64/*.lib win64/*.a
	-$(DELETE) t240/*.o*
	-$(DELETE) t240/libcnsfef.a
	-$(DELETE) t240/libbiosig.*
	-$(DELETE) pdp2gdf
	-$(MAKE) -C java clean
	-$(MAKE) -C matlab clean
	-$(MAKE) -C mma clean
	-$(MAKE) -C php clean
	-$(MAKE) -C perl clean
	-$(MAKE) -C ruby clean
	-$(MAKE) -C tcl clean
	-$(MAKE) -C mex clean

install_sigviewer: sigviewer
	install $(PathToSigViewer)/bin/release/sigviewer $(BIN)

install_ttl2trig: bin/ttl2trig
	install bin/ttl2trig $(BIN)

install_octave: mexSLOAD.mex 
	mkdir -p  			$(DESTDIR)/usr/share/octave/site-m/
	install mex/mexSLOAD.mex* 	$(DESTDIR)/usr/share/octave/site-m/
	install mex/mexSOPEN.mex* 	$(DESTDIR)/usr/share/octave/site-m/

install_libbiosig: libbiosig.a libbiosig.so libbiosig2.a libbiosig2.so libphysicalunits.a libphysicalunits.so
	install biosig.h    		$(DESTDIR)/usr/include
	install physicalunits.h    	$(DESTDIR)/usr/include
	install libbiosig.so.${MAJOR_VERSION}.${MINOR_VERSION}.${STEPPING} $(LIB)
	ln -sf  libbiosig.so.${MAJOR_VERSION}.${MINOR_VERSION}.${STEPPING} $(LIB)/libbiosig.so.${MAJOR_VERSION}.${MINOR_VERSION}
	ln -sf  libbiosig.so.${MAJOR_VERSION}.${MINOR_VERSION} $(LIB)/libbiosig.so
	install libbiosig.a		$(LIB)
	install libphysicalunits.a 	$(LIB)
	install libphysicalunits.so	$(LIB)
	install biosig2.h    		$(DESTDIR)/usr/include
	install libbiosig2.a		$(LIB)
	install libbiosig2.so		$(LIB)

install: save2gdf libbiosig.a doc/save2gdf.1 install_libbiosig
	install -d 			$(BIN)
	install save2gdf 		$(BIN)
	install heka2itx 		$(BIN)
	install save2aecg 		$(BIN)
	install save2scp  		$(BIN)
	-install bin/tt2trig  		$(BIN)
	#install rec2bin  		$(BIN)
	#install bin2rec  		$(BIN)
	install -d			$(LIB) $(DESTDIR)/usr/include
	install biosig.h    		$(DESTDIR)/usr/include
	install physicalunits.h    	$(DESTDIR)/usr/include
	install libbiosig.a libphysicalunits.a	$(LIB)
	mkdir -p  			$(DESTDIR)/usr/share/man/man1
	install doc/*.1  		$(DESTDIR)/usr/share/man/man1
	#install libbiosig.man 		$(DESTDIR)/usr/share/
	#$(MAKE)  install_sigviewer
	#$(MAKE)  install_octave

uninstall remove:
	-$(DELETE) $(BIN)/save2gdf
	-$(DELETE) $(BIN)/heka2itx
	-$(DELETE) $(BIN)/save2scp
	-$(DELETE) $(BIN)/save2aecg
	-$(DELETE) $(BIN)/ttl2trig
	#-$(DELETE) $(BIN)/rec2bin
	#-$(DELETE) $(BIN)/bin2rec
	-$(DELETE) $(BIN)/sigviewer
	-$(DELETE) $(BIN)/eventcodes.txt
	-$(DELETE) $(DESTDIR)/usr/include/biosig.h
	-$(DELETE) $(DESTDIR)/usr/include/biosig2.h
	-$(DELETE) $(DESTDIR)/usr/include/physicalunits.h
	-$(DELETE) $(LIB)/libbiosig.*
	-$(DELETE) $(LIB)/libbiosig2.*
	-$(DELETE) $(LIB)/libphysicalunits.*
	-$(DELETE) $(DESTDIR)/usr/share/man/man1/save2gdf.1
	-$(DELETE) $(DESTDIR)/usr/share/man/man1/mexSLOAD.1
	-$(DELETE) $(DESTDIR)/usr/share/man/man1/sigviewer.1
	#-$(DELETE) $(DESTDIR)/usr/share/man/man1/rec2bin.1
	#-$(DELETE) $(DESTDIR)/usr/share/man/man1/bin2rec.1


#############################################################
#	Testing
#############################################################
testtms: $(DATA_DIR)t1.scp
#	./save2gdf -V8 ~/data/test/tms32/small_test.float32.log
	./save2gdf -f=TMSi $(DATA_DIR)t1.scp $(TEMP_DIR)t2.log
	cat $(TEMP_DIR)t2.log

fetchdata: $(TEMP_DIR)scp/redred/PFE103.scp  $(TEMP_DIR)Osas2002plusQRS.edf $(DATA_DIR)Newtest17-2048.bdf $(DATA_DIR)2003-12\ Schema/example/Example\ aECG.xml

$(TEMP_DIR)scp/redred/PFE103.scp:
	# scp example data sets
	wget  -q -P$(DATA_DIR) http://www.openecg.net/ECGsamples.zip
	wget  -q -P$(DATA_DIR) http://www.openecg.net/ECGsamplesc.zip
	unzip -u $(DATA_DIR)ECGsamples.zip "scp*.zip" -d $(TEMP_DIR)
	unzip -u $(DATA_DIR)ECGsamplesc.zip "scp*.zip" -d $(TEMP_DIR)
	mkdir -p $(TEMP_DIR)scp/high
	mkdir -p $(TEMP_DIR)scp/highc
	mkdir -p $(TEMP_DIR)scp/redred
	mkdir -p $(TEMP_DIR)scp/redredc
	unzip -u $(TEMP_DIR)scp_high.zip -d $(TEMP_DIR)scp/high
	unzip -u $(TEMP_DIR)scp_highc.zip -d $(TEMP_DIR)scp/highc
	unzip -u $(TEMP_DIR)scp_redred.zip -d $(TEMP_DIR)scp/redred
	unzip -u $(TEMP_DIR)scp_redredc.zip -d $(TEMP_DIR)scp/redredc
	rm -rf $(TEMP_DIR)ECGsamples*.zip
	rm -rf $(TEMP_DIR)scp*.zip

$(DATA_DIR)t1.scp: $(TEMP_DIR)scp/redred/PFE103.scp
	$(COPY) "$<" "$@"
	touch "$@"

#"$(DATA_DIR)aECG Release 1 Schema and Example.zip":
$(DATA_DIR)2003-12\ Schema/example/Example\ aECG.xml:
	# HL7aECG example data set
	wget -q -P$(DATA_DIR) https://www.hl7.org/documentcenter/public/wg/rcrim/annecg/aECG%20Release%201%20Schema%20and%20Example.zip
	unzip -u $(DATA_DIR)"aECG Release 1 Schema and Example.zip" -d $(DATA_DIR)

$(TEMP_DIR)t1.hl7: $(DATA_DIR)2003-12\ Schema/example/Example\ aECG.xml
	$(COPY) "$<" "$@"
	#rm -rf "$(TEMP_DIR)aECG Release 1 Schema and Example.zip"
	#rm -rf "$(TEMP_DIR)2003-12 Schema"

$(TEMP_DIR)Osas2002plusQRS.edf:
	# EDF+ example data set
	wget -q -P$(TEMP_DIR) http://www.edfplus.info/downloads/files/osas.zip
	unzip -u "$(TEMP_DIR)osas.zip"  -d $(TEMP_DIR)

$(DATA_DIR)t1.edf: $(TEMP_DIR)Osas2002plusQRS.edf
	$(COPY) "$<" "$@"
	touch "$@"

asc: save2gdf
	./save2gdf -f=ASCII t0.xxx $(TEMP_DIR)t1.asc

bin: save2gdf
	./save2gdf -f=BIN t0.xxx $(TEMP_DIR)t1.bin

testjson: save2gdf  $(DATA_DIR)t1.edf
	./save2gdf -JSON t0.xxx |tee $(TEMP_DIR)t1.xxx.json
	./save2gdf -JSON  $(TEMP_DIR)t1.edf |tee $(TEMP_DIR)t1.edf.json

testbin: save2gdf $(DATA_DIR)t1.edf
	./save2gdf -f=BIN $(DATA_DIR)t1.edf $(TEMP_DIR)t1.hdr
	./save2gdf -f=BIN $(TEMP_DIR)t1.hdr $(TEMP_DIR)t2.hdr
	./save2gdf -f=GDF $(TEMP_DIR)t2.hdr $(TEMP_DIR)t2.gdf

testscp: $(DATA_DIR)t1.scp save2gdf
	# test converting SCP data
	./save2gdf -f=HL7 "$<" $(TEMP_DIR)t1.scp.hl7
	./save2gdf -f=GDF $(TEMP_DIR)t1.scp.hl7 $(TEMP_DIR)t1.scp.hl7.gdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.scp.hl7.gdf $(TEMP_DIR)t1.scp.hl7.gdf.scp
	./save2gdf -f=GDF $(TEMP_DIR)t1.scp.hl7.gdf.scp $(TEMP_DIR)t1.scp.hl7.gdf.scp.gdf
	./save2gdf -f=HL7 $(TEMP_DIR)t1.scp.hl7.gdf.scp.gdf $(TEMP_DIR)t1.scp.hl7.gdf.scp.gdf.hl7
	./save2gdf -f=SCP $(TEMP_DIR)t1.scp.hl7.gdf.scp.gdf.hl7 $(TEMP_DIR)t1.scp.hl7.gdf.scp.gdf.hl7.scp
	./save2gdf -f=GDF "$<" $(TEMP_DIR)t1.scp.gdf
	./save2gdf -f=HL7 $(TEMP_DIR)t1.scp.gdf $(TEMP_DIR)t1.scp.gdf.hl7
	./save2gdf -f=SCP $(TEMP_DIR)t1.scp.gdf.hl7 $(TEMP_DIR)t1.scp.gdf.hl7.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.scp.gdf.hl7.scp $(TEMP_DIR)t1.scp.gdf.hl7.scp.hl7
	./save2gdf -f=GDF $(TEMP_DIR)t1.scp.gdf.hl7.scp.hl7 $(TEMP_DIR)t1.scp.gdf.hl7.scp.hl7.gdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.scp.gdf.hl7.scp.hl7.gdf $(TEMP_DIR)t1.scp.gdf.hl7.scp.hl7.gdf.scp

testscp2: $(DATA_DIR)t1.scp
	find test/scp -iname '*.scp' -exec ./save2gdf -V0 {} /dev/null \;


testhl7: $(TEMP_DIR)t1.hl7 save2gdf
	# test converting HL7aECG data
	./save2gdf -f=GDF "$<" $(TEMP_DIR)t1.hl7.gdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.hl7.gdf $(TEMP_DIR)t1.hl7.gdf.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.hl7.gdf.scp $(TEMP_DIR)t1.hl7.gdf.scp.hl7
	./save2gdf -f=SCP $(TEMP_DIR)t1.hl7.gdf.scp.hl7 $(TEMP_DIR)t1.hl7.gdf.scp.hl7.scp
	./save2gdf -f=GDF $(TEMP_DIR)t1.hl7.gdf.scp.hl7.scp $(TEMP_DIR)t1.hl7.gdf.scp.hl7.scp.gdf
	./save2gdf -f=HL7 $(TEMP_DIR)t1.hl7.gdf.scp.hl7.scp.gdf $(TEMP_DIR)t1.hl7.gdf.scp.hl7.scp.gdf.hl7
	./save2gdf -f=SCP "$<" $(TEMP_DIR)t1.hl7.scp
	./save2gdf -f=GDF $(TEMP_DIR)t1.hl7.scp $(TEMP_DIR)t1.hl7.scp.gdf
	./save2gdf -f=HL7 $(TEMP_DIR)t1.hl7.scp.gdf $(TEMP_DIR)t1.hl7.scp.gdf.hl7
	./save2gdf -f=GDF $(TEMP_DIR)t1.hl7.scp.gdf.hl7 $(TEMP_DIR)t1.hl7.scp.gdf.hl7.gdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.hl7.scp.gdf.hl7.gdf $(TEMP_DIR)t1.hl7.scp.gdf.hl7.gdf.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.hl7.scp.gdf.hl7.gdf.scp $(TEMP_DIR)t1.hl7.scp.gdf.hl7.gdf.scp.hl7

test: $(DATA_DIR)t1.scp save2scp save2aecg save2gdf
	# biosig4python
	# includes test for on-the-fly compression and decompression
	./save2gdf -z  $(DATA_DIR)t1.scp        $(TEMP_DIR)t1.scp.gdf
	./save2gdf -f=SCP -z 	$(DATA_DIR)t1.scp        $(TEMP_DIR)t1.scp.scp
	./save2gdf -f=HL7 -z	$(DATA_DIR)t1.scp        $(TEMP_DIR)t1.scp.hl7
	./save2gdf 	$(TEMP_DIR)t1.scp.gdf.gz $(TEMP_DIR)t1.scp.gdf.gdf
	./save2gdf 	$(TEMP_DIR)t1.scp.scp.gz $(TEMP_DIR)t1.scp.scp.gdf
	./save2gdf 	$(TEMP_DIR)t1.scp.hl7.gz $(TEMP_DIR)t1.scp.hl7.gdf
	./save2gdf -f=SCP 	$(TEMP_DIR)t1.scp.gdf.gz $(TEMP_DIR)t1.scp.gdf.scp
	./save2gdf -f=SCP 	$(TEMP_DIR)t1.scp.scp.gz $(TEMP_DIR)t1.scp.scp.scp
	./save2gdf -f=SCP 	$(TEMP_DIR)t1.scp.hl7.gz $(TEMP_DIR)t1.scp.hl7.scp
	./save2gdf -f=HL7	$(TEMP_DIR)t1.scp.gdf.gz $(TEMP_DIR)t1.scp.gdf.hl7
	./save2gdf -f=HL7	$(TEMP_DIR)t1.scp.scp.gz $(TEMP_DIR)t1.scp.scp.hl7
	./save2gdf -f=HL7	$(TEMP_DIR)t1.scp.hl7.gz $(TEMP_DIR)t1.scp.hl7.hl7
	# python test0/test.py

zip: $(DATA_DIR)t1.scp save2gdf
	# test for on-the-fly compression and decompression
	# on-the-fly compression of output file
	./save2gdf -z -f=GDF $(DATA_DIR)t1.scp $(TEMP_DIR)t1.gdf
	./save2gdf -z -f=GDF1 $(DATA_DIR)t1.scp $(TEMP_DIR)t1.gd1
	./save2gdf -z -f=EDF $(DATA_DIR)t1.scp $(TEMP_DIR)t1.edf
	./save2gdf -z -f=BDF $(DATA_DIR)t1.scp $(TEMP_DIR)t1.bdf
	./save2gdf -z -f=SCP $(DATA_DIR)t1.scp $(TEMP_DIR)t1.scp
	./save2gdf -z -f=CFWB $(DATA_DIR)t1.scp $(TEMP_DIR)t1.cfw
	./save2gdf -z -f=MFER $(DATA_DIR)t1.scp $(TEMP_DIR)t1.mwf
	./save2gdf -z -f=HL7 $(DATA_DIR)t1.scp $(TEMP_DIR)t1.hl7

	gzip -c $(DATA_DIR)t1.scp >$(TEMP_DIR)t1.scp.gz
	# on-the-fly decompression of input file
	./save2gdf -f=GDF $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t1.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t1.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t1.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t1.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t1.hl7
	./save2gdf -f=MFER $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t1.mwf
	./save2gdf -f=CFWB $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t1.cfw

test6: $(DATA_DIR)t1.scp save2gdf
	$(COPY) $(DATA_DIR)t1.scp $(TEMP_DIR)t0.xxx
	#test7: $(DATA_DIR)t1.edf save2gdf
	#$(COPY) $(DATA_DIR)t1.edf $(TEMP_DIR)t0.xxx
	./save2gdf -z -f=GDF1 $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.gd1
	./save2gdf -z -f=GDF $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.gdf
	./save2gdf -z -f=EDF $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.edf
	./save2gdf -z -f=BDF $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.bdf
	./save2gdf -z -f=SCP $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.scp
	./save2gdf    -f=HL7 $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.hl7   # -z not supported for HL7
	gzip -f $(TEMP_DIR)t1.hl7
	./save2gdf -z -f=CFWB $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.cfw
	./save2gdf -z -f=MFER $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.mwf
	./save2gdf -f=BVA $(TEMP_DIR)t0.xxx $(TEMP_DIR)t1.bva
	./save2gdf -f=GDF1 $(TEMP_DIR)t1.gd1.gz $(TEMP_DIR)t2.gd1.gd1
	./save2gdf -f=GDF $(TEMP_DIR)t1.gd1.gz $(TEMP_DIR)t2.gd1.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.gd1.gz $(TEMP_DIR)t2.gd1.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.gd1.gz $(TEMP_DIR)t2.gd1.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.gd1.gz $(TEMP_DIR)t2.gd1.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.gd1.gz $(TEMP_DIR)t2.gd1.hl7
	./save2gdf -f=CFWB $(TEMP_DIR)t1.gd1.gz $(TEMP_DIR)t2.gd1.cfw
	./save2gdf -f=GDF1 $(TEMP_DIR)t1.gdf.gz $(TEMP_DIR)t2.gdf.gd1
	./save2gdf -f=GDF $(TEMP_DIR)t1.gdf.gz $(TEMP_DIR)t2.gdf.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.gdf.gz $(TEMP_DIR)t2.gdf.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.gdf.gz $(TEMP_DIR)t2.gdf.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.gdf.gz $(TEMP_DIR)t2.gdf.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.gdf.gz $(TEMP_DIR)t2.gdf.hl7
	./save2gdf -f=CFWB $(TEMP_DIR)t1.gdf.gz $(TEMP_DIR)t2.gdf.cfw
	./save2gdf -f=GDF1 $(TEMP_DIR)t1.edf.gz $(TEMP_DIR)t2.edf.gd1
	./save2gdf -f=GDF $(TEMP_DIR)t1.edf.gz $(TEMP_DIR)t2.edf.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.edf.gz $(TEMP_DIR)t2.edf.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.edf.gz $(TEMP_DIR)t2.edf.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.edf.gz $(TEMP_DIR)t2.edf.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.edf.gz $(TEMP_DIR)t2.edf.hl7
	./save2gdf -f=CFWB $(TEMP_DIR)t1.edf.gz $(TEMP_DIR)t2.edf.cfw
	./save2gdf -f=GDF1 $(TEMP_DIR)t1.bdf.gz $(TEMP_DIR)t2.bdf.gd1
	./save2gdf -f=GDF $(TEMP_DIR)t1.bdf.gz $(TEMP_DIR)t2.bdf.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.bdf.gz $(TEMP_DIR)t2.bdf.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.bdf.gz $(TEMP_DIR)t2.bdf.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.bdf.gz $(TEMP_DIR)t2.bdf.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.bdf.gz $(TEMP_DIR)t2.bdf.hl7
	./save2gdf -f=CFWB $(TEMP_DIR)t1.bdf.gz $(TEMP_DIR)t2.bdf.cfw
	./save2gdf -f=GDF1 $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t2.scp.gd1
	./save2gdf -f=GDF $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t2.scp.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t2.scp.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t2.scp.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t2.scp.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t2.scp.hl7
	./save2gdf -f=CFWB $(TEMP_DIR)t1.scp.gz $(TEMP_DIR)t2.scp.cfw
	./save2gdf -f=GDF1 $(TEMP_DIR)t1.hl7.gz $(TEMP_DIR)t2.hl7.gd1
	./save2gdf -f=GDF $(TEMP_DIR)t1.hl7.gz $(TEMP_DIR)t2.hl7.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.hl7.gz $(TEMP_DIR)t2.hl7.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.hl7.gz $(TEMP_DIR)t2.hl7.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.hl7.gz $(TEMP_DIR)t2.hl7.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.hl7.gz $(TEMP_DIR)t2.hl7.hl7
	./save2gdf -f=CFWB $(TEMP_DIR)t1.hl7.gz $(TEMP_DIR)t2.hl7.cfw
	./save2gdf -f=GDF1 $(TEMP_DIR)t1.cfw.gz $(TEMP_DIR)t2.cfw.gd1
	./save2gdf -f=GDF $(TEMP_DIR)t1.cfw.gz $(TEMP_DIR)t2.cfw.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.cfw.gz $(TEMP_DIR)t2.cfw.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.cfw.gz $(TEMP_DIR)t2.cfw.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.cfw.gz $(TEMP_DIR)t2.cfw.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.cfw.gz $(TEMP_DIR)t2.cfw.hl7
	./save2gdf -f=GDF1 $(TEMP_DIR)t1.mwf.gz $(TEMP_DIR)t2.mwf.gd1
	./save2gdf -f=GDF $(TEMP_DIR)t1.mwf.gz $(TEMP_DIR)t2.mwf.gdf
	./save2gdf -f=EDF $(TEMP_DIR)t1.mwf.gz $(TEMP_DIR)t2.mwf.edf
	./save2gdf -f=BDF $(TEMP_DIR)t1.mwf.gz $(TEMP_DIR)t2.mwf.bdf
	./save2gdf -f=SCP $(TEMP_DIR)t1.mwf.gz $(TEMP_DIR)t2.mwf.scp
	./save2gdf -f=HL7 $(TEMP_DIR)t1.mwf.gz $(TEMP_DIR)t2.mwf.hl7

$(DATA_DIR)Newtest17-2048.bdf : $(DATA_DIR)BDFtestfiles.zip
	unzip -u "$<" -d $(DATA_DIR)

testbdf : $(DATA_DIR)Newtest17-2048.bdf
	./save2gdf -V4 "$<" | awk '/NoChannels:/ { print $$2 } '
	./save2gdf -V4 "$<" | awk '/NRec:/ { print $$2 } '
	./save2gdf -V4 "$<" | awk '/Fs:/ { print $$2 } '
	./save2gdf -V4 "$<" | awk '/Events\/Annotations:/ { print $$2 } '

testpybdf : $(DATA_DIR)Newtest17-2048.bdf
	python python/demo.py $(TEMP_DIR)*-256.bdf
	python python/example.py $(TEMP_DIR)*-256.bdf 256 && \
	python python/example.py $(TEMP_DIR)*-2048.bdf 2048

$(DATA_DIR)BDFtestfiles.zip :
	wget -P$(DATA_DIR) http://www.biosemi.com/download/BDFtestfiles.zip

.phony stat :
	@echo 'TODO/FIXME:  ';
	@grep -ri '\bTODO\b'  *.c *.h *.i t210/*.c t210/*.h |grep -v '\.svn' |wc -l;
	@grep -ri '\bFIXME\b' *.c *.h *.i t210/*.c t210/*.h |grep -v '\.svn' |wc -l;

test_physicalunits : units.i physicalunits.c physicalunits.h
	gcc -D=TEST_PHYSDIMTABLE_PERFORMANCE -D=WITH_PTHREAD physicalunits.c -o test_physicalunits
	./test_physicalunits
	@echo '--- end of test_physicalunits ---'


testcfs : $(DATA_DIR_CFS) save2gdf 
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)BaseDemo/Actions.CFS
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)BaseDemo/example.cfs
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)BaseDemo/Leak.cfs
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)BaseDemo/NAME_0AA.cfs
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)BaseDemo/SCANexam.CFS
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)BaseDemo/TEST.cfs
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)BaseDemo/TRIAL0AA.cfs
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)100118s1AB.dat
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)100121s1AF.dat
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)Signal/1_23456.000.cfs
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)Signal/1_23456.001.cfs
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)Signal/1_23456.002.cfs
	-./save2gdf $(VERBOSE) $(DATA_DIR_CFS)Signal/1_23456.LAST.cfs

