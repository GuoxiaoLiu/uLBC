`timescale 1ns / 1ps
/*
LFSR function w in tweak update
*/
module LFSR
    #(parameter n=128, parameter inv=0)
    (
        input wire [n-1:0] indata,
        output wire [n-1:0] outdata
    );
	localparam m=n>>4;
    /*
    indices is actually a bit set,
    while a permutation is an ordered list of elements
    */
    localparam [15:0] indices = 
        1'b1 << 0 | 1'b1 << 1 | 1'b1 << 3 | 1'b1 << 4 | 1'b1 << 8 | 1'b1 << 11 | 1'b1 << 13;
    // get inner states
    wire [n-1:0] inIS;// = {<<m{indata}};
    wire [n-1:0] outIS;
    //assign outdata = {<<m{outIS}};
    genvar i;
    generate
		// replace streaming operators
		for(i=0; i<16; i=i+1)begin
			assign inIS[(15-i)*m+:m] = indata[i*m+:m];
			assign outdata[(15-i)*m+:m] = outIS[i*m+:m];
		end
        for(i=0; i<16; i=i+1)begin
            if(indices[i] == 1'b1)
                LFSR_inner #(.n(n), .inv(inv)) inner(inIS[i*m+:m], outIS[i*m+:m]);
            else
                assign outIS[i*m+:m] = inIS[i*m+:m];
        end
    endgenerate
endmodule

/*
inner LFSR
*/
module LFSR_inner
    #(parameter n=128, parameter inv=0)
    (
		indata, outdata
    );
	localparam m=n>>4;        
	input wire [m-1:0] indata;
	output wire [m-1:0] outdata;

    if(m==8)
        LFSR8_inner #(.inv(inv)) inner8(indata, outdata);
    else if(m==4)
        LFSR4_inner #(.inv(inv)) inner4(indata, outdata);
    // otherwise error
endmodule

module LFSR4_inner
    #(parameter inv=0)
    (
    input wire [3:0] indata,
    output wire [3:0] outdata
    );
    generate
        if(inv)
            assign outdata = {indata[2:0], indata[3]^indata[0]};
        else
            assign outdata = {indata[0]^indata[1], indata[3:1]};
    endgenerate
endmodule

module LFSR8_inner
    #(parameter inv=0)
    (
        input wire [7:0] indata,
        output wire [7:0] outdata
    );
    generate
        if(inv)
            assign outdata = {indata[6:0], indata[7]^indata[1]};
        else
            assign outdata = {indata[0]^indata[2], indata[7:1]};
    endgenerate
endmodule

