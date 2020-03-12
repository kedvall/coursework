//Used for combinational logic ONLY
//Executes continuously

//This is a tri-state buffer
assign out = (enable) ? data : 1'bz;