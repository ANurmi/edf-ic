class ClkRstDrv {
    private:
        SimCtx* cx;
        vluint64_t rst_delay;
        vluint64_t rst_counter;
        vluint64_t clk_delay;
        vluint64_t clk_counter;
        uint32_t   clk_per;

    public:
        ClkRstDrv(
            SimCtx*    cx,
            vluint64_t rst_delay,
            vluint64_t clk_delay,
            uint32_t clk_per 
        ){
            this->cx = cx;
            this->rst_delay   = rst_delay;            
            this->clk_delay   = clk_delay;
            this->clk_per     = clk_per;
            this->rst_counter = 0;
            this->clk_counter = 0;
        }

        void reset(){
            if(rst_counter == rst_delay)
                cx->dut->rst_ni = 1;
            else
                rst_counter++;
        }

        void clock() { // todo: add period
            if (clk_counter == clk_delay)
                cx->dut->clk_i ^= 1;
            else
                clk_counter++;
        }
};