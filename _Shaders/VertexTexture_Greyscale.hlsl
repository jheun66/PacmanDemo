//hlsl
// high level shader language ����� ���̴� ���

struct VertexInput
{                      // �ø�ƽ �̸�
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{              // system value(sv) : system���� ���� ��
	float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer TextureBuffer : register(b0)
{
    int index;
}

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

// pixel shader �Լ�
float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0;
    color = sourceTex.Sample(samp, input.uv);
    
    float3 greyscale = 0;
    
    if(index == 0)
    {
        greyscale = (color.r + color.g + color.b) / 3;
    }
    else if(index == 1)
    {
        greyscale = 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
    }
    
    return float4(greyscale, 1);
}