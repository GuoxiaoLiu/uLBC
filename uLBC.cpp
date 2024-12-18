
#include<stdio.h> 
#include<stdlib.h> 
#include<time.h> 
#include <string.h>
// #include "cipher_mode.h"

#include<iostream>
#include <iomanip>
using namespace std;

#define ROUND_BLOCK_64_128s 14
#define ROUND_BLOCK_64_128 20
#define ROUND_BLOCK_64_256 24
#define ROUND_BLOCK_256_256 16
#define ROUND_BLOCK_256_512 20
#define ROUND_128s 18
#define ROUND_128 22
#define ROUND_256s 22
#define ROUND_256 28
#define ROUND_384 30


static const unsigned char uLBC_Box[16] = {
	8, 0, 1, 5, 12, 7, 4, 6, 2, 10, 3, 13, 14, 15, 11, 9
};

static const unsigned char uLBC_Box_Inv[16] = {
	1, 2, 8, 10, 6, 3, 7, 5, 0, 15, 9, 14, 4, 11, 12, 13
};

static const unsigned char uLBC_Box_256[256] = {
	0x10, 0x11, 0x12, 0x79, 0x7d, 0x2c, 0xe8, 0xf6, 0xb5, 0xe3, 0x73, 0xcf, 0xf1, 0x87, 0x4e, 0x25,
    0x42, 0xbc, 0xa6, 0xcc, 0xdd, 0xa9, 0x60, 0x77, 0x3b, 0xa8, 0x0f, 0x6c, 0x9b, 0xb2, 0x2e, 0xd2,
    0x29, 0x28, 0x14, 0x76, 0x84, 0x2a, 0x58, 0x54, 0x9e, 0xf7, 0x91, 0xdb, 0x37, 0xe5, 0x41, 0xa3,
    0x16, 0x45, 0x8d, 0x03, 0xbf, 0x4b, 0xd0, 0xea, 0xf4, 0x8a, 0xc9, 0xce, 0x1d, 0x90, 0x7a, 0x4f,
    0x32, 0x0c, 0x30, 0x1b, 0xbb, 0x31, 0xd1, 0x86, 0x4c, 0x88, 0xa7, 0x92, 0xc8, 0x53, 0xec, 0x9c,
    0xf2, 0x05, 0xd6, 0x6e, 0x89, 0xfd, 0x2f, 0x0e, 0x59, 0xc3, 0x40, 0x95, 0x5d, 0xef, 0x57, 0x62,
    0x1c, 0x22, 0x21, 0xfc, 0x15, 0x20, 0x3f, 0x85, 0xc6, 0x82, 0xed, 0x98, 0x50, 0x99, 0x0b, 0x4d,
    0x47, 0xab, 0x5c, 0x49, 0x43, 0xb7, 0xc1, 0x72, 0x7e, 0xff, 0xd8, 0x96, 0x8c, 0xd3, 0xe2, 0x1e,
    0x69, 0x5e, 0x2b, 0x00, 0xcd, 0xdc, 0x7c, 0x70, 0xe1, 0x3e, 0x35, 0x52, 0xa5, 0xac, 0x01, 0x63,
    0xdf, 0x97, 0x1f, 0x3c, 0x6d, 0x8b, 0xe6, 0xb8, 0x02, 0x67, 0xb6, 0xf8, 0xb9, 0xc2, 0xf3, 0xa2,
    0x56, 0x61, 0xd4, 0x09, 0x9f, 0x23, 0xee, 0xf0, 0x17, 0x5a, 0x83, 0x36, 0xbe, 0x65, 0x08, 0xb1,
    0xfb, 0xc5, 0xe9, 0x0a, 0xa4, 0x3d, 0x74, 0xaa, 0x34, 0xca, 0xad, 0x78, 0x6b, 0x6f, 0xd7, 0xb0,
    0x33, 0xc4, 0x71, 0x1a, 0xa1, 0x46, 0xbd, 0x07, 0xe0, 0x7f, 0xb4, 0xda, 0x64, 0x2d, 0x24, 0x8f,
    0xba, 0x7b, 0x68, 0x19, 0x93, 0x75, 0xae, 0xeb, 0x18, 0x26, 0xfe, 0xf9, 0xd5, 0x4a, 0xa0, 0xc7,
    0xe4, 0x13, 0x66, 0x04, 0x5b, 0x51, 0x8e, 0xd9, 0x48, 0xfa, 0x55, 0x5f, 0xde, 0xb3, 0x3a, 0xf5,
    0x80, 0x81, 0xc0, 0x38, 0x0d, 0x94, 0x9d, 0xe7, 0x39, 0x6a, 0x44, 0x27, 0xaf, 0x06, 0x9a, 0xcb
};

static const unsigned char uLBC_Box_256_Inv[256] = {
	0x83, 0x8e, 0x98, 0x33, 0xe3, 0x51, 0xfd, 0xc7, 0xae, 0xa3, 0xb3, 0x6e, 0x41, 0xf4, 0x57, 0x1a,
	0x00, 0x01, 0x02, 0xe1, 0x22, 0x64, 0x30, 0xa8, 0xd8, 0xd3, 0xc3, 0x43, 0x60, 0x3c, 0x7f, 0x92,
	0x65, 0x62, 0x61, 0xa5, 0xce, 0x0f, 0xd9, 0xfb, 0x21, 0x20, 0x25, 0x82, 0x05, 0xcd, 0x1e, 0x56,
	0x42, 0x45, 0x40, 0xc0, 0xb8, 0x8a, 0xab, 0x2c, 0xf3, 0xf8, 0xee, 0x18, 0x93, 0xb5, 0x89, 0x66, 
	0x5a, 0x2e, 0x10, 0x74, 0xfa, 0x31, 0xc5, 0x70, 0xe8, 0x73, 0xdd, 0x35, 0x48, 0x6f, 0x0e, 0x3f, 
	0x6c, 0xe5, 0x8b, 0x4d, 0x27, 0xea, 0xa0, 0x5e, 0x26, 0x58, 0xa9, 0xe4, 0x72, 0x5c, 0x81, 0xeb, 
	0x16, 0xa1, 0x5f, 0x8f, 0xcc, 0xad, 0xe2, 0x99, 0xd2, 0x80, 0xf9, 0xbc, 0x1b, 0x94, 0x53, 0xbd, 
	0x87, 0xc2, 0x77, 0x0a, 0xb6, 0xd5, 0x23, 0x17, 0xbb, 0x03, 0x3e, 0xd1, 0x86, 0x04, 0x78, 0xc9, 
	0xf0, 0xf1, 0x69, 0xaa, 0x24, 0x67, 0x47, 0x0d, 0x49, 0x54, 0x39, 0x95, 0x7c, 0x32, 0xe6, 0xcf, 
	0x3d, 0x2a, 0x4b, 0xd4, 0xf5, 0x5b, 0x7b, 0x91, 0x6b, 0x6d, 0xfe, 0x1c, 0x4f, 0xf6, 0x28, 0xa4, 
	0xde, 0xc4, 0x9f, 0x2f, 0xb4, 0x8c, 0x12, 0x4a, 0x19, 0x15, 0xb7, 0x71, 0x8d, 0xba, 0xd6, 0xfc, 
	0xbf, 0xaf, 0x1d, 0xed, 0xca, 0x08, 0x9a, 0x75, 0x97, 0x9c, 0xd0, 0x44, 0x11, 0xc6, 0xac, 0x34, 
	0xf2, 0x76, 0x9d, 0x59, 0xc1, 0xb1, 0x68, 0xdf, 0x4c, 0x3a, 0xb9, 0xff, 0x13, 0x84, 0x3b, 0x0b, 
	0x36, 0x46, 0x1f, 0x7d, 0xa2, 0xdc, 0x52, 0xbe, 0x7a, 0xe7, 0xcb, 0x2b, 0x85, 0x14, 0xec, 0x90, 
	0xc8, 0x88, 0x7e, 0x09, 0xe0, 0x2d, 0x96, 0xf7, 0x06, 0xb2, 0x37, 0xd7, 0x4e, 0x6a, 0xa6, 0x5d, 
	0xa7, 0x0c, 0x50, 0x9e, 0x38, 0xef, 0x07, 0x29, 0x9b, 0xdb, 0xe9, 0xb0, 0x63, 0x55, 0xda, 0x79
};

static const unsigned char Pos_Box[32] = {
	0, 25, 18, 15, 4, 1, 30, 19,
	8, 29, 22, 7, 12, 9, 26, 23,
	20, 5, 10, 31, 16, 13, 2, 27,
	28, 17, 6, 3, 24, 21, 14, 11
};

static const unsigned char Pos_Box_Inv[32] = {
	0, 5, 22, 27, 4, 17, 26, 11,
	8, 13, 18, 31, 12, 21, 30, 3,
	20, 25, 2, 7, 16, 29, 10, 15,
	28, 1, 14, 23, 24, 9, 6, 19
};

static const unsigned char Pos_Box_64[16] = {
	0, 10, 5, 15, 14, 4, 11, 1, 9, 3, 12, 6, 7, 13, 2, 8
};

static const unsigned char Pos_Box_Inv_64[16] = {
	0, 7, 14, 9, 5, 2, 11, 12, 15, 8, 1, 6, 10, 13, 4, 3
};

static const unsigned char F_Box[32] = {
	8, 24, 28, 23, 9, 31, 14, 17,
	20, 19, 26, 5, 18, 2, 6, 30,
	4, 15, 11, 25, 10, 29, 12, 3,
	21, 16, 7, 0, 27, 13, 22, 1
};

static const unsigned char lF_Box_1[16] = {
	0, 8, 1, 9, 2, 10, 3, 11, 12, 4, 13, 5, 14, 6, 15, 7
};

static const unsigned char llF_Box[16] = {
	0, 2, 4, 6, 9, 11, 13, 15, 1, 3, 5, 7, 8, 10, 12, 14
};

static const unsigned char F_Box_64[16] = {
	3, 2, 0, 6, 1, 10, 4, 15, 11, 8, 9, 12, 13, 14, 5, 7
};

static const unsigned char lF_Box_2[16] = {
	0, 2, 4, 6, 9, 11, 13, 15, 1, 3, 5, 7, 8, 10, 12, 14
};

static const unsigned char lF_Box_3[16] = {
	0, 4, 9, 13, 3, 7, 10, 14, 2, 6, 11, 15, 1, 5, 8, 12
};

static const unsigned char lF_Box_256[256] = {
	0x0, 0x2, 0x5, 0x7, 0x9, 0xb, 0xc, 0xe, 0x10, 0x12, 0x15, 0x17, 0x19, 0x1b, 0x1c, 0x1e, 
0x20, 0x22, 0x25, 0x27, 0x29, 0x2b, 0x2c, 0x2e, 0x30, 0x32, 0x35, 0x37, 0x39, 0x3b, 0x3c, 0x3e, 
0x40, 0x42, 0x45, 0x47, 0x49, 0x4b, 0x4c, 0x4e, 0x50, 0x52, 0x55, 0x57, 0x59, 0x5b, 0x5c, 0x5e, 
0x60, 0x62, 0x65, 0x67, 0x69, 0x6b, 0x6c, 0x6e, 0x70, 0x72, 0x75, 0x77, 0x79, 0x7b, 0x7c, 0x7e, 
0x81, 0x83, 0x84, 0x86, 0x88, 0x8a, 0x8d, 0x8f, 0x91, 0x93, 0x94, 0x96, 0x98, 0x9a, 0x9d, 0x9f, 
0xa1, 0xa3, 0xa4, 0xa6, 0xa8, 0xaa, 0xad, 0xaf, 0xb1, 0xb3, 0xb4, 0xb6, 0xb8, 0xba, 0xbd, 0xbf, 
0xc1, 0xc3, 0xc4, 0xc6, 0xc8, 0xca, 0xcd, 0xcf, 0xd1, 0xd3, 0xd4, 0xd6, 0xd8, 0xda, 0xdd, 0xdf, 
0xe1, 0xe3, 0xe4, 0xe6, 0xe8, 0xea, 0xed, 0xef, 0xf1, 0xf3, 0xf4, 0xf6, 0xf8, 0xfa, 0xfd, 0xff, 
0x1, 0x3, 0x4, 0x6, 0x8, 0xa, 0xd, 0xf, 0x11, 0x13, 0x14, 0x16, 0x18, 0x1a, 0x1d, 0x1f, 
0x21, 0x23, 0x24, 0x26, 0x28, 0x2a, 0x2d, 0x2f, 0x31, 0x33, 0x34, 0x36, 0x38, 0x3a, 0x3d, 0x3f, 
0x41, 0x43, 0x44, 0x46, 0x48, 0x4a, 0x4d, 0x4f, 0x51, 0x53, 0x54, 0x56, 0x58, 0x5a, 0x5d, 0x5f, 
0x61, 0x63, 0x64, 0x66, 0x68, 0x6a, 0x6d, 0x6f, 0x71, 0x73, 0x74, 0x76, 0x78, 0x7a, 0x7d, 0x7f, 
0x80, 0x82, 0x85, 0x87, 0x89, 0x8b, 0x8c, 0x8e, 0x90, 0x92, 0x95, 0x97, 0x99, 0x9b, 0x9c, 0x9e, 
0xa0, 0xa2, 0xa5, 0xa7, 0xa9, 0xab, 0xac, 0xae, 0xb0, 0xb2, 0xb5, 0xb7, 0xb9, 0xbb, 0xbc, 0xbe, 
0xc0, 0xc2, 0xc5, 0xc7, 0xc9, 0xcb, 0xcc, 0xce, 0xd0, 0xd2, 0xd5, 0xd7, 0xd9, 0xdb, 0xdc, 0xde, 
0xe0, 0xe2, 0xe5, 0xe7, 0xe9, 0xeb, 0xec, 0xee, 0xf0, 0xf2, 0xf5, 0xf7, 0xf9, 0xfb, 0xfc, 0xfe,
};

static const unsigned char uLBC_const_c0[32] = {
	0x8, 0xc, 0xe, 0xf, 0xf, 0x7, 0xb, 0xd,
	0xe, 0xf, 0x7, 0x3, 0x9, 0xc, 0xe, 0x7,
	0xb, 0x5, 0xa, 0xd, 0x6, 0x3, 0x1, 0xc,
	0x8, 0x4, 0xa, 0xd, 0xe, 0x7, 0x3, 0x1
};

static const unsigned char uLBC_const_c1[32] = {
	0x0, 0x0, 0x0, 0x0, 0x8, 0xc, 0xc, 0xc,
	0xc, 0x4, 0x8, 0xc, 0xc, 0xc, 0x4, 0x0,
	0x8, 0xc, 0xc, 0x4, 0x8, 0x4, 0x8, 0x0,
	0x4, 0x0, 0x0, 0x0, 0x8, 0x4, 0x8, 0xc
};

static const unsigned char uLBC_const_256_c0[32] = {
	0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0x7c, 0xbc, 0xdc,
	0xec, 0xf4, 0x78, 0x3c, 0x9c, 0xcc, 0xe4, 0x70,
	0xb8, 0x5c, 0xac, 0xd4, 0x68, 0x34, 0x18, 0x0c,
	0x84, 0x40, 0xa0, 0xd0, 0xe8, 0x74, 0x38, 0x1c
};

static const unsigned char uLBC_const_256_c3[32] = {
    0xa0, 0xac, 0x93, 0x29, 0xac, 0x4b, 0xc9, 0x91,
	0xc2, 0x31, 0x32, 0x19, 0xc1, 0x93, 0xca, 0x81,
	0x44, 0x20, 0xcb, 0x8b, 0x49, 0xcc, 0x9b, 0xa8,
	0x82, 0xc1, 0x04, 0xba, 0x4a, 0x22, 0xc9, 0x18	
};

static const unsigned char uLBC_const_c2_128 = 0x5;
static const unsigned char uLBC_const_c3_128 = 0xa;
static const unsigned char uLBC_const_c4_128 = 0x5;
static const unsigned char uLBC_const_c4_128s = 0x3;
static const unsigned char uLBC_const_c5_128 = 0xa;

