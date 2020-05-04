//=======================================================================
// SystemC-Icarus Verilog VPI sample
//
// File:
//   mod_a.cpp
//
// Abstract:
//   mod_a implementation
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
#include "mod_a.h"

// Reference model
void mod_a::reference() {
  while (true) {
    if (i_valid.read()==1) {
      ref_fifo.write(i_in_a.read() + i_in_b.read());
    }
    wait();
  }
}

// Error checker
void mod_a::monitor() {
  while (true) {
    // Input side
    if (o_valid.read() == 1) {
      std::cout << "Mon Input: #" <<sc_time_stamp() << " ";
      std::cout << "i_valid = "  << std::hex << i_valid.read() << " ";
      std::cout << "i_in_a = "  << std::hex << i_in_a.read() << " ";
      std::cout << "i_in_b = "  << std::hex << i_in_b.read() << std::endl;
    }
    // Output side
    if (o_valid.read() == 1) {
      std::cout << "Mon Output: #" <<sc_time_stamp() << " ";
      std::cout << "o_valid = "  << std::hex << o_valid.read() << " ";
      std::cout << "o_out = "  << std::hex << o_out.read() << std::endl;
       // data check
       o_out_ref = ref_fifo.read();
       if (o_out_ref != o_out.read()) {
          std::cout << "!!!!!! Data mismatch " <<sc_time_stamp() << " ";
          std::cout << "o_out = "  << std::hex << o_out.read() << " ";
          std::cout << "ref = "  << std::hex << o_out_ref << std::endl;
          error = true;
          wait();
       }
    }
    wait();
  }
}

// Simulation done signal,
// due to sc_signal can not be driven by multiple functions
void mod_a::done_signal() {
  while (true) {
    if (error || sim_done) done = 1;
    else done = 0;
    wait();
  }
}

// Test Stimulus 
void mod_a::stimulus() {
  while (true) {
    while (rst_x.read() == 0)
       wait();
    cout<<"#"<<sc_time_stamp()<<" rst_x is deasserted " << endl;
    for (int i = 0; i < 10; i++) {
      i_valid.write(1);
      i_in_a.write(rand());
      i_in_b.write(rand());
      wait();
    }
    i_valid.write(0);
    wait(10);
    sim_done = true;
    std::cout << "=======================================" << std::endl;
    std::cout << " The simulation is successfully done." << std::endl;
    std::cout << "=======================================" << std::endl;
    wait();
  }
}
