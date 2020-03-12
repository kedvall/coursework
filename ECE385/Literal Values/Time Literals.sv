`timescale 100ps/10ps

module time_literals();
	time a;
	initial begin
		$monitor ("@%g a = %t", $time, a);
		#1 a = 1ns;
		#1 a = 0.2ns;
		#1 a = 300ps;
		#1 $finish;
	end
endmodule // time_literals