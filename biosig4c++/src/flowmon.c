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
   Device supported:   
        VPFlowMate inline from VPInstruments

  DONE(+)/TODO(-):
    + units of flow l/min oder m^3/h ??
    + graceful handling of exit (close all handles even when stopped with <CTRL>-C
    + one file per day, appending
    + autostart
    - file management, data compression
    - init.d (flowmon start/stop) 
    - fix appending to *.log.gdf file after restart
    - configure serial number, type etc. 
    + graceful handling of exit (close all handles even when stopped with <CTRL>-C
    + one file per day
*/


#include "biosig-dev.h"

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


#define LENBUF 100
#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1         //POSIX compliant source
#define FALSE 0
#define TRUE 1


int  wait_flag=TRUE;                     //TRUE while no signal received
void signal_handler_IO (int status);    //definition of signal handler
char buf[LENBUF+1];


int fd;
FILE *fid=NULL;
FILE *fid2=NULL;
// biosig
HDRTYPE *hdr = NULL;
struct termios oldtio, newtio;       //place for old and new port settings for serial port

void stop() {
        // reset terminal
        if ( (fd>2) && memcmp(&oldtio,&newtio,sizeof(oldtio)) ) tcsetattr(fd,TCSANOW,&oldtio);
        if (fid) fclose(fid);

        // close gdf file
        if (hdr) destructHDR(hdr);

        // close debug file
        if (fid2) fclose(fid2);
}


int main(int argc, char *argv[]) {

const char *devicename = "/dev/ttyS0";
const char *outFile = NULL;
const char *debugFile = NULL;
struct sigaction saio;               //definition of signal action
long BAUD;                      // derived baud rate from command line
long DATABITS;
long STOPBITS;
long PARITYON;
long PARITY;
int Data_Bits = 8;              // Number of data bits
int Stop_Bits = 1;              // Number of stop bits
int Parity = 0;                 // Parity as follows:
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
		"  This software supports the device 'VPFlowMate inline from VPInstruments'.\n\n"
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
	                if ('0'<=c && c<='9') 
	                        VERBOSE_LEVEL = c-'0';        
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

	if (debugFile) 
	        fid2 = fopen(debugFile,"a");
        else {
		sprintf(debugfile,"flowmon%02d.log.txt",tm->tm_mday);
                fid2 = fopen(debugfile,"a");
        }
        
        {
		hdr = constructHDR(4,0);
		hdr->SampleRate = 1;
		hdr->SPR     =  1;
		hdr->NRec    = -1;
		hdr->EVENT.N =  0;
		hdr->FILE.COMPRESSION = 0;
                {
                        // channel 0: time stamp
			CHANNEL_TYPE *hc = hdr->CHANNEL + 0;
			hc->LeadIdCode = 0;
			strcpy(hc->Label,"time ");
			hc->GDFTYP  = 8;	// uint64
			hc->SPR     = hdr->SPR;
			hc->PhysMax = ldexp(1,32);
			hc->PhysMin = 0;
			hc->DigMax  = ldexp(1,64);
			hc->DigMin  = 0;
                        hc->PhysDimCode = PhysDimCode("d"); 	// days
                        hdr->AS.bpb += GDFTYP_BITS[hc->GDFTYP]>>3;
		}
                {
                        // channel 1: volume
			CHANNEL_TYPE *hc = hdr->CHANNEL + 1;
			hc->LeadIdCode = 0;
			strcpy(hc->Label,"total volume ");
			hc->GDFTYP  = 6;        // uint32
			hc->SPR     = hdr->SPR;
			hc->PhysMax = ldexp(1,32);
			hc->PhysMin = 0;
			hc->DigMax  = ldexp(1,32);
			hc->DigMin  = 0;
                        hc->PhysDimCode = PhysDimCode("l");
                        hdr->AS.bpb += GDFTYP_BITS[hc->GDFTYP]>>3;
		}
                {
                        // channel 2: flow
			CHANNEL_TYPE *hc = hdr->CHANNEL + 2;
			hc->LeadIdCode = 0;
			strcpy(hc->Label,"flow ");
			hc->GDFTYP  = 3;        // int16
			hc->SPR     = hdr->SPR;
			hc->PhysMax = (ldexp(1,15)-1)/10;
			hc->PhysMin =-ldexp(1,15)/10;
			hc->DigMax  = ldexp(1,15)-1;
			hc->DigMin  =-ldexp(1,15);
                        hc->PhysDimCode = 3072; // "l min-1"
                        hdr->AS.bpb += GDFTYP_BITS[hc->GDFTYP]>>3;
		}
                {
                        // channel 3: type of gas
			CHANNEL_TYPE *hc = hdr->CHANNEL + 3;
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
		hdr->ID.Manufacturer.Name  = "VPInstruments";
		hdr->ID.Manufacturer.Model = "VPFlowMate";
		hdr->ID.Manufacturer.Version = "VPF-R0030-M050-D1-S110-E200";
		hdr->ID.Manufacturer.SerialNumber = "103569";
		hdr->FLAG.UCAL = 1;
		hdr->TYPE      = GDF;
		hdr->VERSION   = 2.22;
		hdr->FileName  = outFile;
        }

        if (outFile) {
                // open once write all data into single log file
                hdr->FILE.COMPRESSION = flag_GZIP;
		hdr = sopen(outFile, "a", hdr);
//		hdr2ascii(hdr,stdout,4);

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 010 %i\n", (int)hdr->NRec);

		if (serror()) {
		      fprintf(stderr,"%s (%i)\n",B4C_ERRMSG,B4C_ERRNUM);
		      return(-1);
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

	if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 090\n");

        //open the device(com port) to be non-blocking (read will return immediately)
        fd = open(devicename, O_RDWR | O_NOCTTY);
        if (fd < 0) {
                perror(devicename);
                exit(EXIT_FAILURE);
        }

        //install the serial handler before making the device asynchronous
        saio.sa_handler = signal_handler_IO;
        sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;
        saio.sa_flags = 0;
        saio.sa_restorer = NULL;
        sigaction(SIGIO,&saio,NULL);

        // allow the process to receive SIGIO
        fcntl(fd, F_SETOWN, getpid());
        // Make the file descriptor asynchronous (the manual page says only
        // O_APPEND and O_NONBLOCK, will work with F_SETFL...)
        fcntl(fd, F_SETFL, FASYNC);

        tcgetattr(fd,&oldtio); // save current port settings
        // set new port settings for canonical input processing
        // newtio.c_cflag = BAUD | CRTSCTS | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
        newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
        newtio.c_iflag = IGNPAR;
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;       //ICANON;
        newtio.c_cc[VMIN] = 1;
        newtio.c_cc[VTIME]= 0;
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd,TCSANOW,&newtio);

        /***************************************************************************
         *
         *    processing: data is continuosly read from serial interface and written to log and debug file
         *
         ***************************************************************************/

        fid = fdopen(fd, "r");
        double data[4];
        while (1) {
                /***
                        get data
                ***/
                fgets(buf, LENBUF, fid);
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
			hdr->CHANNEL[2].PhysDimCode = 2976; // make sure stored unit is in "m3/h"
                        sprintf(logfile,"flowmon%04i%02i%02i.log.gdf",T.tm_year+1900,T.tm_mon+1,T.tm_mday);
                        hdr->FILE.COMPRESSION = flag_GZIP;
                        hdr = sopen(logfile, "a", hdr);

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 120 %p\n", hdr);

                        if (serror()) {
                                fprintf(stderr,"%s (%i)\n",B4C_ERRMSG,B4C_ERRNUM);
                                return(-1);
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
                size_t i=0,pos[4];
                pos[0]=0;
		pos[1]=0;
                pos[2]=0;
                pos[3]=strlen(buf);
                while (i<strlen(buf)) {
                        if ( (uint8_t)buf[i+1]==0x47 && (uint8_t)buf[i]==0xfe )
                        switch ( *(uint16_t*)(buf+i+2) ) {
                        case 0x0101: pos[0] = i; break;
                        case 0x0108: pos[1] = i; break;
                        case 0x0201: pos[2] = i; break;
                        }
                        i++;
                }


                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 210\n");


                buf[pos[1]] = 0;
                buf[pos[2]] = 0;
                buf[pos[3]-2] = 0;
//                fprintf(stdout,"%i\t%x\t%c\t%i\t%i\t%i\n",i, (uint8_t)buf[i], buf[i],pos[0],pos[1],pos[2]);
                if (pos[2] > 0) {
                        fprintf(fid2,"|%s|\t|%s|\t|%s|\n",buf+4,buf+pos[1]+4,buf+pos[2]+4);
                        // volume:  data[1] = atof(&(buf[pos[2]+4]));
                        // flow:    data[2] = atof(buf+4)*10;
                        char *t = strchr(buf+4,'.'); t[0]=t[1]; t[1]='.'; // shift decimal point by one digit, multiply by 10
                        // uint16_t flow10 = atol(buf+4);
                        // gas:    data[3] = buf[pos[1]+4];

                        memcpy(hdr->AS.rawdata+hdr->CHANNEL[0].bi,&gdfTime,8);
                        *(uint32_t*)(hdr->AS.rawdata+hdr->CHANNEL[1].bi) = (uint32_t)atol(buf+pos[2]+4);
                        *(uint16_t*)(hdr->AS.rawdata+hdr->CHANNEL[2].bi) = (uint16_t)atof(buf+4);
                        *(uint8_t *)(hdr->AS.rawdata+hdr->CHANNEL[3].bi) = (uint8_t)buf[pos[1]+4];

                /***
                        write data
                ***/
                        fprintf(fid2,"%04d-%02d-%02d %02d:%02d:%02d.%02d \t%u\t%lu\n",tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec, data[2], data[1]);
                        fprintf(fid2,"%f\t%u\t%u\t%u\n",ldexp(gdfTime,-32),*(uint32_t*)(hdr->AS.rawdata+hdr->CHANNEL[1].bi),*(uint16_t*)(hdr->AS.rawdata+hdr->CHANNEL[2].bi),*(uint8_t *)(hdr->AS.rawdata+hdr->CHANNEL[3].bi));
                        //if (hdr) swrite(data,1,hdr);

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 220\n");

                        if (hdr) {

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 230 %i, %i, %i\n", (int)hdr->NRec, hdr->AS.bpb, hdr->FILE.OPEN);

                                hdr->NRec += ifwrite(hdr->AS.rawdata, hdr->AS.bpb, 1, hdr);
                                ifflush(hdr);

                if (VERBOSE_LEVEL>7) fprintf(stdout,"FLOWMON 290 %i\n", (int)hdr->NRec);

                        }
                }
        }
        stop();

return(0);
}



/***************************************************************************
* signal handler. sets wait_flag to FALSE, to indicate above loop that     *
* characters have been received.                                           *
***************************************************************************/

void signal_handler_IO (int status)
{
//      printf("received SIGIO signal.\n");
        wait_flag = FALSE;
}

