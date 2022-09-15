#include "Default_CB1.hlsli"

float4 main( float4 posMS : POSITION ) : SV_POSITION
{
    posMS.w = 1;
    float4 posWS = mul(posMS, MStoWS);
    float4 pos_LightVS = mul(posWS, L_WStoVS);
    float4 pos_LightCS = mul(pos_LightVS, L_VStoCS);
    return pos_LightCS;
}