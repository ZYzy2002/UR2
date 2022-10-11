cbuffer CB0 : register(b0)
{
    matrix MStoWS;
    matrix WStoMS;
}

cbuffer CB2 : register(b2)      //Camera 或 shadowMapLight
{
    matrix VStoWS;
    matrix WStoVS;
    
    matrix VStoCS;
    matrix CStoVS;
    
    float4 CameraPosWS;
    float4 CameraDirWS; //camera -Z轴 在WS的方向
}

cbuffer CB3 : register(b3)
{
    matrix L_VStoWS; //lightSpace 到 worldSpace
    matrix L_WStoVS;
    
    matrix L_VStoCS;
    matrix L_CStoVS;
    
    float4 LightPosWS;
    float4 LightDirWS; //灯光的 -z轴 (不是 光照计算时的 lightDir）
    
    float4 LightColor; //颜色 强度 RGB
    float4 LightRadius; //衰减相关  R
    
    int4 LightType;
}

Texture2D _ShadowMapSpot : register(t0);
TextureCube _ShadowMapPoint : register(t1);
SamplerState _Border : register(s0);
#define _ShadowMap_Precision 1024.f

float SpotLightShadow(
    float4 posWS,
    Texture2D shadowMap, 
    SamplerState shadowMapSamplerState)
{
    float4 pos_LightVS = mul(posWS, L_WStoVS);
    float4 pos_LightCS = mul(pos_LightVS, L_VStoCS);
    float4 pos_LightNDC = pos_LightCS / pos_LightCS.w;
    float2 uv_Light = float2(pos_LightNDC.x / 2.0f + 0.5, -pos_LightNDC.y / 2.0f + 0.5); //注意 y 方向有负号
    
    
    float2 uv_light5[5];
    float offset = 2.0f / _ShadowMap_Precision;
    uv_light5[0] = uv_Light;
    uv_light5[1] = uv_Light + float2(offset, 0);
    uv_light5[2] = uv_Light + float2(-offset, 0);
    uv_light5[3] = uv_Light + float2(0, offset);
    uv_light5[4] = uv_Light + float2(0, -offset);
    float light01Depth[5];
    for (int i = 0; i < 5;i++)
    {
        light01Depth[i] = shadowMap.Sample(shadowMapSamplerState, uv_light5[i]).r;
    }
    bool cmp[5];
    for (int j = 0; j < 5; j++)
    {
        cmp[j] = step(pos_LightNDC.z, light01Depth[j] + 0.00001);
    }
    float shadow = 0;
    for (int k = 0; k < 5; k++)
    {
        shadow += float(cmp[k]);
    }
    shadow /= 5.0;
    
    //SpotLight Mask，聚光灯正前方强度最大，斜向较弱
    float SpotLightMask = saturate(1 - length(uv_Light * 2 - 1));
    return shadow * SpotLightMask;
}
float PointLightShadow(
    float4 posWS,
    TextureCube shadowMap,
    SamplerState shadowMapSamplerState)
{
    float4 pos_LightVS = mul(posWS, L_WStoVS);
    float4 pos_LightCS = mul(pos_LightVS, L_VStoCS);
    float4 pos_LightNDC = pos_LightCS / pos_LightCS.w;
    //float2 uv_Light = float2(pos_LightNDC.x / 2.0f + 0.5, -pos_LightNDC.y / 2.0f + 0.5); //注意 y 方向有负号
    float3 uvz_Light = (posWS - LightPosWS).xyz;
    
/*    float3 uv_light5[5];
    float offset = 2.0f / _ShadowMap_Precision;
    uv_light5[0] = uv_Light;
    uv_light5[1] = uv_Light + float2(offset, 0);
    uv_light5[2] = uv_Light + float2(-offset, 0);
    uv_light5[3] = uv_Light + float2(0, offset);
    uv_light5[4] = uv_Light + float2(0, -offset);
    float light01Depth[5];
    for (int i = 0; i < 5; i++)
    {
        light01Depth[i] = shadowMap.Sample(shadowMapSamplerState, uv_light5[i]).r;
    }
    bool cmp[5];
    for (int j = 0; j < 5; j++)
    {
        cmp[j] = step(pos_LightNDC.z, light01Depth[j] + 0.00001);
    }
    float shadow = 0;
    for (int k = 0; k < 5; k++)
    {
        shadow += float(cmp[k]);
    }
    shadow /= 5.0;
    */
    float light01Depth = shadowMap.Sample(shadowMapSamplerState, uvz_Light);
    float shadow = step( , light01Depth + 0.00001);

}