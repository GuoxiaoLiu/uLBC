`timescale 1ns / 1ps
/*
substitute cells according to s-box
WARNING: S-box8 needs param inv
*/
module SubCells
    #(
        parameter n=128,
        parameter [0:63] sigma=64'b0,
        parameter inv=0
    )
    (
        input wire [n-1:0] indata,
        output wire [n-1:0] outdata
    );
	localparam m=n>>4;
    // FIXED: conversion to IS is innecessary
    //wire [n-1:0] inIS = {<<m{indata}};
    //wire [n-1:0] outIS;
    //assign outdata = {<<m{outIS}};
    genvar i;
    generate
        for(i=0; i<16; i=i+1)begin
            sbox #(.n(n), .sigma(sigma), .inv(inv)) sbox_inst(indata[i*m+:m], outdata[i*m+:m]);
        end
    endgenerate
endmodule
