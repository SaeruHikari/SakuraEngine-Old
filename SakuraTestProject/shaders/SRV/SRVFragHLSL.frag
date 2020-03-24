struct PixelIn
{
    float4 position : SV_Position;
    [[vk::location(0)]] float3 fragColor;
	[[vk::location(1)]] float2 fragTexCoord;
};

struct PixelOut 
{
    [[vk::location(0)]] float4 outColor;
};

[[vk::binding(1, 0)]] SamplerState textureSampler;
[[vk::binding(2, 0)]] Texture2D<float4> _texture;

PixelOut main(PixelIn pin)
{
    PixelOut pout;
    pout.outColor = _texture.Sample(textureSampler, pin.fragTexCoord);
    return pout;
};

//  /c/glslang/bin/glslangValidator.exe -e main -o SRVPixel.spv -V -D SRVFragHLSL.frag