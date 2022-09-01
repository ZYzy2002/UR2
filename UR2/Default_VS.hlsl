#include "CB0 CB2 CB3.hlsli"
//#include "Default_CB1.hlsli"


struct VSIn
{
    float4 posMS : POSITION;
    float4 normalMS : NORMAL;
    float4 tangentMS : TANGENT;
    float2 uv : TEXCOORD;
};
struct VSOut
{
    float4 posWS : TEXCOORD1;
    float4 normalWS : TEXCOORD2;
    float2 uv : TEXCOORD3;
    float4 posCS : SV_Position;
};

VSOut main(VSIn v) 
{
    VSOut o;
    
    float4 posMS = v.posMS;
    posMS.w = 1;
    o.posWS = mul(posMS, MStoWS);
    float4 posVS = mul(o.posWS, WStoVS);
    o.posCS = mul(posVS, VStoCS);
    
    o.normalWS = float4(mul(v.normalMS.xyz, (float3x3) WStoMS), 1);
    o.uv = v.uv;
    return o;
}