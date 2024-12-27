#include <iostream>
#include <memory>
#include <queue>
#include <stdint.h>

#define NR_IRQS 2

#include "Vedf_ic.h"
#include "verilated.h"
#include "verilated_fst_c.h"
#include "SimCtx.h"
#include "EdfScb.h"
#include "CfgDrv.h"
#include "IrqDrv.h"

int main(int argc, char** argv) {
  // init stage
  //Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  VerilatedFstC* tfp      = new VerilatedFstC;
  Vedf_ic*       top      = new Vedf_ic;
  vluint64_t     sim_time = 0;
  int            tx_count = 0;
  bool           first_it = true;

  SimCtx cx(top, tfp, sim_time);
  IrqInTx* tx;
  CfgTx* cfg_tx;
  IrqDrv* idrv       = new IrqDrv(&cx);
  CfgDrv* cdrv       = new CfgDrv(&cx);
  EdfScb* scb        = new EdfScb(&cx);
  IrqInMon* in_mon   = new IrqInMon(top, scb);
  IrqOutMon* out_mon = new IrqOutMon(top, scb);

  cx.dut->trace(cx.trace, 5);
  cx.trace->open("../build/waveform.fst");

  // Run stage
  reset_dut(&cx);
  step_half_cc(&cx, 17);

  while (tx_count < 10){ // main tb loop

    if (first_it) {
      cdrv->drive(new CfgTx(4, 0xDEADBEEF));
      cdrv->drive(new CfgTx(8, 0xC0FEBEBE));
      first_it = false;
    }
    tx = rndIrqInTx();

    idrv->drive(tx);
    in_mon->monitor();
    out_mon->monitor();
    scb->count();

    scb->head();
    //tx_count++;
    //printf("[TB] tx_count %d\n", tx_count);
    tx_count++;

    step_half_cc(&cx, 2);
  }
  
  // end stage
  cx.trace->close();
  delete idrv;
  delete cdrv;
  delete top;
  delete scb;
  delete in_mon;
  delete out_mon;
  delete tfp;

  printf("########################################################\n");
  printf("TB execution complete, wave dump is ./build/waveform.fst\n");
  return 0;
}