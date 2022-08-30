#include "CB0 CB2 CB3.hlsli"
//#include "Default_CB1.hlsli"


struct VSIn
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};
struct VSOut
{
    float2 uv : TEXCOORD;
    float4 posCS : SV_Position;
};

VSOut main(VSIn v) 
{
    VSOut o;
    
    float4 posMS = v.pos;
    posMS.w = 1;

    float4 posWS = mul(v.pos, MStoWS);
    float4 posVS = mul(posWS, WStoVS);
    o.posCS = mul(posVS, VStoCS);
    
    o.uv = v.uv;
    return o;
}