static const unsigned char uLBC_const_c2_256 = 0xa;
static const unsigned char uLBC_const_c3_256 = 0x3;
static const unsigned char uLBC_const_c4_256 = 0xa;
static const unsigned char uLBC_const_c5_256 = 0x3;
static const unsigned char uLBC_const_c5_256s = 0x5;

static const unsigned char uLBC_const_c2_384 = 0x5;
static const unsigned char uLBC_const_c3_384 = 0xa;
static const unsigned char uLBC_const_c4_384 = 0x5;
static const unsigned char uLBC_const_c5_384 = 0xa;
static const unsigned char uLBC_const_c2_128_64 = 0x5;
static const unsigned char uLBC_const_c3_128_64 = 0xa;
static const unsigned char uLBC_const_c4_128_64 = 0x5;
static const unsigned char uLBC_const_c5_128_64 = 0xa;
static const unsigned char uLBC_const_c2_256_64 = 0xa;
static const unsigned char uLBC_const_c3_256_64 = 0x3;
static const unsigned char uLBC_const_c4_256_64 = 0xa;
static const unsigned char uLBC_const_c5_256_64 = 0x3;
static const unsigned char uLBC_const_c1_256_256 = 0x5a;
static const unsigned char uLBC_const_c2_256_256 = 0xa5;
static const unsigned char uLBC_const_c1_256_512 = 0xa3;
static const unsigned char uLBC_const_c2_256_512 = 0x3a;

static const unsigned char uLBC_const_c6[32] = {
	0xa, 0xa, 0x9, 0x2, 0xa, 0x4, 0xc, 0x9,
	0xc, 0x3, 0x3, 0x1, 0xc, 0x9, 0xc, 0x8,
	0x4, 0x2, 0xc, 0x8, 0x4, 0xc, 0x9, 0xa,
	0x8, 0xc, 0x0, 0xb, 0x4, 0x2, 0xc, 0x1
};

static const unsigned char uLBC_const_c7[32] = {
	0x0, 0xc, 0x3, 0x9, 0xc, 0xb, 0x9, 0x1,
	0x2, 0x1, 0x2, 0x9, 0x1, 0x3, 0xa, 0x1,
	0x4, 0x0, 0xb, 0xb, 0x9, 0xc, 0xb, 0x8,
	0x2, 0x1, 0x4, 0xa, 0xa, 0x2, 0x9, 0x8
};

unsigned char MDS_256_2[] = {
    0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254, 195, 193, 199, 197, 203, 201, 207, 205, 211, 209, 215, 213, 219, 217, 223, 221, 227, 225, 231, 229, 235, 233, 239, 237, 243, 241, 247, 245, 251, 249, 255, 253, 131, 129, 135, 133, 139, 137, 143, 141, 147, 145, 151, 149, 155, 153, 159, 157, 163, 161, 167, 165, 171, 169, 175, 173, 179, 177, 183, 181, 187, 185, 191, 189, 67, 65, 71, 69, 75, 73, 79, 77, 83, 81, 87, 85, 91, 89, 95, 93, 99, 97, 103, 101, 107, 105, 111, 109, 115, 113, 119, 117, 123, 121, 127, 125, 3, 1, 7, 5, 11, 9, 15, 13, 19, 17, 23, 21, 27, 25, 31, 29, 35, 33, 39, 37, 43, 41, 47, 45, 51, 49, 55, 53, 59, 57, 63, 61
};

unsigned char MDS_256_0x91[] = {
    0, 145, 225, 112, 1, 144, 224, 113, 2, 147, 227, 114, 3, 146, 226, 115, 4, 149, 229, 116, 5, 148, 228, 117, 6, 151, 231, 118, 7, 150, 230, 119, 8, 153, 233, 120, 9, 152, 232, 121, 10, 155, 235, 122, 11, 154, 234, 123, 12, 157, 237, 124, 13, 156, 236, 125, 14, 159, 239, 126, 15, 158, 238, 127, 16, 129, 241, 96, 17, 128, 240, 97, 18, 131, 243, 98, 19, 130, 242, 99, 20, 133, 245, 100, 21, 132, 244, 101, 22, 135, 247, 102, 23, 134, 246, 103, 24, 137, 249, 104, 25, 136, 248, 105, 26, 139, 251, 106, 27, 138, 250, 107, 28, 141, 253, 108, 29, 140, 252, 109, 30, 143, 255, 110, 31, 142, 254, 111, 32, 177, 193, 80, 33, 176, 192, 81, 34, 179, 195, 82, 35, 178, 194, 83, 36, 181, 197, 84, 37, 180, 196, 85, 38, 183, 199, 86, 39, 182, 198, 87, 40, 185, 201, 88, 41, 184, 200, 89, 42, 187, 203, 90, 43, 186, 202, 91, 44, 189, 205, 92, 45, 188, 204, 93, 46, 191, 207, 94, 47, 190, 206, 95, 48, 161, 209, 64, 49, 160, 208, 65, 50, 163, 211, 66, 51, 162, 210, 67, 52, 165, 213, 68, 53, 164, 212, 69, 54, 167, 215, 70, 55, 166, 214, 71, 56, 169, 217, 72, 57, 168, 216, 73, 58, 171, 219, 74, 59, 170, 218, 75, 60, 173, 221, 76, 61, 172, 220, 77, 62, 175, 223, 78, 63, 174, 222, 79
};

unsigned char MDS_256_0x55[] = {
	0, 85, 170, 255, 151, 194, 61, 104, 237, 184, 71, 18, 122, 47, 208, 133, 25, 76, 179, 230, 142, 219, 36, 113, 244, 161, 94, 11, 99, 54, 201, 156, 50, 103, 152, 205, 165, 240, 15, 90, 223, 138, 117, 32, 72, 29, 226, 183, 43, 126, 129, 212, 188, 233, 22, 67, 198, 147, 108, 57, 81, 4, 251, 174, 100, 49, 206, 155, 243, 166, 89, 12, 137, 220, 35, 118, 30, 75, 180, 225, 125, 40, 215, 130, 234, 191, 64, 21, 144, 197, 58, 111, 7, 82, 173, 248, 86, 3, 252, 169, 193, 148, 107, 62, 187, 238, 17, 68, 44, 121, 134, 211, 79, 26, 229, 176, 216, 141, 114, 39, 162, 247, 8, 93, 53, 96, 159, 202, 200, 157, 98, 55, 95, 10, 245, 160, 37, 112, 143, 218, 178, 231, 24, 77, 209, 132, 123, 46, 70, 19, 236, 185, 60, 105, 150, 195, 171, 254, 1, 84, 250, 175, 80, 5, 109, 56, 199, 146, 23, 66, 189, 232, 128, 213, 42, 127, 227, 182, 73, 28, 116, 33, 222, 139, 14, 91, 164, 241, 153, 204, 51, 102, 172, 249, 6, 83, 59, 110, 145, 196, 65, 20, 235, 190, 214, 131, 124, 41, 181, 224, 31, 74, 34, 119, 136, 221, 88, 13, 242, 167, 207, 154, 101, 48, 158, 203, 52, 97, 9, 92, 163, 246, 115, 38, 217, 140, 228, 177, 78, 27, 135, 210, 45, 120, 16, 69, 186, 239, 106, 63, 192, 149, 253, 168, 87, 2
};

unsigned char MDS_256_0x41[] = {
	0, 65, 130, 195, 199, 134, 69, 4, 77, 12, 207, 142, 138, 203, 8, 73, 154, 219, 24, 89, 93, 28, 223, 158, 215, 150, 85, 20, 16, 81, 146, 211, 247, 182, 117, 52, 48, 113, 178, 243, 186, 251, 56, 121, 125, 60, 255, 190, 109, 44, 239, 174, 170, 235, 40, 105, 32, 97, 162, 227, 231, 166, 101, 36, 45, 108, 175, 238, 234, 171, 104, 41, 96, 33, 226, 163, 167, 230, 37, 100, 183, 246, 53, 116, 112, 49, 242, 179, 250, 187, 120, 57, 61, 124, 191, 254, 218, 155, 88, 25, 29, 92, 159, 222, 151, 214, 21, 84, 80, 17, 210, 147, 64, 1, 194, 131, 135, 198, 5, 68, 13, 76, 143, 206, 202, 139, 72, 9, 90, 27, 216, 153, 157, 220, 31, 94, 23, 86, 149, 212, 208, 145, 82, 19, 192, 129, 66, 3, 7, 70, 133, 196, 141, 204, 15, 78, 74, 11, 200, 137, 173, 236, 47, 110, 106, 43, 232, 169, 224, 161, 98, 35, 39, 102, 165, 228, 55, 118, 181, 244, 240, 177, 114, 51, 122, 59, 248, 185, 189, 252, 63, 126, 119, 54, 245, 180, 176, 241, 50, 115, 58, 123, 184, 249, 253, 188, 127, 62, 237, 172, 111, 46, 42, 107, 168, 233, 160, 225, 34, 99, 103, 38, 229, 164, 128, 193, 2, 67, 71, 6, 197, 132, 205, 140, 79, 14, 10, 75, 136, 201, 26, 91, 152, 217, 221, 156, 95, 30, 87, 22, 213, 148, 144, 209, 18, 83
};

unsigned char MDS_256_0x71[] = {
	0, 113, 226, 147, 7, 118, 229, 148, 14, 127, 236, 157, 9, 120, 235, 154, 28, 109, 254, 143, 27, 106, 249, 136, 18, 99, 240, 129, 21, 100, 247, 134, 56, 73, 218, 171, 63, 78, 221, 172, 54, 71, 212, 165, 49, 64, 211, 162, 36, 85, 198, 183, 35, 82, 193, 176, 42, 91, 200, 185, 45, 92, 207, 190, 112, 1, 146, 227, 119, 6, 149, 228, 126, 15, 156, 237, 121, 8, 155, 234, 108, 29, 142, 255, 107, 26, 137, 248, 98, 19, 128, 241, 101, 20, 135, 246, 72, 57, 170, 219, 79, 62, 173, 220, 70, 55, 164, 213, 65, 48, 163, 210, 84, 37, 182, 199, 83, 34, 177, 192, 90, 43, 184, 201, 93, 44, 191, 206, 224, 145, 2, 115, 231, 150, 5, 116, 238, 159, 12, 125, 233, 152, 11, 122, 252, 141, 30, 111, 251, 138, 25, 104, 242, 131, 16, 97, 245, 132, 23, 102, 216, 169, 58, 75, 223, 174, 61, 76, 214, 167, 52, 69, 209, 160, 51, 66, 196, 181, 38, 87, 195, 178, 33, 80, 202, 187, 40, 89, 205, 188, 47, 94, 144, 225, 114, 3, 151, 230, 117, 4, 158, 239, 124, 13, 153, 232, 123, 10, 140, 253, 110, 31, 139, 250, 105, 24, 130, 243, 96, 17, 133, 244, 103, 22, 168, 217, 74, 59, 175, 222, 77, 60, 166, 215, 68, 53, 161, 208, 67, 50, 180, 197, 86, 39, 179, 194, 81, 32, 186, 203, 88, 41, 189, 204, 95, 46
};

unsigned char MDS_256_0x5a[] = {
	0, 90, 180, 238, 171, 241, 31, 69, 149, 207, 33, 123, 62, 100, 138, 208, 233, 179, 93, 7, 66, 24, 246, 172, 124, 38, 200, 146, 215, 141, 99, 57, 17, 75, 165, 255, 186, 224, 14, 84, 132, 222, 48, 106, 47, 117, 155, 193, 248, 162, 76, 22, 83, 9, 231, 189, 109, 55, 217, 131, 198, 156, 114, 40, 34, 120, 150, 204, 137, 211, 61, 103, 183, 237, 3, 89, 28, 70, 168, 242, 203, 145, 127, 37, 96, 58, 212, 142, 94, 4, 234, 176, 245, 175, 65, 27, 51, 105, 135, 221, 152, 194, 44, 118, 166, 252, 18, 72, 13, 87, 185, 227, 218, 128, 110, 52, 113, 43, 197, 159, 79, 21, 251, 161, 228, 190, 80, 10, 68, 30, 240, 170, 239, 181, 91, 1, 209, 139, 101, 63, 122, 32, 206, 148, 173, 247, 25, 67, 6, 92, 178, 232, 56, 98, 140, 214, 147, 201, 39, 125, 85, 15, 225, 187, 254, 164, 74, 16, 192, 154, 116, 46, 107, 49, 223, 133, 188, 230, 8, 82, 23, 77, 163, 249, 41, 115, 157, 199, 130, 216, 54, 108, 102, 60, 210, 136, 205, 151, 121, 35, 243, 169, 71, 29, 88, 2, 236, 182, 143, 213, 59, 97, 36, 126, 144, 202, 26, 64, 174, 244, 177, 235, 5, 95, 119, 45, 195, 153, 220, 134, 104, 50, 226, 184, 86, 12, 73, 19, 253, 167, 158, 196, 42, 112, 53, 111, 129, 219, 11, 81, 191, 229, 160, 250, 20, 78
};


void KeyGen_128_64(unsigned int MK[4], unsigned int subkey[])
{
	unsigned int subkey_0_64[22*2];
	unsigned int subkey_1_64[22*2];
	for(int i = 0; i < 2; i ++){
		subkey_0_64[i] = MK[i];
		subkey_1_64[i] = MK[i+2];
	}

	for(int R = 1; R <= ROUND_BLOCK_64_128; R ++){
		subkey_0_64[R*2] = subkey_0_64[R*2+1] = 0;
		subkey_1_64[R*2] = subkey_1_64[R*2+1] = 0;
		unsigned int k_tmp[2] = { 0 };
		for(int i = 0; i < 16; i ++){
			int pos = i/8;
			int shift = 28 - (i%8) * 4;
			k_tmp[pos] |= lF_Box_1[(subkey_1_64[2*(R-1) + pos] >> shift) & 0xF] << shift;
		}	
		for(int i = 0; i < 16; i ++){
			int pos = R*2 + i/8;
			int shift = 28 - (i%8) * 4;
			int pos_o = F_Box_64[i]/8;
			int shift_o = 28 - (F_Box_64[i]%8) * 4;
			subkey_0_64[pos] |= ((subkey_0_64[2*(R-1) + pos_o] >> shift_o) & 0xF) << shift;
			subkey_1_64[pos] |= ((k_tmp[pos_o] >> shift_o) & 0xF) << shift;
		}
	}

	for(int i = 0; i < (ROUND_BLOCK_64_128+1)*2; i ++){
		subkey[i] = subkey_0_64[i] ^ subkey_1_64[i];
	}
}

void KeyGen_256_64(unsigned int MK[8], unsigned int subkey[])
{
	unsigned int subkey_0_64[26*2];
	unsigned int subkey_1_64[26*2];
	unsigned int subkey_2_64[26*2];
	unsigned int subkey_3_64[26*2];
	for(int i = 0; i < 2; i ++){
		subkey_0_64[i] = MK[i];
		subkey_1_64[i] = MK[i+2];
		subkey_2_64[i] = MK[i+4];
		subkey_3_64[i] = MK[i+6];
	}

	for(int R = 1; R <= ROUND_BLOCK_64_256; R ++){
		subkey_0_64[R*2] = subkey_0_64[R*2+1] = 0;
		subkey_1_64[R*2] = subkey_1_64[R*2+1] = 0;
		subkey_2_64[R*2] = subkey_2_64[R*2+1] = 0;
		subkey_3_64[R*2] = subkey_3_64[R*2+1] = 0;
		unsigned int k_tmp_1[2] = { 0 };
		unsigned int k_tmp_2[2] = { 0 };
		unsigned int k_tmp_3[2] = { 0 };
		for(int i = 0; i < 16; i ++){
			int pos = i/8;
			int shift = 28 - (i%8) * 4;
			k_tmp_1[pos] |= lF_Box_1[(subkey_1_64[2*(R-1) + pos] >> shift) & 0xF] << shift;
			k_tmp_2[pos] |= lF_Box_2[(subkey_2_64[2*(R-1) + pos] >> shift) & 0xF] << shift;
			k_tmp_3[pos] |= lF_Box_3[(subkey_3_64[2*(R-1) + pos] >> shift) & 0xF] << shift;
		}	
		for(int i = 0; i < 16; i ++){
			int pos = R*2 + i/8;
			int shift = 28 - (i%8) * 4;
			int pos_o = F_Box_64[i]/8;
			int shift_o = 28 - (F_Box_64[i]%8) * 4;
			subkey_0_64[pos] |= ((subkey_0_64[2*(R-1) + pos_o] >> shift_o) & 0xF) << shift;
			subkey_1_64[pos] |= ((k_tmp_1[pos_o] >> shift_o) & 0xF) << shift;
			subkey_2_64[pos] |= ((k_tmp_2[pos_o] >> shift_o) & 0xF) << shift;
			subkey_3_64[pos] |= ((k_tmp_3[pos_o] >> shift_o) & 0xF) << shift;
		}
	}

	for(int i = 0; i < (ROUND_BLOCK_64_256+1)*2; i ++){
		subkey[i] = subkey_0_64[i] ^ subkey_1_64[i] ^ subkey_2_64[i] ^ subkey_3_64[i];
	}
}

