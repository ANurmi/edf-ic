struct SimCtx {
  Vedf_ic *dut;
  VerilatedFstC *trace;
  vluint64_t &sim_time;
  SimCtx(Vedf_ic *dut, VerilatedFstC *trace, vluint64_t &sim_time) :
        dut(dut),
        trace(trace),
        sim_time(sim_time){}
};

void step_half_cc( SimCtx* cx, vluint64_t count){
  for(int it = 0; it < count; it++){
    cx->dut->clk_i ^= 1;
    cx->dut->eval();
    cx->trace->dump(cx->sim_time);
    cx->sim_time++;
  }
}

void step_cc (SimCtx* cx, vluint64_t count) {
  for(int it = 0; it < count; it++)
    step_half_cc(cx, 2);
}

void reset_dut( SimCtx* cx ) {
    cx->dut->clk_i        = 0;
    cx->dut->rst_ni       = 0;
    cx->dut->irq_i        = 0;
    cx->dut->cfg_req_i    = 0;
    cx->dut->cfg_addr_i   = 0;
    cx->dut->cfg_wdata_i  = 0;
    step_half_cc(cx, 23);
    cx->dut->rst_ni       = 1;
}