cbuffer CB0 : register(b0)
{
    matrix MStoWS;
    matrix WStoMS;
}



cbuffer CB2 : register(b2)
{
    matrix VStoWS;
    matrix WStoVS;
    
    matrix VStoCS;
    matrix CStoVS;
    
    float4 CameraPosWS;
    float4 CameraDirWS; //camera -Z�� ��WS�ķ���
}


cbuffer CB3 : register(b3)
{
    matrix L_VStoWS; //lightSpace �� worldSpace
    matrix L_WStoVS;
    
    matrix L_VStoCS;
    matrix L_CStoVS;
    
    float4 LightPosWS;
    float4 LightDirWS; //�ƹ�� -z�� (���� ���ռ���ʱ�� lightDir��
    
    float4 LightColor; //��ɫ ǿ�� RGB
    float4 LightRadius; //˥�����  R
}

Texture2D _ShadowMap : register(t0);
SamplerState _CompareBorder : register(s0);

float SpotLightShadow(float4 posWS, Texture2D shadowMap, 
    SamplerState shadowMapSamplerState)
{
    float4 pos_LightVS = mul(posWS, L_WStoVS);
    float4 pos_LightCS = mul(pos_LightVS, L_VStoCS);
    float4 pos_LightNDC = pos_LightCS / pos_LightCS.w;
    float2 uv_Light = float2(pos_LightNDC.x / 2.0f + 0.5, -pos_LightNDC.y / 2.0f + 0.5); //ע�� y �����и���
    

    float light01Depth = shadowMap.Sample(shadowMapSamplerState, uv_Light).r;
    float shadow = step(pos_LightNDC.z, light01Depth + 0.00001);
    //SpotLight Mask���۹����ǰ��ǿ�����б�����
    float SpotLightMask = saturate(1 - length(uv_Light * 2 - 1));
    return shadow * SpotLightMask;
}