#include "CB0 CB2 CB3.hlsli"
#include "Default_CB1.hlsli"

Texture2D _BaseColor : register(t1);
SamplerState _Warp : register(s1);
SamplerState _DefaultSamplerState;

struct PSIn
{
    float2 uv : TEXCOORD;

};

float4 main(PSIn i) : SV_TARGET
{
    return _BaseColor.Sample(_Warp, i.uv*2) * _ColorTint;
}