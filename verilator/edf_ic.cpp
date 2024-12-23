#include <iostream>
#include <memory>
#include <stdint.h>

#define NR_IRQS 2

#include "Vedf_ic.h"
#include "verilated.h"
#include "verilated_fst_c.h"
#include "utils.h"
#include "vip_edf_ic.h"

int main(int argc, char** argv) {

  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  VerilatedFstC* tfp      = new VerilatedFstC;
  Vedf_ic*       top      = new Vedf_ic;
  vluint64_t     sim_time = 0;
  int            tx_count = 0;

  SimCtx cx(top, tfp, sim_time);

  IrqTx* tx;
  IrqDrv* drv = new IrqDrv(&cx);

  cx.dut->trace(cx.trace, 5);
  cx.trace->open("../build/waveform.fst");

  reset_dut(&cx);
  timestep_half_clock(&cx, 30);
  while (tx_count < 50){

    tx = rndIrqTx();
    drv->drive(tx);

    timestep_half_clock(&cx, 30);

    tx_count++;
    //tx = 
  }
  cx.trace->close();

  delete drv;
  delete top;
  delete tfp;

  printf("########################################################\n");
  printf("TB execution complete, wave file is ./build/waveform.fst\n");
  printf("########################################################\n");
  return 0;
}