class IrqTx {
    public:
        uint32_t vec;
};

IrqTx* rndIrqTx(){
    //33% chance of generating a transaction
    if(rand()%3 == 0){
        IrqTx *tx = new IrqTx();
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

        void drive(IrqTx *tx){
            // Don't drive anything if a transaction item doesn't exist
            if(tx != NULL){
                cx->dut->irq_i = tx->vec;
                printf("[TB] Driving vector %x at time %ld\n", tx->vec, cx->sim_time);
                timestep_half_clock(cx, 2);
                cx->dut->irq_i = 0;
                // Release the memory by deleting the tx item
                // after it has been consumed
                delete tx;
            }
        }
};