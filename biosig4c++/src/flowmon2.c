/*
 #  Copyright (C) 2011,2012 Alois Schloegl, IST Austria <alois.schloegl@ist.ac.at>
 #
 #    This program is free software; you can redistribute it and/or modify
 #    it under the terms of the GNU General Public License as published by
 #    the Free Software Foundation; either version 3 of the License, or
 #    (at your option) any later version.
 #
 #    This program is distributed in the hope that it will be useful,
 #    but WITHOUT ANY WARRANTY; without even the implied warranty of
 #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #    GNU General Public License for more details.
 #
 #    You should have received a copy of the GNU General Public License
 #    along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

/*

   Supported Device(s):
     Flow Meter GSM-D3KA-BN00 from Vögtlin Instruments

  DONE(+)/TODO(-):
    - units of flow l/min oder m^3/h ??
    + graceful handling of exit (close all handles even when stopped with <CTRL>-C
    + one file per day, appending
    + autostart
    - file management, data compression
    - init.d (flowmon start/stop)
    - fix appending to *.log.gdf file after restart

   Requirements:
        g++
        libz 1.2.5 or higher
        libbiosig.a (v1.2 or higher) available from http://biosig.sf.net

*/



#include "../biosig-dev.h"

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>


#define LENBUF 300
#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1         //POSIX compliant source
#define FALSE 0
#define TRUE 1


int  wait_flag=TRUE;                     //TRUE while no signal received
//void signal_handler_IO (int status);    //definition of signal handler
char buf[LENBUF+1]  __attribute__ ((deprecated));
uint8_t idata[LENBUF];
uint8_t odata[LENBUF];
char manufacturer[1000]; 

int fd;
FILE *fid=NULL;
FILE *fid2=NULL;
// biosig
HDRTYPE *hdr = NULL;
struct termios oldtio, newtio;       //place for old and new port settings for serial port

void stop() {
        // reset terminal
        if (fd>2) {
                tcsetattr(fd, TCSANOW, &oldtio);
                close(fd);
        }

        // close gdf file
        if (hdr) destructHDR(hdr);

        // close debug file
        if (fid2) fclose(fid2);
}

uint8_t idata[300];
uint8_t odata[300];
const void *outPtr = odata+3; 

/*
        crc16_a001: computers crc of a modbus packet and writes it at the end;
        (remark: its not CCITT's crc16 but uses 0xA001 polynomial) 
        input :
                data  : packet with is crc
                n     : lenght of data without the crc

        ouput : crc16 value 
*/
#if defined(__LITTLE_ENDIAN) 
uint16_t crc16_a001(uint8_t data[], size_t n)
{
        uint8_t flag;
        size_t i,j;
        union {
                uint16_t u16;
                uint8_t  u8[2];
        } crc;

	crc.u16 = 0xffff;
	for (i=0; i<n; i++) {
		crc.u8[0] = crc.u8[0]^data[i];
		for (j=0; j<8; j++) {
			flag = crc.u8[0] & 0x01;
			crc.u16 = crc.u16 >> 1;
			if (flag) crc.u16 = crc.u16^0xa001;
		}
	}

	// write crc to end of data block
        *(uint16_t*)(data+n) = crc.u16;
	return (uint16_t)crc.u16;
}

int read_register(uint8_t slave, uint8_t cmd, uint16_t reg) { 
 
        idata[0] = slave;		// address
        idata[1] = 3; //cmd;	// read - 
        *(uint16_t*)(idata+2) = bswap_16((uint16_t)reg);  // register 
        *(uint16_t*)(idata+4) = bswap_16(0x0004);  // length
        crc16_a001(idata,6); // compute crc and add at and
        tcflush(fd, TCIOFLUSH);
        int c = write(fd,idata,8);
if (VERBOSE_LEVEL>8)  fprintf(stdout,"\n======= SENT %i bytes\n",c);
        usleep(100000);
        c = read(fd,odata,16);
if (VERBOSE_LEVEL>8)  fprintf(stdout,"\n======= RECEIVED %i bytes\n",c);
        if (c<9 || odata[0] != slave) return (-1);

if (VERBOSE_LEVEL==6)
        fprintf(stdout,"\n======= REG:0x%04x RECEIVED %2i bytes U16[2]: 0x%08x %08x %08x %08x F:%10g %10g<%s> ",\
                reg,c,beu32p(odata),beu32p(odata+4),beu32p(odata+8),beu32p(odata+12), \
                (double)bef32p(odata+3), (double)bef32p(odata+7), (char*)(odata+3));

        uint16_t crc=bswap_16(crc16_a001(odata,c-2));
if (VERBOSE_LEVEL>8)
     fprintf(stdout,"crc 0x%04x 0x%04x",bswap_16(crc),beu16p(odata+c-2));
        usleep(10000);

        if (crc!=beu16p(odata+c-2)) return -1;

        return 0; 
}
#else
#error CRC16 function not defined for big endian platform
#endif 


