`timescale 1ns / 1ps

/*
qarma top
*/

module qarma_top
    #(
        parameter n=128,
        parameter [0:63] sigma=64'b0, // S-box
        parameter [0:63] inv_sigma=64'b0, // inv S-box
        parameter [0:11] MC_abc=12'b0, // rot params for MixColumns
        parameter [0:11] inv_MC_abc=12'b0, // inv rot params for MixColumns
        parameter round=11 // number of rounds, warning: no sanity check
    )
    (
        input wire[2*n-1:0] K, 
        input wire[n-1:0] P, 
        input wire[n-1:0] T, 
        output wire[n-1:0] C
    );

    `include "global_params.sv"
    genvar i;
    generate
        // prepare round constants
        wire [n-1:0] alpha;
		wire [n*round-1:0] tmp; // to replace streaming operator
        wire [n*round-1:0] roundConstants;
        if(n==64)begin
            // (zero-indexed)6-th block of hexPi as alpha
            // c_0 = 0
            // c_i = i-th block of hexPi, when 0 < i <= 5 (round <= 6)
            //     = (i+1)th block of hexPi, when i >= 6 (round >= 7)
            assign alpha = hexPi[6*n+:n];
            if(round <= 6)begin
                assign tmp = {
                    {n{1'b0}}, hexPi[n+:(round-1)*n]
                };
            end
            else begin
                assign tmp = {
                    {n{1'b0}}, hexPi[n+:n*5], hexPi[n*7+:(round-6)*n]
                };
            end
        end
        else if(n==128) begin
            // (zero-indexed)0-th block of hexPi as alpha
            // c_0 = 0
            // c_i = i-th block of hexPi, when i > 0
            assign alpha = hexPi[0:n-1];
            assign tmp = {
                {n{1'b0}}, hexPi[n+:(round-1)*n]
            };
            end
        
		// perform streaming manually
		for(i=0; i<round; i=i+1)begin
			assign roundConstants[(round-1-i)*n+:n] = tmp[i*n+:n];
		end
		
        // only consider the case of encryption
        wire [n-1:0] w0, w1, k0, k1;
        key_spec #(.n(n)) key_spec_inst(K, w0, w1, k0, k1);
        // the inputs&outputs of round rounds, for Round and UpdateTweak, i.e. Pbuf[i] is the input of round i and the output of i-1
        wire [n-1:0] Pbuf[0:round];
        wire [n-1:0] Tbuf[0:round];
        assign Pbuf[0] = P^w0;
        assign Tbuf[0] = T;
        // round 0 to round-1
        for(i=0; i<round; i=i+1)begin
            Round #(.n(n), .short(i==0), .perm(perm_tau), .sigma(sigma), .MC_abc(MC_abc), .inv(0)) 
                round_inst(Tbuf[i]^k0^roundConstants[i*n+:n], Pbuf[i], Pbuf[i+1]);
            UpdateTweak #(.n(n), .perm(perm_h), .inv(0)) update_inst(Tbuf[i], Tbuf[i+1]);
        end
        // R-P-R operation
        wire [n-1:0] invPbuf[0:round]; // the inputs&outputs of round rounds of inverse R
        wire [n-1:0] preP, postP; // vector before and after PReflector
        Round #(.n(n), .short(0), .perm(perm_tau), .sigma(sigma), .MC_abc(MC_abc), .inv(0)) 
            preP_inst(Tbuf[round]^w1, Pbuf[round], preP);
        PReflector #(.n(n), .perm(perm_tau), .inv_perm(inv_tau), .MC_abc(inv_MC_abc)) 
            P_inst(k1, preP, postP);
        Round #(.n(n), .short(0), .perm(inv_tau), .sigma(inv_sigma), .MC_abc(inv_MC_abc), .inv(1)) 
            invR_inst(Tbuf[round]^w0, postP, invPbuf[round]);
        // inverse rounds
        // NOTE: the tweak key sequence is reflective, so the former half of the keys can be reused
        for(i=round-1; i>=0; i=i-1)begin
            Round #(.n(n), .short(i==0), .perm(inv_tau), .sigma(inv_sigma), .MC_abc(inv_MC_abc), .inv(1))
                invR_inst(Tbuf[i]^k0^alpha^roundConstants[i*n+:n], invPbuf[i+1], invPbuf[i]);
        end
        assign C = invPbuf[0]^w1;
    endgenerate
endmodule
