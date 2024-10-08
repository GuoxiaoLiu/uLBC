`timescale 1ns / 1ps
/*
permute IS, e.g. h and tau function
*/
module ShuffleCells
    #(parameter n=128, parameter [0:63] perm=64'b0)
    (
        input wire [n-1:0] indata,
        output wire [n-1:0] outdata
    );
	localparam m=n>>4;
    // invert indata
	// replace streaming operators
    wire [n-1:0] inIS;// = {<<m{indata}};
    wire [n-1:0] outIS;
    //assign outdata = {<<m{outIS}};
    genvar i;
    generate
        for(i=0; i<16; i=i+1)begin
			assign inIS[(15-i)*m+:m] = indata[i*m+:m];
			assign outdata[(15-i)*m+:m] = outIS[i*m+:m];
            assign outIS[i*m+:m] = inIS[perm[i*4+:4]*m+:m];
        end
    endgenerate
endmodule
