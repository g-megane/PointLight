#include <d3dcompiler.h>
#include "Model.h"
#include "MyMath.h"

namespace Lib
{
    // コンストラクタ
    Model::Model()
    {
        world = Matrix::Identify;
        lightPos = Vector3(0.0f, 2.0f, 0.0f);
        init();
    }

    // デストラクタ
    Model::~Model()
    {
    }

    // モデルの描画
    void Model::render(Color &color)
    {
        auto &directX = DirectX11::getInstance();

        // 光源の座標
        float light[4]       = { lightPos.x, lightPos.y, lightPos.z, 0.0f };
        // 光源の距離による減衰パラメータ
        float attenuation[4] = 
        { 
            0.1f, // 一定減衰係数（小さくするとライトの最大の明るさが増える）
            0.5f, // 線形減衰係数（光源付近の減衰を調整）
            0.1f, // 2次減衰係数（遠方の減衰を微調整）
            0.0f
        }; 

        // ライティングされるBoxとFloor
        ConstantBuffer cb;
        cb.world      = Matrix::transpose(world);
        cb.view       = Matrix::transpose(directX.getViewMatrix());
        cb.projection = Matrix::transpose(directX.getProjectionMatrix());
        memcpy(cb.light, light, sizeof(light));
        memcpy(cb.attenuation, attenuation, sizeof(attenuation));
        directX.getDeviceContext()->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

        directX.getDeviceContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
        directX.getDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
        directX.getDeviceContext()->PSSetShader(pixelShader.Get(), nullptr, 0);
        directX.getDeviceContext()->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
        directX.getDeviceContext()->DrawIndexed(42, 0, 0);


        // ライトの位置を示すオブジェクト
        auto mtLight  = Matrix::Identify;
        auto mttLight = Matrix::translate(Vector3(light[0], light[1], light[2]));
        auto mtsLight = Matrix::scale(0.1f, 0.1f, 0.1f);
        mtLight = mtsLight * mttLight;
       
        cb.world      = Matrix::transpose(mtLight);
        directX.getDeviceContext()->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

        directX.getDeviceContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
        directX.getDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
        directX.getDeviceContext()->PSSetShader(pixelShader.Get(), nullptr, 0);
        directX.getDeviceContext()->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
        directX.getDeviceContext()->DrawIndexed(36, 0, 0);
    }

    // ワールド行列を設定
    void Model::setWorldMatrix(Matrix & _world)
    {
        world = _world;
    }

    // ワールド行列を取得
    Matrix Model::getWorldMatrix() const
    {
        return world;
    }

    Vector3& Model::getLightPos()
    {
        return lightPos;
    }

