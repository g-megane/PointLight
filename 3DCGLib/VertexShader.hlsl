cbuffer ConstantBuffer : register(b0)
{
    matrix World;       // ���[���h�s��
    matrix View;        // �r���[�s��
    matrix Projection;  // �ˉe�s��
    float4 Light;       // �������W
    float4 Attenuation; // ���������p�����[�^
}

struct VS_INPUT
{
    float4 Pos    : POSITION0;
    float4 Normal : NORMAL0;
};

struct VS_OUTPUT
{
    float4 Pos  : SV_POSITION; // �e�s��̕ϊ����I�������W
    float4 PosW : POSITION0;   // ���[���h���W�n�̍��W
    float4 Normalw : NORMAL0;  // ���[���h���W�n�̖@��
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