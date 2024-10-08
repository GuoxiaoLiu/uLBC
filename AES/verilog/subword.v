module subword(in, out);
input [31:0] in;
output [31:0] out;
wire [7:0] in0,in1,in2,in3,out0,out1,out2,out3;
assign in0 = in[31:24];
assign in1 = in[23:16];
assign in2 = in[15:8];
assign in3 = in[7:0];
aes_sbox s_0(.a(in0), .d(out0));
aes_sbox s_1(.a(in1), .d(out1));
aes_sbox s_2(.a(in2), .d(out2));
aes_sbox s_3(.a(in3), .d(out3));
assign out[31:0] = {out0,out1,out2,out3};
endmodule
