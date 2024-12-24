class IrqInTx {
    public:
        uint32_t vec;
};

class IrqOutTx {
    public:
        uint32_t id;
};

IrqInTx* rndIrqInTx(){
    //33% chance of generating a transaction
    if(rand()%3 == 0){
        IrqInTx *tx = new IrqInTx();
        tx->vec = 0;

        // set random bits on all irq lines
        for (int i=0; i<NR_IRQS; i++)
            tx->vec |= (rand() % 2) << i;
        return tx;
    } else {
        return NULL;
    }
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
                printf("[TB] Driving vector %x at time %ld\n", tx->vec, cx->sim_time);
                step_half_cc(cx, 2);
                cx->dut->irq_i = 0;
                // Release the memory by deleting the tx item
                // after it has been consumed
                delete tx;
            }
        }
};

class IrqScb
{
    private:
        std::priority_queue<IrqInTx*> in_tx;
        SimCtx* cx;
    public:
        IrqScb(SimCtx* cx){
            this->cx = cx;
        }
    
        void writeIn(IrqInTx* tx){
            this->in_tx.push(tx);
        }

        void writeOut(IrqOutTx* tx){

        }

};


class IrqInMon {
    private:
        Vedf_ic *dut;
        IrqScb *scb;
    public:
        IrqInMon(Vedf_ic *dut, IrqScb *scb){
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
        IrqScb *scb;
    public:
        IrqOutMon(Vedf_ic *dut, IrqScb *scb){
            this->dut = dut;
            this->scb = scb;
        }

        void monitor(){
            IrqOutTx *tx = new IrqOutTx();
            tx->id = dut->irq_id_o;
            scb->writeOut(tx);
        }
};