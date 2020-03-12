

fid = fopen('C:\rekoske2\c6713serial_communication\Debug\c6713test.map','r');
name = '.my_arrs';
stop = 'SORTED BY Symbol Address';
start = 0;
length = 0;
i = 1;
error = 0;
memloc = 0;
while 1
    line = fgetl(fid);
    if ~ischar(line)
       break;
    end
    if strfind(line,name)
        parse = strread(line,'%s');
        start = hex2dec(parse(3));
        length = hex2dec(parse(4));
        break;
    end
end
while 1
    error = false;
    line = fgetl(fid);
    if ~ischar(line)
       break;
    end
    if findstr(line,stop)
        break;
    end
    parse = strread(line,'%s');
    if isempty(parse) == 0
        if isHex(parse(1))
            memloc = hex2dec(parse(1));
            if (memloc > (start-1)) && (memloc < (start+length) && (isHex(parse(2)) == 0))
                my_arrs(i,2) = parse(1);
                str = char(parse(2));
                str = str(2:end);
                my_arrs(i,1) = cellstr(str);
                i = i + 1;
            end
        end
    end
end
my_arrs
fclose(fid);