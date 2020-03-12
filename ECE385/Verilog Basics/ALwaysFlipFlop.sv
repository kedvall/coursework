always @ (posedge clk)
if (reset == 0) begin
	y <= 0;
end else if (sel == 0) begin
	y <= a;
end else begin
	y <= b;
end