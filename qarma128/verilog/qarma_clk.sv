module qarma_clk
	#(
        parameter n=128,
        parameter [0:63] sigma=64'b0, // S-box
        parameter [0:63] inv_sigma=64'b0, // inv S-box
        parameter [0:11] MC_abc=12'b0, // rot params for MixColumns
        parameter [0:11] inv_MC_abc=12'b0, // inv rot params for MixColumns
        parameter round=11 // number of rounds, warning: no sanity check
    )
    (
        input wire clk,
		input wire rst,
		input wire[2*n-1:0] K, 
        input wire[n-1:0] P, 
        input wire[n-1:0] T, 
        output reg[n-1:0] C
    );
	
	reg [2*n-1:0] K_reg;
	reg [n-1:0] P_reg;
	reg [n-1:0] T_reg;
	wire [n-1:0] C_out;
	
	qarma_top #(.n(n), .sigma(sigma), .inv_sigma(inv_sigma), 
		.MC_abc(MC_abc), .inv_MC_abc(inv_MC_abc), .round(round)) qarma_comb(K_reg, P_reg, T_reg, C_out);

	always@(posedge clk)begin
		if(rst == 0)begin
			K_reg <= K;
			P_reg <= P;
			T_reg <= T;
			C <= C_out;
		end
	end
	
endmodule