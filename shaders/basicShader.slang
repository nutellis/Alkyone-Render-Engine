// simple_geo.slang
struct GeometryPass {
    float4x4 mvp;
};

// Slang will automatically assign a Set and Binding to this
ParameterBlock<GeometryPass> gp;

struct VertexInput {
    float3 position : POSITION;
};

struct VertexOutput {
    float4 position : SV_Position;
};

[shader("vertex")]
VertexOutput vertexMain(VertexInput input) {
    VertexOutput output;
    output.position = mul(gp.mvp, float4(input.position, 1.0));
    return output;
}

[shader("fragment")]
float4 fragmentMain(VertexOutput input) : SV_Target {
    return float4(0.8, 0.8, 0.8, 1.0);
}