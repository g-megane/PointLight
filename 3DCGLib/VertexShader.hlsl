cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 Light;
    float4 Attenuation;
}

struct VS_INPUT
{
    float4 Pos    : POSITION0;
    float4 Normal : NORMAL0;
};

struct VS_OUTPUT
{
    float4 Pos  : SV_POSITION;
    float4 PosW : POSITION0;
    float4 Normalw : NORMAL0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    output.PosW = mul(input.Pos, World);
    output.Pos  = mul(output.PosW, View);
    output.Pos  = mul(output.Pos, Projection);

    output.Normalw = mul(input.Normal, World);

    return output;
}