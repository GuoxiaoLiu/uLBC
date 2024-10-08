module qarma64_2_M42_7(
	input wire [2*64-1:0] K,
	input wire [64-1:0] P,
	input wire [64-1:0] T,
	output wire [64-1:0] C
);

`include "conf_options.sv"

qarma_top #(
		.n(64), 
		.sigma(sigma2),
		.inv_sigma(inv_sigma2),
		.MC_abc(M_42),
		.inv_MC_abc(Q_42),
		.round(7)) qarma_top_inst(K, P, T, C);
endmodule