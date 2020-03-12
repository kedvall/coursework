function [echo] = ME446_serialwrite_echo(name,floatval)
%ME446_SERIALWRITE_ECHO
%   ME446_serialwrite_echo(NAME,FLOATVAL) will modify a variable on the DSP
%   where NAME is the name of the variable on the DSP, and FLOATVAL is the 
%   value you want to change that variable to.  Additionally, the value of 
%   the variable on the DSP will be returned to verify the write was 
%   successful.
%
%	For example, to change the float variable "myfloat" on the DSP to a
%	value to 5.5 and store the echoed value to the Matlab single variable
%	"myecho" you would type:
%
%		myecho = ME446_serialwrite_echo('myfloat',5.5);
%

s = instrfind;
if length(s) > 0
    fclose(s);
end
clear s;

filename = dir('../CRSRobotProject/debug/*.map');

map = parseMap(strcat('../CRSRobotProject/debug/',filename.name))

memloc = 0;
arrsize = size(map);
found = 1;

for i=1:arrsize(1)
    if strfind(char(map(i,1)),name)
        memloc = char(map(i,2));
        found = 0;
    end
end
if (found == 1)
    exception = MException('MATLAB:VarNotFound','Variable name not found.');
    throw(exception);
end

hex_str = '2A0B33'; % header
hex_str = strcat(hex_str,memloc);
char_str = char(sscanf(hex_str,'%2X').');
s = serial('COM1');
set(s,'BaudRate',115200);
s.InputBufferSize = 5000;
fopen(s);
fwrite(s,char_str);
fwrite(s,floatval,'float32');
fclose(s)
delete(s)
clear s
echo = ME446_serialread(name,1);
