module edf_ic #(
  parameter  int unsigned NrIrqs     = 4,
  parameter  int unsigned TsWdith    = 64,
  localparam int unsigned IdWidth    = $clog2(NrIrqs)
)(
  input  logic                   clk_i,
  input  logic                   rst_ni,
  input  logic                   cfg_req_i,
  input  logic            [31:0] cfg_addr_i,
  input  logic            [31:0] cfg_wdata_i,
  input  logic            [63:0] mtime_i,
  input  logic      [NrIrqs-1:0] irq_i,
  output logic     [IdWidth-1:0] irq_id_o,
  output logic                   irq_valid_o,
  input  logic                   irq_ready_i
);

for (genvar i=0; i<NrIrqs; i++) begin : g_gateway
  gateway_cell #(
    .TsWdith (64)
  ) i_gw_cell (
    .clk_i,
    .rst_ni,
    .mtime_i,
    .irq_i  (irq_i[i]),
    .dl_o   (),
    .ip_o   ()
  );
end


endmodule : edf_ic
