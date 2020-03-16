#pragma shader_stage(vertex)

struct VertexIn
{
    float2 inPos : SV_POSITION;
    float3 vertColor : COLOR;
};

struct VertexOut
{
    float3 fragColor : COLOR;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
    vout.fragColor = vin.vertColor;
    return vout;
}