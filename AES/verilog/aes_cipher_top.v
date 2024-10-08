/////////////////////////////////////////////////////////////////////
////                                                             ////
////  AES Cipher Top Level                                       ////
////                                                             ////
////                                                             ////
////  Author: Rudolf Usselmann                                   ////
////          rudi@asics.ws                                      ////
////                                                             ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/cores/aes_core/  ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2000-2002 Rudolf Usselmann                    ////
////                         www.asics.ws                        ////
////                         rudi@asics.ws                       ////
////                                                             ////
//// This source file may be used and distributed without        ////
//// restriction provided that this copyright statement is not   ////
//// removed from the file and that any derivative work contains ////
//// the original copyright notice and the associated disclaimer.////
////                                                             ////
////     THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY     ////
//// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   ////
//// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS   ////
//// FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR      ////
//// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,         ////
//// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    ////
//// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE   ////
//// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR        ////
//// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF  ////
//// LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT  ////
//// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  ////
//// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE         ////
//// POSSIBILITY OF SUCH DAMAGE.                                 ////
////                                                             ////
/////////////////////////////////////////////////////////////////////

//  CVS Log
//
//  $Id: aes_cipher_top.v,v 1.1.1.1 2002-11-09 11:22:48 rudi Exp $
//
//  $Date: 2002-11-09 11:22:48 $
//  $Revision: 1.1.1.1 $
//  $Author: rudi $
//  $Locker:  $
//  $State: Exp $
//
// Change History:
//               $Log: not supported by cvs2svn $
//
//
//
//
//

//`include "timescale.v"

module aes_cipher_top(key_in, tx_in, text_out, clk);
input	[127:0]	key_in;
input	[127:0]	tx_in;
input clk;
output	[127:0]	text_out;

////////////////////////////////////////////////////////////////////
//
// Local Wires
//
//reg [127:0]tx_in;
//reg [127:0]key_in;
//reg	[127:0]	text_out;

wire [127:0]F_1;
wire [127:0]F_1_OUT;
wire [127:0]F_2;
wire [127:0]F_2_OUT;
wire [127:0]F_3;
wire [127:0]F_3_OUT;
wire [127:0]F_4;
wire [127:0]F_4_OUT;
wire [127:0]F_5;
wire [127:0]F_5_OUT;
wire [127:0]F_6;
wire [127:0]F_6_OUT;
wire [127:0]F_7;
wire [127:0]F_7_OUT;
wire [127:0]F_8;
wire [127:0]F_8_OUT;
wire [127:0]F_9;
wire [127:0]F_9_OUT;
wire [127:0]F_10;
wire [127:0]F_10_OUT;

wire [127:0]K_1_OUT;
wire [127:0]K_2_OUT;
wire [127:0]K_3_OUT;
wire [127:0]K_4_OUT;
wire [127:0]K_5_OUT;
wire [127:0]K_6_OUT;
wire [127:0]K_7_OUT;
wire [127:0]K_8_OUT;
wire [127:0]K_9_OUT;
wire [127:0]K_10_OUT;



////////////////////////////////////////////////////////////////////
//
// Misc Logic
//

assign text_out = F_10_OUT ^ K_10_OUT;


assign F_1 = tx_in ^ key_in;
assign F_2 = F_1_OUT ^ K_1_OUT;
assign F_3 = F_2_OUT ^ K_2_OUT;
assign F_4 = F_3_OUT ^ K_3_OUT;
assign F_5 = F_4_OUT ^ K_4_OUT;
assign F_6 = F_5_OUT ^ K_5_OUT;
assign F_7 = F_6_OUT ^ K_6_OUT;
assign F_8 = F_7_OUT ^ K_7_OUT;
assign F_9 = F_8_OUT ^ K_8_OUT;
assign F_10 = F_9_OUT ^ K_9_OUT;


enc_round r1(F_1, F_1_OUT);
enc_round r2(F_2, F_2_OUT);
enc_round r3(F_3, F_3_OUT);
enc_round r4(F_4, F_4_OUT);
enc_round r5(F_5, F_5_OUT);
enc_round r6(F_6, F_6_OUT);
enc_round r7(F_7, F_7_OUT);
enc_round r8(F_8, F_8_OUT);
enc_round r9(F_9, F_9_OUT);
enc_round10 r10(F_10,F_10_OUT);

	
enc_key_round kr1 (.key_in(key_in),.round(4'h0),.key_out(K_1_OUT));
enc_key_round kr2 (.key_in(K_1_OUT),.round(4'h1),.key_out(K_2_OUT));
enc_key_round kr3 (.key_in(K_2_OUT),.round(4'h2),.key_out(K_3_OUT));
enc_key_round kr4 (.key_in(K_3_OUT),.round(4'h3),.key_out(K_4_OUT));
enc_key_round kr5 (.key_in(K_4_OUT),.round(4'h4),.key_out(K_5_OUT));
enc_key_round kr6 (.key_in(K_5_OUT),.round(4'h5),.key_out(K_6_OUT));
enc_key_round kr7 (.key_in(K_6_OUT),.round(4'h6),.key_out(K_7_OUT));
enc_key_round kr8 (.key_in(K_7_OUT),.round(4'h7),.key_out(K_8_OUT));
enc_key_round kr9 (.key_in(K_8_OUT),.round(4'h8),.key_out(K_9_OUT));
enc_key_round kr10 (.key_in(K_9_OUT),.round(4'h9),.key_out(K_10_OUT));


//assign text_out = F_10_OUT ^ K_10_OUT;



endmodule


