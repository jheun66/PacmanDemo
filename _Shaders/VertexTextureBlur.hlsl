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


cbuffer BlurBuffer : register(b0)
{
    int _blurCount;
    float3 _textureSize;
}

Texture2D sourceTex : register(t0); // 0~127
SamplerState samp : register(s0);

// pixel shader �Լ�
float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0;
    color = sourceTex.Sample(samp, input.uv);
    
    // ����
    float2 arr[8] =
    {
        float2(-1, -1), float2(0, -1), float2(1, -1),
        float2(-1, 0), float2(1, 0),
        float2(-1, 1), float2(0, 1), float2(1, 1)
    };
    
    // blur�� �Ÿ� ũ�� �� �ȼ����� 0~5�ȼ� �Ÿ��� �ִ� �ֵ� ���� ��ġ�� 
    for (int blur = 0; blur < _blurCount; blur++)
    {
        for (int i = 0; i < 8; i++)
        {
            float x = arr[i].x * (float) blur / _textureSize.x;
            float y = arr[i].y * (float) blur / _textureSize.y;
            
            float2 uv = input.uv + float2(x, y);
            color += sourceTex.Sample(samp, uv);

        }
    }
    
    // ��� ����
    color /= _blurCount * 8 + 1;
    
    return float4(color.rgb, 1);
}