int main(int argc, char *argv[]) {

char *devicename = "/dev/ttyUSB0";
const char *outFile = NULL;
const char *debugFile = NULL;

                  // 00 = NONE, 01 = Odd, 02 = Even, 03 = Mark, 04 = Space
struct timeval tv;
struct timezone tz;
struct tm *tm;
struct tm T;
uint32_t oldDay=0, newDay;
gdf_time gdfTime;
char flag_GZIP = 0;

char logfile[] = "flowmonYYYYMMDD.log.gdf";
char debugfile[] = "flowmonDD.log.txt";


        /***************************************************************************
         *
         *    input arguments
         *
         ***************************************************************************/

	const char help[]=
		"FLOWMON reads data of the flow sensor through the serial terminal and stores it into a data file for archiving.\n"
		"   The current measurement values are also written into the file /var/www/flowsensor.html.\n"
		"   This software supports the device 'Flomw Meter GSM-D3KA-BN00 (BHT Rotary Gas Meter) from Vögtlin Instruments'.\n\n"
		"Usage: flowmon -d devicename [-o outfile] [-D debugfile] [-V#]\n"
		"   devicename: default value is /dev/ttyS0\n"
		"   outfile:    logs the recorded data\n"
                "               If no outfile is provided, the data will be logged into daily files named flowmon<$date>.log.gdf \n"
                "   debugfile:  logs the data in ascii text"
                "               If no outfile is provided, the data will be logged into daily files named flowmon<$day-of-month>.log.txt \n"
                "   -V#		verbose level #=0 is no messages, #=9 is highest level(debugging) messages\n"
                "   -z		save outfile in gzipped format"  
                " \n\n"
	;
	if (argc<2) {
		fprintf(stdout,"%s",help);
//		exit(0);
	}

	/* Sanity checks of input arguments */
	int k = 0;
	while (k<argc) {

                if (VERBOSE_LEVEL>3) fprintf(stdout,"%i/%i\t%s\n",k,argc,argv[k]);

                if (0) {
		}
		else if (!strcmp(argv[k],"-d")) {
			devicename = argv[++k];
		}
		else if (!strcmp(argv[k],"-o")) {
			k++;
			outFile = argv[k];
                }
		else if (!strcmp(argv[k],"-D")) {
			k++;
			debugFile = argv[k];
		}
		else if (!strncmp(argv[k],"-V",2)) {
	                char c = argv[k][2];
#ifndef VERBOSE_LEVEL
	                if ('0'<=c && c<='9') 
	                        VERBOSE_LEVEL = c-'0';
#endif
		}
		else if (!strcmp(argv[k],"-z")) {
		        flag_GZIP = 1;
		}
		k++;
	}

        /***************************************************************************
         *
         *    initialization
         *
         ***************************************************************************/ 

        // clean up at exit
        atexit(&stop);

        gettimeofday(&tv, &tz);
        tm = gmtime(&tv.tv_sec);
        gdfTime = tm_time2gdf_time(gmtime(&tv.tv_sec)) + (uint64_t)ldexp(tv.tv_usec*1e-6/(24*3600),32);
        newDay = gdfTime>>32;
        gdf_time2tm_time_r(gdfTime, &T);


	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 010\n");

        //open the device(com port) to be non-blocking (read will return immediately)
        fd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (fd < 0) {
                perror(devicename);
                exit(EXIT_FAILURE);
        }
        tcgetattr(fd,&oldtio); // save current port settings


	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 020\n");

        // set new port settings for canonical input processing
        // newtio.c_cflag = BAUD | CRTSCTS | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
        newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | CSTOPB;
        newtio.c_iflag = IGNPAR;
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;       //ICANON;
        newtio.c_cc[VMIN] = 1;
        newtio.c_cc[VTIME]= 0;
        tcflush(fd, TCIOFLUSH);
        tcsetattr(fd,TCSANOW,&newtio);

if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 020\n");


#if 1
	if (debugFile)
	        fid2 = fopen(debugFile,"a");
        else {
		sprintf(debugfile,"flowmon%02d.log.txt",tm->tm_mday);
                fid2 = fopen(debugfile,"a");
        }


	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 031\n");

        {
	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 032\n");
		hdr = constructHDR(4 ,0);
	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 035\n");
		hdr->SampleRate = 1;
		hdr->SPR     =  1;
		hdr->NRec    = -1;
		hdr->EVENT.N =  0;
		hdr->FILE.COMPRESSION = 0;
	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 042\n");
                {
                        // channel 1: time stamp
			CHANNEL_TYPE *hc = hdr->CHANNEL + 0;
			hc->LeadIdCode = 0;
			strcpy(hc->Label,"time ");
			hc->GDFTYP  = 8;	// uint64
			hc->SPR     = hdr->SPR;
	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 044\n");
			hc->PhysMax = ldexp(1,32);
			hc->PhysMin = 0;
			hc->DigMax  = ldexp(1,64);
			hc->DigMin  = 0;
                        hc->PhysDimCode = PhysDimCode("d"); 	// days
                        hdr->AS.bpb += GDFTYP_BITS[hc->GDFTYP]>>3;
		}

	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 046\n");

                {
                        // channel 2: volume
			CHANNEL_TYPE *hc = hdr->CHANNEL + 1;
			hc->LeadIdCode = 0;
			strcpy(hc->Label,"total volume ");
			hc->GDFTYP  = 6;        // uint32
			hc->GDFTYP  = 16;        // float32
			hc->SPR     = hdr->SPR;
			hc->PhysMax =  ldexp(1,32);
			hc->PhysMin = -hc->PhysMax;
			hc->DigMax  = hc->PhysMax;
			hc->DigMin  = hc->PhysMin;
                        hdr->AS.bpb += GDFTYP_BITS[hc->GDFTYP]>>3;

                        read_register(247, 3, 0x6384) ; // f32: Skalierung Totalisator, and s8: Einheit des Totalisatorwertes mit 4 Byte offset. 
                        //read_register(247, 3, 0x6386) ; // s8: Einheit des Totalisatorwertes
                        if (!strcmp((char*)(outPtr+4),"ln"))
                                hc->PhysDimCode = PhysDimCode("l");
                        else 
                                hc->PhysDimCode = 0; // "unknown"
                        
		}
                {
                        // channel 3: flow
			CHANNEL_TYPE *hc = hdr->CHANNEL + 2;
			hc->LeadIdCode = 0;
			strcpy(hc->Label,"flow ");
			hc->GDFTYP  = 3;        // int16
			hc->GDFTYP  = 16;        // float32
			hc->SPR     = hdr->SPR;
                        hdr->AS.bpb += GDFTYP_BITS[hc->GDFTYP]>>3;

                        read_register(247, 3, 0x6020) ; // f32: Endwert Messbereich
                        hc->PhysMax = bef32p(outPtr);
                        hc->PhysMin = -hc->PhysMax;
                        hc->DigMax  = hc->PhysMax;
                        hc->DigMin  = hc->PhysMin;
                        read_register(247, 3, 0x6022) ; // s50: Bezeichnung Medium Lang
                        strncpy(hc->Transducer,(char*)(outPtr+1),MAX_LENGTH_TRANSDUCER);
                        read_register(247, 3, 0x6042) ; // s8: Bezeichnung Medium kurz
                        strncpy(hc->Transducer,(char*)(outPtr),MAX_LENGTH_LABEL);
                        
                        read_register(247, 3, 0x6046) ; // s8: Einheit Messwert
                        if (!strcmp((char*)outPtr,"ln/h")) {
                                //hc->PhysDimCode = 3072; // "l min-1"
                                hc->PhysDimCode = 3104; // "l h-1"
                        }        
                        else 
                                hc->PhysDimCode = 0; // "unknown"
                                
                        // read_register(247, 3, 0x6046) ; // s8: Einheit Messwert

                        read_register(247, 3, 0x6120) ; // u16: Verstärkung, Amplification; und u16 Heizleistung (2 byte offset), und u16: Dynamik (4 Byte offset) 
                        //read_register(247, 3, 0x6121) ; // u16: Heizleistung
                        //read_register(247, 3, 0x6122) ; // u16: Dynamik
		}
                {
                        // channel 4: temparature
			CHANNEL_TYPE *hc = hdr->CHANNEL + 3;
			hc->LeadIdCode = 0;
			strcpy(hc->Label,"Temperature");
			hc->GDFTYP  = 16;        // float32
			hc->SPR     = hdr->SPR;
			hc->PhysMax = 500;
			hc->PhysMin = -500;
			hc->DigMax  = 500;
			hc->DigMin  = -500;
                        hc->PhysDimCode = 6048; // degree Celsius
                        hdr->AS.bpb += GDFTYP_BITS[hc->GDFTYP]>>3;
		}
                if (hdr->NS > 4) {
                        // channel 5: type of gas
			CHANNEL_TYPE *hc = hdr->CHANNEL + 4;
			hc->LeadIdCode = 0;
			strcpy(hc->Label,"typ of gas");
			hc->GDFTYP  = 2;        // uint8
			hc->SPR     = hdr->SPR;
			hc->PhysMax = 255;
			hc->PhysMin = 0;
			hc->DigMax  = 255;
			hc->DigMin  = 0;
                        hc->PhysDimCode = 0;
                        hdr->AS.bpb += GDFTYP_BITS[hc->GDFTYP]>>3;
		}
		for (k=0; k<hdr->NS; k++) {
		      CHANNEL_TYPE *hc = hdr->CHANNEL + k;
		      hc->LeadIdCode = 0;
		      hc->SPR = 1;
		}

		hdr->AS.rawdata = (uint8_t*)realloc(hdr->AS.rawdata,hdr->AS.bpb);

	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 017\n");

		hdr->ID.Manufacturer.Name  = "Voegtlin";
/*
		hdr->ID.Manufacturer.Model = "Flow Meter";
		hdr->ID.Manufacturer.Version = "GSM-D3KA-BN00";
		hdr->ID.Manufacturer.SerialNumber = "150427";
*/
                read_register(247, 3, 0x0023) ;	// Typencode 1: Gerätebezeichnung Teil 1
                hdr->ID.Manufacturer.Model = (char*)manufacturer;
                strcpy(manufacturer,"FlowMeter ");
                strcat(manufacturer, outPtr);
                read_register(247, 3, 0x1004) ; // Typencode 2: Gerätebezeichnung Teil 2
                strcat(manufacturer, outPtr);
        
                read_register(247, 3, 0x0020);	// u16: Hardware Version number,  
                uint16_t HardWareVersion = beu16p(outPtr); 
                // read_register(247, 3, 0x0021);  // u16: Software Version number: is available also from previous request with 2 byte offset
                uint16_t SoftWareVersion = beu16p(outPtr+2); // u16: Software Version number 
                hdr->ID.Manufacturer.Version = manufacturer+strlen(manufacturer)+1;
                sprintf((char*)(hdr->ID.Manufacturer.Version),"Hardware: %d.%d.%d Software: %d.%d.%d", \
                        HardWareVersion>>8, \
                        (HardWareVersion & 0xF0)>>4, \
                        HardWareVersion & 0x0F, \
                        SoftWareVersion>>8, \
                        (SoftWareVersion & 0xF0)>>4, \
                        SoftWareVersion & 0x0F \
                        );
        
                read_register(247, 3, 0x001e) ; // u32: serial number 
                hdr->ID.Manufacturer.SerialNumber = hdr->ID.Manufacturer.Version+strlen(hdr->ID.Manufacturer.Version)+1;
                sprintf((char*)(hdr->ID.Manufacturer.SerialNumber),"%d",beu32p(outPtr)); 
        

	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 018\n");

		        
		hdr->FLAG.UCAL = 1;
		hdr->TYPE      = GDF;
		hdr->VERSION   = 2.0;
		if (outFile) hdr->FileName  = strdup(outFile);


	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 019\n");

        }
#endif

        read_register(247, 3, 0x0000) ;	// float32: Messwert Gasdurchfluss
        read_register(247, 3, 0x0002) ; // float32: Temperatur
        // read_register(247, 3, 0x0006) ; // float32: Sollwert Gasdurchfluss
        // read_register(247, 3, 0x0008) ;	// float32: Messwert Analogeingang
         
        read_register(247, 3, 0x000c) ; // u16: Alarmmeldungen
        read_register(247, 3, 0x000d) ; // u16: Hardwarefehler
        //read_register(247, 3, 0x000e) ; // u16: Regelmode
        //read_register(247, 3, 0x000f) ; // u16: Ramp
                                 
         
        read_register(247, 3, 0x0013) ; // device address: default 247 (0xf7)
        //read_register(247, 3, 0x0020) ; 
        //read_register(247, 3, 0x0021) ; 

        // read_register(247, 3, 0x5200) ; // u16: baud rate
        
        
        read_register(247, 3, 0x0004) ;	// f32: Totalisator 1
        read_register(247, 3, 0x6380) ; // f32: Totalisator 1, and f32 Totalisator 2 with 4 Byte offset. 
        //read_register(247, 3, 0x6382) ; // f32: Totalisator 2 - nicht rückstellbar
        read_register(247, 3, 0x6384) ; // f32: Skalierung Totalisator, and s8: Einheit des Totalisatorwertes mit 4 Byte offset. 
        //read_register(247, 3, 0x6386) ; // s8: Einheit des Totalisatorwertes
        

        if (outFile) {
                // open once write all data into single log file
                hdr->FILE.COMPRESSION = flag_GZIP;
		hdr = sopen(outFile, "a", hdr);
		if (VERBOSE_LEVEL>7) hdr2ascii(hdr,stdout,4);

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 010 %i\n", (int)hdr->NRec);

		if (serror2(hdr)) {
			destructHDR(hdr); 
			return(EXIT_FAILURE);
		}

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 020\n");

//		if (VERBOSE_LEVEL>6) hdr2ascii(hdr, stdout, 3);
		if (hdr->FILE.OPEN < 2) {
			destructHDR(hdr);
			hdr = NULL;
			fprintf(stderr,"Could not open output file  %s\n", outFile);
			exit(-1);
		}

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 030\n");

		hdr->AS.rawdata = (uint8_t*)realloc(hdr->AS.rawdata,hdr->AS.bpb);
		if (hdr->NRec < 0) hdr->NRec = 0;
	}

        /***************************************************************************
         *
         *    processing: data is continuosly read from serial interface and written to log and debug file
         *
         ***************************************************************************/

        fid = fdopen(fd, "r+");
        while (1) {
                /***
                        get data
                ***/

                if (read_register(247, 3, 0x0004)) continue;	// f32: Totalisator 1
                float TotalVolume = bef32p(outPtr); 

                if (read_register(247, 3, 0x0000)) continue;	// float32: Messwert Gasdurchfluss, and f32: Temperatur with 4 byte offset
                //read_register(247, 3, 0x0002) ; // float32: Temperatur
                float Flow = bef32p(outPtr); 
                float Temperature = bef32p(outPtr+4); 

                /*
                if (read_register(247, 3, 0x6380)) continue;	// f32: Totalisator 2
                float TotalVolume2 = bef32p(outPtr); 
                float TotalVolume3 = bef32p(outPtr+4); 
                */

                gettimeofday(&tv, &tz);
                tm = gmtime(&tv.tv_sec);
                gdfTime = tm_time2gdf_time(gmtime(&tv.tv_sec)) + (uint64_t)ldexp(tv.tv_usec*1e-6/(24*3600),32);
                newDay = gdfTime>>32;
                gdf_time2tm_time_r(gdfTime, &T);
                if ( (newDay != oldDay) && !outFile) {
                        // open/close daily log file  

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 110\n");

                        sclose(hdr);
			hdr->NRec = -1;
                        sprintf(logfile,"flowmon%04i%02i%02i.log.gdf",T.tm_year+1900,T.tm_mon+1,T.tm_mday);
                        hdr->FILE.COMPRESSION = flag_GZIP;
                        hdr = sopen(logfile, "a", hdr);

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 120 %p\n", hdr);

                        if (serror2(hdr)) {
				destructHDR(hdr); 
				return(EXIT_FAILURE);
                        }

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 125 %i\n", (int)hdr->NRec);

                        if (!hdr->FILE.OPEN) {
				destructHDR(hdr);
				hdr = NULL;
        			fprintf(stderr,"Could not open output file  %s\n", logfile);
			}

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 130\n");

			hdr->AS.rawdata = (uint8_t*)realloc(hdr->AS.rawdata,hdr->AS.bpb);
			if (hdr->NRec<0) hdr->NRec = 0;
                }

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 140\n");

                if ( (newDay != oldDay) && !debugFile) {
                        // open/close daily debug file
			if (fid2>0) fclose(fid2);
			sprintf(debugfile,"flowmon%02d.log.txt",T.tm_mday);
			fid2 = fopen(debugfile,"a");
                }

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 150\n");

                if (newDay != oldDay) {
                        oldDay = newDay;
                };


                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 190\n");

                /***
                        parse data
                ***/

                memcpy(hdr->AS.rawdata+hdr->CHANNEL[0].bi,&gdfTime,8);
                *(float*)(hdr->AS.rawdata+hdr->CHANNEL[1].bi)=TotalVolume;
                *(float*)(hdr->AS.rawdata+hdr->CHANNEL[2].bi)=Flow;
                *(float*)(hdr->AS.rawdata+hdr->CHANNEL[3].bi)=Temperature;

                if (hdr) {

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 230 %i, %i, %i\n", (int)hdr->NRec, hdr->AS.bpb, hdr->FILE.OPEN);
                if (VERBOSE_LEVEL>8) fprintf(stdout,"FLOWMON 240 %g, %g, %g\n", TotalVolume,Flow,Temperature);

                        hdr->NRec += ifwrite(hdr->AS.rawdata, hdr->AS.bpb, 1, hdr);
                        ifflush(hdr);

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 290 %i\n", (int)hdr->NRec);

			/*******************************************************************
			 * Write HTML file 
			 *******************************************************************/
                        char buf[100];
                        strftime(buf,100,"%Y-%m-%d %H:%M:%S UTC",tm);
                        FILE *fid3 = fopen("/var/www/flowsensor.html","w");
                        if (fid3 != NULL) {
                                fprintf(fid3,"<html><body><h1>Carbogen FlowSensor</h1><br><pre>Time:\t\t%s\n",buf);
                                fprintf(fid3,"Manufacturer:\t%s \nModel:\t\t%s\nSerialNo:\t%s\nVersion:\t%s\n",hdr->ID.Manufacturer.Name, hdr->ID.Manufacturer.Model,hdr->ID.Manufacturer.SerialNumber,hdr->ID.Manufacturer.Version);
                                fprintf(fid3,"Total Volume:\t%10.3f  l_n\n",TotalVolume);
                                //fprintf(fid3,"Total Volume 2:\t%10.3f  l_n (nicht ruecksetzbar)\n",TotalVolume2);
                                //fprintf(fid3,"Total Volume -:\t%10.3f  l_n (nicht ruecksetzbar)\n",TotalVolume3);
                                fprintf(fid3,"Flow:\t\t %10.4f l_n/h\n",Flow);
                                fprintf(fid3,"Temperature:\t%6.3f C\n</pre><META HTTP-EQUIV=\"refresh\" CONTENT=\"1\"></body></html>",Temperature);
                                fclose(fid3);
                        }
                }

        }
        stop();

return(0);
}



