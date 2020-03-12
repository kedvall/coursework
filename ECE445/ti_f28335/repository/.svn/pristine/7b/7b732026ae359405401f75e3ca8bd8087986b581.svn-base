function [my_arrs] = parseMap(maploc)
fid = fopen(maploc,'r');
arr_name1 = '.my_arrs';
var_name2 = '.my_vars';
%      stop1 = 'SORTED ALPHABETICALLY BY Name';
stop1 = 'SORTED BY Symbol Address';
arr_start = 0;
arr_length = 0;
var_start = 0;
var_length = 0;
i = 1;
error = 0;
memloc = 0;
while 1
    line = fgetl(fid);
    if ~ischar(line)
       break;
    end
    if strfind(line,arr_name1)
        if arr_start == 0
            parse = strread(line,'%s');
            arr_start = hex2dec(parse(3));
            arr_length = hex2dec(parse(4));
        end
    end
    if strfind(line,var_name2)
        if var_start == 0
            parse = strread(line,'%s');
            var_start = hex2dec(parse(3));
            var_length = hex2dec(parse(4));
        end
    end
    if findstr(line,stop1)
        break;
    end
end
while 1
    error = false;
    line = fgetl(fid);
    if ~ischar(line)
       break;
    end
    parse = strread(line,'%s');
    if isempty(parse) == 0
        if isHex(parse(2))% changed to parse(2)
            memloc = hex2dec(parse(2)); % changed to parse (2)
            if (memloc > (arr_start-1)) && (memloc < (arr_start+arr_length) && (isHex(parse(3)) == 0))
                if (i > 1) && (memloc == hex2dec(my_arrs(i-1,2)))
                    str = char(parse(3));
                    str = str(2:end);
                    my_arrs(i-1,1) = cellstr(str);
                else
                    my_arrs(i,3) = cellstr('Array Variable');
                    my_arrs(i,2) = parse(2);
                    str = char(parse(3));
                    str = str(2:end);
                    my_arrs(i,1) = cellstr(str);
                    i = i + 1;
                end
            elseif (memloc > (var_start-1)) && (memloc < (var_start+var_length) && (isHex(parse(3)) == 0))
                if (i > 1) && (memloc == hex2dec(my_arrs(i-1,2)))
                    str = char(parse(3));
                    str = str(2:end);
                    my_arrs(i-1,1) = cellstr(str);
                else
                    my_arrs(i,3) = cellstr('Float Variable');
                    my_arrs(i,2) = parse(2);
                    str = char(parse(3));
                    str = str(2:end);
                    my_arrs(i,1) = cellstr(str);
                    i = i + 1;
                end
            end
        end
    end
end
fclose(fid);