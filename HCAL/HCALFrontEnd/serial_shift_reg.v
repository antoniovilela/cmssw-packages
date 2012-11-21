`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:11:49 11/06/2012 
// Design Name: 
// Module Name:    serial_shift_reg 
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
module serial_shift_reg
   #(parameter width = 2)
   (input clk, input reset,
    input [width-1:0] input_data, 
    output serial_out);

   reg [width-1:0] data;
   reg out;
   //reg [3:0] count;
   //reg finish;

   //assign serial_out = data[0];
   assign serial_out = out;

   always @ (posedge clk, negedge clk) begin
      out  <= data[0];
      data <= {data[0],data[width-1:1]};
   end

   always @ (posedge reset) begin
      if(reset) data <= input_data;
   end

endmodule
