#include "CB0 CB2 CB3.hlsli"


//ÌùÍ¼ ShadowMapÔÚCB0 CB2 CB3.hlsli
Texture2D _BaseColor : register(t1);
SamplerState _Warp : register(s1);


//Cbuffer
cbuffer Paras : register(b1)
{
    float4 _ColorTint;
}