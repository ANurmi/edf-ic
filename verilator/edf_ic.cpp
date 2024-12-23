#include <iostream>
#include <memory>

#include "Vedf_ic.h"
#include "verilated.h"

vluint64_t main_time = 0;
double sc_time_stamp() { return main_time; }

int main(int argc, char** argv) {

  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  
  const std::unique_ptr<Vedf_ic> top{new Vedf_ic};
/*
  while ( !Verilated::gotFinish() ) { 
    if(main_time != -1) {
      top->eval();    
    }
    main_time = top->nextTimeSlot();
  }

  top->final();
*/
  printf("TEST\n");

  return 0;
}