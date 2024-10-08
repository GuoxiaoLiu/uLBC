`timescale 1ns / 1ps
/*
the round function
WARNING: S-box8 needs param inv
*/
module Round
    #(
        parameter short=0,
        parameter n=128,
        parameter [0:63] perm=64'b0,
        parameter [0:63] sigma=64'b0,
        parameter [0:11] MC_abc=12'b0,
        parameter inv=0
    )
    (
        input wire [n-1:0] tk,
        input wire [n-1:0] indata,
        output wire [n-1:0] outdata
    );
    generate
        wire [n-1:0] first;
        wire [n-1:0] last;
        if(inv)begin
            SubCells #(.n(n), .sigma(sigma), .inv(inv)) sub_inst(indata, first);
            if(!short)begin
                wire [n-1:0] middle;
                MixColumns #(.n(n), .abc(MC_abc)) mix_inst(first, middle);
                ShuffleCells #(.n(n), .perm(perm)) shuffle_inst(middle, last);
            end
            else
                assign last = first;
            assign outdata = tk^last;
        end
        else begin
            assign first = tk^indata;
            if(!short)begin
                wire [n-1:0] middle;
                ShuffleCells #(.n(n), .perm(perm)) shuffle_inst(first, middle);
                MixColumns #(.n(n), .abc(MC_abc)) mix_inst(middle, last);
            end
            else
                assign last = first;
            SubCells #(.n(n), .sigma(sigma), .inv(inv)) sub_inst(last, outdata);
        end
    endgenerate
endmodule
