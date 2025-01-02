module gateway_cell #(
  parameter int unsigned TsWdith = 64
)(
  input  logic               clk_i,
  input  logic               rst_ni,
  input  logic        [63:0] mtime_i,
  input  logic               irq_i,
  input  logic               claim_i,
  input  logic               cfg_req_i,
  input  logic               cfg_we_i,
  input  logic        [31:0] cfg_wdata_i,
  output logic        [31:0] cfg_rdata_o,
  output logic [TsWdith-1:0] dl_o,
  output logic               ip_o
);

logic [TsWdith-1:0] timestamp, timestamp_r;
logic [TsWdith-1:0] offset, offset_r;
logic               ip_d, ip_q;

always_comb
  begin
    timestamp = timestamp_r;
    offset    = offset_r;
    ip_d      = ip_q;
    if (irq_i) begin
      timestamp = mtime_i;
      ip_d      = irq_i;
    end
    if (claim_i) begin
      ip_d      = 0;
    end
  end

always_ff @(posedge clk_i or negedge rst_ni)
  begin : g_regs
    if (~rst_ni) begin
      timestamp_r <= 0;
      offset_r    <= 0;
      ip_q        <= 0;
    end else begin
      timestamp_r <= timestamp;
      offset_r    <= offset;
      ip_q        <= ip_d;
    end
  end

assign dl_o = offset_r + timestamp_r;
assign ip_o = ip_q;


endmodule : gateway_cell
