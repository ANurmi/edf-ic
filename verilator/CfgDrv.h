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
            printf("[CfgDrv] Writing %08x to reg addr %08x at time %ld\n", 
                        tx->wdata, tx->addr, cx->sim_time);
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
            scb->writeCfg(tx);
        }
};
