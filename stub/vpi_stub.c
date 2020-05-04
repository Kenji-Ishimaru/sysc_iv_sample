//=======================================================================
// SystemC-Icarus Verilog VPI sample
//
// File:
//   vpi_stub.c
//
// Abstract:
//   Verilog VPI stub file
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

#include <stdio.h>
#include <stdlib.h>
#include <vpi_user.h>
#include <veriuser.h>

#include "mod_a_tb_ports.h"
#include "mod_a_tb_exports.h"

#define CLK_HALF_CYCLE 5

// RTL-SystemC communitation data
typedef struct mod_a {
  vpiHandle  clk;
  vpiHandle  rst_x;
  vpiHandle  i_valid;
  vpiHandle  i_in_a;
  vpiHandle  i_in_b;
  vpiHandle  o_valid;
  vpiHandle  o_out;
} t_if;

int sc_mod_a_tf(char *user_data);
int sc_clk_callback(p_cb_data cb_data);
int sc_async_callback(p_cb_data cb_data);

static void my_task(void);

int sc_mod_a_tf(char *user_data) {
  vpiHandle  inst_h, args;
  s_vpi_value  value_s;
  s_vpi_time  time_s;
  s_cb_data  cb_data_s;
  s_cb_data  cb_data_as;
  t_if *ip;

  ip = (t_if *)malloc(sizeof(t_if));
  
  // get arguments from RTL 
  inst_h = vpi_handle(vpiSysTfCall, 0);
  args = vpi_iterate(vpiArgument, inst_h);
  // set arguments
  ip->clk     = vpi_scan(args); 
  ip->rst_x   = vpi_scan(args); 
  ip->i_valid   = vpi_scan(args); 
  ip->i_in_a    = vpi_scan(args); 
  ip->i_in_b    = vpi_scan(args); 
  ip->o_valid    = vpi_scan(args); 
  ip->o_out    = vpi_scan(args); 

  vpi_free_object(args);
  
  // setup callback (clk sync)
  cb_data_s.user_data = (char *)ip;
  cb_data_s.reason    = cbValueChange;
  cb_data_s.cb_rtn    = sc_clk_callback; // callback
  cb_data_s.time      = &time_s;
  cb_data_s.value     = &value_s;

  time_s.type         = vpiSimTime;
  value_s.format      = vpiIntVal;

  cb_data_s.obj  = ip->clk;
  vpi_register_cb(&cb_data_s);

  // setup callback (clk async)
  cb_data_s.user_data = (char *)ip;
  cb_data_s.reason    = cbValueChange;
  cb_data_s.cb_rtn    = sc_async_callback; // callback
  cb_data_s.time      = &time_s;
  cb_data_s.value     = &value_s;

  time_s.type         = vpiSimTime;
  value_s.format      = vpiIntVal;

  cb_data_s.obj  = ip->rst_x;
  vpi_register_cb(&cb_data_s);

  init_sc();  // Initialize SystemC

  return(0);
}

// callback at Value change
int sc_clk_callback(p_cb_data cb_data)
{
  t_if  *ip;
  s_vpi_value  value_s;
  
  //static unsigned long SimNow = 0;

  // IO ports systemC testbench
  static IN_VECTOR   invector;
  static OUT_VECTOR  outvector;
  
  ip = (t_if *)cb_data->user_data;
  
  // Read current RTL value
  value_s.format = vpiIntVal;
  vpi_get_value(ip->clk, &value_s);
  invector.clk = value_s.value.integer;
  vpi_get_value(ip->o_valid, &value_s);
  invector.o_valid = value_s.value.integer;
  vpi_get_value(ip->o_out, &value_s);
  invector.o_out = value_s.value.integer;

  // SystemC Execution
  exec_sc(&invector, &outvector, (CLK_HALF_CYCLE));

  // Write current RTL value
  value_s.value.integer = outvector.i_valid;
  vpi_put_value(ip->i_valid, &value_s, 0, vpiNoDelay);
  value_s.value.integer = outvector.i_in_a;
  vpi_put_value(ip->i_in_a, &value_s, 0, vpiNoDelay);
  value_s.value.integer = outvector.i_in_b;
  vpi_put_value(ip->i_in_b, &value_s, 0, vpiNoDelay);

  if (outvector.done) {
    exit_sc();
    tf_dofinish();
  }

  return(0);
}

int sc_async_callback(p_cb_data cb_data)
{
  t_if  *ip;
  s_vpi_value  value_s;
  
  //static unsigned long SimNow = 0;

  // IO ports systemC testbench
  static IN_VECTOR   invector;
  
  ip = (t_if *)cb_data->user_data;
  
  // Read current RTL value
  value_s.format = vpiIntVal;
  vpi_get_value(ip->rst_x, &value_s);
  invector.rst_x = value_s.value.integer;

  // SystemC Execution
  sample_sig(&invector);

  return(0);
}

// my task
static void my_task()
{
      s_vpi_systf_data tf_data;

      tf_data.type      = vpiSysTask;
      tf_data.tfname    = "$sc_stub";
      tf_data.calltf    = sc_mod_a_tf;
      tf_data.compiletf = 0;
      tf_data.sizetf    = 0;
      vpi_register_systf(&tf_data);
}

// register my task
void (*vlog_startup_routines[])() = {
      my_task,
      0
};
