//Logic is contained within modules 
module Example( input a, b, z, enable, clk, reset,
			 	output z);

/*================================================*/
	//"always_comb" procedure
	always_comb
	begin: myComb //Name for readability, does not affect actual execution
		z = a + b;
	end myComb //End named block here


/*================================================*/
	//"always_latch" procedure
	always_latch
	begin
		if (enable) //begin and end needed if IF is longer than one line, like {}
			z <= a+b;
		}
	end


/*================================================*/
	//"always_ff" procedure
	always_ff @ (posedge clk or posedge reset) 	//ordering does not matter
	begin										//sensitive for positive edge of clk and reset
		if (reset) begin
			z <= 0;
		end else begin
			z <= a+b;
		end
	end
	
endmodule // Example