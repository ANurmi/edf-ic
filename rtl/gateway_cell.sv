module gateway_cell #(
  parameter int unsigned TsWdith = 64
)(
  input  logic               clk_i,
  input  logic               rst_ni,
  input  logic        [63:0] mtime_i,
  input  logic               irq_i,
  input  logic               cfg_req_i,
  input  logic               cfg_we_i,
  input  logic        [31:0] cfg_wdata_i,
  output logic        [31:0] cfg_rdata_o,
  output logic [TsWdith-1:0] dl_o,
  output logic               ip_o
);

logic [TsWdith-1:0] timestamp, timestamp_r;
logic [TsWdith-1:0] offset, offset_r;

always_comb
  begin
    timestamp = timestamp_r;
    offset    = offset_r;
    if(irq_i)
      timestamp = mtime_i;
  end

always_ff @(posedge clk_i or negedge rst_ni)
  begin : g_regs
    if (~rst_ni) begin
      timestamp_r <= 0;
      offset_r    <= 0;
    end else begin
      timestamp_r <= timestamp;
      offset_r    <= offset;
    end
  end

assign dl_o = offset_r + timestamp_r;


endmodule : gateway_cell
