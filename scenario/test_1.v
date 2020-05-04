//=======================================================================
// SystemC-Icarus Verilog VPI sample
//
// File:
//   test_1.v
//
// Abstract:
//   verilog simulation test bench
//
//  Created:
//    21 March 2020
//======================================================================
//
// Copyright (c) 2020, Kenji Ishimaru
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  -Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//  -Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

`timescale 1ns/1ns

module test_module;

reg clk;
reg rst_x;

reg i_valid;
reg [7:0] i_in_a;
reg [7:0] i_in_b;
wire       o_valid;
wire [7:0] o_out;

mod_a _mod_a (
  .clk(clk),
  .rst_x(rst_x),
  .i_valid(i_valid),
  .i_in_a(i_in_a),
  .i_in_b(i_in_b),
  .o_valid(o_valid),
  .o_out(o_out)
);

parameter CLK_HALF_CYCLE=5;

initial begin
  clk = 0;  // clear 0, not 1 otherwise systemc clk.pos() flipped
  forever begin
    #CLK_HALF_CYCLE clk = ~clk;
  end
end

integer i;
initial begin
  $display("Icarus Verilog started");
  $dumpvars;
  // Testbench Connection   
  $sc_stub(clk,
           rst_x,
           i_valid,
           i_in_a,
           i_in_b,
           o_valid,
           o_out);
  i_valid = 0;
  reset;
end

task reset;
  begin
    rst_x = 0;
    repeat (10) @(posedge clk);
    @(negedge clk);
    rst_x = 1;
    @(posedge clk);
  end
endtask


endmodule
