// 1. A grouped block (Tests Descriptor Set creation & UBO offset calculation)
struct SceneData
{
    float4x4 viewProj;
    float3 ambientColor;
};
ParameterBlock<SceneData> scene;

// 2. A small standalone buffer (Tests your Push Constant heuristic: size <= 128)
struct ObjectData
{
    float4x4 model;
};
ConstantBuffer<ObjectData> object;

// 3. Global resources (Tests Texture and Sampler binding indices)
Texture2D testTexture;
SamplerState testSampler;

struct VertexInput
{
    float3 position : POSITION;
    float2 uv       : TEXCOORD0;
};

struct VertexOutput
{
    float4 position : SV_Position;
    float2 uv       : TEXCOORD0;
};

[shader("vertex")]
VertexOutput vertexMain(VertexInput input)
{
    VertexOutput output;
    float4 worldPos = mul(object.model, float4(input.position, 1.0));
    output.position = mul(scene.viewProj, worldPos);
    output.uv = input.uv;
    return output;
}

[shader("fragment")]
float4 fragmentMain(VertexOutput input) : SV_Target
{
    float4 color = testTexture.Sample(testSampler, input.uv);
    return color * float4(scene.ambientColor, 1.0);
}