// 
void KeyGen_128(unsigned int MK[4], unsigned int subkey[])
{
	for(int i = 0; i < 4; i ++){
		subkey[i] = MK[i];
	}

	for(int R = 1; R <= ROUND_128; R ++){
		subkey[R*4] = subkey[R*4+1] = subkey[R*4+2] = subkey[R*4+3] = 0;
		for(int i = 0; i < 32; i ++){
			int pos = R*4 + i/8;
			int shift = 28 - (i%8) * 4;
			int pos_o = (R-1) * 4 + F_Box[i]/8;
			int shift_o = 28 - (F_Box[i]%8) * 4;
			subkey[pos] |= ((subkey[pos_o] >> shift_o) & 0xF) << shift;
		}
	}
}

// 

unsigned int subkey_0[32*4];
unsigned int subkey_1[32*4];
unsigned int subkey_2[32*4];
void KeyGen_256(unsigned int MK[8], unsigned int subkey[])
{
	for(int i = 0; i < 4; i ++){
		subkey_0[i] = MK[i];
		subkey_1[i] = MK[i+4];
	}

	for(int R = 1; R <= ROUND_256; R ++){
		subkey_0[R*4] = subkey_0[R*4+1] = subkey_0[R*4+2] = subkey_0[R*4+3] = 0;
		subkey_1[R*4] = subkey_1[R*4+1] = subkey_1[R*4+2] = subkey_1[R*4+3] = 0;
		unsigned int k_tmp[4] = { 0 };
		for(int i = 0; i < 32; i ++){
			int pos = i/8;
			int shift = 28 - (i%8) * 4;
			k_tmp[pos] |= lF_Box_1[(subkey_1[4*(R-1) + pos] >> shift) & 0xF] << shift;
		}	
		for(int i = 0; i < 32; i ++){
			int pos = R*4 + i/8;
			int shift = 28 - (i%8) * 4;
			int pos_o = F_Box[i]/8;
			int shift_o = 28 - (F_Box[i]%8) * 4;
			subkey_0[pos] |= ((subkey_0[4*(R-1) + pos_o] >> shift_o) & 0xF) << shift;
			subkey_1[pos] |= ((k_tmp[pos_o] >> shift_o) & 0xF) << shift;
		}
	}

	for(int i = 0; i < (ROUND_256+1)*4; i ++){
		subkey[i] = subkey_0[i] ^ subkey_1[i];
	}

}


void KeyGen_384(unsigned int MK[12], unsigned int subkey[]){
	for(int i = 0; i < 4; i ++){
		subkey_0[i] = MK[i];
		subkey_1[i] = MK[i+4];
		subkey_2[i] = MK[i+8];
	}
	for(int R = 1; R <= ROUND_384; R ++){
		subkey_0[R*4] = subkey_0[R*4+1] = subkey_0[R*4+2] = subkey_0[R*4+3] = 0;
		subkey_1[R*4] = subkey_1[R*4+1] = subkey_1[R*4+2] = subkey_1[R*4+3] = 0;
		subkey_2[R*4] = subkey_2[R*4+1] = subkey_2[R*4+2] = subkey_2[R*4+3] = 0;
		unsigned int k_tmp_l[4] = { 0 };
		unsigned int k_tmp_ll[4] = { 0 };
		for(int i = 0; i < 32; i ++){
			int pos = i/8;
			int shift = 28 - (i%8) * 4;
			k_tmp_l[pos] |= lF_Box_1[(subkey_1[4*(R-1) + pos] >> shift) & 0xF] << shift;
			k_tmp_ll[pos] |= llF_Box[(subkey_2[4*(R-1) + pos] >> shift) & 0xF] << shift;
		}	
		for(int i = 0; i < 32; i ++){
			int pos = R*4 + i/8;
			int shift = 28 - (i%8) * 4;
			int pos_o = F_Box[i]/8;
			int shift_o = 28 - (F_Box[i]%8) * 4;
			subkey_0[pos] |= ((subkey_0[4*(R-1) + pos_o] >> shift_o) & 0xF) << shift;
			subkey_1[pos] |= ((k_tmp_l[pos_o] >> shift_o) & 0xF) << shift;
			subkey_2[pos] |= ((k_tmp_ll[pos_o] >> shift_o) & 0xF) << shift;
		}
	}
	for(int i = 0; i < (ROUND_384+1)*4; i ++){
		subkey[i] = subkey_0[i] ^ subkey_1[i] ^ subkey_2[i];
	}
}

void KeyGen_256_256(unsigned int MK[8], unsigned int subkey[]){
	for(int i = 0; i < 8; i ++){
		subkey[i] = MK[i];
	}

	for(int R = 1; R <= ROUND_BLOCK_256_256; R ++){
		subkey[R*8] = subkey[R*8+1] = subkey[R*8+2] = subkey[R*8+3] = 0;
		subkey[R*8+4] = subkey[R*8+5] = subkey[R*8+6] = subkey[R*8+7] = 0;
		for(int i = 0; i < 32; i ++){
			int pos = R*8 + i/4;
			int shift = 24 - (i%4) * 8;
			int pos_o = (R-1) * 8 + F_Box[i]/4;
			int shift_o = 24 - (F_Box[i]%4) * 8;
			subkey[pos] |= ((subkey[pos_o] >> shift_o) & 0xFF) << shift;
		}
	}
}

void KeyGen_256_512(unsigned int MK[16], unsigned int subkey[]){
	unsigned int subkey_0[22*8];
	unsigned int subkey_1[22*8];
	for(int i = 0; i < 8; i ++){
		subkey_0[i] = MK[i];
		subkey_1[i] = MK[i+8];
	}

	for(int R = 1; R <= ROUND_BLOCK_256_512; R ++){
		subkey_0[R*8] = subkey_0[R*8+1] = subkey_0[R*8+2] = subkey_0[R*8+3] = 0;
		subkey_0[R*8+4] = subkey_0[R*8+5] = subkey_0[R*8+6] = subkey_0[R*8+7] = 0;
		subkey_1[R*8] = subkey_1[R*8+1] = subkey_1[R*8+2] = subkey_1[R*8+3] = 0;
		subkey_1[R*8+4] = subkey_1[R*8+5] = subkey_1[R*8+6] = subkey_1[R*8+7] = 0;
		unsigned int k_tmp[8] = { 0 };
		for(int i = 0; i < 32; i ++){
			int pos = i/4;
			int shift = 24 - (i%4) * 8;
			k_tmp[pos] |= lF_Box_256[(subkey_1[8*(R-1) + pos] >> shift) & 0xFF] << shift;
		}	
		for(int i = 0; i < 32; i ++){
			int pos = R*8 + i/4;
			int shift = 24 - (i%4) * 8;
			int pos_o = F_Box[i]/4;
			int shift_o = 24 - (F_Box[i]%4) * 8;
			subkey_0[pos] |= ((subkey_0[8*(R-1) + pos_o] >> shift_o) & 0xFF) << shift;
			subkey_1[pos] |= ((k_tmp[pos_o] >> shift_o) & 0xFF) << shift;
		}
	}

	for(int i = 0; i < (ROUND_BLOCK_256_512+1)*8; i ++){
		subkey[i] = subkey_0[i] ^ subkey_1[i];
	}
}

void _ENC_uLBC_128_64(unsigned int X[2], unsigned int subkey[], bool S_mode = false)
{
	unsigned int ENC_ROUND_BLOCK_64_128 = S_mode ? ROUND_BLOCK_64_128s : ROUND_BLOCK_64_128;
	unsigned char X_char[16];
	unsigned char X_calced[16];
	unsigned char subkey_char[(ENC_ROUND_BLOCK_64_128+1) * 16];
	for(int i = 0; i < 16; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (ENC_ROUND_BLOCK_64_128+1) * 16; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < 16; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int round = 1; round <= ENC_ROUND_BLOCK_64_128 - 1; round++){
		//subbytes
		for(int i = 0; i < 16; i ++){
			X_char[i] = uLBC_Box[X_char[i]];
		}

		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_128_64;
		X_char[3] ^= uLBC_const_c3_128_64;
		X_char[4] ^= uLBC_const_c4_128_64;
		X_char[5] ^= uLBC_const_c5_128_64;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];

		//posPerm
		for(int i = 0; i < 16; i ++){
			X_calced[i] = X_char[Pos_Box_64[i]];
		}
		for(int i = 0; i < 16; i ++){
			X_char[i] = X_calced[i];
		}


		//mixColumn
		for(int i = 0; i < 4; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 16; i ++){
			X_char[i] = X_calced[i];
		}

		//rk
		for(int i = 0; i < 16; i ++){
			X_char[i] ^= subkey_char[round*16 + i];
		}
		#ifdef DEBUG
		cout << "第" << dec << round << "轮：" << endl;
		cout << "轮密钥: ";
		for(int i = 0; i < 16; i ++){
			cout << hex << (int)subkey_char[round*16 + i];
			if (i % 8 == 7) cout << " ";
		}cout << endl;

		cout << "密文: ";
		for(int i = 0; i < 16; i ++){
			cout << hex << (int)X_char[i];
			if (i % 8 == 7) cout << " ";
		}cout << endl;
		#endif
	}

	for(int i = 0; i < 16; i ++){
		X_char[i] = uLBC_Box[X_char[i]];
	}
	X_char[0] ^= uLBC_const_c0[ENC_ROUND_BLOCK_64_128 - 1];
	X_char[1] ^= uLBC_const_c1[ENC_ROUND_BLOCK_64_128 - 1];
	X_char[2] ^= uLBC_const_c2_128_64;
	X_char[3] ^= uLBC_const_c3_128_64;
	X_char[4] ^= uLBC_const_c4_128_64;
	X_char[5] ^= uLBC_const_c5_128_64;
	X_char[6] ^= uLBC_const_c6[ENC_ROUND_BLOCK_64_128 - 1];
	X_char[7] ^= uLBC_const_c7[ENC_ROUND_BLOCK_64_128 - 1];
	for(int i = 0; i < 16; i ++){
		X_char[i] ^= subkey_char[ENC_ROUND_BLOCK_64_128*16 + i];
	}

	#ifdef DEBUG
	cout << "第" << dec << ENC_ROUND_BLOCK_64_128 << "轮：" << endl;
	cout << "轮密钥: ";
	for(int i = 0; i < 16; i ++){
		cout << hex << (int)subkey_char[ENC_ROUND_BLOCK_64_128*16 + i];
		if (i % 8 == 7) cout << " ";
	}cout << endl;

	cout << "密文: ";
	for(int i = 0; i < 16; i ++){
		cout << hex << (int)X_char[i];
		if (i % 8 == 7) cout << " ";
	}cout << endl;
	#endif
	for(int i = 0; i < 2; i ++){ X[i] = 0; }
	for(int i = 0; i < 16; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}

void _ENC_uLBC_256_64(unsigned int X[2], unsigned int subkey[])
{
	unsigned char X_char[16];
	unsigned char X_calced[16];
	unsigned char subkey_char[(ROUND_BLOCK_64_256+1) * 16];
	for(int i = 0; i < 16; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (ROUND_BLOCK_64_256+1) * 16; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < 16; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int round = 1; round <= ROUND_BLOCK_64_256 - 1; round++){
		//subbytes
		for(int i = 0; i < 16; i ++){
			X_char[i] = uLBC_Box[X_char[i]];
		}

		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_256_64;
		X_char[3] ^= uLBC_const_c3_256_64;
		X_char[4] ^= uLBC_const_c4_256_64;
		X_char[5] ^= uLBC_const_c5_256_64;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];

		//posPerm
		for(int i = 0; i < 16; i ++){
			X_calced[i] = X_char[Pos_Box_64[i]];
		}
		for(int i = 0; i < 16; i ++){
			X_char[i] = X_calced[i];
		}

		//mixColumn
		for(int i = 0; i < 4; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 16; i ++){
			X_char[i] = X_calced[i];
		}

		//rk
		for(int i = 0; i < 16; i ++){
			X_char[i] ^= subkey_char[round*16 + i];
		}

		#ifdef DEBUG
		cout << "第" << dec << round << "轮：" << endl;
		cout << "轮密钥: ";
		for(int i = 0; i < 16; i ++){
			cout << hex << (int)subkey_char[round*16 + i];
			if (i % 8 == 7) cout << " ";
		}cout << endl;

		cout << "密文: ";
		for(int i = 0; i < 16; i ++){
			cout << hex << (int)X_char[i];
			if (i % 8 == 7) cout << " ";
		}cout << endl;
		#endif
	}

	for(int i = 0; i < 16; i ++){
		X_char[i] = uLBC_Box[X_char[i]];
	}
	X_char[0] ^= uLBC_const_c0[ROUND_BLOCK_64_256 - 1];
	X_char[1] ^= uLBC_const_c1[ROUND_BLOCK_64_256 - 1];
	X_char[2] ^= uLBC_const_c2_256_64;
	X_char[3] ^= uLBC_const_c3_256_64;
	X_char[4] ^= uLBC_const_c4_256_64;
	X_char[5] ^= uLBC_const_c5_256_64;
	X_char[6] ^= uLBC_const_c6[ROUND_BLOCK_64_256 - 1];
	X_char[7] ^= uLBC_const_c7[ROUND_BLOCK_64_256 - 1];
	for(int i = 0; i < 16; i ++){
		X_char[i] ^= subkey_char[ROUND_BLOCK_64_256*16 + i];
	}

	#ifdef DEBUG
	cout << "第" << dec << ROUND_BLOCK_64_256 << "轮：" << endl;
	cout << "轮密钥: ";
	for(int i = 0; i < 16; i ++){
		cout << hex << (int)subkey_char[ROUND_BLOCK_64_256*16 + i];
		if (i % 8 == 7) cout << " ";
	}cout << endl;

	cout << "密文: ";
	for(int i = 0; i < 16; i ++){
		cout << hex << (int)X_char[i];
		if (i % 8 == 7) cout << " ";
	}cout << endl;
	#endif
	for(int i = 0; i < 2; i ++){ X[i] = 0; }
	for(int i = 0; i < 16; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}

void ENC_uLBC_256_MDS(unsigned char state[32])
{
    unsigned char temp[32];
    memcpy(temp, state, 32);
    for(int i = 0; i < 8; i ++){
        state[4*i+0] = temp[4*i+0] ^ temp[4*i+1] ^ MDS_256_2[temp[4*i+2]] ^ MDS_256_0x91[temp[4*i+3]];
        state[4*i+1] = temp[4*i+0] ^ MDS_256_2[temp[4*i+1]] ^ MDS_256_0x91[temp[4*i+2]] ^ temp[4*i+3];
        state[4*i+2] = MDS_256_2[temp[4*i+0]] ^ MDS_256_0x91[temp[4*i+1]] ^ temp[4*i+2] ^ temp[4*i+3];
        state[4*i+3] = MDS_256_0x91[temp[4*i+0]] ^ temp[4*i+1] ^ temp[4*i+2] ^ MDS_256_2[temp[4*i+3]];
    }
}

