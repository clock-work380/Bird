// =============================================================================
//  FlappyController.v  —  2-layer neural net, 5-stage pipeline
//  Verilator 5.x compatible (no implicit nets, explicit port directions)
//  Inputs:  4 x signed 8-bit (h_err, velocity, pipe_dist, gap_center)
//  Hidden:  3 neurons, tanh approximated as clamp(x>>2, -128, 127)
//  Output:  1-bit flap
// =============================================================================
module FlappyController (
    input  wire        clk,
    input  wire        rst,

    input  wire signed [7:0]  in0,
    input  wire signed [7:0]  in1,
    input  wire signed [7:0]  in2,
    input  wire signed [7:0]  in3,

    // Hidden layer weights: 3 neurons x 4 inputs
    input  wire signed [15:0] wh00, wh01, wh02, wh03,
    input  wire signed [15:0] wh10, wh11, wh12, wh13,
    input  wire signed [15:0] wh20, wh21, wh22, wh23,

    // Output layer weights
    input  wire signed [15:0] wo0, wo1, wo2,

    output reg         flap,
    output reg signed [15:0] activation,

    // Pipeline stage outputs for visualisation
    output reg signed [31:0] s2_h0, s2_h1, s2_h2,
    output reg signed [15:0] s3_h0, s3_h1, s3_h2,
    output reg signed [31:0] s5_acc
);

// Stage registers
reg signed [31:0] s1_h0, s1_h1, s1_h2;
reg signed [31:0] s2_h0_r, s2_h1_r, s2_h2_r;
reg signed [15:0] s3_h0_r, s3_h1_r, s3_h2_r;
reg signed [31:0] s4_h0, s4_h1, s4_h2;
reg signed [33:0] s5_r;

// Tanh approximation: saturating shift
function automatic signed [15:0] approx_tanh;
    input signed [31:0] x;
    reg signed [31:0] scaled;
    begin
        scaled = x >>> 8;
        if      (scaled >  127) approx_tanh = 16'sh007F;
        else if (scaled < -128) approx_tanh = -16'sh0080;
        else                    approx_tanh = scaled[15:0];
    end
endfunction

always @(posedge clk or posedge rst) begin
    if (rst) begin
        s1_h0<=0; s1_h1<=0; s1_h2<=0;
        s2_h0_r<=0; s2_h1_r<=0; s2_h2_r<=0;
        s3_h0_r<=0; s3_h1_r<=0; s3_h2_r<=0;
        s4_h0<=0; s4_h1<=0; s4_h2<=0;
        s5_r<=0; flap<=0; activation<=0;
        s2_h0<=0; s2_h1<=0; s2_h2<=0;
        s3_h0<=0; s3_h1<=0; s3_h2<=0;
        s5_acc<=0;
    end else begin
        // S1: first multiply per neuron
        s1_h0 <= in0 * wh00;
        s1_h1 <= in0 * wh10;
        s1_h2 <= in0 * wh20;

        // S2: full dot product
        s2_h0_r <= s1_h0 + in1*wh01 + in2*wh02 + in3*wh03;
        s2_h1_r <= s1_h1 + in1*wh11 + in2*wh12 + in3*wh13;
        s2_h2_r <= s1_h2 + in1*wh21 + in2*wh22 + in3*wh23;

        // S3: tanh activation
        s3_h0_r <= approx_tanh(s2_h0_r);
        s3_h1_r <= approx_tanh(s2_h1_r);
        s3_h2_r <= approx_tanh(s2_h2_r);

        // S4: output multiply
        s4_h0 <= s3_h0_r * wo0;
        s4_h1 <= s3_h1_r * wo1;
        s4_h2 <= s3_h2_r * wo2;

        // S5: sum + threshold
        s5_r     <= s4_h0 + s4_h1 + s4_h2;
        flap     <= (s5_r > 0) ? 1'b1 : 1'b0;
        activation <= s5_r[23:8];

        // Expose for visualisation
        s2_h0 <= s2_h0_r; s2_h1 <= s2_h1_r; s2_h2 <= s2_h2_r;
        s3_h0 <= s3_h0_r; s3_h1 <= s3_h1_r; s3_h2 <= s3_h2_r;
        s5_acc <= s5_r[31:0];
    end
end

endmodule
