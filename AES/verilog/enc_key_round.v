module enc_key_round(
input [127:0] key_in,
input [3:0] round,
output [127:0] key_out
);

	wire [31:0] s0,s1,s2,s3;
	wire	[31:0]	temp;
	wire	[31:0]	subword;
	wire	[31:0]	rcon;
	wire [31:0] w0,w1,w2,w3;
	assign s0 = key_in[127:96];
	assign s1 = key_in[95:64];
	assign s2 = key_in[63:32];
	assign s3 = key_in[31:0];
	assign w0 = s0^subword^rcon;
	assign w1 = s0^s1^subword^rcon;
	assign w2 = s0^s2^s1^subword^rcon;
	assign w3 = s0^s3^s2^s1^subword^rcon;
	assign temp = {s3[23:16],s3[15:08],s3[07:00],s3[31:24]};
	subword subword0(.in(temp),.out(subword));
	aes_rcon r0(.i(round), .out(rcon));
	assign key_out[127:0] = {w0,w1,w2,w3};
endmodule