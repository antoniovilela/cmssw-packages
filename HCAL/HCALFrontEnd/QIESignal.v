`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:02:02 11/06/2012 
// Design Name: 
// Module Name:    QIESignal 
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
module QIESignal(
   input Qie_Ck, Qie_Reset,
   input [1:0] Cap_Id,
   input [5:0] Adc_Mant_Data,
   input [1:0] Adc_Exp_Data, 
   input [5:0] Tdc_Data,
   output [7:0] Qie_Out,
   output Qie_CkOut);

   //reg int_clk;
   reg read;
   wire [15:0] data;

   /*initial begin
      int_clk = 0;
   end

   always @ (posedge Qie_Ck,negedge Qie_Ck) begin
      int_clk = ~int_clk;
      #6.25 int_clk = ~int_clk;
   end*/

   initial begin
      read = 0;
   end
   
   always @ (Qie_Ck) begin
      #2 read = ~Qie_Ck;
   end

   assign data = { Cap_Id, {Adc_Mant_Data, {Adc_Exp_Data, Tdc_Data} } };

   serial_shift_reg serial_data_0(
	 .clk(Qie_Ck),
	 .reset(read),
	 .input_data(data[1:0]),
	 .serial_out(Qie_Out[0])
	 );

   serial_shift_reg serial_data_1(
	 .clk(Qie_Ck),
	 .reset(read),
	 .input_data(data[3:2]),
	 .serial_out(Qie_Out[1])
	 );

   serial_shift_reg serial_data_2(
	 .clk(Qie_Ck),
	 .reset(read),
	 .input_data(data[5:4]),
	 .serial_out(Qie_Out[2])
	 );

   serial_shift_reg serial_data_3(
	 .clk(Qie_Ck),
	 .reset(read),
	 .input_data(data[7:6]),
	 .serial_out(Qie_Out[3])
	 );

   serial_shift_reg serial_data_4(
	 .clk(Qie_Ck),
	 .reset(read),
	 .input_data(data[9:8]),
	 .serial_out(Qie_Out[4])
	 );

   serial_shift_reg serial_data_5(
	 .clk(Qie_Ck),
	 .reset(read),
	 .input_data(data[11:10]),
	 .serial_out(Qie_Out[5])
	 );

   serial_shift_reg serial_data_6(
	 .clk(Qie_Ck),
	 .reset(read),
	 .input_data(data[13:12]),
	 .serial_out(Qie_Out[6])
	 );

   serial_shift_reg serial_data_7(
	 .clk(Qie_Ck),
	 .reset(read),
	 .input_data(data[15:14]),
	 .serial_out(Qie_Out[7])
	 );

   assign Qie_CkOut = Qie_Ck;

endmodule
