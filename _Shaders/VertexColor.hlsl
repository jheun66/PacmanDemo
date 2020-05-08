//hlsl
// high level shader language 구수준 쉐이더 언어


struct VertexInput
{                      // 시멘틱 이름
	float4 position : POSITION0;
	float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float4 color : COLOR0;
};

cbuffer WorldBuffer : register(b0) // 슬롯은 최대 13까지 있음(14개)
{
    //// 열우선 행렬을 row_mojor하면 행우선으로 바뀜
    //row_major matrix _world;
    //row_major matrix _view;
    //row_major matrix _proj;
    matrix _world;
}

cbuffer ViewProjection : register(b1) // 슬롯은 최대 13까지 있음(14개)
{
    matrix _view;
    matrix _proj;
}
// vertex shader 함수
PixelInput VS(VertexInput input)
{
	PixelInput output;

    // 해당 공간으로 이동된 위치벡터 = 위치벡터 * 공간 행렬
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _proj);
    // NDC 좌표계
    
    
	output.color = input.color;

	return output;
}

// pixel shader 함수
float4 PS(PixelInput input) : SV_Target
{
	return input.color;
}