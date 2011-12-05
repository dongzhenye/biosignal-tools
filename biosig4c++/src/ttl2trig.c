/*

    $Id$
    Copyright (C) Alois Schloegl, IST Austria <alois.schloegl@ist.ac.at>

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



#include <alsa/asoundlib.h>

#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef WITH_BIOSIG
#include "biosig.h" 
extern int VERBOSE_LEVEL;
#else
int VERBOSE_LEVEL=3;
#endif


#define TO_STOP_PRESS_ANY_KEY


#ifdef TO_STOP_PRESS_ANY_KEY
/* copied from http://ubuntuforums.org/showthread.php?t=936816 on 2011-Oct-28 */
#include <termios.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
int is_key_pressed(void)
{
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds); 

	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &fds);
}
#endif 



int main(int argc, const char* argv[]) {

/*********************************************
	read arguments 
**********************************************/
	const char *cmd  = NULL; 
	const char *cmdf = NULL; 
	const char *cmdr = NULL; 
	int k; 
	int rc;
	int size;

	const char *pcm_name=NULL;
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *hwparams;
	int dir;
	snd_pcm_uframes_t frames;
	int16_t *buffer;

	unsigned int Fs = 44100;
	unsigned chan = 0; 		// default channel 
	unsigned winlen = 0; 

	const float WINLEN = 0.001; 	// window lengths for computing steepness: default is 1 ms
	float TH = 0.0/0.0; 
	const char *outFile = NULL; 

#ifdef TO_STOP_PRESS_ANY_KEY
	/* initialization for IS_KEY_PRESSED() */	
	struct termios old_terminal_settings, new_terminal_settings;

	// Get the current terminal settings
	if (tcgetattr(0, &old_terminal_settings) < 0)  perror("tcgetattr()");

	memcpy(&new_terminal_settings, &old_terminal_settings, sizeof(struct termios));

	// disable canonical mode processing in the line discipline driver
	new_terminal_settings.c_lflag &= ~ICANON;

	// apply our new settings
	if (tcsetattr(0, TCSANOW, &new_terminal_settings) < 0) perror("tcsetattr ICANON");
#endif


#ifdef WITH_BIOSIG
	HDRTYPE *hdr = NULL; 
#endif

	const char help[]= 
		"TTL2TRIG reads a signal from an audio channel, in order to trigger the execution of a shell command\n"
                "  Whenever the difference within a distance of 1ms exceeds the threshold, the command string is executed.\n"
 		"  Copyright (C) 2011 Alois Schloegl, IST Austria, <alois.schloegl@ist.ac.at>.\n"
		"  This program is licensed under the GNU GPL v3 or later.\n\n"
		"Usage: ttl2trig -c \"command string\" -i hw --chan=1 --Threshold=+0.5 [-o outfile]\n"
		" -c	\"command string executed on raising edge when TH>0 or on falling edge when TH<0\"\n" 
		" -r	\"command string executed on raising edge when slope becomes larger than abs(TH)\"\n" 
		" -f	\"command string executed on falling edge when slope becomes smaller than -abs(TH)\"\n" 
		" --chan=#	# represents than channel number used for triggering, default 1\n" 
		" -th TH\n" 
		" --threshold=TH\n"
		"	when y( t ) - y(t - 1 ms) becomes larger than +abs(TH), a raising edge is detected,\n" 
		"	when y( t ) - y(t - 1 ms) becomes smaller than -abs(TH), a falling edge is detected,\n" 
#ifdef WITH_BIOSIG
		" -o	outfile, logs the recorded signal data; this can be useful for debugging and for identifying the proper threshold and window length.\n" 
#endif
		" -i	<hwparams> or \n" 
		" --hwparams=<hwparams> where <hwparams> is the input channel like hw:2,0 \n" 
		" The later arguments have higher precedence than the earlier ones.\n" 
		" Specifically -c overrides -r and -f; and -r and -f override -c. \n\n"
		" Example:\n\t./bin/ttl2trig -c \"date\" -i hw:2,0 --chan=0 --Threshold=.25 \n\n" 
	;	
	if (argc<2) {
		fprintf(stdout,"%s",help); 
		exit(0);
	}

	k = 0;
	while (k<argc) {

                if (VERBOSE_LEVEL>3) fprintf(stdout,"%i/%i\t%s\n",k,argc,argv[k]);
                
                if (0) {
		}
		else if (isdigit(argv[k][0]) && ( strstr(argv[k],"hz") || strstr(argv[k],"Hz") ) ) {
			char *tmp;
			Fs = strtod(argv[k],&tmp);
		}
		else if (!strcmp(argv[k],"-c")) {
			cmd  = argv[++k];
			cmdf = NULL; 
			cmdr = NULL; 
		}
		else if (!strcmp(argv[k],"-r")) {
			cmdr = argv[++k];
			cmd  = NULL; 
		}
		else if (!strcmp(argv[k],"-f")) {
			cmdf = argv[++k];
			cmd  = NULL; 
		}
		else if (!strncmp(argv[k],"--chan=",7)) {
			chan = atoi(argv[k]+7);
			if (chan>0) chan--;	// change from one-based to zero-based indexing
		}
		else if (!strncmp(argv[k],"--threshold=",12) || !strncmp(argv[k],"--Threshold=",12) ) {
			TH = atof(argv[k]+12);
		}
		else if ( !strcmp(argv[k],"-th") ) {
			TH = atof(argv[++k]);
		}
		else if (!strncmp(argv[k],"-v",2)) {
			VERBOSE_LEVEL = atoi(argv[k]+2);
		}
		else if (!strcmp(argv[k],"-h") || !strcmp(argv[k],"--help")) {
			fprintf(stdout,"%s",help);
			exit(0);
		}
#ifdef WITH_BIOSIG
		else if (!strcmp(argv[k],"-o")) {
			k++;
			outFile = argv[k];
		}
#endif
		else if (!strcmp(argv[k],"--hwparams=")) {
			pcm_name = argv[k]+11;
		}
		else if (!strcmp(argv[k],"-i")) {
			k++;
			pcm_name = argv[k];
		}
		k++;
	}

	/* Sanity checks of input arguments */
	if ( (TH==0) || (TH != TH) ) {
		fprintf(stderr,"Threshold %g undefined or invalid\n",TH);
//		exit(-1);
	}

	if (VERBOSE_LEVEL>6) fprintf(stdout,"pcm_name:\t%s \n", pcm_name);

	if (VERBOSE_LEVEL>7) {
		int val;

		printf("ALSA library version: %s\n", SND_LIB_VERSION_STR);

		printf("\nPCM stream types:\n");
		for (val = 0; val <= SND_PCM_STREAM_LAST; val++) printf("  %s\n", snd_pcm_stream_name((snd_pcm_stream_t)val));

		printf("\nPCM access types:\n");
		for (val = 0; val <= SND_PCM_ACCESS_LAST; val++) printf("  %s\n", snd_pcm_access_name((snd_pcm_access_t)val));

		printf("\nPCM formats:\n");
		for (val = 0; val <= SND_PCM_FORMAT_LAST; val++)
			if (snd_pcm_format_name((snd_pcm_format_t)val)!= NULL) 
				printf("  %s (%s)\n", snd_pcm_format_name((snd_pcm_format_t)val), snd_pcm_format_description((snd_pcm_format_t)val));

		printf("\nPCM subformats:\n");
		for (val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++) 
			printf("  %s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val), snd_pcm_subformat_description((snd_pcm_subformat_t)val));

		printf("\nPCM states:\n");
		for (val = 0; val <= SND_PCM_STATE_LAST; val++) printf("  %s\n", snd_pcm_state_name((snd_pcm_state_t)val));
	}	



/*********************************************
	initialization
**********************************************/


  /* Open PCM device for recording (capture). */
	rc = snd_pcm_open(&pcm_handle, pcm_name, SND_PCM_STREAM_CAPTURE, 0);
	if (rc < 0) {
		fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
		exit(1);
	}

  /* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&hwparams);

  /* Fill it in with default values. */
	snd_pcm_hw_params_any(pcm_handle, hwparams);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
	snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE);
//	snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_FLOAT_LE );
#ifdef WITH_BIOSIG
	uint16_t gdftyp = 3; 
	double DigMax = ldexp(1,15)-1;
	double PhysMax = 1; 
	TH *= DigMax/PhysMax; 
#endif

  /* Two channels (stereo) */
	unsigned int minChan, maxChan;
	snd_pcm_hw_params_get_channels_min(hwparams, &minChan);
	snd_pcm_hw_params_get_channels_max(hwparams, &maxChan);
	if (VERBOSE_LEVEL>7) printf("chans = [%i %i]\n", minChan, maxChan);

	if ( chan >= minChan) {
		fprintf(stderr,"ERROR: Channel %i not available.\n", chan);
		snd_pcm_close(pcm_handle);
		exit(-1);
	}


	snd_pcm_hw_params_set_channels(pcm_handle, hwparams, minChan);

  /* 44100 bits/second sampling rate (CD quality) */
	snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &Fs, &dir);

  /* Set period size to 32 frames. */
	frames = 1;//Fs*0.01;	// 10 ms 
	winlen = Fs*WINLEN;	// window length 	
