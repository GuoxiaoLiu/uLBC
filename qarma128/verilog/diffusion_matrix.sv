`timescale 1ns / 1ps
/*
warning: function is local to the module in which it is declared,
so the trick of parameterizing a function by wrapping it in a module doesn't work
*/

/*
a mixed-column like operation
left multiply IS matrix with circ(0, rho^a, rho^b, rho^c)
*/
module MixColumns
    #(parameter n=128, parameter [0:11] abc=12'b0)
    (
        input [n-1:0] indata,
        output [n-1:0] outdata
    );
	localparam m=n>>4;
    // internal states
    wire [n-1:0] inIS;// = {<<m{indata}};
    wire [n-1:0] outIS;
    //assign outdata = {<<m{outIS}};
    // compute
    genvar col, i;
    generate
		// replace streaming operators
		for(i=0; i<16; i=i+1)begin
			assign inIS[(15-i)*m+:m] = indata[i*m+:m];
			assign outdata[(15-i)*m+:m] = outIS[i*m+:m];
		end
        for(col=0; col<4; col=col+1)begin
            RotCol #(.n(n), .abc(abc)) rot(
                {inIS[m*(col+12)+:m], inIS[m*(col+8)+:m], inIS[m*(col+4)+:m], inIS[m*(col)+:m]},
                {outIS[m*(col+12)+:m], outIS[m*(col+8)+:m], outIS[m*(col+4)+:m], outIS[m*(col)+:m]});
        end
    endgenerate
endmodule

/*
perform mix col for a single column
*/
module RotCol
    #(
        parameter n=128, 
        parameter [0:11] abc = 12'b0 // 3 * 4-bit
    )
    (
		inCols, outCols
    );
	localparam m=n>>4;
	input wire [m*4-1:0] inCols;
	output wire [m*4-1:0] outCols;
    // inCols & outCols layout: bottom to top, i.e (v3, v2, v1, v0)
    localparam a = abc[0+:4], b = abc[4+:4], c = abc[8+:4];
    genvar i;
    generate
        for(i=0; i<4; i=i+1)begin
            wire [m*4-1:0] shiftedCol;
            wire [m-1:0] rotCells[0:2];
            Rot #(.m(4*m), .din(-i*m)) shiftR(inCols, shiftedCol);
            Rot #(.m(m), .din(a)) rota(shiftedCol[m+:m], rotCells[0]);
            Rot #(.m(m), .din(b)) rotb(shiftedCol[2*m+:m], rotCells[1]);
            Rot #(.m(m), .din(c)) rotc(shiftedCol[3*m+:m], rotCells[2]);
            assign outCols[i*m+:m] = rotCells[0]^rotCells[1]^rotCells[2];
        end
    endgenerate
endmodule

/*
circular shift a m-bit wire left by dist bits
*/
module Rot
    #(
        parameter m=4,
        parameter din=0)
    (
        input wire [m-1:0] indata,
        output wire [m-1:0] outdata
    );
    localparam d = (m + (din % m)) % m;
    generate
        if(d == 0)
            assign outdata = indata;
        else
            assign outdata = {indata[m-1-d:0], indata[m-1:m-d]};
    endgenerate
endmodule