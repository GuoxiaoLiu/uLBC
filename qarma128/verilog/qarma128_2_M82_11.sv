module qarma128_2_M82_11(
	input wire [2*128-1:0] K,
	input wire [128-1:0] P,
	input wire [128-1:0] T,
	output wire [128-1:0] C
);

`include "conf_options.sv"

qarma_top #(
		.n(128), 
		.sigma(sigma2),
		.inv_sigma(inv_sigma2),
		.MC_abc(M_82),
		.inv_MC_abc(Q_82),
		.round(11)) qarma_top_inst(K, P, T, C);
endmodule