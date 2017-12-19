cbuffer ConstantBuffer : register(b0)
{
    matrix World;       // ワールド行列
    matrix View;        // ビュー行列
    matrix Projection;  // 射影行列
    float4 Light;       // 光源座標
    float4 Attenuation; // 光源減衰パラメータ
}

struct VS_INPUT
{
    float4 Pos    : POSITION0;
    float4 Normal : NORMAL0;
};

struct VS_OUTPUT
{
    float4 Pos  : SV_POSITION; // 各行列の変換を終えた座標
    float4 PosW : POSITION0;   // ワールド座標系の座標
    float4 Normalw : NORMAL0;  // ワールド座標系の法線
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