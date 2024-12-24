class CfgTx {
    public:
        uint32_t addr;
        uint32_t wdata;

        CfgTx(uint32_t addr, uint32_t wdata){
            this->addr  = addr;
            this->wdata = wdata;
        }
};

class CfgDrv {
    private:
        SimCtx* cx;
    public:
        CfgDrv(SimCtx* cx){
            this->cx = cx;
        }

        void drive(CfgTx *tx){
            cx->dut->cfg_req_i   = 1;
            cx->dut->cfg_addr_i  = tx->addr;
            cx->dut->cfg_wdata_i = tx->wdata;
            printf("[TB] Writing %x to reg addr %x at time %ld\n", 
                        tx->wdata, tx->addr, cx->sim_time);
            step_half_cc(cx, 2);
            cx->dut->cfg_req_i   = 0;
            cx->dut->cfg_addr_i  = 0;
            cx->dut->cfg_wdata_i = 0;
            delete tx;
        }
};