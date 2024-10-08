/*
configurable params in qarma,
including 3 types of S-box and various MixColumn matrix
*/
// S-boxes
// involutory
localparam [0:63] sigma0 = {
    4'h0, 4'he, 4'h2, 4'ha, 4'h9, 4'hf, 4'h8, 4'hb,
    4'h6, 4'h4, 4'h3, 4'h7, 4'hd, 4'hc, 4'h1, 4'h5
};
localparam [0:63] inv_sigma0 = sigma0;

// involutory
localparam [0:63] sigma1 = {
    4'ha, 4'hd, 4'he, 4'h6, 4'hf, 4'h7, 4'h3, 4'h5,
    4'h9, 4'h8, 4'h0, 4'hc, 4'hb, 4'h1, 4'h2, 4'h4
};
localparam [0:63] inv_sigma1 = sigma1;

localparam [0:63] sigma2 = {
    4'hb, 4'h6, 4'h8, 4'hf, 4'hc, 4'h0, 4'h9, 4'he,
    4'h3, 4'h7, 4'h4, 4'h5, 4'hd, 4'h2, 4'h1, 4'ha
};
localparam [0:63] inv_sigma2 = {
    4'h5, 4'he, 4'hd, 4'h8, 4'ha, 4'hb, 4'h1, 4'h9,
    4'h2, 4'h6, 4'hf, 4'h0, 4'h4, 4'hc, 4'h7, 4'h3
};

// new sbox
// 0,8,9,13,2,10,14,12,4,11,1,15,7,3,6,5
localparam [0:63] sigma49 = {
    4'h0, 4'h8, 4'h9, 4'hd, 4'h2, 4'ha, 4'he, 4'hc,
    4'h4, 4'hb, 4'h1, 4'hf, 4'h7, 4'h3, 4'h6, 4'h5
};
// 0,10,4,13,8,15,14,12,1,2,5,9,7,3,6,11
localparam [0:63] inv_sigma49 = {
    4'h0, 4'ha, 4'h4, 4'hd, 4'h8, 4'hf, 4'he, 4'hc,
    4'h1, 4'h2, 4'h5, 4'h9, 4'h7, 4'h3, 4'h6, 4'hb
};
// 0,8,2,10,9,13,14,12,4,11,7,3,1,15,6,5
localparam [0:63] sigma146 = {
    4'h0, 4'h8, 4'h2, 4'ha, 4'h9, 4'hd, 4'he, 4'hc,
    4'h4, 4'hb, 4'h7, 4'h3, 4'h1, 4'hf, 4'h6, 4'h5
};
// 0,12,2,11,8,15,14,10,1,4,3,9,7,5,6,13
localparam [0:63] inv_sigma146 = {
    4'h0, 4'hc, 4'h2, 4'hb, 4'h8, 4'hf, 4'he, 4'ha,
    4'h1, 4'h4, 4'h3, 4'h9, 4'h7, 4'h5, 4'h6, 4'hd
};



// MixColumns params
// involutary
localparam [0:11] M_42 = {4'h1, 4'h2, 4'h1};
localparam [0:11] Q_42 = M_42;

// involutory
localparam [0:11] M_82 = {4'h1, 4'h4, 4'h5};
localparam [0:11] Q_82 = M_82;