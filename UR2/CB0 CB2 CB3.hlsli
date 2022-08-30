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
}