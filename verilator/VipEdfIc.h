// Clock period in picoseconds, used for generating waveforms
const uint32_t CLOCK_PERIOD_PS = /* 100 MHz */ 10000;

class VipEdfIc {
  public:
    Vedf_ic       *m_dut;
    VerilatedFstC* m_trace;
    uint64_t       m_tickcount;

    VipEdfIc(Vedf_ic* dut, const char* fst_name) : m_trace(NULL), m_tickcount(01) {
      m_dut = dut;
      Verilated::traceEverOn(true);
      m_dut->clk_i = 0;
      m_dut->eval();
      open_trace(fst_name);
    }

    ~VipEdfIc(void){
      close_trace();
      m_dut = NULL;
    }

    void raise_reset(void)

    void test(void){ 
      tick(); 
      tick();
      tick();
      tick();
      tick();
    }

  private:
    void open_trace(const char* fst_name) {
      if (!m_trace) {
        m_trace = new VerilatedFstC;
        m_dut->trace(m_trace, 99);
        m_trace->open(fst_name);
      }
    }

    void close_trace(void) {
      if (m_trace) {
        m_trace->close();
        delete m_trace;
        m_trace = NULL;
      }
    }

    void tick(void) {
      m_tickcount++;
      m_dut->eval();
      if (m_trace) m_trace->dump((vluint64_t)(CLOCK_PERIOD_PS*m_tickcount-CLOCK_PERIOD_PS/5));
      m_dut->clk_i = 1;
      m_dut->eval();
      if (m_trace) m_trace->dump((vluint64_t)(CLOCK_PERIOD_PS*m_tickcount));
      m_dut->clk_i = 0;
      m_dut->eval();
      if (m_trace) {
        m_trace->dump((vluint64_t)(CLOCK_PERIOD_PS*m_tickcount+CLOCK_PERIOD_PS/2));
        m_trace->flush();
      }
    }

};
