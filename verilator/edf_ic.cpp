#include <iostream>
#include <memory>
#include <queue>
#include <stdint.h>

#define NR_IRQS  4
#define PRESCALE 8

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
  VerilatedFstC* tfp       = new VerilatedFstC;
  Vedf_ic*       top       = new Vedf_ic;
  vluint64_t     sim_time  = 0;
  vluint64_t     mtimer    = 0;
  uint8_t        mtime_pre = 0;
  int            tx_count  = 0;
  int            cfg_instr = 0;

  SimCtx cx(top, tfp, sim_time);
  IrqInTx* tx;
  CfgTx* cfg_tx;
  IrqDrv* idrv       = new IrqDrv(&cx);
  CfgDrv* cdrv       = new CfgDrv(&cx);
  EdfScb* scb        = new EdfScb(&cx);
  CfgMon* cfg_mon    = new CfgMon(top, scb);
  IrqInMon* in_mon   = new IrqInMon(top, scb);
  IrqOutMon* out_mon = new IrqOutMon(top, scb);

  cx.dut->trace(cx.trace, 5);
  cx.trace->open("../build/waveform.fst");

  // Run stage
  reset_dut(&cx);
  step_half_cc(&cx, 17);

  while (cfg_instr < NR_IRQS) {
      CfgTx* cfg = new CfgTx(0, 0);

      switch (cfg_instr)
      {
        case 0:
          cfg->addr  = 0;
          cfg->wdata = 0xDEADBEEF;
          break;
        case 1:
          cfg->addr  = 4;
          cfg->wdata = 0x1000;
          break;
        case 2:
          cfg->addr  = 8;
          cfg->wdata = 0x2000;
          break;

        case 3:
          cfg->addr  = 12;
          cfg->wdata = 0x0100;
          break;


        default:
          break;
      }

      cdrv->drive(cfg);
      cfg_mon->monitor();
      step_cc(&cx, 1);
      cfg_instr++;
  }

  while (tx_count < 10){ // main tb loop

    tx = rndIrqInTx();

    if (tx != NULL) {
      tx_count++;
    } else {
      tx = new IrqInTx();
      tx->vec = 0;
    }

    idrv->drive(tx);
    in_mon->monitor();
    out_mon->monitor();

    step_half_cc(&cx, 2);

    if (mtime_pre == PRESCALE-1) {
      mtime_pre = 0;
      mtimer++;
    } else
      mtime_pre++;
    cx.dut->mtime_i = mtimer;
  }
  
  // end stage
  scb->print_queue();

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