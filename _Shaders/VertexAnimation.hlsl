//hlsl
// high level shader language 구수준 쉐이더 언어


struct VertexInput
{                      // 시멘틱 이름
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{              // system value(sv) : system에서 쓰는 값
	float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer WorldBuffer : register(b0) // 슬롯은 최대 13까지 있음(14개)
{
    matrix _world;
}

cbuffer VPBuffer : register(b1)
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
    
    output.uv = input.uv;

	return output;
}

Texture2D sourceTex : register(t0); // 0~127
SamplerState samp : register(s0);

// pixel shader 함수
float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0;
    
    color = sourceTex.Sample(samp, input.uv);
    //if(color.r == 1 && color.g == 0 && color.b == 1)
    //    discard;
    
    //clip(color.a - 0.9);
    
    return color;
}