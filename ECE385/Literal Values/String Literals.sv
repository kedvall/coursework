`timescale 1ns/100ps

module string_literals();

	string a;

	initial begin
		$display("@%gns a = %s", $time, a);
		a = "Hello Kanyon";

		$display("@%gns a = %s", $time, a);
		#1 a = "Over writing old string";

		$display("@%gns a = %s", $time, a);
		#1 a = "This is a multi line comment \
				and this is the second line";

		$display("@%gns a = %s", $time, a);
		#1 $finish;
	end
endmodule // string_literals