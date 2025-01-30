// TODO: also poke the claim mechanism
module edf_ic #(
    parameter  int unsigned NrIrqs   = 4,
    parameter  int unsigned TsWidth  = 64,
    parameter  int unsigned BaseAddr = 0,
    localparam int unsigned IdWidth  = $clog2(NrIrqs)
) (

    input logic clk_i,
    input logic rst_ni,

    // csr in interface
    // this controls interrupt deadlines
    input  logic        cfg_req_i,
    input  logic        cfg_we_i,
    input  logic [31:0] cfg_addr_i,
    input  logic [31:0] cfg_wdata_i,
    output logic [31:0] cfg_rdata_o,


    input  logic [       63:0] mtime_i,
    // interrupt inputs
    input  logic [ NrIrqs-1:0] irq_i,
    // current arbitration winner
    output logic [IdWidth-1:0] irq_id_o,
    // there is an active interrupt
    output logic               irq_valid_o,
    // irq_valid_o ACK
    input  logic               irq_ready_i
);
  logic [      31:0] local_addr;
  logic [NrIrqs-1:0] reqs;
  logic [NrIrqs-1:0] ips;

  assign local_addr = (cfg_addr_i - BaseAddr) / 4;
  assign reqs       = (1 << local_addr);

  logic [TsWidth-1:0] dls[NrIrqs];
  logic [TsWidth-1:0] masked_dls[NrIrqs];
  for (genvar i = 0; i < NrIrqs; i++) begin : g_gateway
    logic local_req;
    assign local_req = reqs[i] & cfg_req_i;

    gateway_cell #(
        .TsWidth(64)
    ) i_gw_cell (
        .clk_i,
        .rst_ni,
        .mtime_i,
        .cfg_req_i(local_req),
        .cfg_we_i,
        .cfg_wdata_i,
        .cfg_rdata_o,
        .irq_i    (irq_i[i]),
        .claim_i  (irq_ready_i),
        .dl_o     (dls[i]),
        .ip_o     (ips[i])
    );
    // TODO: also take into account the enable status of the interrupt
    assign masked_dls[i] = ips[i] == 1 ? dls[i] : 'hFFFFFFFF;
  end

  tree #(
      .TreeValWidth(TsWidth),
      .TreeWidth(NrIrqs),
      .TreePolarity(1)
  ) arb_tree (
      .values(masked_dls),
      .out(irq_id_o)
  );

  //TEMPORARY
  assign irq_valid_o = |ips;

endmodule : edf_ic
