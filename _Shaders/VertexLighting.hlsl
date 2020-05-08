//hlsl
// high level shader language ������ ���̴� ���


struct VertexInput
{ // �ø�ƽ �̸�
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{ // system value(sv) : system���� ���� ��
    float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer WorldBuffer : register(b0) // ������ �ִ� 13���� ����(14��)
{
    matrix _world;
}

cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _proj;
}

// vertex shader �Լ�
PixelInput VS(VertexInput input)
{
    PixelInput output;

    // �ش� �������� �̵��� ��ġ���� = ��ġ���� * ���� ���
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _proj);
    // NDC ��ǥ��
    
    output.uv = input.uv;

    return output;
}


Texture2D sourceTex : register(t0); // 0~127
SamplerState samp : register(s0);

cbuffer LightBuffer : register(b0)
{
    float3 _mPos;
}

float GetLightAmount(float2 uv, float2 pos)
{
    float dx = (uv.x - pos.x) * 1280 / 720;
    float dy = (uv.y - (1 - pos.y));
    
    // sqrt ������, pow ����
    float dist = sqrt(pow(dx, 2) + pow(dy, 2));
    
    if(dist < 0.2f)
        return (0.2f - dist) / 0.2f;
    else
        return 0.0f;
}


// pixel shader �Լ�
float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0;
    color = sourceTex.Sample(samp, input.uv);
    float amount = GetLightAmount(input.uv, _mPos.xy);
    
    amount += GetLightAmount(input.uv, float2(0.3f, 0.4f));
    amount = clamp(amount, 0.0f, 1.0f); //amount = saturate(amount); �� ����
    
    color.rgb *= amount;
   
    return color;
}