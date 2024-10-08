`timescale 1ns / 1ps
/*
Pseudo Reflector
*/
module PReflector
    #(
        parameter n=128,
        parameter [0:63] perm=64'b0,
        parameter [0:63] inv_perm=64'b0,
        parameter [0:11] MC_abc=12'b0 
    )
    (
        input wire [n-1:0] tk,
        input wire [n-1:0] indata,
        output wire [n-1:0] outdata
    );
    wire [n-1:0] first, middle, last;
    ShuffleCells #(.n(n), .perm(perm)) shuffle_inst(indata, first);
    MixColumns #(.n(n), .abc(MC_abc)) mix_inst(first, middle);
    assign last = middle^tk;
    ShuffleCells #(.n(n), .perm(inv_perm)) inv_shuffle_inst(last, outdata);
endmodule
