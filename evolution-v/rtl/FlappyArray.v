// =============================================================================
//  FlappyArray.v  --  30 neural nets evaluated IN PARALLEL
//  Uses flat packed buses to be Verilator-friendly.
//  Each signal is packed as [N*W-1:0] where W is the signal width.
//  Bird i occupies bits [(i+1)*W-1 : i*W]
// =============================================================================
`define N 30

module FlappyArray (
    input  wire clk,
    input  wire rst,

    // Packed inputs: 30 x 8-bit = 240-bit buses
    input  wire [(`N*8)-1:0]  in0_bus,
    input  wire [(`N*8)-1:0]  in1_bus,
    input  wire [(`N*8)-1:0]  in2_bus,
    input  wire [(`N*8)-1:0]  in3_bus,

    // Packed weights: 30 x 16-bit = 480-bit buses
    input  wire [(`N*16)-1:0] wh00_bus, wh01_bus, wh02_bus, wh03_bus,
    input  wire [(`N*16)-1:0] wh10_bus, wh11_bus, wh12_bus, wh13_bus,
    input  wire [(`N*16)-1:0] wh20_bus, wh21_bus, wh22_bus, wh23_bus,
    input  wire [(`N*16)-1:0] wo0_bus,  wo1_bus,  wo2_bus,

    // Packed outputs
    output wire [(`N*1)-1:0]  flap_bus,
    output wire [(`N*16)-1:0] activation_bus,
    output wire [(`N*32)-1:0] s2_h0_bus, s2_h1_bus, s2_h2_bus,
    output wire [(`N*16)-1:0] s3_h0_bus, s3_h1_bus, s3_h2_bus,
    output wire [(`N*32)-1:0] s5_acc_bus
);

genvar i;
generate
    for (i = 0; i < `N; i = i + 1) begin : bird
        FlappyController ctrl (
            .clk(clk), .rst(rst),
            .in0(in0_bus[i*8+7 : i*8]),
            .in1(in1_bus[i*8+7 : i*8]),
            .in2(in2_bus[i*8+7 : i*8]),
            .in3(in3_bus[i*8+7 : i*8]),
            .wh00(wh00_bus[i*16+15 : i*16]),
            .wh01(wh01_bus[i*16+15 : i*16]),
            .wh02(wh02_bus[i*16+15 : i*16]),
            .wh03(wh03_bus[i*16+15 : i*16]),
            .wh10(wh10_bus[i*16+15 : i*16]),
            .wh11(wh11_bus[i*16+15 : i*16]),
            .wh12(wh12_bus[i*16+15 : i*16]),
            .wh13(wh13_bus[i*16+15 : i*16]),
            .wh20(wh20_bus[i*16+15 : i*16]),
            .wh21(wh21_bus[i*16+15 : i*16]),
            .wh22(wh22_bus[i*16+15 : i*16]),
            .wh23(wh23_bus[i*16+15 : i*16]),
            .wo0(wo0_bus[i*16+15 : i*16]),
            .wo1(wo1_bus[i*16+15 : i*16]),
            .wo2(wo2_bus[i*16+15 : i*16]),
            .flap(flap_bus[i]),
            .activation(activation_bus[i*16+15 : i*16]),
            .s2_h0(s2_h0_bus[i*32+31 : i*32]),
            .s2_h1(s2_h1_bus[i*32+31 : i*32]),
            .s2_h2(s2_h2_bus[i*32+31 : i*32]),
            .s3_h0(s3_h0_bus[i*16+15 : i*16]),
            .s3_h1(s3_h1_bus[i*16+15 : i*16]),
            .s3_h2(s3_h2_bus[i*16+15 : i*16]),
            .s5_acc(s5_acc_bus[i*32+31 : i*32])
        );
    end
endgenerate

endmodule
