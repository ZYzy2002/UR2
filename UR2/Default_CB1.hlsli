#include "CB0 CB2 CB3.hlsli"


//贴图 ShadowMap在CB0 CB2 CB3.hlsli, slot 从2开始
Texture2D _BaseColor : register(t2);
SamplerState SamplerState2 : register(s2);


//Cbuffer
cbuffer Paras : register(b1)
{
    float4 _ColorTint;
}