void DEC_uLBC_256_MDS(unsigned char state[32])
{
    unsigned char temp[32];
    memcpy(temp, state, 32);
    for(int i = 0; i < 8; i ++){
        state[4*i+0] = MDS_256_0x55[temp[4*i+0]] ^ MDS_256_0x41[temp[4*i+1]] ^ MDS_256_0x71[temp[4*i+2]] ^ MDS_256_0x5a[temp[4*i+3]];
        state[4*i+1] = MDS_256_0x41[temp[4*i+0]] ^ MDS_256_0x71[temp[4*i+1]] ^ MDS_256_0x5a[temp[4*i+2]] ^ MDS_256_0x55[temp[4*i+3]];
        state[4*i+2] = MDS_256_0x71[temp[4*i+0]] ^ MDS_256_0x5a[temp[4*i+1]] ^ MDS_256_0x55[temp[4*i+2]] ^ MDS_256_0x41[temp[4*i+3]];
        state[4*i+3] = MDS_256_0x5a[temp[4*i+0]] ^ MDS_256_0x55[temp[4*i+1]] ^ MDS_256_0x41[temp[4*i+2]] ^ MDS_256_0x71[temp[4*i+3]];
    }
}

void _ENC_uLBC_256_256(unsigned int X[8], unsigned int subkey[])
{
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(ROUND_BLOCK_256_256+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/4] >> (24 - (i%4) * 8)) & 0XFF;
	}
	for(int i = 0; i < (ROUND_BLOCK_256_256+1) * 32; i++){
		subkey_char[i] = (subkey[i/4] >> (24 - (i%4) * 8)) & 0XFF;
	}
	for(int i = 0; i < 32; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int round = 1; round <= ROUND_BLOCK_256_256 - 1; round++){
		//subbytes
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box_256[X_char[i]];
		}

		//addConst
		X_char[0] ^= uLBC_const_256_c0[round - 1];
		X_char[1] ^= uLBC_const_c1_256_256;
		X_char[2] ^= uLBC_const_c2_256_256;
		X_char[3] ^= uLBC_const_256_c3[round - 1];
		X_char[8] ^= uLBC_const_c1_256_256;
		X_char[9] ^= uLBC_const_256_c3[round - 1];
		X_char[10] ^= uLBC_const_256_c0[round - 1];
		X_char[11] ^= uLBC_const_c2_256_256;

		//posPerm
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}

		//mixColumn
		ENC_uLBC_256_MDS(X_char);

		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}
		#ifdef DEBUG
		cout << "第" << dec << round << "轮：" << endl;
		cout << "轮密钥: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << setw(2) << setfill('0') << (int)subkey_char[round*32 + i];
			if (i % 4 == 3) cout << " ";
		}cout << endl;

		cout << "密文: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << setw(2) << setfill('0') << (int)X_char[i];
			if (i % 4 == 3) cout << " ";
		}cout << endl;
		#endif
	
	}

	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box_256[X_char[i]];
	}
	X_char[0] ^= uLBC_const_256_c0[ROUND_BLOCK_256_256 - 1];
	X_char[1] ^= uLBC_const_c1_256_256;
	X_char[2] ^= uLBC_const_c2_256_256;
	X_char[3] ^= uLBC_const_256_c3[ROUND_BLOCK_256_256 - 1];
	X_char[8] ^= uLBC_const_c1_256_256;
	X_char[9] ^= uLBC_const_256_c3[ROUND_BLOCK_256_256 - 1];
	X_char[10] ^= uLBC_const_256_c0[ROUND_BLOCK_256_256 - 1];
	X_char[11] ^= uLBC_const_c2_256_256;
	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[ROUND_BLOCK_256_256*32 + i];
	}
	#ifdef DEBUG
	cout << "第" << dec << ROUND_BLOCK_256_256 << "轮：" << endl;
	cout << "轮密钥: ";
	for(int i = 0; i < 32; i ++){
		cout << hex << setw(2) << setfill('0') << (int)subkey_char[ROUND_BLOCK_256_256*32 + i];
		if (i % 4 == 3) cout << " ";
	}cout << endl;

	cout << "密文: ";
	for(int i = 0; i < 32; i ++){
		cout << hex << setw(2) << setfill('0') << (int)X_char[i];
		if (i % 4 == 3) cout << " ";
	}cout << endl;
	#endif

	for(int i = 0; i < 8; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/4] |= ((unsigned int)(X_char[i] & 0XFF) << (24 - (i%4) * 8)) ;
	}
}

void _ENC_uLBC_512_256(unsigned int X[8], unsigned int subkey[])
{
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(ROUND_BLOCK_256_512+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/4] >> (24 - (i%4) * 8)) & 0XFF;
	}
	for(int i = 0; i < (ROUND_BLOCK_256_512+1) * 32; i++){
		subkey_char[i] = (subkey[i/4] >> (24 - (i%4) * 8)) & 0XFF;
	}
	for(int i = 0; i < 32; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int round = 1; round <= ROUND_BLOCK_256_512 - 1; round++){
		//subbytes
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box_256[X_char[i]];
		}

		//addConst
		X_char[0] ^= uLBC_const_256_c0[round - 1];
		X_char[1] ^= uLBC_const_c1_256_512;
		X_char[2] ^= uLBC_const_c2_256_512;
		X_char[3] ^= uLBC_const_256_c3[round - 1];
		X_char[8] ^= uLBC_const_c1_256_512;
		X_char[9] ^= uLBC_const_256_c3[round - 1];
		X_char[10] ^= uLBC_const_256_c0[round - 1];
		X_char[11] ^= uLBC_const_c2_256_512;

		//posPerm
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}


		//mixColumn
		ENC_uLBC_256_MDS(X_char);

		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}

		#ifdef DEBUG
		cout << "第" << dec << round << "轮：" << endl;
		cout << "轮密钥: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << setw(2) << setfill('0') << (int)subkey_char[round*32 + i];
			if (i % 4 == 3) cout << " ";
		}cout << endl;

		cout << "密文: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << setw(2) << setfill('0') << (int)X_char[i];
			if (i % 4 == 3) cout << " ";
		}cout << endl;
		#endif
	}

	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box_256[X_char[i]];
	}
	X_char[0] ^= uLBC_const_256_c0[ROUND_BLOCK_256_512 - 1];
	X_char[1] ^= uLBC_const_c1_256_512;
	X_char[2] ^= uLBC_const_c2_256_512;
	X_char[3] ^= uLBC_const_256_c3[ROUND_BLOCK_256_512 - 1];
	X_char[8] ^= uLBC_const_c1_256_512;
	X_char[9] ^= uLBC_const_256_c3[ROUND_BLOCK_256_512 - 1];
	X_char[10] ^= uLBC_const_256_c0[ROUND_BLOCK_256_512 - 1];
	X_char[11] ^= uLBC_const_c2_256_512;
	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[ROUND_BLOCK_256_512*32 + i];
	}
	#ifdef DEBUG
	cout << "第" << dec << ROUND_BLOCK_256_512 << "轮：" << endl;
	cout << "轮密钥: ";
	for(int i = 0; i < 32; i ++){
		cout << hex << setw(2) << setfill('0') << (int)subkey_char[ROUND_BLOCK_256_512*32 + i];
		if (i % 4 == 3) cout << " ";
	}cout << endl;

	cout << "密文: ";
	for(int i = 0; i < 32; i ++){
		cout << hex << setw(2) << setfill('0') << (int)X_char[i];
		if (i % 4 == 3) cout << " ";
	}cout << endl;
	#endif
	for(int i = 0; i < 8; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/4] |= ((unsigned int)(X_char[i] & 0XFF) << (24 - (i%4) * 8)) ;
	}
}
//
void _ENC_uLBC_128(unsigned int X[4], unsigned int subkey[], bool S_mode = false)
{
	unsigned int ENC_ROUND_128 = S_mode ? ROUND_128s : ROUND_128;
	// 切分成半字节方便计算
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(ENC_ROUND_128+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (ENC_ROUND_128+1) * 32; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}

	for(int i = 0; i < 32; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int round = 1; round <= ENC_ROUND_128 - 1; round++){

		//subbytes
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box[X_char[i]];
		}
			
		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_128;
		X_char[3] ^= uLBC_const_c3_128;
		X_char[4] ^= S_mode ? uLBC_const_c4_128s : uLBC_const_c4_128;
		X_char[5] ^= uLBC_const_c5_128;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];

		//posPerm
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}

		
		
		//mixColumn
		for(int i = 0; i < 8; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}


		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}
		
		#ifdef DEBUG
		cout << "第" << dec << round << "轮：" << endl;
		cout << "轮密钥: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << (int)subkey_char[round*32 + i];
			if (i % 8 == 7) cout << " ";
		}cout << endl;
		cout << "密文: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << (int)X_char[i];
			if (i % 8 == 7) cout << " ";
		}cout << endl;
		#endif
	}

	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box[X_char[i]];
	}
	X_char[0] ^= uLBC_const_c0[ENC_ROUND_128 - 1];
	X_char[1] ^= uLBC_const_c1[ENC_ROUND_128 - 1];
	X_char[2] ^= uLBC_const_c2_128;
	X_char[3] ^= uLBC_const_c3_128;
	X_char[4] ^= S_mode ? uLBC_const_c4_128s : uLBC_const_c4_128;
	X_char[5] ^= uLBC_const_c5_128;
	X_char[6] ^= uLBC_const_c6[ENC_ROUND_128 - 1];
	X_char[7] ^= uLBC_const_c7[ENC_ROUND_128 - 1];
	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[ENC_ROUND_128*32 + i];
	}
	#ifdef DEBUG
	cout << "第" << dec << ENC_ROUND_128 << "轮：" << endl;
	cout << "轮密钥: ";
	for(int i = 0; i < 32; i ++){
		cout << hex << (int)subkey_char[(ENC_ROUND_128)*32 + i];
		if (i % 8 == 7) cout << " ";
	}cout << endl;
	cout << "密文: ";
	for(int i = 0; i < 32; i ++){
		cout << hex << (int)X_char[i];
		if (i % 8 == 7) cout << " ";
	}cout << endl;
	#endif

	for(int i = 0; i < 4; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}

//
void _ENC_uLBC_256(unsigned int X[4], unsigned int subkey[], bool S_mode = false)
{
	unsigned int ENC_ROUND_256 = S_mode ? ROUND_256s : ROUND_256;
	// 切分成半字节方便计算
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(ENC_ROUND_256+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (ENC_ROUND_256+1) * 32; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}

	for(int i = 0; i < 32; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int round = 1; round <= ENC_ROUND_256 - 1; round++){
		//subbytes
		#ifdef DEBUG
		cout << "第" << dec << round << "轮：" << endl;
		#endif
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box[X_char[i]];
		}

		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_256;
		X_char[3] ^= uLBC_const_c3_256;
		X_char[4] ^= uLBC_const_c4_256;
		X_char[5] ^= S_mode ? uLBC_const_c5_256s : uLBC_const_c5_256;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];

		//posPerm
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}

		//mixColumn
		for(int i = 0; i < 8; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}

		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}

		#ifdef DEBUG
		cout << "轮密钥: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << (int)subkey_char[round*32 + i];
			if (i % 8 == 7) cout << " ";
		}cout << endl;

		cout << "密文: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << (int)X_char[i];
			if (i % 8 == 7) cout << " ";
		}cout << endl;
		#endif
	}
	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box[X_char[i]];
	}
	X_char[0] ^= uLBC_const_c0[ENC_ROUND_256 - 1];
	X_char[1] ^= uLBC_const_c1[ENC_ROUND_256 - 1];
	X_char[2] ^= uLBC_const_c2_256;
	X_char[3] ^= uLBC_const_c3_256;
	X_char[4] ^= uLBC_const_c4_256;
	X_char[5] ^= S_mode ? uLBC_const_c5_256s : uLBC_const_c5_256;
	X_char[6] ^= uLBC_const_c6[ENC_ROUND_256 - 1];
	X_char[7] ^= uLBC_const_c7[ENC_ROUND_256 - 1];
	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[ENC_ROUND_256*32 + i];
	}
	#ifdef DEBUG
	cout << "第" << dec << ENC_ROUND_256 << "轮：" << endl;
	cout << "轮密钥: ";
	for(int i = 0; i < 32; i ++){
		cout << hex << (int)subkey_char[(ENC_ROUND_256)*32 + i];
		if (i % 8 == 7) cout << " ";
	}cout << endl;
	cout << "密文: ";
	for(int i = 0; i < 32; i ++){
		cout << hex << (int)X_char[i];
		if (i % 8 == 7) cout << " ";
	}cout << endl;
	#endif
	for(int i = 0; i < 4; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}


void _ENC_uLBC_384(unsigned int X[4], unsigned int subkey[]){
	// 切分成半字节方便计算
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(ROUND_384+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (ROUND_384+1) * 32; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}

	for(int i = 0; i < 32; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int round = 1; round <= ROUND_384 - 1; round++){
		//subbytes
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box[X_char[i]];
		}

		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_384;
		X_char[3] ^= uLBC_const_c3_384;
		X_char[4] ^= uLBC_const_c4_384;
		X_char[5] ^= uLBC_const_c5_384;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];

		//posPerm
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}

		//mixColumn
		for(int i = 0; i < 8; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}

		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}

		#ifdef DEBUG
		cout << "轮密钥: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << (int)subkey_char[round*32 + i];
		}cout << endl;
		cout << "密文: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << (int)X_char[i];
		}cout << endl;
		#endif
	}
	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box[X_char[i]];
	}
	// X_char[0] ^= uLBC_const_c0[ROUND_384 - 1];
	// X_char[1] ^= uLBC_const_c1[ROUND_384 - 1];
	// X_char[2] ^= uLBC_const_c2_384;
	// X_char[3] ^= uLBC_const_c3_384;
	// X_char[4] ^= uLBC_const_c4_384;
	// X_char[5] ^= uLBC_const_c5_384;
	// X_char[6] ^= uLBC_const_c6[ROUND_384 - 1];
	// X_char[7] ^= uLBC_const_c7[ROUND_384 - 1];
	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[ROUND_384*32 + i];
	}

	for(int i = 0; i < 4; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}

void _DEC_uLBC_128_64(unsigned int X[2], unsigned int subkey[], bool S_mode = false)
{
	unsigned int DEC_ROUND_BLOCK_64_128 = S_mode ? ROUND_BLOCK_64_128s : ROUND_BLOCK_64_128;
	unsigned char X_char[16];
	unsigned char X_calced[16];
	unsigned char subkey_char[(DEC_ROUND_BLOCK_64_128+1) * 16];
	for(int i = 0; i < 16; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (DEC_ROUND_BLOCK_64_128+1) * 16; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < 16; i ++){
		X_char[i] ^= subkey_char[DEC_ROUND_BLOCK_64_128*16 + i];
	}
	X_char[0] ^= uLBC_const_c0[DEC_ROUND_BLOCK_64_128 - 1];
	X_char[1] ^= uLBC_const_c1[DEC_ROUND_BLOCK_64_128 - 1];
	X_char[2] ^= uLBC_const_c2_128_64;
	X_char[3] ^= uLBC_const_c3_128_64;
	X_char[4] ^= uLBC_const_c4_128_64;
	X_char[5] ^= uLBC_const_c5_128_64;
	X_char[6] ^= uLBC_const_c6[DEC_ROUND_BLOCK_64_128 - 1];
	X_char[7] ^= uLBC_const_c7[DEC_ROUND_BLOCK_64_128 - 1];
	for(int i = 0; i < 16; i ++){
		X_char[i] = uLBC_Box_Inv[X_char[i]];
	}
	for(int round = DEC_ROUND_BLOCK_64_128 - 1; round > 0; round --){
		//rk
		for(int i = 0; i < 16; i ++){
			X_char[i] ^= subkey_char[round*16 + i];
		}

		//mixColumn
		for(int i = 0; i < 4; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 16; i ++){
			X_char[i] = X_calced[i];
		}

		//posPerm-1
		for(int i = 0; i < 16; i ++){
			X_calced[i] = X_char[Pos_Box_Inv_64[i]];
		}
		for(int i = 0; i < 16; i ++){
			X_char[i] = X_calced[i];
		}

		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_128_64;
		X_char[3] ^= uLBC_const_c3_128_64;
		X_char[4] ^= uLBC_const_c4_128_64;
		X_char[5] ^= uLBC_const_c5_128_64;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];

		for(int i = 0; i < 16; i ++){
			X_char[i] = uLBC_Box_Inv[X_char[i]];
		}
		
	}

	for(int i = 0; i < 16; i ++)
		X_char[i] ^= subkey_char[i];

	for(int i = 0; i < 2; i ++){ X[i] = 0; }
	for(int i = 0; i < 16; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}

