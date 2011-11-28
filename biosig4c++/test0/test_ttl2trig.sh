#!/bin/bash
# This file is for testing ttl2trig

# Copyright 2011 Alois Schloegl, IST Austria <alois.schloegl@ist.ac.at>

../bin/ttl2trig -c "date" -i hw:2,0 --chan=0 --Threshold=.25 
