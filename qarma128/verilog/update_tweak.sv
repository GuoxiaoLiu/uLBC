`timescale 1ns / 1ps

module UpdateTweak
    #(
        parameter n=128,
        parameter [0:63] perm=64'b0,
        parameter inv=0
    )
    (
        input wire [n-1:0] tk,
        output wire [n-1:0] newtk
    );
    generate
        wire [n-1:0] middle;
        if(inv)begin
            LFSR #(.n(n), .inv(inv)) w_inst(tk, middle);
            ShuffleCells #(.n(n), .perm(perm)) h_inst(middle, newtk);
        end
        else begin
            ShuffleCells #(.n(n), .perm(perm)) h_inst(tk, middle);
            LFSR #(.n(n), .inv(inv)) w_inst(middle, newtk);
        end
    endgenerate
endmodule
