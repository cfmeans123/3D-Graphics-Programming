#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Position              0   xyzw        0     NONE   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
// COLOR                    0   xyzw        2     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// COLOR                    0   xyzw        0     NONE   float   xyzw
// COLOR                    1   xyzw        1     NONE   float   xyzw
// TEXCOORD                 0   xy          2     NONE   float   xy  
// SV_Position              0   xyzw        3      POS   float   xyzw
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c1         cb0             0         1  ( FLT, FLT, FLT, FLT)
// c2         cb0             3         5  ( FLT, FLT, FLT, FLT)
//
//
// Runtime generated constant mappings:
//
// Target Reg                               Constant Description
// ---------- --------------------------------------------------
// c0                              Vertex Shader position offset
//
//
// Level9 shader bytecode:
//
    vs_2_0
    def c7, 0, 1, 0, 0
    dcl_texcoord v0  // vin<0,1,2,3>
    dcl_texcoord1 v1  // vin<4,5>
    dcl_texcoord2 v2  // vin<6,7,8,9>

#line 40 "C:\Users\chand\OneDrive\Documents\GitHub\3D-Graphics-Programming\MEngine\External\DirectXTK\src\Shaders\Common.fxh"
    dp4 oPos.z, v0, c5  // ::VSAlphaTestVc<12>

#line 11
    dp4 r0.x, v0, c2
    max r0.x, r0.x, c7.x
    min oT1.w, r0.x, c7.y  // ::VSAlphaTestVc<7>

#line 62 "C:\Users\chand\OneDrive\Documents\GitHub\3D-Graphics-Programming\MEngine\External\DirectXTK\src\Shaders\AlphaTestEffect.fx"
    mul oT0, v2, c1  // ::VSAlphaTestVc<0,1,2,3>

#line 40 "C:\Users\chand\OneDrive\Documents\GitHub\3D-Graphics-Programming\MEngine\External\DirectXTK\src\Shaders\Common.fxh"
    dp4 r0.x, v0, c3  // ::vout<0>
    dp4 r0.y, v0, c4  // ::vout<1>
    dp4 r0.z, v0, c6  // ::vout<3>

#line 54 "C:\Users\chand\OneDrive\Documents\GitHub\3D-Graphics-Programming\MEngine\External\DirectXTK\src\Shaders\AlphaTestEffect.fx"
    mad oPos.xy, r0.z, c0, r0  // ::VSAlphaTestVc<10,11>
    mov oPos.w, r0.z  // ::VSAlphaTestVc<13>

#line 42 "C:\Users\chand\OneDrive\Documents\GitHub\3D-Graphics-Programming\MEngine\External\DirectXTK\src\Shaders\Common.fxh"
    mov oT1.xyz, c7.x  // ::VSAlphaTestVc<4,5,6>

#line 61 "C:\Users\chand\OneDrive\Documents\GitHub\3D-Graphics-Programming\MEngine\External\DirectXTK\src\Shaders\AlphaTestEffect.fx"
    mov oT2.xy, v1  // ::VSAlphaTestVc<8,9>

// approximately 12 instruction slots used
vs_4_0
dcl_constantbuffer CB0[8], immediateIndexed
dcl_input v0.xyzw
dcl_input v1.xy
dcl_input v2.xyzw
dcl_output o0.xyzw
dcl_output o1.xyzw
dcl_output o2.xy
dcl_output_siv o3.xyzw, position
mul o0.xyzw, v2.xyzw, cb0[0].xyzw
dp4_sat o1.w, v0.xyzw, cb0[3].xyzw
mov o1.xyz, l(0,0,0,0)
mov o2.xy, v1.xyxx
dp4 o3.x, v0.xyzw, cb0[4].xyzw
dp4 o3.y, v0.xyzw, cb0[5].xyzw
dp4 o3.z, v0.xyzw, cb0[6].xyzw
dp4 o3.w, v0.xyzw, cb0[7].xyzw
ret 
// Approximately 0 instruction slots used
#endif

