#!/bin/sh 

# $Id$ 
# Copyright (C) 2012 Alois Schloegl
# This file is part of the "BioSig for C/C++" repository
# (biosig4c++) at http://biosig.sf.net/

swig -ruby biosig.i
ruby extconf.rb 
make 

