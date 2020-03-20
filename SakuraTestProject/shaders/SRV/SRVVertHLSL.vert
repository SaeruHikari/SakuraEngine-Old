[[vk::binding(0)]] cbuffer PushConst
{
    float4x4 model;
    float4x4 view;
    float4x4 proj;
}

struct VertexIn
{
    [[vk::location(0)]] float3 inPosition;
	[[vk::location(1)]] float3 inColor;
	[[vk::location(2)]] float2 inTexCoord;
};

struct VertexOut
{
    float4 position : SV_Position;
    [[vk::location(0)]] float3 fragColor;
    [[vk::location(1)]] float2 fragTexCoord;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    float4x4 MVP = model * view * proj;
    vout.position = mul(MVP, float4(vin.inPosition, 1.0));
    vout.fragColor = vin.inColor;
    vout.fragTexCoord = vin.inTexCoord;
    return vout;
}

// /c/glslang/bin/glslangValidator.exe -e main -o SRVVertex.spv -V -D SRVVertHLSL.vert