/*   Design Name:   HF_RM_FPGA                Module Name:   HF_RM_FPGA_top 
Specs on: https://cms-docdb.cern.ch/cgi-bin/DocDB/ShowDocument?docid=11545
  */

`timescale 1ns / 1ps

module HF_RM_FPGA_top(
 input MClk_P, MClk_N, // Master clock of the FPGA (from the backplane, probably via the GBTX)
       GBTX_ClkDesP, GBTX_ClkDesN,  // can be an 80, 160 or 320 MHz clock.
 // **********  Fast control from the backplane (40 MHz commands) :
       BkPln_RST_PLL, BkPln_RST_QIE, BkPln_RST_PWR, BkPln_RST,
 // **********  Slow control from the backplane (RBXbus) :
 input BKPLN_SERCLK,
 inout BKPLN_SERDAT, 
/* Signals named Qie*_<name> are connected to the QIE10 ASIC, we keep the same <name> as
 in the QIE10P5 specs, so the direction (in/out) is from the point of view of the QIE10. */ 
// **********  Data inputs from the twelve QIE ASICs:
 input [7:0] Qie1_Out, Qie2_Out, Qie3_Out, Qie4_Out, Qie5_Out, Qie6_Out, 
             Qie7_Out, Qie8_Out, Qie9_Out, Qie10_Out, Qie11_Out, Qie12_Out,
 input Qie1_CkOut, Qie2_CkOut, Qie3_CkOut, Qie4_CkOut, Qie5_CkOut, Qie6_CkOut,
       Qie7_CkOut, Qie8_CkOut, Qie9_CkOut, Qie10_CkOut, Qie11_CkOut, Qie12_CkOut,
 input Qie1_DiscOut, Qie2_DiscOut, Qie3_DiscOut, Qie4_DiscOut, Qie5_DiscOut, Qie6_DiscOut,
       Qie7_DiscOut, Qie8_DiscOut, Qie9_DiscOut, Qie10_DiscOut, Qie11_DiscOut, Qie12_DiscOut,
// ***************  Control of the QIE ASICs:
// Some of the QIE control signals can be shared between all the chips 
// (to be reviewed, depends on pin availability, PCB design, etc):
 output QieAll_SRin, QieAll_SRreset, QieAll_SRread,
 output Qie1_SRck, Qie2_SRck, Qie3_SRck, Qie4_SRck, Qie5_SRck, Qie6_SRck,
        Qie7_SRck, Qie8_SRck, Qie9_SRck, Qie10_SRck, Qie11_SRck, Qie12_SRck,
 output Qie1_SRload, Qie2_SRload, Qie3_SRload, Qie4_SRload, Qie5_SRload, Qie6_SRload,
        Qie7_SRload, Qie8_SRload, Qie9_SRload, Qie10_SRload, Qie11_SRload, Qie12_SRload,
 output Qie1_CImode, Qie2_CImode, Qie3_CImode, Qie4_CImode, Qie5_CImode, Qie6_CImode,
        Qie7_CImode, Qie8_CImode, Qie9_CImode, Qie10_CImode, Qie11_CImode, Qie12_CImode,
 input  Qie1_SRout, Qie2_SRout, Qie3_SRout, Qie4_SRout, Qie5_SRout, Qie6_SRout,
        Qie7_SRout, Qie8_SRout, Qie9_SRout, Qie10_SRout, Qie11_SRout, Qie12_SRout,        
/***************************/ 
 // Status:
 output BkPln_PWR_TRIP,
// ********** Data outputs to the three GBTX's :
 output [39:0] GBTX1_DInP, GBTX1_DInN, GBTX2_DInP, GBTX2_DInN, GBTX3_DInP, GBTX3_DInN,
// 8b10b mode needs 4 pairs, but it would be better to support also the Widebus (16 pairs):
 output [15:0] GBTX1_DIO_P, GBTX1_DIO_N, GBTX2_DIO_P, GBTX2_DIO_N, GBTX3_DIO_P, GBTX3_DIO_N
);

// Instatiation of the differential buffers:
CLKBUF_LVDS  MClk_BUF        (.PADP(MClk_P),  .PADN(MClk_N), .Y(MClk));   // verify it will be LVDS or LVPECL
CLKBUF_LVDS  GBTX_ClkDes_BUF (.PADP(GBTX_ClkDesP), .PADN(GBTX_ClkDesN), .Y(GBTX_ClkDes)); // LVDS buffer for SLVS input
// For LVDS outputs to GBTX use something like:
//  OUTBUF_LVDS  OUTBUF_LVDS1 (.D(GBTX1_internalSignal), .PADP(GBTX1_DInP[1]),  .PADN(GBTX1_DInN[1]) );

endmodule

