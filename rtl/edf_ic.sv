module edf_ic #(
  parameter  int unsigned SerLatency = 1,
  parameter  int unsigned NrParIrqs  = 4,
  localparam int unsigned IdWidth    = $clog2(NrParIrqs),
  // This fraction must be an integer
  localparam int unsigned PqClkMult  = (NrParIrqs / SerLatency)
)(
  input  logic                   clk_i,
  input  logic                   rst_ni,
  input  logic                   cfg_req_i,
  input  logic            [31:0] cfg_addr_i,
  input  logic            [31:0] cfg_wdata_i,
  input  logic   [NrParIrqs-1:0] irq_i,
  output logic     [IdWidth-1:0] irq_id_o
);

seq_prio_queue #(
  .NrParIrqs
) i_pq (
  .clk_i,
  .rst_ni,
  .irq_id_o
);

endmodule : edf_ic
