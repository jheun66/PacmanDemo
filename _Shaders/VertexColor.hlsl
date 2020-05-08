//hlsl
// high level shader language ������ ���̴� ���


struct VertexInput
{                      // �ø�ƽ �̸�
	float4 position : POSITION0;
	float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float4 color : COLOR0;
};

cbuffer WorldBuffer : register(b0) // ������ �ִ� 13���� ����(14��)
{
    //// ���켱 ����� row_mojor�ϸ� ��켱���� �ٲ�
    //row_major matrix _world;
    //row_major matrix _view;
    //row_major matrix _proj;
    matrix _world;
}

cbuffer ViewProjection : register(b1) // ������ �ִ� 13���� ����(14��)
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
    
    
	output.color = input.color;

	return output;
}

// pixel shader �Լ�
float4 PS(PixelInput input) : SV_Target
{
	return input.color;
}