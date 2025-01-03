#include <iostream>
#include <memory>
#include <queue>
#include <stdint.h>

#define NR_IRQS    4
#define PRESCALE   2
#define CLOCK_STEP 4

#include "Vedf_ic.h"
#include "verilated.h"
#include "verilated_fst_c.h"
#include "SimCtx.h"
#include "ClkRstDrv.h"
#include "EdfScb.h"
#include "CfgDrv.h"
#include "IrqDrv.h"

int main(int argc, char** argv) {
  // init stage
  //Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  VerilatedFstC*   tfp          = new VerilatedFstC;
  Vedf_ic*         top          = new Vedf_ic;
  vluint64_t       sim_time     = 0;
  vluint64_t       rst_delay    = 80;
  vluint64_t       clk_delay    = 20;
  bool             cfg_done     = 0;
  bool             sw_pend_done = 0;
  bool             sim_done     = 0;
  int              tx_count     = 0;
  int              cfg_instr    = 0;

  SimCtx cx(top, tfp, sim_time);
  ClkRstDrv* rdrv    = new ClkRstDrv(&cx);
  IrqInDrv*  idrv    = new IrqInDrv(&cx);
  IrqOutDrv* odrv    = new IrqOutDrv(&cx);
  CfgDrv*    cdrv    = new CfgDrv(&cx);
  EdfScb*    scb     = new EdfScb(&cx);
  CfgMon*    cfg_mon = new CfgMon(top, scb);
  IrqInMon*  in_mon  = new IrqInMon(top, scb);
  IrqOutMon* out_mon = new IrqOutMon(top, scb);

  cx.dut->trace(cx.trace, 5);
  cx.trace->open("../build/waveform.fst");

  while (!sim_done){
    bool after_re = (cx.dut->clk_i  && 
                     cx.dut->rst_ni &&
                    (sim_time % (CLOCK_STEP*2) == 0));
    rdrv->reset(rst_delay);
    rdrv->clock(clk_delay, CLOCK_STEP);
    rdrv->mtime(PRESCALE, CLOCK_STEP);

    // drive inputs just after rising edge
    if (after_re) {
      if (cfg_instr != NR_IRQS) {
        switch (cfg_instr){
          case 0:
            cdrv->drive(new CfgTx(0, 0x12));
            break;
          case 1:
            cdrv->drive(new CfgTx(4, 0x6));
            break;
          case 2:
            cdrv->drive(new CfgTx(8, 0x22));
            break;
          case 3:
            cdrv->drive(new CfgTx(12, 0x15));
            break;
          default:
            break;
        }
        cfg_instr++;
      } else if (!sw_pend_done) {
        cdrv->drive(new CfgTx(4, 0x01000006));        
        sw_pend_done = 1;
      } else {
        cdrv->clear();

        if (tx_count < 10){ // main tb loop
          IrqInTx* tx = rndIrqInTx();

          if (tx != NULL) {
            tx_count++;
          } else {
            tx = new IrqInTx();
            tx->vec = 0;
          }
  
          idrv->drive(tx);
        } else
          sim_done = 1;
      }
  
      odrv->drive();

      cfg_mon->monitor();
      in_mon->monitor();
      out_mon->monitor();
    }
    timestep(&cx);
  }

  // end stage
  scb->print_queue();

  cx.trace->close();
  delete rdrv;
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