const BYTE AlphaTestEffect_VSAlphaTestVc[] =
{
     68,  88,  66,  67, 203, 107, 
     92, 240, 122, 118, 127,   7, 
    180, 232, 253,  53,  31,  71, 
     64,  13,   1,   0,   0,   0, 
    184,   7,   0,   0,   4,   0, 
      0,   0,  48,   0,   0,   0, 
     72,   5,   0,   0, 184,   6, 
      0,   0,  44,   7,   0,   0, 
     65, 111, 110,  57,  16,   5, 
      0,   0,  16,   5,   0,   0, 
      0,   2, 254, 255, 208,   4, 
      0,   0,  64,   0,   0,   0, 
      2,   0,  36,   0,   0,   0, 
     60,   0,   0,   0,  60,   0, 
      0,   0,  36,   0,   1,   0, 
     60,   0,   0,   0,   0,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      5,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   2, 254, 255, 254, 255, 
    244,   0,  68,  66,  85,  71, 
     40,   0,   0,   0, 164,   3, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  24,   1, 
      0,   0,  16,   0,   0,   0, 
     32,   1,   0,   0,   3,   0, 
      0,   0, 104,   3,   0,   0, 
    160,   1,   0,   0,  67,  58, 
     92,  85, 115, 101, 114, 115, 
     92,  99, 104,  97, 110, 100, 
     92,  79, 110, 101,  68, 114, 
    105, 118, 101,  92,  68, 111, 
     99, 117, 109, 101, 110, 116, 
    115,  92,  71, 105, 116,  72, 
    117,  98,  92,  51,  68,  45, 
     71, 114,  97, 112, 104, 105, 
     99, 115,  45,  80, 114, 111, 
    103, 114,  97, 109, 109, 105, 
    110, 103,  92,  77,  69, 110, 
    103, 105, 110, 101,  92,  69, 
    120, 116, 101, 114, 110,  97, 
    108,  92,  68, 105, 114, 101, 
     99, 116,  88,  84,  75,  92, 
    115, 114,  99,  92,  83, 104, 
     97, 100, 101, 114, 115,  92, 
     67, 111, 109, 109, 111, 110, 
     46, 102, 120, 104,   0,  67, 
     58,  92,  85, 115, 101, 114, 
    115,  92,  99, 104,  97, 110, 
    100,  92,  79, 110, 101,  68, 
    114, 105, 118, 101,  92,  68, 
    111,  99, 117, 109, 101, 110, 
    116, 115,  92,  71, 105, 116, 
     72, 117,  98,  92,  51,  68, 
     45,  71, 114,  97, 112, 104, 
    105,  99, 115,  45,  80, 114, 
    111, 103, 114,  97, 109, 109, 
    105, 110, 103,  92,  77,  69, 
    110, 103, 105, 110, 101,  92, 
     69, 120, 116, 101, 114, 110, 
     97, 108,  92,  68, 105, 114, 
    101,  99, 116,  88,  84,  75, 
     92, 115, 114,  99,  92,  83, 
    104,  97, 100, 101, 114, 115, 
     92,  65, 108, 112, 104,  97, 
     84, 101, 115, 116,  69, 102, 
    102, 101,  99, 116,  46, 102, 
    120,   0, 171, 171,  40,   0, 
      0,   0, 155,   0,   0,   0, 
      0,   0, 255, 255, 216,   3, 
      0,   0,   0,   0, 255, 255, 
    240,   3,   0,   0,   0,   0, 
    255, 255, 252,   3,   0,   0, 
      0,   0, 255, 255,   8,   4, 
      0,   0,  40,   0,   0,   0, 
     20,   4,   0,   0,  11,   0, 
      0,   0,  36,   4,   0,   0, 
     11,   0,   0,   0,  52,   4, 
      0,   0,  11,   0,   0,   0, 
     68,   4,   0,   0,  62,   0, 
      1,   0,  84,   4,   0,   0, 
     40,   0,   0,   0, 100,   4, 
      0,   0,  40,   0,   0,   0, 
    116,   4,   0,   0,  40,   0, 
      0,   0, 132,   4,   0,   0, 
     54,   0,   1,   0, 148,   4, 
      0,   0,  54,   0,   1,   0, 
    168,   4,   0,   0,  42,   0, 
      0,   0, 180,   4,   0,   0, 
     61,   0,   1,   0, 192,   4, 
      0,   0,  86,  83,  65, 108, 
    112, 104,  97,  84, 101, 115, 
    116,  86,  99,   0,  68, 105, 
    102, 102, 117, 115, 101,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     83, 112, 101,  99, 117, 108, 
     97, 114,   0,  84, 101, 120, 
     67, 111, 111, 114, 100,   0, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   2,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     80, 111, 115, 105, 116, 105, 
    111, 110,  80,  83,   0, 171, 
    174,   1,   0,   0, 184,   1, 
      0,   0, 200,   1,   0,   0, 
    184,   1,   0,   0, 209,   1, 
      0,   0, 220,   1,   0,   0, 
    236,   1,   0,   0, 184,   1, 
      0,   0,   5,   0,   0,   0, 
      1,   0,  14,   0,   1,   0, 
      4,   0, 248,   1,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,  12,   0, 255, 255, 
      7,   0,   0,   0, 255, 255, 
    255, 255, 255, 255,   7,   0, 
      8,   0,   0,   0,   0,   0, 
      1,   0,   2,   0,   3,   0, 
     12,   0,   0,   0,  10,   0, 
     11,   0, 255, 255, 255, 255, 
     13,   0,   0,   0, 255, 255, 
    255, 255, 255, 255,  13,   0, 
     14,   0,   0,   0,   4,   0, 
      5,   0,   6,   0, 255, 255, 
     15,   0,   0,   0,   8,   0, 
      9,   0, 255, 255, 255, 255, 
    118, 105, 110,   0,  80, 111, 
    115, 105, 116, 105, 111, 110, 
      0,  67, 111, 108, 111, 114, 
      0, 171, 128,   2,   0,   0, 
    184,   1,   0,   0, 209,   1, 
      0,   0, 220,   1,   0,   0, 
    137,   2,   0,   0, 184,   1, 
      0,   0,   5,   0,   0,   0, 
      1,   0,  10,   0,   1,   0, 
      3,   0, 144,   2,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      1,   0,   2,   0,   3,   0, 
      2,   0,   0,   0,   4,   0, 
      5,   0, 255, 255, 255, 255, 
      3,   0,   0,   0,   6,   0, 
      7,   0,   8,   0,   9,   0, 
    118, 111, 117, 116,   0,  80, 
    111, 115,  95, 112, 115,   0, 
      1,   0,   3,   0,   1,   0, 
      3,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  70, 111, 
    103,  70,  97,  99, 116, 111, 
    114,   0, 171, 171,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 225,   2,   0,   0, 
    184,   1,   0,   0, 174,   1, 
      0,   0, 184,   1,   0,   0, 
    200,   1,   0,   0, 232,   2, 
      0,   0, 248,   2,   0,   0, 
      4,   3,   0,   0,   5,   0, 
      0,   0,   1,   0,  12,   0, 
      1,   0,   4,   0,  20,   3, 
      0,   0,   9,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
    255, 255,  10,   0,   0,   0, 
    255, 255,   1,   0, 255, 255, 
    255, 255,  11,   0,   0,   0, 
    255, 255, 255, 255,   3,   0, 
    255, 255,   0,   0,   0,   0, 
    160,   1,   0,   0,  24,   2, 
      0,   0,   7,   0,   0,   0, 
     40,   2,   0,   0, 160,   1, 
      0,   0, 124,   2,   0,   0, 
    168,   2,   0,   0,   3,   0, 
      0,   0, 184,   2,   0,   0, 
      0,   0,   0,   0, 220,   2, 
      0,   0,  52,   3,   0,   0, 
      3,   0,   0,   0,  68,   3, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
     81,   0,   0,   5,   7,   0, 
     15, 160,   0,   0,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   0,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      1, 128,   1,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      2, 128,   2,   0,  15, 144, 
      9,   0,   0,   3,   0,   0, 
      4, 192,   0,   0, 228, 144, 
      5,   0, 228, 160,   9,   0, 
      0,   3,   0,   0,   1, 128, 
      0,   0, 228, 144,   2,   0, 
    228, 160,  11,   0,   0,   3, 
      0,   0,   1, 128,   0,   0, 
      0, 128,   7,   0,   0, 160, 
     10,   0,   0,   3,   1,   0, 
      8, 224,   0,   0,   0, 128, 
      7,   0,  85, 160,   5,   0, 
      0,   3,   0,   0,  15, 224, 
      2,   0, 228, 144,   1,   0, 
    228, 160,   9,   0,   0,   3, 
      0,   0,   1, 128,   0,   0, 
    228, 144,   3,   0, 228, 160, 
      9,   0,   0,   3,   0,   0, 
      2, 128,   0,   0, 228, 144, 
      4,   0, 228, 160,   9,   0, 
      0,   3,   0,   0,   4, 128, 
      0,   0, 228, 144,   6,   0, 
    228, 160,   4,   0,   0,   4, 
      0,   0,   3, 192,   0,   0, 
    170, 128,   0,   0, 228, 160, 
      0,   0, 228, 128,   1,   0, 
      0,   2,   0,   0,   8, 192, 
      0,   0, 170, 128,   1,   0, 
      0,   2,   1,   0,   7, 224, 
      7,   0,   0, 160,   1,   0, 
      0,   2,   2,   0,   3, 224, 
      1,   0, 228, 144, 255, 255, 
      0,   0,  83,  72,  68,  82, 
    104,   1,   0,   0,  64,   0, 
      1,   0,  90,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      2,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3,  50,  32,  16,   0, 
      2,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  56,   0,   0,   8, 
    242,  32,  16,   0,   0,   0, 
      0,   0,  70,  30,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  17,  32, 
      0,   8, 130,  32,  16,   0, 
      1,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     54,   0,   0,   8, 114,  32, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     50,  32,  16,   0,   2,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,  17,   0, 
      0,   8,  18,  32,  16,   0, 
      3,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     17,   0,   0,   8,  34,  32, 
     16,   0,   3,   0,   0,   0, 
     70,  30,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  17,   0,   0,   8, 
     66,  32,  16,   0,   3,   0, 
      0,   0,  70,  30,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  17,   0, 
      0,   8, 130,  32,  16,   0, 
      3,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     62,   0,   0,   1,  73,  83, 
     71,  78, 108,   0,   0,   0, 
      3,   0,   0,   0,   8,   0, 
      0,   0,  80,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,  15, 
      0,   0,  92,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   3,   3, 
      0,   0, 101,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,  15,  15, 
      0,   0,  83,  86,  95,  80, 
    111, 115, 105, 116, 105, 111, 
    110,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0,  67, 
     79,  76,  79,  82,   0, 171, 
     79,  83,  71,  78, 132,   0, 
      0,   0,   4,   0,   0,   0, 
      8,   0,   0,   0, 104,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 104,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   0,   0,   0, 110,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,  12,   0,   0, 119,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     15,   0,   0,   0,  67,  79, 
     76,  79,  82,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0,  83,  86,  95,  80, 111, 
    115, 105, 116, 105, 111, 110, 
      0, 171
};