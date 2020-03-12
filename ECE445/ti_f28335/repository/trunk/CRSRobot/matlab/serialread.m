function [out3] = serialread(name,map,varsize)

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

hexsize = dec2hex(varsize,4);
hex_str = '2A0932'; % header
hex_str = strcat(hex_str,memloc,hexsize);
char_str = char(sscanf(hex_str,'%2X').');

s = serial('COM1');
set(s,'BaudRate',115200);
s.InputBufferSize = 5000;
fopen(s);
fwrite(s,char_str)

count = 0;
while 1
    inchar = fread(s,1);
    if inchar == 42
        inchar = fread(s,1);
        inchar = fread(s,1);
        if inchar == 51
            out3 = fread(s,varsize,'float32');
            break;
        end
    end
    count = count + 1;
    if count > 100
        break;
    end
end
        



%verify = fread(
%out3 = fread(s,varsize,'float32');
fclose(s)
delete(s)
clear s