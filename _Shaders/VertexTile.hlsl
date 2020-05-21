//hlsl
// high level shader language 구수준 쉐이더 언어


struct VertexInput
{ // 시멘틱 이름
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float2 uv2 : TEXCOORD1;
    float4 color : COLOR0;
    uint index : INDEX0;
};

struct PixelInput
{              // system value(sv) : system에서 쓰는 값
	float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float2 uv2 : TEXCOORD1;
    float4 color : COLOR0;
    uint index : INDEX0;
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

// pixel shader 함수
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