//hlsl
// high level shader language ������ ���̴� ���


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
    //if(input.uv.x < 0.5f && input.uv.y < 0.5f)
    //    color = sourceTex.Sample(samp, input.uv);
    //else if(input.uv.x > 0.5f && input.uv.y < 0.5f)
    //    color = float4(1, 0, 0, 1);
    //else if (input.uv.x > 0.5f && input.uv.y > 0.5f)
    //    color = float4(0, 1, 0, 1);
    //else 
    //    color = float4(0, 0, 1, 1);
    color = sourceTex.Sample(samp, input.uv);
    return color;
}