#include <Windows.h>
#include "Window.h"
#include "DirectX11.h"
#include "Model.h"
#include "Matrix.h"
#include "MyMath.h"

using namespace Lib;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    // ウィンドウの作成
    auto w = std::make_shared<Lib::Window>(L"3DCGLib", 1026, 768);

    // DirectXのインスタンスの取得
    auto& directX = Lib::DirectX11::getInstance();
    directX.initDevice(w);

    // ViewMatrixの初期化
    Vector3 eye = Vector3(0.0f, 2.0f, -2.0f); // カメラの座標
    Vector3 at  = Vector3(0.0f, 1.0f,  0.0f); // 注視対象
    Vector3 up  = Vector3::UP;                // 現在のワールド座標の上方向
    auto view   = Matrix::LookAtLH(eye, at, up);
    directX.setViewMatrix(view);

    // ProjectionMatrixの初期化
    LONG windowWidth  = w->getWindowRect().right  - w->getWindowRect().left;
    LONG windowHeight = w->getWindowRect().bottom - w->getWindowRect().top;
    auto projection   = Matrix::perspectiveFovLH(MyMath::PIDIV2, windowWidth / static_cast<float>(windowHeight), 0.01f, 100.0f);
    directX.setProjectionMatrix(projection);
    
    // モデルの作成
    Model model = Model();
    Matrix world;
    world = Matrix::Identify;
    model.setWorldMatrix(world);

    float posX = 0.0f;
    float posY = 0.0f;
    float posZ = 0.0f;

    // 説明
    MessageBox(w->getHWND(), L"「W」「A」「S」「D」でモデルの回転", L"操作説明", MB_OK | MB_ICONINFORMATION);

    // 更新処理
    while (w->Update().message != WM_QUIT) {
        directX.begineFrame();

        // 移動        
        posX = posY = posZ = 0.0f;

        if (w->getKeyDown('W')) {
            posZ = 0.01f;
        }
        else if (w->getKeyDown('S')) {
            posZ = -0.01f;
        }
        if (w->getKeyDown('A')) {
            posX = -0.01f;
        }
        else if (w->getKeyDown('D')) {
            posX = 0.01f;
        }
        if (w->getKeyDown('E')) {
            posY = 0.01f;
        }
        else if (w->getKeyDown('Q')) {
            posY = -0.01f;
        }

        // オーバーフローの制御
        //posX = MyMath::clamp<float>(posX, -10.0f, 10.0f);
        //posZ = MyMath::clamp<float>(posZ, -10.0f, 10.0f);

        // ライトのモデルの制御
        model.getLightPos().translate(posX, posY, posZ);

        // 描画
        model.render(Lib::Color(Lib::Color::BLUE));

        directX.endFrame();
    }

    return 0;
}