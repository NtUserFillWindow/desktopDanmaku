#include <windows.h>
#include "main.hpp"

int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                    [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{

    danmaku::mainWindow mainWindowObj;
    mainWindowObj.create(L"桌面弹幕", 500, 300).show();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}