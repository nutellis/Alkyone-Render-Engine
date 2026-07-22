struct VertexInput {
    float2 Position : Position;
    float3 Color : VertexColor;
};

struct VertexOutput {
    float4 PositionCS : SV_Position;
    float3 Color : VertexColor;
};

static const float3 colors[3] = {
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
};
[shader("vertex")]
VertexOutput vertex(VertexInput input) {
    return VertexOutput(
        float4(input.Position.x, -input.Position.y, 0.0, 1.0),
        float3(1.0, 1.0, 1.0)
    );
}

[shader("fragment")]
float4 fragment(VertexOutput vertIn): SV_Target {
    return float4(vertIn.Color, 1.0);
}