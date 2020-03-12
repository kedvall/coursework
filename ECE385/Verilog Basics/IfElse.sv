if (enable == 1'b1) begin
	data = 10; //Decimal assigned
	address = 16'hDEAD; //Hexadecimal
	wr_enable = 1'b1; //Binary
end else begin
	data = 32'b0;
	wr_enable = 1'b0;
	address = address + 1;
end