####### Makefile to compile sload.exe for loading data supported by libbiosig into Mathematica
###
###  $Id$
###  Copyright (C) 2010,2011,2012 Alois Schloegl, IST Austria <alois.schloegl@ist.ac.at>
###  This file is part of the "BioSig for C/C++" repository
###  (biosig4c++) at http://biosig.sf.net/
###
##############################################################


###########################################
# user-defined variables 
########################################### 
##########################################################
## set Mathematica variables
MLINKDIR = /usr/local/Wolfram/gridMathematicaServer/7.0/SystemFiles/Links/MathLink/DeveloperKit
MLINKDIR = /usr/local/Wolfram/Mathematica/8.0/SystemFiles/Links/MathLink/DeveloperKit

ifneq ($(shell which mathematica), )
  MLINKDIR  := $(dir $(shell readlink -f $(shell which mathematica) ) )../SystemFiles/Links/MathLink/DeveloperKit
endif
SYS       = Linux-x86-64# Set this value with the result of evaluating $SystemID
MLLIB     = ML64i3 # ML32i3 # Set this to ML64i3 if using a 64-bit system
#
# Windows
#MLINKDIR  = /usr/local/Wolfram/gridMathematicaServer/7.0/SystemFiles/Links/MathLink/DeveloperKit
SYSwin       = Windows# Set this value with the result of evaluating $SystemID
MLLIBwin     = ml32i3m# Set this to ML64i3 if using a 64-bit system
SYSwin64     = Windows-x86-64# Set this value with the result of evaluating $SystemID
MLLIBwin64   = ml64i3m# Set this to ML64i3 if using a 64-bit system
###########################################

CADDSDIR  = ${MLINKDIR}/${SYS}/CompilerAdditions
INCDIR    = ${CADDSDIR}
LIBDIR    = ${CADDSDIR}

EXTRALIBS = -lm -lpthread -lrt -lstdc++ $(LDLIBS) # Set these with appropriate libs for your system.

MPREP     = ${CADDSDIR}/mprep

all : sload sload.exe sload.w64.exe
win win32 : sload.exe

../libbiosig.a:
	make -C .. libbiosig.a
../win32/libbiosig.a: 
	make -C .. win32/libbiosig.a
../win64/libbiosig.a: 
	make -C .. win64/libbiosig.a


.c.o:
	${CC} -c -I${INCDIR} "$<"

sloadtm.c: sload.tm
	${MPREP} "$<" -o "$@"
	# enable macro expansion
	sed -i 's# __DATE__# ",__DATE__," ",__TIME__,"#' "$@"

sload: sload.o sloadtm.o
	${CC} sloadtm.o sload.o -L.. -lbiosig -L${LIBDIR} -l${MLLIB} ${EXTRALIBS} -o "$@"
	# copy to users directory if available
	-cp $@ ../../mma/
	-cp $@ ../../mma/sload-$(shell date +%Y%m%d)


###########################################
#   Win32 - cross-compilation 
###########################################

CCw32        = $(CROSS)-gcc
CADDSDIRwin  = ${MLINKDIR}/${SYSwin}/CompilerAdditions/mldev32
INCDIRwin    = ${CADDSDIRwin}/include
LIBDIRwin    = ${CADDSDIRwin}/lib

EXTRALIBSwin = -lgdi32 -lm -lpthread -lstdc++ -lws2_32 $(LDLIBS) # Set these with appropriate libs for your system.

MPREPwin     = ${CADDSDIRwin}/bin/mprep.exe

sloadtm.win32.c : sload.tm
	wine ${MPREPwin} $? -o "$@"
	sed -i 's# __DATE__# ",__DATE__," ",__TIME__,"#' "$@"

sload.obj : sload.c
	${CCw32} -c -I${INCDIRwin} $< -o "$@"
sloadtm.obj : sloadtm.win32.c
	${CCw32} -c -I${INCDIRwin} $< -o "$@"

sload.exe : sload.obj sloadtm.obj ../win32/libbiosig.a
	${CCw32} sload.obj sloadtm.obj -static-libgcc -static-libstdc++ ../win32/libbiosig.a -L${LIBDIRwin} -l${MLLIBwin} ${EXTRALIBSwin} -o "$@"
	# copy to users directory if available
	-cp $@ ../../mma/
	-cp $@ ../../mma/sload-$(shell date +%Y%m%d).exe


###########################################
#   Win64 - cross-compilation 
###########################################

CCw64         = $(CROSS64)-gcc
CADDSDIRwin64 = ${MLINKDIR}/${SYSwin64}/CompilerAdditions/mldev64
INCDIRwin64   = ${CADDSDIRwin64}/include
LIBDIRwin64   = ${CADDSDIRwin64}/lib

MPREPwin64     = ${CADDSDIRwin64}/bin/mprep.exe

sloadtm.win64.c : sload.tm
	wine ${MPREPwin} $? -o "$@"   ## use 32bit mprep.exe because wine can not handle the 64bit mprep.exe
	sed -i 's# __DATE__# ",__DATE__," ",__TIME__,"#' "$@"

sload.w64.obj : sload.c
	${CCw64} -c -I${INCDIRwin64} "$<" -o "$@"
sloadtm.w64.obj : sloadtm.win64.c
	${CCw64} -c -I${INCDIRwin64} "$<" -o "$@"

sload.w64.exe : sload.w64.obj sloadtm.w64.obj ../win64/libbiosig.a
	${CCw64} sload.w64.obj sloadtm.w64.obj -static-libgcc -static-libstdc++  ../win64/libbiosig.a -L${LIBDIRwin64} -l${MLLIBwin64} ${EXTRALIBSwin} -o "$@"
	# copy to users directory if available
	-cp $@ ../../mma/
	-cp $@ ../../mma/sload-$(shell date +%Y%m%d).win64.exe

	
###########################################
clean:
	-$(RM) *.o
	-$(RM) *.obj
	-$(RM) *tm.c
	-$(RM) sload.exe
	-$(RM) sload 
	
