#include "CB0 CB2 CB3.hlsli"


//��ͼ ShadowMap��CB0 CB2 CB3.hlsli, slot ��2��ʼ
Texture2D _BaseColor : register(t2);
SamplerState SamplerState2 : register(s2);


//Cbuffer
cbuffer Paras : register(b1)
{
    float4 _ColorTint;
}