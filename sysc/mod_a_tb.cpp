//=======================================================================
// SystemC-Icarus Verilog VPI sample
//
// File:
//   mod_a_tb.cpp
//
// Abstract:
//   systemc mod_a instance, VPI communitcation functions
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

#include "systemc.h"
#include "mod_a.h"

#include "mod_a_tb_ports.h"
#include "mod_a_tb_exports.h"

// test_moudle instance
mod_a  u_mod_a("u_mod_a");

// signals
sc_signal<bool>      clk;
sc_signal<bool>      rst_x;
sc_signal<bool>      i_valid;
sc_signal<sc_uint<8> >    i_in_a;
sc_signal<sc_uint<8> >    i_in_b;
sc_signal<bool>      o_valid;
sc_signal<sc_uint<8> >    o_out;
sc_signal<int>      done;
  
// Top-Level testbench
void init_sc() {
  // Port mapping
  u_mod_a.clk(clk);
  u_mod_a.rst_x(rst_x);
  u_mod_a.i_valid(i_valid);
  u_mod_a.i_in_a(i_in_a);
  u_mod_a.i_in_b(i_in_b);
  u_mod_a.o_valid(o_valid);
  u_mod_a.o_out(o_out);
  u_mod_a.done(done);
  // Initialize SC
  sc_start(0,SC_NS);
  cout<<"#"<<sc_time_stamp()<<" SystemC started"<<endl;
}

void sample_hdl(void *In_vector) {
  IN_VECTOR *p = (IN_VECTOR *)In_vector;
  clk.write(p->clk);
  o_valid.write(p->o_valid);
  o_out.write(p->o_out);
}

void drive_hdl(void *Out_vector) {
  OUT_VECTOR *p = (OUT_VECTOR *)Out_vector;
  p->i_valid =  i_valid.read();
  p->i_in_a =  i_in_a.read();
  p->i_in_b =  i_in_b.read();
  p->done =  done;
}

void advance_sim(unsigned long simtime) {
  sc_start((int)simtime,SC_NS);
}

void sample_sig(void *In_vector) {
  IN_VECTOR *p = (IN_VECTOR *)In_vector;
  rst_x.write(p->rst_x);
}

void exec_sc(void *invector, void *outvector, unsigned long simtime) {
  sample_hdl(invector);
  advance_sim(simtime);  // rise and fall
  if (clk.read() == 0)
    drive_hdl(outvector);
}

void exit_sc() {
  cout<<"#"<<sc_time_stamp()<<" SystemC stopped"<<endl;
  sc_stop();
}
