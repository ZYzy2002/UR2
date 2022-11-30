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
    float2 uv = i.uv*10;
    
    
    //shadow attenuation
    float shadow;
    float atten;
    float3 LightDir;
    if (LightType.x == 0)
    {
        shadow = SpotLightShadow(posWS, _ShadowMapSpot, _Border);
        atten = 1.0 / (0.5 * pow(length(LightPosWS.xyz - posWS.xyz), 2) + 1.0);
        LightDir = normalize(LightPosWS.xyz - posWS.xyz);//逆光线发射方向
    }
    else if (LightType.x == 1)
    {
        shadow = PointLightShadow(posWS, _ShadowMapPoint, _Border);
        atten = 1.0 / (0.5 * pow(length(LightPosWS.xyz - posWS.xyz), 2) + 1.0);
        LightDir = normalize(LightPosWS.xyz - posWS.xyz); //逆光线发射方向
    }
    else
    {
        shadow = DirectLightShadow(posWS, _ShadowMapSpot, _Border);
        atten = 1.0;
        LightDir = normalize(-LightDirWS); //逆光线发射方向
    }
    
    
    //
    float3 albedo = _BaseColor.Sample(SamplerState2, uv) * _ColorTint;
    float3 lightColor = LightColor.xyz * LightColor.w ;
    
    //Lambert
    float3 LambertColor = lightColor * albedo * saturate(dot(normalWS, LightDir));

    //return float4(LambertColor, 1);
      
    

    return float4(LambertColor * shadow * atten, 1.0);

}


