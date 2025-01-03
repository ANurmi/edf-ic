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
  VerilatedFstC*   tfp       = new VerilatedFstC;
  Vedf_ic*         top       = new Vedf_ic;
  vluint64_t       sim_time  = 0;
  vluint64_t       rst_delay = 80;
  vluint64_t       clk_delay = 20;
  vluint64_t       mtimer    = 0;
  uint8_t          mtime_pre = 0;
  bool             cfg_done  = 0;
  bool             sim_done  = 0;
  int              tx_count  = 0;
  int              cfg_instr = 0;

  SimCtx cx(top, tfp, sim_time);
  //IrqInTx* tx;
  //CfgTx* cfg_tx;
  ClkRstDrv* rdrv    = new ClkRstDrv(&cx, 
                                    rst_delay, 
                                    clk_delay, 
                                    CLOCK_STEP);
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
    // drive inputs just after rising edge
    bool after_re = (cx.dut->clk_i  && 
                     cx.dut->rst_ni &&
                    (sim_time % (CLOCK_STEP*2) == 0));
    rdrv->reset();
    rdrv->clock();
    //idrv->drive(tx);
    odrv->drive();

    if (after_re) {
      if (cfg_instr != NR_IRQS) {

        switch (cfg_instr)
        {
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
      } else {
        cx.dut->cfg_req_i   = 0;
        cx.dut->cfg_addr_i  = 0;
        cx.dut->cfg_wdata_i = 0;
      }
    }

    cfg_mon->monitor();
    in_mon->monitor();
    out_mon->monitor();

    timestep(&cx);

    if (sim_time > 300){
      sim_done = 1;
    }
  }
  // Run stage
  //reset_dut(&cx);
  //step_half_cc(&cx, 17);

  // Clock off, reset active
  //timestep(&cx, 40);

  // Clock on, reset active
  //clock(&cx, 40);
  //top->rst_ni = 1;
  //clock(&cx, 40);


/*
  for (int i=0; i<40; i++)
  {
      if (sim_time % CLOCK_STEP == 0){
        clock = !clock;
        printf("%d\n", (sim_time % CLOCK_STEP == 0));
        top->clk_i = clock;
      }
      timestep(&cx,1 );

  }
  while (cfg_instr < NR_IRQS) {
      CfgTx* cfg = new CfgTx(0, 0);

      switch (cfg_instr)
      {
        case 0:
          cfg->addr  = 0;
          cfg->wdata = 0x12;
          break;
        case 1:
          cfg->addr  = 4;
          cfg->wdata = 0x6;
          break;
        case 2:
          cfg->addr  = 8;
          cfg->wdata = 0x22;
          break;

        case 3:
          cfg->addr  = 12;
          cfg->wdata = 0x15;
          break;


        default:
          break;
      }

      cdrv->drive(cfg);
      cfg_mon->monitor();
      //step_half_cc(&cx, 1);
      cfg_instr++;

      if (sim_time % 4 == 0){
        //clock = !clock;
        printf("%d\n", (sim_time % 4 == 0));
        //top->clk_i = clock;

        cfg_instr++;
      }
  }
  cx.dut->cfg_req_i   = 0;
  cx.dut->cfg_addr_i  = 0;
  cx.dut->cfg_wdata_i = 0;

  while (tx_count < 10){ // main tb loop

    tx = rndIrqInTx();

    if (tx != NULL) {
      tx_count++;
    } else {
      tx = new IrqInTx();
      tx->vec = 0;
    }

    idrv->drive(tx);
    odrv->drive();
    in_mon->monitor();
    out_mon->monitor();

    //step_half_cc(&cx, 1);

    if (mtime_pre == PRESCALE-1) {
      mtime_pre = 0;
      mtimer++;
    } else
      mtime_pre++;
    cx.dut->mtime_i = mtimer;
    timestep(&cx, 1);

  }
  */

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