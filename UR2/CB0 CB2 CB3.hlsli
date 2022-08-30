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