module edf_ic #(
    parameter  int unsigned NrIrqs   = 4,
    parameter  int unsigned TsWidth  = 24,
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
    input  logic [63:0] mtime_i,
    // interrupt inputs
    input  logic [NrIrqs-1:0] irq_i,
    // current arbitration winner
    output logic [IdWidth-1:0] irq_id_o,
    // there is an active interrupt
    output logic irq_valid_o,
    // irq_valid_o ACK
    input  logic irq_ready_i
);

typedef struct packed {
  logic ip;
  logic ie;
  logic trig_type;
  logic trig_pol;
  logic [TsWidth-1:0] dl;
} line_t;

line_t [NrIrqs-1:0] lines_d, lines_q;

logic [IdWidth-1:0] local_addr;
assign local_addr = cfg_addr_i[(IdWidth+2)-1:2];

logic read_event, write_event;
assign read_event  = cfg_req_i & ~cfg_we_i;
assign write_event = cfg_req_i &  cfg_we_i;

always_ff @(posedge clk_i or negedge rst_ni) begin
  if (~rst_ni) begin
    lines_q <= '0;
  end else begin
    lines_q <= lines_d;
  end
end

always_comb begin : cfg_access

  lines_d = lines_q;
  cfg_rdata_o = 32'h0;

  if (write_event) begin
    lines_d[local_addr].dl = cfg_wdata_i[31:8];
    lines_d[local_addr].trig_pol  = cfg_wdata_i[3];
    lines_d[local_addr].trig_type = cfg_wdata_i[2];
    lines_d[local_addr].ip = cfg_wdata_i[1];
    lines_d[local_addr].ie = cfg_wdata_i[0];
  end
  else if (read_event) begin
    cfg_rdata_o = {
      lines_q[local_addr].dl,
      4'b0,
      lines_q[local_addr].trig_pol,
      lines_q[local_addr].trig_type,
      lines_q[local_addr].ip,
      lines_q[local_addr].ie
      };
  end

end : cfg_access

irq_arbiter #(
  .NrInputs (NrIrqs),
  .PrioWidth(TsWidth)
) i_arb (
  .valid_i (),
  .valid_o (),
  .prio_i  (),
  .prio_o  (),
  .idx_o   ()
);

irq_gateway #(
  .NrInputs (NrIrqs)
) i_gw (
  .clk_i,
  .rst_ni,
  .trig_polarity_i (),
  .trig_type_i (),
  .ip_i   (),
  .irqs_i (),
  .irqs_o ()
);

endmodule : edf_ic

