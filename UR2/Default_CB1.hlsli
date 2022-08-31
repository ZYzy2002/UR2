
Texture2D _ShadowMap : register(t0);
SamplerState _Border : register(s0);

Texture2D _BaseColor : register(t1);
SamplerState _Warp : register(s1);

cbuffer Paras : register(b1)
{
    float4 _ColorTint;
}