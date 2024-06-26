module edf_intctl_top #(
  parameter int unsigned NrSyncIrqs  = 32,
  parameter int unsigned NrAsyncIrqs = 32,
  parameter int unsigned IrqIdWidth  = 8
)(
  input  logic                   clk_i,
  input  logic                   rst_ni,
  input  logic [ NrSyncIrqs-1:0] sync_irqs_i,
  input  logic [NrAsyncIrqs-1:0] async_irqs_i,
  //TODO: config IF
  output logic                   irq_req_o,
  input  logic                   irq_ack_i,
  output logic [ IrqIdWidth-1:0] irq_id_o
);

endmodule : edf_intctl_top
