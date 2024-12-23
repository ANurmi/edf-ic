class IrqTx {
    public:
        uint32_t vec;
};

IrqTx* rndIrqTx(){
    //20% chance of generating a transaction
    if(rand()%5 == 0){
        IrqTx *tx = new IrqTx();
        tx->vec = 0;

        // set random bits on all irq lines
        for (int i=0; i<NR_IRQS-1; i++)
            tx->vec |= (rand() / 2) << i;

        return tx;
    } else {
        return NULL;
    }
}


class IrqDrv {
    private:
        Vedf_ic *dut;
    public:
        IrqDrv(Vedf_ic *dut){
            this->dut = dut;
        }

        void drive(IrqTx *tx){
            // Don't drive anything if a transaction item doesn't exist
            if(tx != NULL){
                dut->irq_i = tx->vec;
                
                // Release the memory by deleting the tx item
                // after it has been consumed
                delete tx;
            }
        }
};
