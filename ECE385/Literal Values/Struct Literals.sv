`timescale 1ns/100ps

//Type define a struct
typedef struct {
	byte a;
	reg b;
	shortint unsigned c;
} myStruct;

module struct_literals();

	myStruct object = '{10,0,100};

	myStruct objectArray [0:1] = '{'{10,0,100}, '{11,1,101}};

	initial begin
		$display ("a = %b b = %b c = %h", object.a, object.b, object.c);
 		object.a = 15;
		$display ("a = %b b = %b c = %h", object.a, object.b, object.c);
		object.c = 16'hDEAD; 
		$display ("a = %b b = %b c = %h", object.a, object.b, object.c);
		$display ("Printing array of objects");
		$display ("a = %b b = %b c = %h", 
		objectArray[0].a, objectArray[0].b, objectArray[0].c);
 		$display ("a = %b b = %b c = %h", 
 		objectArray[1].a, objectArray[1].b, objectArray[1].c);

 		#1 $finish;
 	end
 endmodule