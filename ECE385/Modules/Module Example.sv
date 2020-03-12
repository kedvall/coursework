//Logic is contained within modules
//Think of it as a single piece of hardware that can be combined to form larger design

module ExampleModule(input clk, enable, //Single-bit inputs, default to type logic
					 input [2:0] data, //Three bit input, default to type logic
					 output out); //Single-bit output, default to type logic
	
	initial //initial procedure block initializes the variables
	begin
		out = 0;
	end

	always @ (posedge clk) //always procedure block triggered by the rising edge of clk
	begin
		if (enable)
			out <= /*do something with the input*/
	end
endmodule // ExampleModule