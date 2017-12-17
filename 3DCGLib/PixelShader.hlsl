cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 Light;
    float4 Attenuation;
}

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 PosW : POSITION0;
    float4 NormalW : NORMAL0;
};

float4 PS(PS_INPUT input) : SV_Target
{
    float3 dir;
    float len;
    float colD;
    float colA;
    float col;

    // �_�����̕���
    dir = Light.xyz - input.PosW.xyz;

    // �_�����̋���
    len = length(dir);

    // �_�����̕����𐳋K��
    dir = dir / len;

    // �g�U
    colD = saturate(dot(normalize(input.NormalW.xyz), dir));

    // ����
    colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));

    col = colD * colA;

    return float4(col, col, col, 1.0f);
}