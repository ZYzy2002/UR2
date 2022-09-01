#include "CB0 CB2 CB3.hlsli"
#include "Default_CB1.hlsli"


struct PSIn
{
    float4 posWS : TEXCOORD1;
    float4 normalWS : TEXCOORD2;
    float2 uv : TEXCOORD3;
};

float4 main(PSIn i) : SV_TARGET
{
    
    //return float4(1, 1, 1, 1);
    
    float4 posWS = float4(i.posWS.xyz, 1);
    float3 normalWS = normalize(i.normalWS.xyz);
    float2 uv = i.uv;
    
    float shadow = SpotLightShadow(posWS, _ShadowMap, _Border);
    //attenuation
    float atten = 1.0 / (0.5 * pow(length(LightPosWS.xyz - posWS.xyz), 2) + 1.0);
    
    //Lambert
    float3 LambertColor = LightColor.xyz * LightColor.w * saturate(dot(normalWS, normalize(LightPosWS.xyz - posWS.xyz)));


    //return float4(LambertColor, 1);
    return float4(LambertColor * shadow * atten, 1.0);

    
    

    
    

    

}


