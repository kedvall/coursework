//Sensitive list tells always when to execute
//Cannot drive wire data, only reg and integer

//This is a 2:1 MUX
always @ (a or b or sel)
begin
	y = 0;
	if (sel == 0) begin
		y = a;
	end else begin
		y = b;
	end
end