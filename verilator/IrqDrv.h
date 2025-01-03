IrqInTx* rndIrqInTx(){
    if (rand() % 20 == 0){
        
        IrqInTx *tx = new IrqInTx();
        tx->vec = 0;

        for (int i=0; i<NR_IRQS; i++)
            tx->vec |= (rand() % 2) << i;
    
        return tx;
    } else
        return NULL;
}


class IrqInDrv {
    private:
        SimCtx* cx;
    public:
        IrqInDrv(SimCtx* cx){
            this->cx = cx;
        }

        void drive(IrqInTx *tx){

            // Don't drive anything if a transaction item doesn't exist
            if(tx != NULL){
                    cx->dut->irq_i = tx->vec;
                    if (tx->vec != 0) {
                        printf("[IrqInDrv] Driving vector %02x at time %ld\n", tx->vec, cx->sim_time);
                    }
                    delete tx;
            }
        }
};

class IrqOutDrv {
    private:
        SimCtx* cx;
        uint8_t delay;
        bool    prev_valid;
    public:
        IrqOutDrv(SimCtx* cx){
            this->cx         = cx;
            this->delay      = 0;
            this->prev_valid = 0;
        }

        // drive irq_ready_i with randomized latency after valid_o
        void drive(){
            cx->dut->irq_ready_i = 0;
            if (cx->dut->irq_valid_o) {
                if (prev_valid) {
                    prev_valid = 0;
                    delay += rand() % 200;
                    if (delay >= 200) {
                        delay = 0;
                        cx->dut->irq_ready_i = 1;
                    }
                }
                else
                    prev_valid = 1;
            }
        }
};

class IrqInMon {
    private:
        Vedf_ic *dut;
        EdfScb *scb;
    public:
        IrqInMon(Vedf_ic *dut, EdfScb *scb){
            this->dut = dut;
            this->scb = scb;
        }

        void monitor(){
            IrqInTx *tx = new IrqInTx();
            tx->vec = dut->irq_i;
            scb->writeIn(tx);
        }
};

class IrqOutMon {
    private:
        Vedf_ic *dut;
        EdfScb *scb;
    public:
        IrqOutMon(Vedf_ic *dut, EdfScb *scb){
            this->dut = dut;
            this->scb = scb;
        }

        void monitor(){
            IrqOutTx *tx = new IrqOutTx();
            if (dut->irq_valid_o)
                tx->id = dut->irq_id_o;
            if (dut->irq_ready_i)
                scb->writeOut(tx);
        }
};