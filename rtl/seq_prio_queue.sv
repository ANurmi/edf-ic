module seq_prio_queue #(
  parameter  int unsigned NrParIrqs  = 2,
  localparam int unsigned IdWidth    = $clog2(NrParIrqs)
)(
  input  logic               clk_i,
  input  logic               rst_ni,
  output logic [IdWidth-1:0] irq_id_o
);

always_ff @( posedge clk_i or negedge rst_ni ) begin : g_test_assign
  if(~rst_ni) begin
    irq_id_o <= '0;
  end else begin
    irq_id_o <= '1;
  end
end

endmodule : seq_prio_queue
