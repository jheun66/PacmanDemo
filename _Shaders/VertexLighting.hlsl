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
    
    // sqrt 제곱근, pow 지수
    float dist = sqrt(pow(dx, 2) + pow(dy, 2));
    
    if(dist < 0.2f)
        return (0.2f - dist) / 0.2f;
    else
        return 0.0f;
}


// pixel shader 함수
float4 PS(PixelInput input) : SV_Target
{
    float4 color = 0;
    color = sourceTex.Sample(samp, input.uv);
    float amount = GetLightAmount(input.uv, _mPos.xy);
    
    amount += GetLightAmount(input.uv, float2(0.3f, 0.4f));
    amount = clamp(amount, 0.0f, 1.0f); //amount = saturate(amount); 와 동일
    
    color.rgb *= amount;
   
    return color;
}