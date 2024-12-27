
class EdfScb
{
    private:
        std::priority_queue<IrqInTx*, std::vector<IrqInTx*>, Compare> in_tx;
        SimCtx* cx;
    public:
        EdfScb(SimCtx* cx){
            this->cx = cx;
        }
    
        void writeIn(IrqInTx* tx){
            this->in_tx.push(tx);
        }

        void writeCfg(CfgTx* tx) {}

        void writeOut(IrqOutTx* tx){

        }
        void count() {
            printf("[Scb] num. entries %d\n", in_tx.size());
        }
        void head() {
            if (!in_tx.empty())
                printf("[Scb] head: %x\n", in_tx.top()->vec);
        }

};

