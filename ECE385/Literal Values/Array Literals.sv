`timescale 1ns/100ps

module array_literals();

	byte a [0:1][0:2] = '{'{0,1,2}, '{3{8'h9}}};

	initial begin
		$display ("a [0][0] = %d", a[0][0]);
		$display ("a [0][1] = %d", a[0][1]);
		$display ("a [1][0] = %d", a[1][0]);
		$display ("a [1][1] = %d", a[1][1]);
		$display ("a [1][2] = %d", a[1][2]);
		#1 $finish;
	end
endmodule // array_literals