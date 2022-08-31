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
    
    float4 posWS = float4(i.posWS.xyz, 1);
    float3 normalWS = normalize(i.normalWS.xyz);
    float2 uv = i.uv;
    
    //SpotLight Shadow
    float4 pos_LightVS = mul(posWS, L_WStoVS);
    float4 pos_LightCS = mul(pos_LightVS, L_VStoCS);
    float4 pos_LightNDC = pos_LightCS / pos_LightCS.w;
    float2 uv_Light = float2(pos_LightNDC.x / 2.0f + 0.5, -pos_LightNDC.y / 2.0f + 0.5);    //注意 y 方向有负号
    float light01Depth = _ShadowMap.Sample(_Border, uv_Light).r;
    float shadow = step(pos_LightNDC.z, light01Depth + 0.00001);
    //SpotLight Mask，聚光灯正前方强度最大，斜向较弱
    float SpotLightMask = saturate(1 - length(uv_Light * 2 - 1));
    //attenuation
    float atten = 1.0 / (0.5 * pow(length(LightPosWS.xyz - posWS.xyz), 2) + 1.0);
    
    return shadow.xxxx *SpotLightMask *atten;

    
    

    
    

    

}


