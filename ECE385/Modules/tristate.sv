//Tri-state buffer in Verilog
//NOT FOR SYSTEM VERILOG!!!!

module tristate(inarr, enable, outarr); //Interface and behaverial definitions
	input [15:0] inarr;
	input enable;
	output [15:0] outarr;

	reg [15:0] outarr;

	assign outarr = (enable) ? inarr : 16'z; //C style conditional statement
endmodule // tristate