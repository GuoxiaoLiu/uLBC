/*
fixed (inconfigurable) params for qarma,
including perm h used in tweak update, perm tau used in ShuffleCells,
and hexadecimal pi used in round constant generation
*/

localparam [0:63] perm_h = {
    4'h6, 4'h5, 4'he, 4'hf, 4'h0, 4'h1, 4'h2, 4'h3,
    4'h7, 4'hc, 4'hd, 4'h4, 4'h8, 4'h9, 4'ha, 4'hb
};
localparam [0:63] inv_h = {
    4'h4, 4'h5, 4'h6, 4'h7, 4'hb, 4'h1, 4'h0, 4'h8,
    4'hc, 4'hd, 4'he, 4'hf, 4'h9, 4'ha, 4'h2, 4'h3
};

localparam [0:63] perm_tau = {
    4'h0, 4'hb, 4'h6, 4'hd, 4'ha, 4'h1, 4'hc, 4'h7,
    4'h5, 4'he, 4'h3, 4'h8, 4'hf, 4'h4, 4'h9, 4'h2
};
localparam [0:63] inv_tau = {
    4'h0, 4'h5, 4'hf, 4'ha, 4'hd, 4'h8, 4'h2, 4'h7,
    4'hb, 4'he, 4'h4, 4'h1, 4'h6, 4'h3, 4'h9, 4'hc
};

// hex of fractional part of pi, used to generate alpha and c_i
localparam [0:256*6-1] hexPi = {
    256'h243F6A8885A308D313198A2E03707344A4093822299F31D0082EFA98EC4E6C89,
    256'h452821E638D01377BE5466CF34E90C6CC0AC29B7C97C50DD3F84D5B5B5470917,
    256'h9216D5D98979FB1BD1310BA698DFB5AC2FFD72DBD01ADFB7B8E1AFED6A267E96,
    256'hBA7C9045F12C7F9924A19947B3916CF70801F2E2858EFC16636920D871574E69,
    256'hA458FEA3F4933D7E0D95748F728EB658718BCD5882154AEE7B54A41DC25A59B5,
    256'h9C30D5392AF26013C5D1B023286085F0CA417918B8DB38EF8E79DCB0603A180E
};
