function [echo] = ME446_serialwrite_echo_address(address,floatval)
%ME446_SERIALWRITE_ECHO_ADDRESS
%   ME446_serialwrite_echo(ADDRESS,FLOATVAL) will modify a variable on the 
%   DSP at the memory location ADDRESS.  FLOATVAL is the value you want to 
%   change that variable to.  Additionally, the value of the variable
%	on the DSP will be returned to verify the write was successful.
%
%	For example, to change the float variable at the memory location 
%   "0001f5b0" on the DSP to a	value to 5.5 and store the echoed value to 
%   the Matlab single variable "myecho" you would type:
%
%		myecho = ME446_serialwrite_echo_address(128432,5.5);
%

s = instrfind;
if length(s) > 0
    fclose(s);
end
clear s;

filename = dir('../CRSRobotProject/debug/*.map');

map = parseMap(strcat('../CRSRobotProject/debug/',filename.name));

memloc = dec2hex(address,8);

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
echo = ME446_serialread_address(address,1);
