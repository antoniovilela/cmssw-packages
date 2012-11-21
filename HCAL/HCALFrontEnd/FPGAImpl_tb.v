`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:53:28 11/06/2012 
// Design Name: 
// Module Name:    FPGAImpl_tb 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module FPGAImpl_tb;

   reg clk, reset;
   reg Qie_Ck[11:0]; 
   reg [1:0] Cap_Id[11:0];
   reg [5:0] Adc_Mant_Data[11:0];
   reg [1:0] Adc_Exp_Data[11:0];
   reg [5:0] Tdc_Data[11:0];
   reg Qie_DiscOut[11:0];
   wire [7:0] Qie_Out[11:0];
   wire Qie_CkOut[11:0]; 

   QIESignal qie_0(
      .Qie_Ck(Qie_Ck[0]), .Qie_Reset(reset),
      .Cap_Id(Cap_Id[0]),
      .Adc_Mant_Data(Adc_Mant_Data[0]),
      .Adc_Exp_Data(Adc_Exp_Data[0]), 
      .Tdc_Data(Tdc_Data[0]),
      .Qie_Out(Qie_Out[0]),
      .Qie_CkOut(Qie_CkOut[0])
      );  

   FPGATop fpga_impl(
      .clk(clk), 
      .Qie1_Out(Qie_Out[0]),.Qie2_Out(Qie_Out[1]),.Qie3_Out(Qie_Out[2]),.Qie4_Out(Qie_Out[3]),
      .Qie5_Out(Qie_Out[4]),.Qie6_Out(Qie_Out[5]),.Qie7_Out(Qie_Out[6]),.Qie8_Out(Qie_Out[7]),
      .Qie9_Out(Qie_Out[8]),.Qie10_Out(Qie_Out[9]),.Qie11_Out(Qie_Out[10]),.Qie12_Out(Qie_Out[11]),
      .Qie1_CkOut(Qie_CkOut[0]),.Qie2_CkOut(Qie_CkOut[1]),.Qie3_CkOut(Qie_CkOut[2]),.Qie4_CkOut(Qie_CkOut[3]),
      .Qie5_CkOut(Qie_CkOut[4]),.Qie6_CkOut(Qie_CkOut[5]),.Qie7_CkOut(Qie_CkOut[6]),.Qie8_CkOut(Qie_CkOut[7]),
      .Qie9_CkOut(Qie_CkOut[8]),.Qie10_CkOut(Qie_CkOut[9]),.Qie11_CkOut(Qie_CkOut[10]),.Qie12_CkOut(Qie_CkOut[11]),
      .Qie1_DiscOut(Qie_DiscOut[0]),.Qie2_DiscOut(Qie_DiscOut[1]),.Qie3_DiscOut(Qie_DiscOut[2]),
      .Qie4_DiscOut(Qie_DiscOut[3]),.Qie5_DiscOut(Qie_DiscOut[4]),.Qie6_DiscOut(Qie_DiscOut[5]),
      .Qie7_DiscOut(Qie_DiscOut[6]),.Qie8_DiscOut(Qie_DiscOut[7]),.Qie9_DiscOut(Qie_DiscOut[8]),
      .Qie10_DiscOut(Qie_DiscOut[9]),.Qie11_DiscOut(Qie_DiscOut[10]),.Qie12_DiscOut(Qie_DiscOut[11])
   );

   /*assign qie_disc_out_1 = qie_disc_1;
   assign qie_disc_out_2 = qie_disc_2;
   assign qie_disc_out_3 = qie_disc_3;*/ 	

   initial begin
      clk = 0;
      reset = 0;
      Qie_Ck[0] <= 1'bX;
      Qie_DiscOut[0] <= 1'bX;
      Cap_Id[0] <= 2'bX;
      Adc_Mant_Data[0] <= 6'bX; 
      Adc_Exp_Data[0] <= 2'bX;
      Tdc_Data[0] <= 6'bX;
   end

   initial begin
      #2 clk = 1;
   end

   initial begin
      #5 Qie_Ck[0] = 1;
   end

   always begin
      #12.5 clk = ~clk;		
      #3 Qie_Ck[0] = ~Qie_Ck[0];
   end

   initial begin
      #80
      reset <= 1;		
      Qie_DiscOut[0] <= 1'b1;
      Cap_Id[0] <= 2'b01;
      Adc_Mant_Data[0] <= 6'b010100; 
      Adc_Exp_Data[0] <= 2'b01;
      Tdc_Data[0] <= 6'b000110;   
   end

   always @ (negedge Qie_Ck) begin
      if(reset) begin
	 Cap_Id[0] <= Cap_Id[0] + 1;
	 Adc_Mant_Data[0] <= Adc_Mant_Data[0] + 2; 
	 Adc_Exp_Data[0] <= Adc_Exp_Data[0] + 1;
	 Tdc_Data[0] <= Tdc_Data[0] + 1;   
      end
   end

endmodule
