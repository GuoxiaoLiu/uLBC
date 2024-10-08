`timescale 1ns / 1ps
/*
key specialization
only encryption is taken into consideration
*/
module key_spec
    # (parameter n=128) // block size, defaults to qarma_128
    (
        input wire [2*n-1:0] key,
        output wire [n-1:0] w0,
        output wire [n-1:0] w1,
        output wire [n-1:0] k0,
        output wire [n-1:0] k1
    );
    assign w0 = key[n+:n];
    assign k0 = key[0+:n];
    assign w1 = {w0[0], w0[n-1:2], w0[1] ^ w0[n-1]};
    assign k1 = k0;
endmodule
