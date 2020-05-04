//=======================================================================
// SystemC-Icarus Verilog VPI sample
//
// File:
//   mod_a.h
//
// Abstract:
//   mod_a header file
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

#ifndef MOD_A_H
#define MOD_A_H

#include "systemc.h"

SC_MODULE (mod_a) {
  sc_in<bool>          clk;
  sc_in<bool>          rst_x; 
  sc_out<bool>         i_valid; 
  sc_out<sc_uint<8> >  i_in_a;
  sc_out<sc_uint<8> >  i_in_b;
  sc_in<bool>          o_valid; 
  sc_in<sc_uint<8> >   o_out;
  sc_out<int>          done;  // done signal       

  sc_fifo<sc_uint<8> > ref_fifo;  // reference fifo

  void reference();
  void monitor();
  void done_signal();
  void stimulus();

  sc_uint<8>  o_out_ref;
  bool        error;
  bool        sim_done;

  SC_CTOR(mod_a) {
    error = false;
    sim_done = false;
    SC_THREAD(monitor);
    sensitive << clk.pos();
    SC_THREAD(reference);
    sensitive << clk.pos();
    SC_THREAD(done_signal);
    sensitive << clk.pos();
    SC_CTHREAD(stimulus,clk.pos());
  }
};

#endif
