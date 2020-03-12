//Two different ways to make an AND gate

//"assign" statement
module AND_Gate(input a, b,
				output z);

	assign z = a & b;

endmodule // AND_Gate


//Procedure block
module AND_Gate2(	input a, b,
					output z);

	always @ (a or b)
	begin
		z = a & b;
	end
endmodule