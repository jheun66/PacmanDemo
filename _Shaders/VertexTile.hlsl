//hlsl
// high level shader language ������ ���̴� ���


struct VertexInput
{ // �ø�ƽ �̸�
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float2 uv2 : TEXCOORD1;
    float4 color : COLOR0;
    uint index : INDEX0;
};

struct PixelInput
{              // system value(sv) : system���� ���� ��
	float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float2 uv2 : TEXCOORD1;
    float4 color : COLOR0;
    uint index : INDEX0;
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
    output.color = input.color;
    output.uv2 = input.uv2;
    output.index = input.index;

	return output;
}

cbuffer TileBuffer : register(b0)
{
    uint _tileIndex;
}

Texture2D sourceTex : register(t0); // 0~127
SamplerState samp : register(s0);

// pixel shader �Լ�
float4 PS(PixelInput input) : SV_Target
{
    float4 result;
    float4 color = sourceTex.Sample(samp, input.uv);

  
    if(input.uv.x > 0.0f && input.uv.y > 0.0f)
        result = color;
    else 
        result = input.color;
    
    //if(_tileIndex == input.index)
    //{
    //    if ( input.uv2.x < 0.1f
    //        || input.uv2.x > 0.9f
    //        || input.uv2.y < 0.1f
    //        || input.uv2.y > 0.9f)
    //        result = float4(1, 0, 0, 1);
    //}
    
    if (_tileIndex != -1 && _tileIndex == input.index)
    {
        result = float4(1, 0, 0, 1);
    }
    
    
    return result;
    
    //color = saturate(color + input.color);
    //return color;
}