void _DEC_uLBC_256_64(unsigned int X[2], unsigned int subkey[], bool S_mode = false)
{
	unsigned char X_char[16];
	unsigned char X_calced[16];
	unsigned char subkey_char[(ROUND_BLOCK_64_256+1) * 16];
	for(int i = 0; i < 16; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (ROUND_BLOCK_64_256+1) * 16; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < 16; i ++){
		X_char[i] ^= subkey_char[ROUND_BLOCK_64_256*16 + i];
	}
	X_char[0] ^= uLBC_const_c0[ROUND_BLOCK_64_256 - 1];
	X_char[1] ^= uLBC_const_c1[ROUND_BLOCK_64_256 - 1];
	X_char[2] ^= uLBC_const_c2_256_64;
	X_char[3] ^= uLBC_const_c3_256_64;
	X_char[4] ^= uLBC_const_c4_256_64;
	X_char[5] ^= uLBC_const_c5_256_64;
	X_char[6] ^= uLBC_const_c6[ROUND_BLOCK_64_256 - 1];
	X_char[7] ^= uLBC_const_c7[ROUND_BLOCK_64_256 - 1];
	for(int i = 0; i < 16; i ++){
		X_char[i] = uLBC_Box_Inv[X_char[i]];
	}
	for(int round = ROUND_BLOCK_64_256 - 1; round > 0; round --){
		//rk
		for(int i = 0; i < 16; i ++){
			X_char[i] ^= subkey_char[round*16 + i];
		}

		//mixColumn
		for(int i = 0; i < 4; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 16; i ++){
			X_char[i] = X_calced[i];
		}

		//posPerm-1
		for(int i = 0; i < 16; i ++){
			X_calced[i] = X_char[Pos_Box_Inv_64[i]];
		}
		for(int i = 0; i < 16; i ++){
			X_char[i] = X_calced[i];
		}

		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_256_64;
		X_char[3] ^= uLBC_const_c3_256_64;
		X_char[4] ^= uLBC_const_c4_256_64;
		X_char[5] ^= uLBC_const_c5_256_64;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];

		for(int i = 0; i < 16; i ++){
			X_char[i] = uLBC_Box_Inv[X_char[i]];
		}
		
	}

	for(int i = 0; i < 16; i ++)
		X_char[i] ^= subkey_char[i];

	for(int i = 0; i < 2; i ++){ X[i] = 0; }
	for(int i = 0; i < 16; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}

void _DEC_uLBC_256_256(unsigned int X[8], unsigned int subkey[])
{
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(ROUND_BLOCK_256_256+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/4] >> (24 - (i%4) * 8)) & 0XFF;
	}
	for(int i = 0; i < (ROUND_BLOCK_256_256+1) * 32; i++){
		subkey_char[i] = (subkey[i/4] >> (24 - (i%4) * 8)) & 0XFF;
	}
	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[ROUND_BLOCK_256_256*32 + i];
	}	
	X_char[0] ^= uLBC_const_256_c0[ROUND_BLOCK_256_256 - 1];
	X_char[1] ^= uLBC_const_c1_256_256;
	X_char[2] ^= uLBC_const_c2_256_256;
	X_char[3] ^= uLBC_const_256_c3[ROUND_BLOCK_256_256 - 1];
	X_char[8] ^= uLBC_const_c1_256_256;
	X_char[9] ^= uLBC_const_256_c3[ROUND_BLOCK_256_256 - 1];
	X_char[10] ^= uLBC_const_256_c0[ROUND_BLOCK_256_256 - 1];
	X_char[11] ^= uLBC_const_c2_256_256;
	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box_256_Inv[X_char[i]];
	}
	
	for(int round = ROUND_BLOCK_256_256 - 1; round >= 1; round--){
		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}
		//mixColumn
		DEC_uLBC_256_MDS(X_char);

		//posPerm
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box_Inv[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}

		//addConst
		X_char[0] ^= uLBC_const_256_c0[round - 1];
		X_char[1] ^= uLBC_const_c1_256_256;
		X_char[2] ^= uLBC_const_c2_256_256;
		X_char[3] ^= uLBC_const_256_c3[round - 1];
		X_char[8] ^= uLBC_const_c1_256_256;
		X_char[9] ^= uLBC_const_256_c3[round - 1];
		X_char[10] ^= uLBC_const_256_c0[round - 1];
		X_char[11] ^= uLBC_const_c2_256_256;
		
		//subbytes
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box_256_Inv[X_char[i]];
		}
	
	}

	for(int i = 0; i < 32; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int i = 0; i < 8; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/4] |= ((unsigned int)(X_char[i] & 0XFF) << (24 - (i%4) * 8)) ;
	}
}

void _DEC_uLBC_512_256(unsigned int X[8], unsigned int subkey[])
{
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(ROUND_BLOCK_256_512+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/4] >> (24 - (i%4) * 8)) & 0XFF;
	}
	for(int i = 0; i < (ROUND_BLOCK_256_512+1) * 32; i++){
		subkey_char[i] = (subkey[i/4] >> (24 - (i%4) * 8)) & 0XFF;
	}
	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[ROUND_BLOCK_256_512*32 + i];
	}	
	X_char[0] ^= uLBC_const_256_c0[ROUND_BLOCK_256_512 - 1];
	X_char[1] ^= uLBC_const_c1_256_512;
	X_char[2] ^= uLBC_const_c2_256_512;
	X_char[3] ^= uLBC_const_256_c3[ROUND_BLOCK_256_512 - 1];
	X_char[8] ^= uLBC_const_c1_256_512;
	X_char[9] ^= uLBC_const_256_c3[ROUND_BLOCK_256_512 - 1];
	X_char[10] ^= uLBC_const_256_c0[ROUND_BLOCK_256_512 - 1];
	X_char[11] ^= uLBC_const_c2_256_512;
	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box_256_Inv[X_char[i]];
	}
	

	for(int round = ROUND_BLOCK_256_512 - 1; round >= 1; round--){
		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}
		//mixColumn
		DEC_uLBC_256_MDS(X_char);

		//posPerm
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box_Inv[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}

		//addConst
		X_char[0] ^= uLBC_const_256_c0[round - 1];
		X_char[1] ^= uLBC_const_c1_256_512;
		X_char[2] ^= uLBC_const_c2_256_512;
		X_char[3] ^= uLBC_const_256_c3[round - 1];
		X_char[8] ^= uLBC_const_c1_256_512;
		X_char[9] ^= uLBC_const_256_c3[round - 1];
		X_char[10] ^= uLBC_const_256_c0[round - 1];
		X_char[11] ^= uLBC_const_c2_256_512;
		
		//subbytes
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box_256_Inv[X_char[i]];
		}
	
	}

	for(int i = 0; i < 32; i++){
		X_char[i] ^= subkey_char[i];
	}

	for(int i = 0; i < 8; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/4] |= ((unsigned int)(X_char[i] & 0XFF) << (24 - (i%4) * 8)) ;
	}
}

//
void _DEC_uLBC_128(unsigned int X[4], unsigned int subkey[], bool S_mode = false){
	unsigned int DEC_ROUND_128 = S_mode ? ROUND_128s : ROUND_128;
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(DEC_ROUND_128+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (DEC_ROUND_128+1) * 32; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}

	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[DEC_ROUND_128*32 + i];
	}
	X_char[0] ^= uLBC_const_c0[DEC_ROUND_128 - 1];
	X_char[1] ^= uLBC_const_c1[DEC_ROUND_128 - 1];
	X_char[2] ^= uLBC_const_c2_128;
	X_char[3] ^= uLBC_const_c3_128;
	X_char[4] ^= S_mode ? uLBC_const_c4_128s : uLBC_const_c4_128;
	X_char[5] ^= uLBC_const_c5_128;
	X_char[6] ^= uLBC_const_c6[DEC_ROUND_128 - 1];
	X_char[7] ^= uLBC_const_c7[DEC_ROUND_128 - 1];
	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box_Inv[X_char[i]];
	}

	for(int round = DEC_ROUND_128 - 1; round > 0; round --){
		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}
		//mixColumn
		for(int i = 0; i < 8; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}
		//posPerm-1
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box_Inv[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}
		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_128;
		X_char[3] ^= uLBC_const_c3_128;
		X_char[4] ^= S_mode ? uLBC_const_c4_128s : uLBC_const_c4_128;
		X_char[5] ^= uLBC_const_c5_128;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];
		//subbytes-1
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box_Inv[X_char[i]];
		}
		#ifdef DEBUG
		cout << "第" << dec << round << "轮：" << endl;
		cout << "轮密钥: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << (int)subkey_char[round*32 + i];
		}cout << endl;
		cout << "明文: ";
		for(int i = 0; i < 32; i ++){
			cout << hex << (int)X_char[i];
		}cout << endl;
		#endif
	}

	for(int i = 0; i < 32; i ++)
		X_char[i] ^= subkey_char[i];

	for(int i = 0; i < 4; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}

//
void _DEC_uLBC_256(unsigned int X[4], unsigned int subkey[], bool S_mode = false){
	unsigned int DEC_ROUND_256 = S_mode ? ROUND_256s : ROUND_256;
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(DEC_ROUND_256+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (DEC_ROUND_256+1) * 32; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}

	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[DEC_ROUND_256*32 + i];
	}
	X_char[0] ^= uLBC_const_c0[DEC_ROUND_256 - 1];
	X_char[1] ^= uLBC_const_c1[DEC_ROUND_256 - 1];
	X_char[2] ^= uLBC_const_c2_256;
	X_char[3] ^= uLBC_const_c3_256;
	X_char[4] ^= uLBC_const_c4_256;
	X_char[5] ^= S_mode ? uLBC_const_c5_256s : uLBC_const_c5_256;
	X_char[6] ^= uLBC_const_c6[DEC_ROUND_256 - 1];
	X_char[7] ^= uLBC_const_c7[DEC_ROUND_256 - 1];
	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box_Inv[X_char[i]];
	}


	for(int round = DEC_ROUND_256-1; round > 0; round --){
		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}
		//mixColumn
		for(int i = 0; i < 8; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}
		//posPerm-1
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box_Inv[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}
		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_256;
		X_char[3] ^= uLBC_const_c3_256;
		X_char[4] ^= uLBC_const_c4_256;
		X_char[5] ^= S_mode ? uLBC_const_c5_256s : uLBC_const_c5_256;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];
		//subbytes
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box_Inv[X_char[i]];
		}
	}

	for(int i = 0; i < 32; i ++)
		X_char[i] ^= subkey_char[i];

	for(int i = 0; i < 4; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}

//
void _DEC_uLBC_384(unsigned int X[4], unsigned int subkey[]){
	unsigned char X_char[32];
	unsigned char X_calced[32];
	unsigned char subkey_char[(ROUND_384+1) * 32];
	for(int i = 0; i < 32; i++){
		X_char[i] = (X[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}
	for(int i = 0; i < (ROUND_384+1) * 32; i++){
		subkey_char[i] = (subkey[i/8] >> (28 - (i%8) * 4)) & 0XF;
	}

	for(int i = 0; i < 32; i ++){
		X_char[i] ^= subkey_char[ROUND_384*32 + i];
	}
	for(int i = 0; i < 32; i ++){
		X_char[i] = uLBC_Box_Inv[X_char[i]];
	}


	for(int round = ROUND_384-1; round > 0; round --){
		//rk
		for(int i = 0; i < 32; i ++){
			X_char[i] ^= subkey_char[round*32 + i];
		}
		//mixColumn
		for(int i = 0; i < 8; i ++){
			X_calced[i*4] = X_char[i*4+1] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+1] = X_char[i*4] ^ X_char[i*4+2] ^ X_char[i*4+3];
			X_calced[i*4+2] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+3];
			X_calced[i*4+3] = X_char[i*4] ^ X_char[i*4+1] ^ X_char[i*4+2];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}
		//posPerm-1
		for(int i = 0; i < 32; i ++){
			X_calced[i] = X_char[Pos_Box_Inv[i]];
		}
		for(int i = 0; i < 32; i ++){
			X_char[i] = X_calced[i];
		}
		//addConst
		X_char[0] ^= uLBC_const_c0[round - 1];
		X_char[1] ^= uLBC_const_c1[round - 1];
		X_char[2] ^= uLBC_const_c2_384;
		X_char[3] ^= uLBC_const_c3_384;
		X_char[4] ^= uLBC_const_c4_384;
		X_char[5] ^= uLBC_const_c5_384;
		X_char[6] ^= uLBC_const_c6[round - 1];
		X_char[7] ^= uLBC_const_c7[round - 1];
		//subbytes
		for(int i = 0; i < 32; i ++){
			X_char[i] = uLBC_Box_Inv[X_char[i]];
		}
	}

	for(int i = 0; i < 32; i ++)
		X_char[i] ^= subkey_char[i];

	for(int i = 0; i < 4; i ++){ X[i] = 0; }
	for(int i = 0; i < 32; i ++){
		X[i/8] |= ((unsigned int)(X_char[i] & 0XF) << (28 - (i%8) * 4)) ;
	}
}


/*
blocksize==64: 0(128s)/1(128)/2(256)加密 3(128s)/4(128)/5(256)解密
blocksize==128: 0(128s)/1(128)/2(256s)/3(256)/4(384)加密 5(128s)/6(128)/7(256s)/8(256)/9(384)解密
blocksize==256: 0(256)/1(512)加密 2(256)/3(512)解密
*/
int uLBC_Block(int mode, unsigned int X[], unsigned int MK[], int block_size = 128)
{
	if (block_size == 256) {
		if (mode == 0){
			unsigned int subkey[300] = {0};
			KeyGen_256_256(MK, subkey);
			_ENC_uLBC_256_256(X, subkey);
		}
		else if (mode == 1){
			unsigned int subkey[300] = {0};
			KeyGen_256_512(MK, subkey);
			_ENC_uLBC_512_256(X, subkey);
		}
		else if (mode == 2){
			unsigned int subkey[300] = {0};
			KeyGen_256_256(MK, subkey);
			_DEC_uLBC_256_256(X, subkey);
		}
		else if (mode == 3){
			unsigned int subkey[300] = {0};
			KeyGen_256_512(MK, subkey);
			_DEC_uLBC_512_256(X, subkey);
		}
		return 1;
	}
	unsigned int subkey[31*4] = { 0 };

	if(mode == 0){
		KeyGen_128(MK, subkey);
		_ENC_uLBC_128(X, subkey, true);
	}
	else if(mode == 1){
		KeyGen_128(MK, subkey);
		_ENC_uLBC_128(X, subkey);
	}
	else if(mode == 2){
		KeyGen_256(MK, subkey);
		_ENC_uLBC_256(X, subkey, true);
	}
	else if(mode == 3){
		KeyGen_256(MK, subkey);
		_ENC_uLBC_256(X, subkey);
	}
	else if(mode == 4){
		KeyGen_384(MK, subkey);
		_ENC_uLBC_384(X, subkey);
	}
	else if(mode == 5){
		KeyGen_128(MK, subkey);
		_DEC_uLBC_128(X, subkey, true);
	}
	else if(mode == 6){
		KeyGen_128(MK, subkey);
		_DEC_uLBC_128(X, subkey);
	}
	else if(mode == 7){
		KeyGen_256(MK, subkey);
		_DEC_uLBC_256(X, subkey, true);
	}
	else if(mode == 8){
		KeyGen_256(MK, subkey);
		_DEC_uLBC_256(X, subkey);
	}
	else if(mode == 9){
		KeyGen_384(MK, subkey);
		_DEC_uLBC_384(X, subkey);
	}
	return 1;
}

