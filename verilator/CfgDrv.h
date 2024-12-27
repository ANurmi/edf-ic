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

class CfgMon {
    private:
        Vedf_ic *dut;
        EdfScb *scb;
    public:
        CfgMon(Vedf_ic *dut, EdfScb *scb){
            this->dut = dut;
            this->scb = scb;
        }

        void monitor(){
            CfgTx *tx = new CfgTx(dut->cfg_addr_i, dut->cfg_wdata_i);
            printf("[CfgMon] addr: %x\n", tx->addr);
            printf("[CfgMon] wdata: %x\n", tx->wdata);
            scb->writeCfg(tx);
        }
};
