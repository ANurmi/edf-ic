struct IrqStruct
{
    int idx;
    vluint64_t deadline;
    IrqStruct(int idx, vluint64_t deadline) {
        this->idx      = idx;
        this->deadline = deadline;
    }
};

class Compare {
    public:
        bool operator() (IrqStruct* a, IrqStruct* b) {
            return a->deadline > b->deadline;
        }
};

class EdfScb
{
    private:
        //std::priority_queue<IrqInTx*, std::vector<IrqInTx*>, Compare> in_tx;
        std::priority_queue<IrqStruct*, std::vector<IrqStruct*>, Compare> pq;
        std::vector<uint32_t> cfg_regs{std::vector<uint32_t>(NR_IRQS, 0)};
        SimCtx* cx;
    public:
        EdfScb(SimCtx* cx){
            this->cx = cx;
        }
    
        void writeIn(IrqInTx* tx){
            //printf("[writeIn] writing %x\n", tx->vec);
            for (int i=0; i<NR_IRQS; i++){
                if (tx->vec & (1 << i)){
                    if (pq.size() < NR_IRQS) {
                        IrqStruct* entry = new IrqStruct(i, cfg_regs[i] + cx->dut->mtime_i);
                        pq.push(entry);
                    } else if (tx->vec != 0)
                        printf("[Scb] Queue full! Push rejected\n");
                }
            }
        }

        void writeCfg(CfgTx* tx) {
            cfg_regs[(tx->addr)/4] = tx->wdata;
        }

        void print_config(){
            printf("[Scb] printing cfg reg values:\n");
            for (int i=0; i<NR_IRQS; i++)
            {
                printf("[Scb] cfg_reg[%d]: %x\n", i, cfg_regs[i]);
            }
        }

        void writeOut(IrqOutTx* tx){
            if (cx->dut->irq_ready_i) {
                printf("Received ready_i, time: %ld, queue size: %ld\n", cx->sim_time, pq.size());
                printf("Serve interrupt %d\n", pq.top()->idx);
                
                //if (!pq.empty()) 
                //    if (tx->id != pq.top()->idx)
                //        printf("[Scb] output mismatch! DUT: %d | Scoreboard: %d\n", 
                //                    tx->id, pq.top()->idx);
                //pq.pop();
            }
            
        }
        void get_count() {
            printf("[Scb] num. entries %ld\n", pq.size());
        }
        void get_head() {
            if (!pq.empty())
                printf("[Scb] head: %lx\n", pq.top()->deadline);
        }
        void print_queue() {
            printf("[Scb] Printing internal priority queue:\n");
            int i = 0;
            while (!pq.empty()){
                IrqStruct* item = pq.top();
                printf("| pos: %d | irq_idx: %x | deadline: %08lx |\n", i, item->idx, item->deadline);
                pq.pop();
                i++;
            }
            
        }

};

