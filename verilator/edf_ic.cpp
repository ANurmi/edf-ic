#include <iostream>
#include <memory>

#include "Vedf_ic.h"
#include "verilated.h"
#include "verilated_fst_c.h"
#include "utils.h"

int main(int argc, char** argv) {

  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  VerilatedFstC* tfp      = new VerilatedFstC;
  Vedf_ic*       top      = new Vedf_ic;
  vluint64_t     sim_time = 0;

  SimCtx cx(top, tfp, sim_time);

  cx.dut->trace(tfp, 5);
  cx.trace->open("../build/waveform.fst");

  reset_dut(cx);
  timestep_half_clock(cx, 30);

  cx.trace->close();

  delete top;

  printf("########################################################\n");
  printf("TB execution complete, wave file is ./build/waveform.fst\n");
  printf("########################################################\n");
  return 0;
}