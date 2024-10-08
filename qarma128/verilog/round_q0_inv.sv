module round_q0_inv(
	input wire [127:0] tk,
	input wire [127:0] indata,
	output wire [127:0] outdata
);

// .n(128), 
// .sigma(sigma0),
// .inv_sigma(inv_sigma0),
// .MC_abc(M_82),
// .inv_MC_abc(Q_82),


`include "conf_options.sv"
`include "global_params.sv"

Round #(.n(128), .short(0), .perm(inv_tau), .sigma(inv_sigma0), .MC_abc(Q_82), .inv(1)) 
                round_inst(tk, indata, outdata);

endmodule