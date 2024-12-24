#include <iostream>
#include <memory>
#include <queue>
#include <stdint.h>

#define NR_IRQS 2

#include "Vedf_ic.h"
#include "verilated.h"
#include "verilated_fst_c.h"
#include "utils.h"
#include "IrqDrv.h"
#include "CfgDrv.h"

int main(int argc, char** argv) {
  // init stage
  //Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  VerilatedFstC* tfp      = new VerilatedFstC;
  Vedf_ic*       top      = new Vedf_ic;
  vluint64_t     sim_time = 0;
  int            tx_count = 0;

  SimCtx cx(top, tfp, sim_time);
  IrqInTx* tx;
  CfgTx* cfg_tx;
  IrqDrv* idrv = new IrqDrv(&cx);
  CfgDrv* cdrv = new CfgDrv(&cx);
  IrqScb* scb  = new IrqScb(&cx);

  cx.dut->trace(cx.trace, 5);
  cx.trace->open("../build/waveform.fst");

  // Run stage
  reset_dut(&cx);
  step_half_cc(&cx, 17);

  cdrv->drive(new CfgTx(4, 0xDEADBEEF));
  cdrv->drive(new CfgTx(8, 0xC0FEBEBE));

  while (tx_count < 50){
    tx = rndIrqInTx();
    if (tx != NULL)
      tx_count++;
    idrv->drive(tx);
  }
  
  // end stage
  cx.trace->close();
  delete idrv;
  delete cdrv;
  delete top;
  delete scb;
  delete tfp;

  printf("########################################################\n");
  printf("TB execution complete, wave dump is ./build/waveform.fst\n");
  return 0;
}