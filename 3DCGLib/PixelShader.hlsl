cbuffer ConstantBuffer : register(b0)
{
    matrix World;       // ワールド行列
    matrix View;        // ビュー行列
    matrix Projection;  // 射影行列
    float4 Light;       // 光源座標
    float4 Attenuation; // 光源減衰パラメータ
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

    // 点光源の方向
    dir = Light.xyz - input.PosW.xyz;

    // 点光源の距離
    len = length(dir);

    // 点光源の方向を正規化
    dir = normalize(dir);

    // 拡散(「点光源の方向」と「法線の方向」の内積から明るさを算出)
    colD = saturate(dot(normalize(input.NormalW.xyz), dir));

    // 減衰
    colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));

    col = colD * colA;

    return float4(col, col, col, 1.0f);
}