    // 初期化
    HRESULT Model::init()
    {
        auto &directX = DirectX11::getInstance();
        auto hr = S_OK;

        // VertexShaderの読み込み
        auto VSBlob = shaderCompile(L"VertexShader.hlsl", "VS", "vs_4_0");
        if (VSBlob == nullptr) {
            MessageBox(nullptr, L"shaderCompile()の失敗(VS)", L"Error", MB_OK);
            return hr;
        }

        // VertexShaderの作成
        hr = directX.getDevice()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"VSコンパイル失敗", L"Error", MB_OK);
            return hr;
        }

        // InputLayouの定義
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            {   "NORMAL", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        // InputLayoutの作成
        hr = directX.getDevice()->CreateInputLayout(layout, numElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), vertexLayout.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"CreateInputLayoutの失敗", L"Error", MB_OK);
            return hr;
        }

        // InputLayoutをセット
        directX.getDeviceContext()->IASetInputLayout(vertexLayout.Get());

        // PixelShaderの読み込み
        auto PSBlob = shaderCompile(L"PixelShader.hlsl", "PS", "ps_4_0");
        if (PSBlob == nullptr) {
            MessageBox(nullptr, L"shaderCompile()の失敗(PS)", L"Error", MB_OK);
            return hr;
        }

        // VertexShaderの作成
        hr = directX.getDevice()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createPixelShader()の失敗", L"Error", MB_OK);
            return hr;
        }

        // VertexBufferの定義
        SimpleVertex vertices[] =
        {
            { { -0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
            { {  0.5f,  0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
            { { -0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
            { {  0.5f, -0.5f, -0.5f }, {  0.0f,  0.0f, -1.0f } },
 
            { { -0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
            { { -0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
            { {  0.5f,  0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
            { {  0.5f, -0.5f,  0.5f }, {  0.0f,  0.0f,  1.0f } },
 
            { { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } },
            { { -0.5f,  0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } },
            { { -0.5f, -0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f } },
            { { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f } },
 
            { {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } },
            { {  0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f } },
            { {  0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } },
            { {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f } },
 
            { { -0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } },
            { {  0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f } },
            { { -0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } },
            { {  0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f } },
 
            { { -0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } },
            { { -0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } },
            { {  0.5f, -0.5f,  0.5f }, {  0.0f, -1.0f,  0.0f } },
            { {  0.5f, -0.5f, -0.5f }, {  0.0f, -1.0f,  0.0f } },
 
 
            { { -5.0f, -0.5f,  5.0f }, {  0.0f,  1.0f,  0.0f } },
            { {  5.0f, -0.5f,  5.0f }, {  0.0f,  1.0f,  0.0f } },
            { { -5.0f, -0.5f, -5.0f }, {  0.0f,  1.0f,  0.0f } },
            { {  5.0f, -0.5f, -5.0f }, {  0.0f,  1.0f,  0.0f } },
        };

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 28;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        ZeroMemory(&initData, sizeof(initData));
        initData.pSysMem = vertices;
        hr = directX.getDevice()->CreateBuffer(&bd, &initData, vertexBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()の失敗", L"Error", MB_OK);
            return hr;
        }

        // VertexBufferをセット
        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;
        directX.getDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

        // 頂点バッファの作成
        WORD indices[] =
        {
            0,  1,  2,
            3,  2,  1,
           
            4,  5,  6,
            7,  6,  5,
           
            8,  9, 10, 
            11, 10,  9,

            12, 13, 14,
            15, 14, 13,

            16, 17, 18,
            19, 18, 17,

            20, 21, 22,
            23, 22, 21, 
 
            24, 25, 26,
            27, 26, 25, 
        };
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(WORD) * 42; // 36頂点、12三角形
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        initData.pSysMem = indices;
        hr = directX.getDevice()->CreateBuffer(&bd, &initData, indexBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()の失敗", L"Error", MB_OK);
            return hr;
        }

        // 頂点バッファをセット
        directX.getDeviceContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        // PrimitiveTopologyをセット
        directX.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // ConstantBufferの作成
        bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.ByteWidth      = sizeof(ConstantBuffer);
        bd.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        hr = directX.getDevice()->CreateBuffer(&bd, nullptr, constantBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()の失敗", L"Error", MB_OK);
            return hr;
        }

        return S_OK;
    }

    // シェーダーの読み込み
    Microsoft::WRL::ComPtr<ID3DBlob> Model::shaderCompile(WCHAR * filename, LPCSTR entryPoint, LPCSTR shaderModel)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> blobOut = nullptr;
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

        DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
        shaderFlags |= D3DCOMPILE_DEBUG;
#endif

        auto hr = D3DCompileFromFile(
            filename,
            nullptr,
            nullptr,
            entryPoint,
            shaderModel,
            shaderFlags,
            0,
            blobOut.GetAddressOf(),
            errorBlob.GetAddressOf()
        );

        if (FAILED(hr)) {
            if (errorBlob != nullptr) {
                MessageBox(nullptr, static_cast<LPWSTR>(errorBlob->GetBufferPointer()), nullptr, MB_OK);
            }
            if (errorBlob) {
                errorBlob.Get()->Release();
            }
        }
        if (errorBlob) {
            errorBlob.Get()->Release();
        }

        return Microsoft::WRL::ComPtr<ID3DBlob>(blobOut);
    }
}