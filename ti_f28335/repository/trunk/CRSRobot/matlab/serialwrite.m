function [echo] = serialwrite(name,map,floatval)

s = instrfind;
if length(s) > 0
    fclose(s);
end
clear s;

memloc = 0;
arrsize = size(map);
found = 1;

for i=1:arrsize(1)
    if strfind(char(map(i,1)),name)
        memloc = char(map(i,2))
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
echo = serialread(name,map,1);