//	frames = 2*winlen; 
	snd_pcm_hw_params_set_period_size_near(pcm_handle, hwparams, &frames, &dir);

  /* Write the parameters to the driver */
	rc = snd_pcm_hw_params(pcm_handle, hwparams);
	if (rc < 0) {
		fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
		exit(1);
	}

  /* Use a buffer large enough to hold one period */
	snd_pcm_hw_params_get_period_size(hwparams, &frames, &dir);
	size = frames * 2; /* 2 bytes/sample, 2 channels */
	size = 2 * winlen * minChan * 2; /* 2 bytes/sample, 2 channels */
	buffer = (int16_t *) malloc(size*2);
	for (k=0; k < size; k++) buffer[k] = 0x8000;

#ifdef WITH_BIOSIG
	if (outFile) {
		hdr = constructHDR(minChan,0); 
		hdr->SampleRate = Fs; 
		hdr->SPR     =  1; 
		hdr->NRec    = -1;
		hdr->EVENT.N =  0; 
		for (k = 0; k < hdr->NS; k++) {
			CHANNEL_TYPE *hc = hdr->CHANNEL + k;
			hc->LeadIdCode = 0; 
			strcpy(hc->Label,"chan ");
			hc->Label[6]= k + '0';
			hc->GDFTYP  = gdftyp; 
			hc->SPR     = hdr->SPR; 
			hc->PhysMax = PhysMax; 
			hc->PhysMin =-PhysMax;
			hc->DigMax  = DigMax; 
			hc->DigMin  =-DigMax;
		}
		hdr->FLAG.UCAL = 0; 
		hdr->TYPE      = GDF; 
		hdr->VERSION   = 3.0;
		hdr->FileName  = outFile;
		sopen(outFile, "w", hdr);
		if (VERBOSE_LEVEL>6) hdr2ascii(hdr, stdout, 3);
		if (hdr->FILE.OPEN < 2) {
			destructHDR(hdr); 
			hdr = NULL;
		}
	}
