module cipher_top(
	input wire [2*128-1:0] key_in,
	input wire [128-1:0] tx_in,
	input wire [128-1:0] tweak,
	input wire clk,
	output wire [128-1:0] text_out
);

`include "conf_options.sv"

qarma_top #(
		.n(128), 
		.sigma(sigma0),
		.inv_sigma(inv_sigma0),
		.MC_abc(M_82),
		.inv_MC_abc(Q_82),
		.round(11)) qarma_top_inst(key_in, tx_in, tweak, text_out);
endmodule