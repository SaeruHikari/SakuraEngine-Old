#pragma shader_stage(fragment)

struct VertexOut
{
    float3 fragColor : COLOR;
};

float4 main(VertexOut pin)
{
    return float4(pin.fragColor, 1.f);
}
