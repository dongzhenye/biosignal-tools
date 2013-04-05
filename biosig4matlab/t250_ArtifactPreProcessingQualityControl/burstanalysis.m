function [HDR, s] = burstanalysis(fn, varargin)
% BURSTANALYSIS extracts additional parameters from detection bursts 
% including:
%	Peak Amplitude
%	Peak time
%	Minimum after peak
%	Maximum of Minima after Peak
%
% usage: 
%	[HDR, s] = burstanalysis(fn)
%	...        burstanalysis(fn, args)
%	optional args for spike2bursts conversion		
%
%	fn   name of file containing sample data and 
%	visually corrected spike detections. The spike2burst 
%	conversion 
%
% see also: DETECT_SPIKE_BURSTS, SPIKE2BURSTS

%    Copyright (C) 2013 by Alois Schloegl <alois.schloegl@gmail.com>	
%    This is part of the BIOSIG-toolbox http://biosig.sf.net/
%
%    BioSig is free software: you can redistribute it and/or modify
%    it under the terms of the GNU General Public License as published by
%    the Free Software Foundation, either version 3 of the License, or
%    (at your option) any later version.
%
%    BioSig is distributed in the hope that it will be useful,
%    but WITHOUT ANY WARRANTY; without even the implied warranty of
%    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%    GNU General Public License for more details.
%
%    You should have received a copy of the GNU General Public License
%    along with BioSig.  If not, see <http://www.gnu.org/licenses/>.


verbose = 0; 	% 1 for visualizing results, 0: no visualization 

if (1)
	[HDR, s] = detect_spikes_bursts(fn);
else
	%% use visual scoring 
	[s, HDR] = sload(fn);
end;
[HDR] = spikes2bursts(HDR, varargin);

%hdr2ascii(HDR);

ix201 = find(HDR.EVENT.TYP==hex2dec('0201'));
ix202 = find(HDR.EVENT.TYP==hex2dec('0202'));
A = repmat(nan,length(ix201),6);
B = repmat(nan,length(ix202),4);
m = 0;
for i = 1:length(ix202),
	u  = s(HDR.EVENT.POS(ix202(i)) + [ 0 : HDR.EVENT.DUR(ix202(i)) ], HDR.EVENT.CHN(ix202(i)) );
	t0 = HDR.EVENT.POS(ix201) - HDR.EVENT.POS(ix202(i));
	t0 = t0(0 <= t0 & t0 <= HDR.EVENT.DUR(ix202(i)));
	t0(end+1) = HDR.EVENT.DUR(ix202(i));
	if (verbose>0) clf; plot(u); end; 
	B(i,1) = i;
	B(i,3) = HDR.EVENT.POS(ix202(i));
	B(i,2) = length(t0)-1;
	for k  = 1:length(t0)-1,
		m = m + 1;	
		[tmp, T1] = max(u(t0(k)+1:t0(k+1)));
		[tmp, T2] = min(u(t0(k)+T1:t0(k+1)));
		A(m,1:2) = [i,k];
		%A(m,3) = T1 + t0(k) + HDR.EVENT.POS(ix202(i)); 	% peak time 
		A(m,3) = T1 + t0(k); 		% peak time
		A(m,4) = u(T1 + t0(k)); 	% peak amplitude

		if (k >= length(t0)-1); break; end

		A(m,5) = A(m,3) + T2 - 1;	% time of minimum after peak
		A(m,6) = u(T2 + T1 + t0(k)-1); 	% minimum after peak
		if (isnan(B(i,4)) || (B(i,4) < A(m,6)) )
			B(i, 4) = A(m, 6);
		end;
		if (verbose>0)
			disp(A(m,:));
			hold on 
			plot(T1 + t0(k), u(T1 + t0(k)),'r+');
			plot(T2 + T1 + t0(k)-1, u(T2 + T1 + t0(k)-1),'g+');
		end; 
		%A(m,:) = [i, T1+t0(k)+HDR.EVENT.POS(ix201(i)), T2+T1+t0(k)+HDR.EVENT.POS(ix201(i))];
	end
	if (verbose>0) pause; end; 
end;
A   = A(1:m,:);


% 0x0201 Spike 
% 0x0202 Burst
HDR.EVENT.Desc{10} = 'Peak';
HDR.EVENT.Desc{11} = 'Minimum after peak';
HDR.EVENT.Desc{12} = 'Maximum of minima after Peak';
HDR.EVENT.Desc{13} = 'Burst onset from triangulation';

POS10 = B(A(:,1),3) + A(:,3);
TYP10 = repmat(10,size(A,1),1);

POS11 = B(A(:,1),3) + A(:,5);
TYP11 = repmat(11,size(A,1),1);

POS12 = B(:,4);
TYP12 = repmat(12,size(B,1),1);



A(:,[3,5]) = A(:,[3,5])/HDR.SampleRate;
B(:,[3]) = B(:,3)/HDR.SampleRate;
fid = fopen([HDR.FILE.Name,'.spikepeak.csv'],'w'); 
fprintf(fid,'burst#\tspike#\tpeak time\tpeak amplitude\ttime of minimum after peak\tminimum after peak\n');
fprintf(fid,'%i\t%i\t%f\t%f\t%f\t%f\n',A');
if fid > 2, fclose(fid); end; 

fid = fopen([HDR.FILE.Name,'.burstpeak.csv'],'w'); 
fprintf(fid,'\n\n\nburst#\tnumberOfSpikes\tonset\tmaximum of minima after peak\n');
fprintf(fid,'%i\t%i\t%f\t%f\n',B');
if fid > 2, fclose(fid); end; 


