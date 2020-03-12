//System verilog basic module construct
module SysVerilogConstruct( input clk, enable,
							input [2:0] data,
							output out);

	//Data types always default to logic

	always @ (posedge clk)
	begin
		if (enable)
			out <= /* DO SOMETHING HERE */
	end
endmodule // SysVerilogConstruct