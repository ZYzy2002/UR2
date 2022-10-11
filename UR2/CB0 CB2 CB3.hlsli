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


static matrix cube[6] =
{
    //+x
    matrix(0, 0, -1, 0,
            0, 1, 0, 0,
            1, 0, 0, 0,
            0, 0, 0, 1),
    //-x
    matrix(0, 0, 1, 0,
            0, 1, 0, 0,
            -1, 0, 0, 0,
            0, 0, 0, 1),
    //+y
    matrix(1, 0, 0, 0,
            0, 0, -1, 0,
            0, 1, 0, 0,
            0, 0, 0, 1),
    //-y
    matrix(1, 0, 0, 0,
            0, 0, 1, 0,
            0, -1, 0, 0,
            0, 0, 0, 1),
    //+z
    matrix(-1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, -1, 0,
            0, 0, 0, 1),
    //-z
    matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1)
};

float PointLightShadow(
    float4 posWS,
    TextureCube shadowMap,
    SamplerState shadowMapSamplerState)
{
    float3 uvw_Light = normalize((posWS - LightPosWS).xyz);
    matrix L_WStoVS_CubeFaceChoose = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    
    float3 cmpResult = float3(0, 0, 0);
    cmpResult.x += step(abs(uvw_Light.y), abs(uvw_Light.x));
    cmpResult.x += step(abs(uvw_Light.z), abs(uvw_Light.x));
    cmpResult.y += step(abs(uvw_Light.x), abs(uvw_Light.y));
    cmpResult.y += step(abs(uvw_Light.z), abs(uvw_Light.y));
    cmpResult.z += step(abs(uvw_Light.x), abs(uvw_Light.z));
    cmpResult.z += step(abs(uvw_Light.y), abs(uvw_Light.z));
    if(cmpResult.x==2)
    {
        if (step(0, uvw_Light.x))
        {
            L_WStoVS_CubeFaceChoose = mul(cube[0], L_WStoVS);
        }
        else
        {
            L_WStoVS_CubeFaceChoose = mul(cube[1], L_WStoVS);
        }
    }
    if (cmpResult.y == 2)
    {
        if (step(0, uvw_Light.y))
        {
            L_WStoVS_CubeFaceChoose = mul(cube[2], L_WStoVS);
        }
        else
        {
            L_WStoVS_CubeFaceChoose = mul(cube[3], L_WStoVS);
        }
    }
    if (cmpResult.z == 2)
    {
        if (step(0, uvw_Light.z))
        {
            L_WStoVS_CubeFaceChoose = mul(cube[4], L_WStoVS);
        }
        else
        {
            L_WStoVS_CubeFaceChoose = mul(cube[5], L_WStoVS);
        }
    }
    
    
    float4 pos_LightVS = mul(posWS, L_WStoVS_CubeFaceChoose);
    float4 pos_LightCS = mul(pos_LightVS, L_VStoCS);
    float4 pos_LightNDC = pos_LightCS / pos_LightCS.w;
    
    float light01Depth = shadowMap.Sample(shadowMapSamplerState, uvw_Light);
    
    float shadow = step( pos_LightNDC.z, light01Depth + 0.00001);
    

    return shadow;
}
