module edf_ic #(
  parameter  int unsigned NrIrqs     = 4,
  parameter  int unsigned TsWdith    = 64,
  parameter  int unsigned BaseAddr   = 0,
  localparam int unsigned IdWidth    = $clog2(NrIrqs)
)(
  input  logic                   clk_i,
  input  logic                   rst_ni,
  input  logic                   cfg_req_i,
  input  logic                   cfg_we_i,
  input  logic            [31:0] cfg_addr_i,
  input  logic            [31:0] cfg_wdata_i,
  output logic            [31:0] cfg_rdata_o,
  input  logic            [63:0] mtime_i,
  input  logic      [NrIrqs-1:0] irq_i,
  output logic     [IdWidth-1:0] irq_id_o,
  output logic                   irq_valid_o,
  input  logic                   irq_ready_i
);

logic       [31:0] local_addr;
logic [NrIrqs-1:0] reqs;
logic [NrIrqs-1:0] ips;

assign local_addr = (cfg_addr_i - BaseAddr)/4;
assign reqs       = (1 << local_addr);

for (genvar i=0; i<NrIrqs; i++) begin : g_gateway
  logic local_req;
  assign local_req = reqs[i] & cfg_req_i;

  gateway_cell #(
    .TsWdith (64)
  ) i_gw_cell (
    .clk_i,
    .rst_ni,
    .mtime_i,
    .cfg_req_i   (local_req),
    .cfg_we_i,
    .cfg_wdata_i,
    .cfg_rdata_o,
    .irq_i       (irq_i[i]),
    .claim_i     (irq_ready_i),
    .dl_o        (),
    .ip_o        (ips[i])
  );
end

//TEMPORARY
assign irq_valid_o = |ips;

endmodule : edf_ic
