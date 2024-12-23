#include <iostream>
#include <memory>

#include "Vedf_ic.h"
#include "verilated.h"
#include "verilated_fst_c.h"


vluint64_t main_time = 0;

int main(int argc, char** argv) {

  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  VerilatedFstC* tfp = new VerilatedFstC;
  
  const std::unique_ptr<Vedf_ic> top{new Vedf_ic};

  top->trace(tfp, 1);
  tfp->open("../build/waveform.fst");

  while ( main_time < 100 ) { 

    main_time++;
  }


  tfp->close();

  //top->final();

  return 0;
}