//Uses component full_adder

module ADDER4 (	input [3:0] A, B,
				input c_in,
				output [3:0] S,
				output c_out);

	//Internal carries in the 4 bit adder
	wire c0, c1, c2;

	//Letlists with named (explicit) port connection

	//Syntax: <module><name>(.<parameter_name>(<connection_name>), .....)
	full_adder FA0(.x(A[0]), .y(B[0]), .z(c_in), .s(S[0]), .c(c0));
	full_adder FA0(.x(A[1]), .y(B[1]), .z(c0), .s(S[1]), .c(c1));
	full_adder FA0(.x(A[2]), .y(B[2]), .z(c1), .s(S[2]), .c(c2));
	full_adder FA0(.x(A[3]), .y(B[3]), .z(c2), .s(S[3]), .c(c_out));

endmodule // ADDER4


//See IST (into to SV) page 18 or 19 for more info