module round_49(
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

Round #(.n(128), .short(0), .perm(perm_tau), .sigma(sigma49), .MC_abc(M_82), .inv(0)) 
                round_inst(tk, indata, outdata);

endmodule