//1)	加密函数接口（ECB模式）最后的分组不考虑,0失败，1成功
int Crypt_Enc_Block(unsigned char *input, int in_len, unsigned char  *output, int *out_len, unsigned char *key, int keylen)
{
	unsigned int subkey[33*4] = { 0 };
	unsigned int X[4] = { 0 };
	if (in_len < 16)
	{
		printf("The length of data less than 16, exit");
		return 0;
	}
	if (keylen != 16 && keylen != 32)
	{
		printf("The length of key is 16 or 32, exit");
		return 0;
	}
	int blocknum = in_len >> 4;
	int i, j;
	if(keylen == 16){
		unsigned int MK[4] = { 0 };
		MK[0] = (key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3];
		MK[1] = (key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7];
		MK[2] = (key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11];
		MK[3] = (key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15];
		// KeyGen_128_fast(MK, subkey);
		KeyGen_128(MK, subkey);
	}
	else{
		unsigned int MK[8] = { 0 };
		MK[0] = (key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3];
		MK[1] = (key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7];
		MK[2] = (key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11];
		MK[3] = (key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15];
		MK[4] = (key[16] << 24) | (key[17] << 16) | (key[18] << 8) | key[19];
		MK[5] = (key[20] << 24) | (key[21] << 16) | (key[22] << 8) | key[23];
		MK[6] = (key[24] << 24) | (key[25] << 16) | (key[26] << 8) | key[27];
		MK[7] = (key[28] << 24) | (key[29] << 16) | (key[30] << 8) | key[31];
		// KeyGen_256_fast(MK, subkey);
		KeyGen_256(MK, subkey);
	}
	for (i = 0; i < blocknum; i++)
	{
		j = i << 4;
		X[0] = (input[j] << 24) | (input[j + 1] << 16) | (input[j + 2] << 8) | input[j + 3];
		j += 4;
		X[1] = (input[j] << 24) | (input[j + 1] << 16) | (input[j + 2] << 8) | input[j + 3];
		j += 4;
		X[2] = (input[j] << 24) | (input[j + 1] << 16) | (input[j + 2] << 8) | input[j + 3];
		j += 4;
		X[3] = (input[j] << 24) | (input[j + 1] << 16) | (input[j + 2] << 8) | input[j + 3];

		if(keylen == 16)
			// ENC_uLBC_128_fast(X, subkey);
			_ENC_uLBC_128(X, subkey);
		else
			// ENC_uLBC_256_fast(X, subkey);
			_ENC_uLBC_256(X, subkey);

		j = i << 4;
		output[j] = (X[0] >> 24) & 0xff;
		output[j + 1] = (X[0] >> 16) & 0xff;
		output[j + 2] = (X[0] >> 8) & 0xff;
		output[j + 3] = X[0] & 0xff;
		j += 4;
		output[j] = (X[1] >> 24) & 0xff;
		output[j + 1] = (X[1] >> 16) & 0xff;
		output[j + 2] = (X[1] >> 8) & 0xff;
		output[j + 3] = X[1] & 0xff;
		j += 4;
		output[j] = (X[2] >> 24) & 0xff;
		output[j + 1] = (X[2] >> 16) & 0xff;
		output[j + 2] = (X[2] >> 8) & 0xff;
		output[j + 3] = X[2] & 0xff;
		j += 4;
		output[j] = (X[3] >> 24) & 0xff;
		output[j + 1] = (X[3] >> 16) & 0xff;
		output[j + 2] = (X[3] >> 8) & 0xff;
		output[j + 3] = X[3] & 0xff;
	}
	*out_len = blocknum << 4;
	return 1;
}

//
//基于计数器模式的加解密函数接口，数据长度小于2^{20}字节（ECB模式）
/*
iv建议由随机数64比特和时间戳48比特构成
*/
int Crypt_CTR_IV14(unsigned char *iv, int iv_len, unsigned char *input, int in_len, unsigned char *output, int *out_len, unsigned char *key, int keylen, int mode)
{
	unsigned int subkey[33*4] = { 0 };
	unsigned int X[4] = { 0 };
	if (iv_len != 14)
	{
		printf("The length of iv should be 14 bytes, exit");
		return 0;
	}
	if (in_len > (1<<20))
	{
		printf("The length of data less than 1M bytes, exit");
		return 0;
	}
	
	int blocknum = in_len >> 4;
	int i, j;
	
	if(keylen == 16){
		unsigned int MK[4] = { 0 };
		MK[0] = (key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3];
		MK[1] = (key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7];
		MK[2] = (key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11];
		MK[3] = (key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15];
		if(mode == 0)
			// KeyGen_128_fast(MK, subkey);
			KeyGen_128(MK, subkey);
		else
			KeyGen_128(MK, subkey);
	}
	else{
		unsigned int MK[8] = { 0 };
		MK[0] = (key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3];
		MK[1] = (key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7];
		MK[2] = (key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11];
		MK[3] = (key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15];
		MK[4] = (key[16] << 24) | (key[17] << 16) | (key[18] << 8) | key[19];
		MK[5] = (key[20] << 24) | (key[21] << 16) | (key[22] << 8) | key[23];
		MK[6] = (key[24] << 24) | (key[25] << 16) | (key[26] << 8) | key[27];
		MK[7] = (key[28] << 24) | (key[29] << 16) | (key[30] << 8) | key[31];
		if(mode == 0)
			// KeyGen_256_fast(MK, subkey);
			KeyGen_256(MK, subkey);
		else
			KeyGen_256(MK, subkey);
	}

	for (i = 0; i < blocknum; i++)
	{
		X[0] = (iv[0] << 24) | (iv[1] << 16) | (iv[2] << 8) | iv[3];
		X[1] = (iv[4] << 24) | (iv[5] << 16) | (iv[6] << 8) | iv[7];
		X[2] = (iv[8] << 24) | (iv[9] << 16) | (iv[10] << 8) | iv[11];
		X[3] = (iv[12] << 24) | (iv[13] << 16);
		X[3] += i;

		if(keylen == 16){
			if(mode == 0)
				// ENC_uLBC_128_fast(X, subkey);
				_ENC_uLBC_128(X, subkey);
			else
				_DEC_uLBC_128(X, subkey);
		}	
		else{
			if(mode == 0)
				// ENC_uLBC_256_fast(X, subkey);
				_ENC_uLBC_256(X, subkey);
			else
				_DEC_uLBC_256(X, subkey);
		}

		j = i << 4;
		output[j] = ((X[0] >> 24) & 0xff) ^ input[j];
		output[j + 1] = ((X[0] >> 16) & 0xff) ^ input[j + 1];
		output[j + 2] = ((X[0] >> 8) & 0xff) ^ input[j + 2];
		output[j + 3] = (X[0] & 0xff) ^ input[j + 3];
		j += 4;
		output[j] = ((X[1] >> 24) & 0xff) ^ input[j];
		output[j + 1] = ((X[1] >> 16) & 0xff) ^ input[j + 1];
		output[j + 2] = ((X[1] >> 8) & 0xff) ^ input[j + 2];
		output[j + 3] = (X[1] & 0xff) ^ input[j + 3];
		j += 4;
		output[j] = ((X[2] >> 24) & 0xff) ^ input[j];
		output[j + 1] = ((X[2] >> 16) & 0xff) ^ input[j + 1];
		output[j + 2] = ((X[2] >> 8) & 0xff) ^ input[j + 2];
		output[j + 3] = (X[2] & 0xff) ^ input[j + 3];
		j += 4;
		output[j] = ((X[3] >> 24) & 0xff) ^ input[j];
		output[j + 1] = ((X[3] >> 16) & 0xff) ^ input[j + 1];
		output[j + 2] = ((X[3] >> 8) & 0xff) ^ input[j + 2];
		output[j + 3] = (X[3] & 0xff) ^ input[j + 3];
	}
	X[0] = (iv[0] << 24) | (iv[1] << 16) | (iv[2] << 8) | iv[3];
	X[1] = (iv[4] << 24) | (iv[5] << 16) | (iv[6] << 8) | iv[7];
	X[2] = (iv[8] << 24) | (iv[9] << 16) | (iv[10] << 8) | iv[11];
	X[3] = (iv[12] << 24) | (iv[13] << 16);
	X[3] += i;

	if(keylen == 16){
		if(mode == 0)
			// ENC_uLBC_128_fast(X, subkey);
			_ENC_uLBC_128(X, subkey);
		else
			_DEC_uLBC_128(X, subkey);
	}	
	else{
		if(mode == 0)
			// ENC_uLBC_256_fast(X, subkey);
			_ENC_uLBC_256(X, subkey);
		else
			_DEC_uLBC_256(X, subkey);
	}
	i = blocknum << 4;
	int t = 0;
	for (t = 0; t < in_len - i; t++)
	{
		output[i + t] = ((X[t / 4] >> (24 - (t % 4) * 8)) & 0xff) ^ input[i + t];
	}
	*out_len = in_len;
	return 1;
}


//
//遥测加密接口，仅提供加密功能
/*
iv 14个字节，预留2个字节作为计数器，进行内部加密
input输入明文，明文字节长度in_len<256,帧长10个比特计数
*/
int Crypt_Enc_YCe(unsigned char *iv,  unsigned char *input, int in_len, unsigned char *output, int *out_len, unsigned char *key, int keylen)
{
	if (keylen != 16 && keylen != 32)
	{
		printf("The length of key is 16 or 32, exit");
		return 0;
	}
	int tag = Crypt_CTR_IV14(iv, 14, input, in_len, output, out_len, key, keylen, 0);

	return tag;
}

// 
////遥测加密接口，仅提供加密功能
/*
iv 14个字节，预留2个字节作为计数器，进行内部加密
input输入明文，明文字节长度in_len<256,帧长10个比特计数
*/
int Crypt_Dec_YCe(unsigned char *iv, unsigned char *input, int in_len, unsigned char *output, int *out_len, unsigned char *key, int keylen)
{
	if (keylen != 16 && keylen != 32)
	{
		printf("The length of key is 16 or 32, exit");
		return 0;
	}
	int tag = Crypt_CTR_IV14(iv, 14, input, in_len, output, out_len, key, keylen, 1);

	return tag;
}

//
//数传加密接口，仅提供加密功能
/*
iv 14个字节，预留2个字节作为计数器，进行内部加密
input输入明文，明文字节长度in_len<256,帧长10个比特计数
*/
int Crypt_Enc_SC(unsigned char *iv, unsigned char *input, int in_len, unsigned char *output, int *out_len, unsigned char *key, int keylen)
{
	int tag = Crypt_CTR_IV14(iv, 14, input, in_len, output, out_len, key, keylen, 0);

	return tag;
}

//
////数传加密接口，仅提供加密功能
/*
iv 14个字节，预留2个字节作为计数器，进行内部加密
input输入明文，明文字节长度in_len<256,帧长10个比特计数
*/
int Crypt_Dec_SC(unsigned char *iv,  unsigned char *input, int in_len, unsigned char *output, int *out_len, unsigned char *key, int keylen)
{
	int tag = Crypt_CTR_IV14(iv, 14, input, in_len, output, out_len, key, keylen, 1);

	return tag;
}



void Double(unsigned int LI[4])
{
	int i = 0;
	unsigned int flag = 0x80000000 & LI[0];
	for (i = 0; i < 3; i++)
	{
		LI[i] = (LI[i] << 1) | (LI[i + 1] >> 31);
	}
	LI[3] = LI[3] << 1;
	if (flag)
		LI[3] = LI[3] ^ 0x00000087;
}


// set L[32 * 4], so message length less than 2^32 * 128 bits
void SetUp(unsigned int subkey[], unsigned int LS[4], unsigned int LD[4], unsigned int L[128], int keylen)
{
	unsigned int tmp[4] = { 0 };
	if(keylen == 16)
		// ENC_uLBC_128_fast(tmp, subkey);
		_ENC_uLBC_128(tmp, subkey);
	else
		// ENC_uLBC_256_fast(tmp, subkey);
		_ENC_uLBC_256(tmp, subkey);

	LS[0] = tmp[0];
	LS[1] = tmp[1];
	LS[2] = tmp[2];
	LS[3] = tmp[3];
	Double(tmp);
	LD[0] = tmp[0];
	LD[1] = tmp[1];
	LD[2] = tmp[2];
	LD[3] = tmp[3];
	int i = 0;
	for (i = 0; i < 32; i++)
	{
		Double(tmp);
		L[4 * i + 0] = tmp[0];
		L[4 * i + 1] = tmp[1];
		L[4 * i + 2] = tmp[2];
		L[4 * i + 3] = tmp[3];
	}

}

unsigned int ntz(unsigned int i)
{
	unsigned int j = 0;
	for (j = 0; j < 32; j++)
	{
		if ((i >> j) & 0x00000001)
		{
			return j;
		}
	}
	return 0;
}

void Hash(unsigned int subkey[], unsigned char input_a[], unsigned int length_a, unsigned int LS[4], unsigned int L[128], unsigned int sum[4], int keylen)
{
	unsigned int tmp[4] = { 0 };
	unsigned int tmp_sum[4] = { 0 };
	unsigned int tmp_offset[4] = { 0 };
	unsigned int A[4] = { 0 };
	unsigned int blocknum = length_a >> 4;
	unsigned int restnum = length_a - (blocknum << 4);
	unsigned int i, j = 0;
	for (i = 1; i <= blocknum; i++)
	{
		j = (i - 1) << 4;
		A[0] = (input_a[j] << 24) | (input_a[j + 1] << 16) | (input_a[j + 2] << 8) | input_a[j + 3];
		j += 4;
		A[1] = (input_a[j] << 24) | (input_a[j + 1] << 16) | (input_a[j + 2] << 8) | input_a[j + 3];
		j += 4;
		A[2] = (input_a[j] << 24) | (input_a[j + 1] << 16) | (input_a[j + 2] << 8) | input_a[j + 3];
		j += 4;
		A[3] = (input_a[j] << 24) | (input_a[j + 1] << 16) | (input_a[j + 2] << 8) | input_a[j + 3];

		j = ntz(i);
		tmp_offset[0] ^= L[4 * j + 0];
		tmp_offset[1] ^= L[4 * j + 1];
		tmp_offset[2] ^= L[4 * j + 2];
		tmp_offset[3] ^= L[4 * j + 3];

		tmp[0] = A[0] ^ tmp_offset[0];
		tmp[1] = A[1] ^ tmp_offset[1];
		tmp[2] = A[2] ^ tmp_offset[2];
		tmp[3] = A[3] ^ tmp_offset[3];

		if(keylen == 16)
			// ENC_uLBC_128_fast(tmp, subkey);
			_ENC_uLBC_128(tmp, subkey);
		else
			// ENC_uLBC_256_fast(tmp, subkey);
			_ENC_uLBC_256(tmp, subkey);
		tmp_sum[0] ^= tmp[0];
		tmp_sum[1] ^= tmp[1];
		tmp_sum[2] ^= tmp[2];
		tmp_sum[3] ^= tmp[3];
	}
	// we set |A| must be 8 * n (n is integer)
	if (restnum)
	{
		unsigned int wordnum = restnum >> 2;
		j = 0;
		memset(A, 0, sizeof(A));
		for (i = 0; i < wordnum; i++)
		{
			A[i] = (input_a[(blocknum << 4) + j] << 24) | (input_a[(blocknum << 4) + j + 1] << 16) | (input_a[(blocknum << 4) + j + 2] << 8) | input_a[(blocknum << 4) + j + 3];
			j += 4;
		}
		unsigned int restbytenum = restnum - (wordnum << 2);
		for (i = 0; i < restbytenum; i++)
		{
			A[wordnum] |= input_a[(blocknum << 4) + j] << (24 - (i << 3));
			j += 1;
		}
		A[wordnum] |= 0x80 << (24 - (restbytenum << 3));//error

		tmp_offset[0] ^= LS[0];
		tmp_offset[1] ^= LS[1];
		tmp_offset[2] ^= LS[2];
		tmp_offset[3] ^= LS[3];


		A[0] ^= tmp_offset[0];
		A[1] ^= tmp_offset[1];
		A[2] ^= tmp_offset[2];
		A[3] ^= tmp_offset[3];

		if(keylen == 16)
			// ENC_uLBC_128_fast(A, subkey);
			_ENC_uLBC_128(A, subkey);
		else
			// ENC_uLBC_256_fast(A, subkey);
			_ENC_uLBC_256(A, subkey);

		tmp_sum[0] ^= A[0];
		tmp_sum[1] ^= A[1];
		tmp_sum[2] ^= A[2];
		tmp_sum[3] ^= A[3];

	}
	//printf("%08x %08x %08x %08x\n", tmp_sum[0], tmp_sum[1], tmp_sum[2], tmp_sum[3]);
	memcpy(sum, tmp_sum, sizeof(tmp_sum));
}
//int Crypt_YKDV__AEAD(unsigned char* iv, unsigned char* input_en, int enin_len, unsigned char* input_mac, int macin_len, unsigned char* output_en, int* out_len_en, unsigned char* ver_mac, int out_len_mac, unsigned char* key, int keylen);

