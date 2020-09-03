# SystemC - Icarus Verilog VPI sample

Co-simulation exsample of SystemC and Icarus Verilog.

SystemC testbench provides stimulus to Verilog DUT, and captures responses from the Verilog DUT. 

## Prerequisites

* Centos7

* Icarus Verilog version 11.0

* SystemC version 2.3.3

## Setup
Configure the SYSTEMC_DIR in bin/run_iv.sh to fit to your environment.
The default is "/usr/local/systemc-2.3.3".

```
SYSTEMC_DIR=/usr/local/systemc-2.3.3
```

## Running the simulation

```
git https://github.com/Kenji-Ishimaru/sysc_iv_sample.git

cd sysc-iv-sample/work
../bin/run_iv.sh ../scenario/test_1.v
```

## Result
### Log

```
Making vpi_stub.vpi from  vpi_stub.o mod_a.cpp.o mod_a_tb.cpp.o...
Icarus Verilog started
VCD info: dumpfile dump.vcd opened for output.
#0 s SystemC started
#100 ns rst_x is deasserted 
Mon Input: #120 ns i_valid = 1 i_in_a = 069 i_in_b = 073
Mon Output: #120 ns o_valid = 1 o_out = 02d
Mon Input: #130 ns i_valid = 1 i_in_a = 051 i_in_b = 0ff
Mon Output: #130 ns o_valid = 1 o_out = 0dc
Mon Input: #140 ns i_valid = 1 i_in_a = 04a i_in_b = 0ec
Mon Output: #140 ns o_valid = 1 o_out = 050
Mon Input: #150 ns i_valid = 1 i_in_a = 029 i_in_b = 0cd
Mon Output: #150 ns o_valid = 1 o_out = 036
Mon Input: #160 ns i_valid = 1 i_in_a = 0ba i_in_b = 0ab
Mon Output: #160 ns o_valid = 1 o_out = 0f6
Mon Input: #170 ns i_valid = 1 i_in_a = 0f2 i_in_b = 0fb
Mon Output: #170 ns o_valid = 1 o_out = 065
Mon Input: #180 ns i_valid = 1 i_in_a = 0e3 i_in_b = 046
Mon Output: #180 ns o_valid = 1 o_out = 0ed
Mon Input: #190 ns i_valid = 1 i_in_a = 07c i_in_b = 0c2
Mon Output: #190 ns o_valid = 1 o_out = 029
Mon Input: #200 ns i_valid = 1 i_in_a = 054 i_in_b = 0f8
Mon Output: #200 ns o_valid = 1 o_out = 03e
Mon Input: #210 ns i_valid = 0 i_in_a = 054 i_in_b = 0f8
Mon Output: #210 ns o_valid = 1 o_out = 04c
=======================================
 The simulation is successfully done.
=======================================
#310 ns SystemC stopped

Info: /OSCI/SystemC: Simulation stopped by user.

```

### Waveform

![waveform](/images/result.jpg)

## License

This project is licensed under the MIT License - see the LICENSE.md file for details
