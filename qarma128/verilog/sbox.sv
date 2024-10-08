`timescale 1ns / 1ps
/*
 in verilog, unpacked array of parameters is not allowed
 the paramter sigma is a concatenation of 16*4 bits into 64 bits
 and it is conventional that we order packed bits from msb to lsb
 and unpacked array from lsb to msb
*/

/*
general sbox
*/
module sbox
    #(
        parameter n=128, 
        parameter [0:63] sigma = 64'b0, 
        parameter inv=0)
    (
		indata, outdata
    );
	localparam m=n>>4;
	input wire [m-1:0] indata;
	output wire [m-1:0] outdata;

    if(m==4)
        sbox4 #(.sigma(sigma)) sbox4_inst(indata, outdata);
    else if(m==8)
        sbox8 #(.sigma(sigma), .inv(inv)) sbox8_inst(indata, outdata);
endmodule

/*
S-box that permutes 4 bits
*/
module sbox4 
    #(parameter [0:63] sigma = 64'b0)
    (
        input wire [3:0] indata,
        output wire [3:0] outdata
    );
    assign outdata = sigma[indata*4+:4];
endmodule

/*
S-box that permutes 8 bits, instantiated with 2 4-bit S-boxes
WARNING: inverve S-box8 needs to be implemented seperately
*/
module sbox8
    #(
        parameter [0:63] sigma = 64'b0,
        parameter inv=0)
    (
        input wire [7:0] indata,
        output wire [7:0] outdata
    );
    generate
    if(inv)begin
        // mingled input, sequential output
        sbox4 #(.sigma(sigma)) sbox4_1_inv({indata[6], indata[4], indata[2], indata[0]},
            outdata[3:0]);
        sbox4 #(.sigma(sigma)) sbox4_2_inv({indata[7], indata[5], indata[3], indata[1]},
            outdata[7:4]);
    end
    else begin
        // sequential input, mingle output
        sbox4 #(.sigma(sigma)) sbox4_1(indata[3:0], 
            {outdata[6], outdata[4], outdata[2], outdata[0]});
        sbox4 #(.sigma(sigma)) sbox4_2(indata[7:4],
            {outdata[7], outdata[5], outdata[3], outdata[1]});
    end
    endgenerate
endmodule