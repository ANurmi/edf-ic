IrqInTx* rndIrqInTx(){
    if (rand() % 10 == 0){
        
        IrqInTx *tx = new IrqInTx();
        tx->vec = 0;

        for (int i=0; i<NR_IRQS; i++)
            tx->vec |= (rand() % 2) << i;
    
        return tx;
    } else
        return NULL;
}


class IrqDrv {
    private:
        SimCtx* cx;
    public:
        IrqDrv(SimCtx* cx){
            this->cx = cx;
        }

        void drive(IrqInTx *tx){
            // Don't drive anything if a transaction item doesn't exist
            if(tx != NULL){
                cx->dut->irq_i = tx->vec;
                if (tx->vec != 0)
                    printf("[IrqDrv] Driving vector %02x at time %ld\n", tx->vec, cx->sim_time);
                delete tx;
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
            tx->id = dut->irq_id_o;
            scb->writeOut(tx);
        }
};