int ENC_OCB_TAGLEN128(unsigned char* iv, unsigned char* input_en, int enin_len, unsigned char* input_mac, int macin_len, unsigned char* output_en, int* out_len_en, unsigned char* ver_mac, int out_len_mac, unsigned char* key, int keylen)
{
	unsigned char* input_n = iv;
	int* len_output = out_len_en;
	unsigned int length_p = enin_len;
	unsigned char* input_p = input_en;
	unsigned char* output = output_en;
	unsigned char* input_a = input_mac;
	unsigned int length_a = macin_len;
	unsigned char* tag = ver_mac;

	if (enin_len < 0)
	{
		printf("enin_len < 0.\n");
		return 0;
	}
	if (out_len_mac != 16)
	{
		printf("out_len_mac != 16.\n");
		return 0;
	}

	unsigned int subkey[31*4] = { 0 };

	if(keylen == 16){
		unsigned int MK[4] = { 0 };
		MK[0] = (key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3];
		MK[1] = (key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7];
		MK[2] = (key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11];
		MK[3] = (key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15];
		// KeyGen_128_fast(MK, subkey);
		KeyGen_128(MK, subkey);
	}
	else{
		unsigned int MK[8] = { 0 };
		MK[0] = (key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3];
		MK[1] = (key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7];
		MK[2] = (key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11];
		MK[3] = (key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15];
		MK[4] = (key[16] << 24) | (key[17] << 16) | (key[18] << 8) | key[19];
		MK[5] = (key[20] << 24) | (key[21] << 16) | (key[22] << 8) | key[23];
		MK[6] = (key[24] << 24) | (key[25] << 16) | (key[26] << 8) | key[27];
		MK[7] = (key[28] << 24) | (key[29] << 16) | (key[30] << 8) | key[31];
		// KeyGen_256_fast(MK, subkey);
		KeyGen_256(MK, subkey);
	}

	unsigned int LS[4] = { 0 }, LD[4] = { 0 }, L[128] = { 0 };
	SetUp(subkey, LS, LD, L, keylen);
	unsigned int tmp[4] = { 0 };
	unsigned int* Nonce = tmp;
	unsigned int i = 0, j = 0;
	//unsigned int wordnum = 14 >> 2;//3
	//unsigned int restbytenum = 14 - (3 << 2);//error//2
	Nonce[0] |= 0x01 << 16;
	j = 0;
	for (i = 0; i < 2; i++)
	{
		Nonce[0] |= input_n[j] << ((1 - i) << 3);
		j += 1;
	}
	Nonce[1] = (input_n[j] << 24) | (input_n[j + 1] << 16) | (input_n[j + 2] << 8) | (input_n[j + 3]);
	j += 4;
	Nonce[2] = (input_n[j] << 24) | (input_n[j + 1] << 16) | (input_n[j + 2] << 8) | (input_n[j + 3]);
	j += 4;
	Nonce[3] = (input_n[j] << 24) | (input_n[j + 1] << 16) | (input_n[j + 2] << 8) | (input_n[j + 3]);

	unsigned int bottom = Nonce[3] & 0x0000003f;
	Nonce[3] &= 0xffffffc0;
	//printf("%08x %08x %08x %08x \n", Nonce[0], Nonce[1], Nonce[2], Nonce[3]);

	if(keylen == 16)
		// ENC_uLBC_128_fast(Nonce, subkey);
		_ENC_uLBC_128(Nonce, subkey);
	else
		// ENC_uLBC_256_fast(Nonce, subkey);
		_ENC_uLBC_256(Nonce, subkey);
	unsigned int* Ktop = Nonce;
	unsigned int Stretch[6] = { 0 };
	Stretch[0] = Ktop[0];
	Stretch[1] = Ktop[1];
	Stretch[2] = Ktop[2];
	Stretch[3] = Ktop[3];
	Stretch[4] = Ktop[0] ^ ((Ktop[0] << 8) | Ktop[1] >> 24);
	Stretch[5] = Ktop[1] ^ ((Ktop[1] << 8) | Ktop[2] >> 24);

	unsigned int tmp_offset[4] = { 0 };

	if (bottom == 0)
	{
		tmp_offset[0] = Stretch[0];
		tmp_offset[1] = Stretch[1];
		tmp_offset[2] = Stretch[2];
		tmp_offset[3] = Stretch[3];
	}
	else if (bottom == 32)
	{
		tmp_offset[0] = Stretch[1];
		tmp_offset[1] = Stretch[2];
		tmp_offset[2] = Stretch[3];
		tmp_offset[3] = Stretch[4];
	}
	else if (bottom > 32)
	{
		tmp_offset[0] = (Stretch[1] << (bottom - 32)) | (Stretch[2] >> (64 - bottom));
		tmp_offset[1] = (Stretch[2] << (bottom - 32)) | (Stretch[3] >> (64 - bottom));
		tmp_offset[2] = (Stretch[3] << (bottom - 32)) | (Stretch[4] >> (64 - bottom));
		tmp_offset[3] = (Stretch[4] << (bottom - 32)) | (Stretch[5] >> (64 - bottom));
	}
	else
	{
		tmp_offset[0] = (Stretch[0] << bottom) | (Stretch[1] >> (32 - bottom));
		tmp_offset[1] = (Stretch[1] << bottom) | (Stretch[2] >> (32 - bottom));
		tmp_offset[2] = (Stretch[2] << bottom) | (Stretch[3] >> (32 - bottom));
		tmp_offset[3] = (Stretch[3] << bottom) | (Stretch[4] >> (32 - bottom));
	}

	unsigned int tmp_sum[4] = { 0 };
	unsigned int P[4] = { 0 };
	*len_output = 0;
	unsigned int blocknum = length_p >> 4;
	unsigned int C[4] = { 0 };
	for (i = 1; i <= blocknum; i++)
	{
		j = (i - 1) << 4;
		P[0] = (input_p[j] << 24) | (input_p[j + 1] << 16) | (input_p[j + 2] << 8) | input_p[j + 3];
		j += 4;
		P[1] = (input_p[j] << 24) | (input_p[j + 1] << 16) | (input_p[j + 2] << 8) | input_p[j + 3];
		j += 4;
		P[2] = (input_p[j] << 24) | (input_p[j + 1] << 16) | (input_p[j + 2] << 8) | input_p[j + 3];
		j += 4;
		P[3] = (input_p[j] << 24) | (input_p[j + 1] << 16) | (input_p[j + 2] << 8) | input_p[j + 3];

		j = ntz(i);
		//printf("tmp_offset: %08x %08x %08x %08x \n", tmp_offset[0], tmp_offset[1], tmp_offset[2], tmp_offset[3]);
		//printf("LI: %08x %08x %08x %08x \n", L[4 * j + 0], L[4 * j + 1], L[4 * j + 2], L[4 * j + 3]);
		tmp_offset[0] ^= L[4 * j + 0];
		tmp_offset[1] ^= L[4 * j + 1];
		tmp_offset[2] ^= L[4 * j + 2];
		tmp_offset[3] ^= L[4 * j + 3];
		tmp[0] = P[0] ^ tmp_offset[0];
		tmp[1] = P[1] ^ tmp_offset[1];
		tmp[2] = P[2] ^ tmp_offset[2];
		tmp[3] = P[3] ^ tmp_offset[3];
		//printf("enc_in: %08x %08x %08x %08x \n", tmp[0], tmp[1], tmp[2], tmp[3]);
		if(keylen == 16)
			// ENC_uLBC_128_fast(tmp, subkey);
			_ENC_uLBC_128(tmp, subkey);
		else
			// ENC_uLBC_256_fast(tmp, subkey);
			_ENC_uLBC_256(tmp, subkey);
		//printf("enc_out: %08x %08x %08x %08x \n", tmp[0], tmp[1], tmp[2], tmp[3]);

		//printf("tmp_offset: %08x %08x %08x %08x \n", tmp_offset[0], tmp_offset[1], tmp_offset[2], tmp_offset[3]);
		C[0] = tmp_offset[0] ^ tmp[0];
		C[1] = tmp_offset[1] ^ tmp[1];
		C[2] = tmp_offset[2] ^ tmp[2];
		C[3] = tmp_offset[3] ^ tmp[3];
		//printf("C: %08x %08x %08x %08x \n", C[0], C[1], C[2], C[3]);


		j = (i - 1) << 4;
		output[j] = (C[0] >> 24) & 0xff;
		output[j + 1] = (C[0] >> 16) & 0xff;
		output[j + 2] = (C[0] >> 8) & 0xff;
		output[j + 3] = C[0] & 0xff;
		j += 4;
		output[j] = (C[1] >> 24) & 0xff;
		output[j + 1] = (C[1] >> 16) & 0xff;
		output[j + 2] = (C[1] >> 8) & 0xff;
		output[j + 3] = C[1] & 0xff;
		j += 4;
		output[j] = (C[2] >> 24) & 0xff;
		output[j + 1] = (C[2] >> 16) & 0xff;
		output[j + 2] = (C[2] >> 8) & 0xff;
		output[j + 3] = C[2] & 0xff;
		j += 4;
		output[j] = (C[3] >> 24) & 0xff;
		output[j + 1] = (C[3] >> 16) & 0xff;
		output[j + 2] = (C[3] >> 8) & 0xff;
		output[j + 3] = C[3] & 0xff;

		tmp_sum[0] = P[0] ^ tmp_sum[0];
		tmp_sum[1] = P[1] ^ tmp_sum[1];
		tmp_sum[2] = P[2] ^ tmp_sum[2];
		tmp_sum[3] = P[3] ^ tmp_sum[3];
	}
	*len_output = blocknum << 4;
	unsigned int restnum = length_p - (blocknum << 4);


	if (restnum)
	{
		unsigned int wordnum = restnum >> 2;
		j = 0;
		memset(P, 0, sizeof(P));
		memset(C, 0, sizeof(C));
		for (i = 0; i < wordnum; i++)
		{
			P[i] = (input_p[(blocknum << 4) + j] << 24) | (input_p[(blocknum << 4) + j + 1] << 16) | (input_p[(blocknum << 4) + j + 2] << 8) | input_p[(blocknum << 4) + j + 3];
			j += 4;
			//printf("\n%08x\n", P[i]);
		}
		unsigned int restbytenum = restnum - (wordnum << 2);
		for (i = 0; i < restbytenum; i++)
		{
			P[wordnum] |= input_p[(blocknum << 4) + j] << (24 - (i << 3));
			j += 1;
		}
		P[wordnum] |= 0x80 << (24 - (restbytenum << 3));
		//printf("\n%08x\n", P[wordnum]);
		//printf("tmp_offset: %08x %08x %08x %08x \n", tmp_offset[0], tmp_offset[1], tmp_offset[2], tmp_offset[3]);
		//printf("LI: %08x %08x %08x %08x \n", L[4 * j + 0], L[4 * j + 1], L[4 * j + 2], L[4 * j + 3]);

		tmp_offset[0] ^= LS[0];
		tmp_offset[1] ^= LS[1];
		tmp_offset[2] ^= LS[2];
		tmp_offset[3] ^= LS[3];
		//printf("tmp_offset: %08x %08x %08x %08x \n", tmp_offset[0], tmp_offset[1], tmp_offset[2], tmp_offset[3]);
		
		memcpy(tmp, tmp_offset, sizeof(tmp_offset));
		//printf("enc_in: %08x %08x %08x %08x \n", tmp[0], tmp[1], tmp[2], tmp[3]);
		if(keylen == 16)
			// ENC_uLBC_128_fast(tmp, subkey);
			_ENC_uLBC_128(tmp, subkey);
		else
			// ENC_uLBC_256_fast(tmp, subkey);
			_ENC_uLBC_256(tmp, subkey);
		//printf("enc_out: %08x %08x %08x %08x \n", tmp[0], tmp[1], tmp[2], tmp[3]);
		unsigned int* pad = tmp;

		C[0] = P[0] ^ pad[0];
		C[1] = P[1] ^ pad[1];
		C[2] = P[2] ^ pad[2];
		C[3] = P[3] ^ pad[3];
		//printf("C: %08x %08x %08x %08x \n", C[0], C[1], C[2], C[3]);

		j = blocknum << 4;
		for (size_t i = 0; i < wordnum; i++)
		{
			output[j] = (C[i] >> 24) & 0xff;
			output[j + 1] = (C[i] >> 16) & 0xff;
			output[j + 2] = (C[i] >> 8) & 0xff;
			output[j + 3] = C[i] & 0xff;
			j += 4;
		}
		for (i = 0; i < restbytenum; i++)
		{
			output[j + i] = (C[wordnum] >> (24 - (i << 3))) & 0xff;
		}
		*len_output = (blocknum << 4) + restnum;

		tmp_offset[0] ^= P[0];
		tmp_offset[1] ^= P[1];
		tmp_offset[2] ^= P[2];
		tmp_offset[3] ^= P[3];

	}
	//printf("tmp_sum: %08x %08x %08x %08x \n", tmp_sum[0], tmp_sum[1], tmp_sum[2], tmp_sum[3]);
	tmp_sum[0] ^= LD[0] ^ tmp_offset[0];
	tmp_sum[1] ^= LD[1] ^ tmp_offset[1];
	tmp_sum[2] ^= LD[2] ^ tmp_offset[2];
	tmp_sum[3] ^= LD[3] ^ tmp_offset[3];
	//printf("tmp_sum: %08x %08x %08x %08x \n", tmp_sum[0], tmp_sum[1], tmp_sum[2], tmp_sum[3]);

	if(keylen == 16)
		// ENC_uLBC_128_fast(tmp_sum, subkey);
		_ENC_uLBC_128(tmp_sum, subkey);
	else
		// ENC_uLBC_256_fast(tmp_sum, subkey);
		_ENC_uLBC_256(tmp_sum, subkey);
	//printf("enc_out: %08x %08x %08x %08x \n", tmp_sum[0], tmp_sum[1], tmp_sum[2], tmp_sum[3]);
	//printf("\n%08x %08x %08x %08x\n", tmp[0], tmp[1], tmp[2], tmp[3]);
	Hash(subkey, input_a, length_a, LS, L, tmp, keylen);
	//printf("tmp: %08x %08x %08x %08x \n", tmp[0], tmp[1], tmp[2], tmp[3]);

	tmp[0] ^= tmp_sum[0];
	tmp[1] ^= tmp_sum[1];
	tmp[2] ^= tmp_sum[2];
	tmp[3] ^= tmp_sum[3];


	tag[0] = (tmp[0] >> 24) & 0xff;
	tag[1] = (tmp[0] >> 16) & 0xff;
	tag[2] = (tmp[0] >> 8) & 0xff;
	tag[3] = tmp[0] & 0xff;
	tag[4] = (tmp[1] >> 24) & 0xff;
	tag[5] = (tmp[1] >> 16) & 0xff;
	tag[6] = (tmp[1] >> 8) & 0xff;
	tag[7] = tmp[1] & 0xff;
	tag[8] = (tmp[2] >> 24) & 0xff;
	tag[9] = (tmp[2] >> 16) & 0xff;
	tag[10] = (tmp[2] >> 8) & 0xff;
	tag[11] = tmp[2] & 0xff;
	tag[12] = (tmp[3] >> 24) & 0xff;
	tag[13] = (tmp[3] >> 16) & 0xff;
	tag[14] = (tmp[3] >> 8) & 0xff;
	tag[15] = tmp[3] & 0xff;

	return 1;
}