#endif

#ifdef TO_STOP_PRESS_ANY_KEY
	fprintf(stdout, "\n ====== PRESS ANY KEY TO STOP ======\n");
#else
	fprintf(stdout, "\n ====== PRESS <ENTER> TO STOP ======\n");
	// set stdin to non-blocking 
	int flags = fcntl(0, F_GETFL, 0);   /* get current file status flags */
	flags |= O_NONBLOCK;		/* turn off blocking flag */
	fcntl(0, F_SETFL, flags);	/* set up non-blocking read */
#endif

	size_t count = 0, pos = 0;
	float delta, lastdelta = 0.0/0.0;
	while (1) {

		/* stop when key pressed */
#ifdef TO_STOP_PRESS_ANY_KEY
		if (is_key_pressed()) break; 
#else
		char tmpbuf[4];
		if (fgets(tmpbuf, sizeof(tmpbuf), stdin) != NULL) break;
#endif 

		/* read data from alsa */
		int16_t *buf = buffer + pos % size;	
		rc   = snd_pcm_readi(pcm_handle, buf, 1);
		pos += rc * minChan;

		if (rc == -EPIPE) {
			/* EPIPE means overrun */
			fprintf(stderr, "overrun occurred\n");
			snd_pcm_prepare(pcm_handle);
		} 
		else if (rc < 0) {
			fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
		} 

#ifdef WITH_BIOSIG
		if (hdr) {
			/* write data to GDF file */
			count += fwrite(buf, 2*hdr->NS, rc, hdr->FILE.FID);
		}
#else
		count = rc; 
#endif


/*********************************************
	trigger command 	
**********************************************/

		/* compute dY and compare with Threshold */
		delta = buf[chan] - buffer[ (pos - winlen + chan) % size ];
		if ( ( (TH > 0.0) && (delta > TH) && (lastdelta < TH) )
                  || ( (TH < 0.0) && (delta < TH) && (lastdelta > TH) ) ) {
			if (cmd) system(cmd);	
		}
		if ( (cmdr != NULL) && (delta > TH) && (lastdelta < TH)) {
			system(cmdr);	
		}
		if ( (cmdf != NULL) && (delta < -TH) && (lastdelta > -TH) ) {
			system(cmdf);	
		}
		lastdelta = delta; 
	}

	fprintf(stdout, "\n ====== STOPPED ====== \n");

	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(buffer);

/*********************************************
	clean up 
**********************************************/

#ifdef WITH_BIOSIG
	if (hdr) {
		hdr->NRec = count; 	
		sclose(hdr); 
		destructHDR(hdr); 
	}
#endif

}

