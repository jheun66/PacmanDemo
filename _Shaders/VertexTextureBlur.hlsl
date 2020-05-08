//hlsl
// high level shader language 구수준 쉐이더 언어


struct VertexInput
{ // 시멘틱 이름
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{ // system value(sv) : system에서 쓰는 값
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


cbuffer BlurBuffer : register(b0)
{
    int _blurCount;
    float3 _textureSize;
}

Texture2D sourceTex : register(t0); // 0~127
SamplerState samp : register(s0);

// pixel shader 함수
float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0;
    color = sourceTex.Sample(samp, input.uv);
    
    // 방향
    float2 arr[8] =
    {
        float2(-1, -1), float2(0, -1), float2(1, -1),
        float2(-1, 0), float2(1, 0),
        float2(-1, 1), float2(0, 1), float2(1, 1)
    };
    
    // blur는 거리 크기 한 픽셀에서 0~5픽셀 거리에 있는 애들 전부 합치고 
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
    
    // 평균 내줌
    color /= _blurCount * 8 + 1;
    
    return float4(color.rgb, 1);
}