// return 1 vaild, return 0 invalid
int DEC_OCB_TAGLEN128(unsigned char* iv, unsigned char* input_en, int enin_len, unsigned char* input_mac, int macin_len, unsigned char* output_en, int* out_len_en, unsigned char* ver_mac, int out_len_mac, unsigned char* key, int keylen)
{
	unsigned char* input_n = iv;
	int* len_output = out_len_en;
	unsigned int length_c = enin_len;
	unsigned char* input_c = input_en;
	unsigned char* output = output_en;
	unsigned char* input_a = input_mac;
	unsigned int length_a = macin_len;
	unsigned char* tag = ver_mac;

	if (enin_len < 0)
	{
		printf("enin_len < 0.\n");
		return 0;
	}
	if (keylen != 32)
	{
		printf("keylen != 32.\n");
		return 0;
	}
	if (out_len_mac != 16)
	{
		printf("out_len_mac != 16.\n");
		return 0;
	}

	unsigned int subkey[31*4] = { 0 };
	unsigned int subkey_dec[31*4] = { 0 };

	if(keylen == 16){
		unsigned int MK[4] = { 0 };
		MK[0] = (key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3];
		MK[1] = (key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7];
		MK[2] = (key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11];
		MK[3] = (key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15];
		// KeyGen_128_fast(MK, subkey);
		KeyGen_128(MK, subkey);
		KeyGen_128(MK, subkey_dec);
	}
	else{
		unsigned int MK[8] = { 0 };
		MK[0] = (key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3];
		MK[1] = (key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7];
		MK[2] = (key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11];
		MK[3] = (key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15];
		MK[4] = (key[16] << 24) | (key[17] << 16) | (key[18] << 8) | key[19];
		MK[5] = (key[20] << 24) | (key[21] << 16) | (key[22] << 8) | key[23];
		MK[6] = (key[24] << 24) | (key[25] << 16) | (key[26] << 8) | key[27];
		MK[7] = (key[28] << 24) | (key[29] << 16) | (key[30] << 8) | key[31];
		// KeyGen_256_fast(MK, subkey);
		KeyGen_256(MK, subkey);
		KeyGen_256(MK, subkey_dec);
	}
	unsigned int LS[4] = { 0 }, LD[4] = { 0 }, L[128] = { 0 };
	SetUp(subkey, LS, LD, L, keylen);
	unsigned int tmp[4] = { 0 };
	unsigned int* Nonce = tmp;
	unsigned int i = 0, j = 0;
	Nonce[0] |= 0x01 << 16;
	j = 0;
	for (i = 0; i < 2; i++)
	{
		Nonce[0] |= input_n[j] << ((1 - i) << 3);
		j += 1;
	}
	Nonce[1] = (input_n[j] << 24) | (input_n[j + 1] << 16) | (input_n[j + 2] << 8) | (input_n[j + 3]);
	j += 4;
	Nonce[2] = (input_n[j] << 24) | (input_n[j + 1] << 16) | (input_n[j + 2] << 8) | (input_n[j + 3]);
	j += 4;
	Nonce[3] = (input_n[j] << 24) | (input_n[j + 1] << 16) | (input_n[j + 2] << 8) | (input_n[j + 3]);

	unsigned int bottom = Nonce[3] & 0x0000003f;
	Nonce[3] &= 0xffffffc0;

	if(keylen == 16)
		// ENC_uLBC_128_fast(Nonce, subkey);
		_ENC_uLBC_128(Nonce, subkey);
	else
		// ENC_uLBC_256_fast(Nonce, subkey);
		_ENC_uLBC_256(Nonce, subkey);

	unsigned int* Ktop = Nonce;
	unsigned int Stretch[6] = { 0 };
	Stretch[0] = Ktop[0];
	Stretch[1] = Ktop[1];
	Stretch[2] = Ktop[2];
	Stretch[3] = Ktop[3];
	Stretch[4] = Ktop[0] ^ ((Ktop[0] << 8) | Ktop[1] >> 24);
	Stretch[5] = Ktop[1] ^ ((Ktop[1] << 8) | Ktop[2] >> 24);

	unsigned int tmp_offset[4] = { 0 };

	if (bottom == 0)
	{
		tmp_offset[0] = Stretch[0];
		tmp_offset[1] = Stretch[1];
		tmp_offset[2] = Stretch[2];
		tmp_offset[3] = Stretch[3];
	}
	else if (bottom == 32)
	{
		tmp_offset[0] = Stretch[1];
		tmp_offset[1] = Stretch[2];
		tmp_offset[2] = Stretch[3];
		tmp_offset[3] = Stretch[4];
	}
	else if (bottom > 32)
	{
		tmp_offset[0] = (Stretch[1] << (bottom - 32)) | (Stretch[2] >> (64 - bottom));
		tmp_offset[1] = (Stretch[2] << (bottom - 32)) | (Stretch[3] >> (64 - bottom));
		tmp_offset[2] = (Stretch[3] << (bottom - 32)) | (Stretch[4] >> (64 - bottom));
		tmp_offset[3] = (Stretch[4] << (bottom - 32)) | (Stretch[5] >> (64 - bottom));
	}
	else
	{
		tmp_offset[0] = (Stretch[0] << bottom) | (Stretch[1] >> (32 - bottom));
		tmp_offset[1] = (Stretch[1] << bottom) | (Stretch[2] >> (32 - bottom));
		tmp_offset[2] = (Stretch[2] << bottom) | (Stretch[3] >> (32 - bottom));
		tmp_offset[3] = (Stretch[3] << bottom) | (Stretch[4] >> (32 - bottom));
	}


	unsigned int tmp_sum[4] = { 0 };
	unsigned int C[4] = { 0 };
	*len_output = 0;
	unsigned int blocknum = length_c >> 4;
	unsigned int P[4] = { 0 };
	for (i = 1; i <= blocknum; i++)
	{
		j = (i - 1) << 4;
		C[0] = (input_c[j] << 24) | (input_c[j + 1] << 16) | (input_c[j + 2] << 8) | input_c[j + 3];
		j += 4;
		C[1] = (input_c[j] << 24) | (input_c[j + 1] << 16) | (input_c[j + 2] << 8) | input_c[j + 3];
		j += 4;
		C[2] = (input_c[j] << 24) | (input_c[j + 1] << 16) | (input_c[j + 2] << 8) | input_c[j + 3];
		j += 4;
		C[3] = (input_c[j] << 24) | (input_c[j + 1] << 16) | (input_c[j + 2] << 8) | input_c[j + 3];

		j = ntz(i);
		tmp_offset[0] ^= L[4 * j + 0];
		tmp_offset[1] ^= L[4 * j + 1];
		tmp_offset[2] ^= L[4 * j + 2];
		tmp_offset[3] ^= L[4 * j + 3];
		tmp[0] = C[0] ^ tmp_offset[0];
		tmp[1] = C[1] ^ tmp_offset[1];
		tmp[2] = C[2] ^ tmp_offset[2];
		tmp[3] = C[3] ^ tmp_offset[3];
		if(keylen == 16)
			_DEC_uLBC_128(tmp, subkey_dec);
		else
			_DEC_uLBC_256(tmp, subkey_dec);

		P[0] = tmp_offset[0] ^ tmp[0];
		P[1] = tmp_offset[1] ^ tmp[1];
		P[2] = tmp_offset[2] ^ tmp[2];
		P[3] = tmp_offset[3] ^ tmp[3];
		j = (i - 1) << 4;
		output[j] = (P[0] >> 24) & 0xff;
		output[j + 1] = (P[0] >> 16) & 0xff;
		output[j + 2] = (P[0] >> 8) & 0xff;
		output[j + 3] = P[0] & 0xff;
		j += 4;
		output[j] = (P[1] >> 24) & 0xff;
		output[j + 1] = (P[1] >> 16) & 0xff;
		output[j + 2] = (P[1] >> 8) & 0xff;
		output[j + 3] = P[1] & 0xff;
		j += 4;
		output[j] = (P[2] >> 24) & 0xff;
		output[j + 1] = (P[2] >> 16) & 0xff;
		output[j + 2] = (P[2] >> 8) & 0xff;
		output[j + 3] = P[2] & 0xff;
		j += 4;
		output[j] = (P[3] >> 24) & 0xff;
		output[j + 1] = (P[3] >> 16) & 0xff;
		output[j + 2] = (P[3] >> 8) & 0xff;
		output[j + 3] = P[3] & 0xff;

		tmp_sum[0] = P[0] ^ tmp_sum[0];
		tmp_sum[1] = P[1] ^ tmp_sum[1];
		tmp_sum[2] = P[2] ^ tmp_sum[2];
		tmp_sum[3] = P[3] ^ tmp_sum[3];
	}
	*len_output = blocknum << 4;
	unsigned int restnum = length_c - (blocknum << 4);


	if (restnum)
	{
		unsigned int wordnum = restnum >> 2;
		j = 0;
		memset(P, 0, sizeof(P));
		memset(C, 0, sizeof(C));
		for (i = 0; i < wordnum; i++)
		{
			C[i] = (input_c[(blocknum << 4) + j] << 24) | (input_c[(blocknum << 4) + j + 1] << 16) | (input_c[(blocknum << 4) + j + 2] << 8) | input_c[(blocknum << 4) + j + 3];
			j += 4;
		}
		unsigned int restbytenum = restnum - (wordnum << 2);
		for (i = 0; i < restbytenum; i++)
		{
			C[wordnum] |= input_c[(blocknum << 4) + j] << (24 - (i << 3));
			j += 1;
		}


		tmp_offset[0] ^= LS[0];
		tmp_offset[1] ^= LS[1];
		tmp_offset[2] ^= LS[2];
		tmp_offset[3] ^= LS[3];

		memcpy(tmp, tmp_offset, sizeof(tmp_offset));
		//printf("enc_in: %08x %08x %08x %08x \n", tmp[0], tmp[1], tmp[2], tmp[3]);
		if(keylen == 16)
			// ENC_uLBC_128_fast(tmp, subkey);
			_ENC_uLBC_128(tmp, subkey_dec);
		else
			// ENC_uLBC_256_fast(tmp, subkey);
			_ENC_uLBC_256(tmp, subkey_dec);
		//printf("enc_out: %08x %08x %08x %08x \n", tmp[0], tmp[1], tmp[2], tmp[3]);
		unsigned int* pad = tmp;

		P[0] = C[0] ^ pad[0];
		P[1] = C[1] ^ pad[1];
		P[2] = C[2] ^ pad[2];
		P[3] = C[3] ^ pad[3];


		memset(tmp, 0, sizeof(tmp));

		j = blocknum << 4;
		for (size_t i = 0; i < wordnum; i++)
		{
			tmp[i] = P[i];
			//printf("\n%08x\n", tmp[i]);
			output[j] = (P[i] >> 24) & 0xff;
			output[j + 1] = (P[i] >> 16) & 0xff;
			output[j + 2] = (P[i] >> 8) & 0xff;
			output[j + 3] = P[i] & 0xff;
			j += 4;
		}
		for (i = 0; i < restbytenum; i++)
		{
			output[j + i] = (P[wordnum] >> (24 - (i << 3))) & 0xff;
			tmp[wordnum] |= ((P[wordnum] >> (24 - (i << 3))) & 0xff) << (24 - (i << 3));
		}
		//printf("\n%08x\n", tmp[wordnum]);
		*len_output = (blocknum << 4) + restnum;
		tmp[wordnum] |= 0x80 << (24 - (restbytenum << 3));
		tmp_offset[0] ^= tmp[0];
		tmp_offset[1] ^= tmp[1];
		tmp_offset[2] ^= tmp[2];
		tmp_offset[3] ^= tmp[3];

	}

	tmp_sum[0] ^= LD[0] ^ tmp_offset[0];
	tmp_sum[1] ^= LD[1] ^ tmp_offset[1];
	tmp_sum[2] ^= LD[2] ^ tmp_offset[2];
	tmp_sum[3] ^= LD[3] ^ tmp_offset[3];

	if(keylen == 16)
		// ENC_uLBC_128_fast(tmp_sum, subkey);
		_ENC_uLBC_128(tmp_sum, subkey);
	else
		// ENC_uLBC_256_fast(tmp_sum, subkey);
		_ENC_uLBC_256(tmp_sum, subkey);

	Hash(subkey, input_a, length_a, LS, L, tmp, keylen);

	tmp[0] ^= tmp_sum[0];
	tmp[1] ^= tmp_sum[1];
	tmp[2] ^= tmp_sum[2];
	tmp[3] ^= tmp_sum[3];

	unsigned char newtag[16] = { 0 };
	newtag[0] = (tmp[0] >> 24) & 0xff;
	newtag[1] = (tmp[0] >> 16) & 0xff;
	newtag[2] = (tmp[0] >> 8) & 0xff;
	newtag[3] = tmp[0] & 0xff;
	newtag[4] = (tmp[1] >> 24) & 0xff;
	newtag[5] = (tmp[1] >> 16) & 0xff;
	newtag[6] = (tmp[1] >> 8) & 0xff;
	newtag[7] = tmp[1] & 0xff;
	newtag[8] = (tmp[2] >> 24) & 0xff;
	newtag[9] = (tmp[2] >> 16) & 0xff;
	newtag[10] = (tmp[2] >> 8) & 0xff;
	newtag[11] = tmp[2] & 0xff;
	newtag[12] = (tmp[3] >> 24) & 0xff;
	newtag[13] = (tmp[3] >> 16) & 0xff;
	newtag[14] = (tmp[3] >> 8) & 0xff;
	newtag[15] = tmp[3] & 0xff;

	for (i = 0; i < 16; i++)
	{
		if (newtag[i] != tag[i])
		{
			return 0;
		}
	}
	return 1;
}

int main(){
	// genTestFile();
	// return 0;
	// tempFunc();
    // time_t t;
    // srand((unsigned) time(&t));
	// 0123456789ABCDEFFEDCBA98765432100123456789ABCDEFFEDCBA9876543210
	unsigned int plaint[4] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
	unsigned int keyt[12] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210,0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
	for(int i = 0; i < 4; i ++){
		plaint[i] = 0;
	}
	for(int j = 0; j < 12; j ++){
		keyt[j] = 0;
	}
	if (uLBC_Block(4, plaint, keyt, 128)){
		// if (uLBC_Block(5, plaint, keyt, 128)){
			printf("enc_out: %08x %08x %08x %08x\n", plaint[0], plaint[1], plaint[2], plaint[3]);
			// printf("enc_out: %08x %08x %08x %08x\n", plaint[4], plaint[5], plaint[6], plaint[7]);
		// }
	}
	// for (int times = 0; times < 10000; times ++){
	// 	uLBC_Block(2, plaint, keyt, 64);
	// }printf("enc_out: %08x %08x\n", plaint[0], plaint[1]);
	
    // printf("ECB\n");
    // testspeed();
    // printf("CTR\n");
    // testspeed_ctr();
    // unsigned char plain[64] = {1, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
    //                            1, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
    //                            1, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
    //                            1, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    // unsigned char key[32] = {1, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
    //                         0x0f, 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f, 0x9f, 0xaf, 0xbf, 0xcf, 0xdf, 0xef, 0xff};
    // unsigned char cipher[64] = {0};
    // unsigned char iv[14] = {1, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd,
    //                         0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32};
    // int outlen;
    // Crypt_CTR_IV14(iv, 14, plain, 64, cipher, &outlen, key, 32, 0);
    // for(int i = 0; i < outlen; i ++){
    //     printf("%02x", cipher[i]);
    // }
    // printf("\n");
    // memcpy(plain, cipher, 64);
    // Crypt_CTR_IV14(iv, 14, plain, 64, cipher, &outlen, key, 32, 1);
    // for(int i = 0; i < outlen; i ++){
    //     printf("%02x", cipher[i]);
    // }
    return 0;
}
