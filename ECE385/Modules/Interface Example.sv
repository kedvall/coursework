//Interface declaration
interface Bus #(SIZE=2); //Interface declaration with varable SIZE
	logic [SIZE-1:0] m2s; //Master to slave connection
	logic [SIZE-1:0] s2m; //Slave to master connection

	modport master (input s2m, output m2s); //Master view of interface
	modport slave (input m2s, output s2m); //Slave veiw of interface

endinterface

//Master module
module Master(Bus.master interface1);
	//do something using interface1.m2s and interface1.s2m
endmodule

//Slave module
module Slave(Bus.slave interface2);
	//do something userful with interface2.m2s and interface2.s2m
endmodule


//Top-level module
module Example();

	Bus myInterface;
	Master myMaster (myInterface);
	Slave mySlave (myInterface);

endmodule