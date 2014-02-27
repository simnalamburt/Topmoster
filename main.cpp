#include "pch.h"
#include "ErrorReporter.h"



HINSTANCE InstanceHandle;

LPCWSTR ClassName = L"TOPMOSTER";

HWND WindowHandle;
LPCWSTR WindowTitle = L"Topmoster";
LONG WindowWidth = 200;
LONG WindowHeight = 30;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HWND Button;
LPCWSTR ButtonTitle = L"Select Window";



int WINAPI wWinMain(HINSTANCE InstanceHandle, HINSTANCE, PWSTR, int ShowCommand)
{
    ::InstanceHandle = InstanceHandle;


    //// Register a window class for subsequent use in calls to the CreateWindow
    WNDCLASSEX wcex = {};
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProcedure;
    wcex.lpszClassName = ClassName;
    wcex.hInstance = InstanceHandle;
    wcex.hIcon = wcex.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.cbSize = sizeof(wcex);
    FALSE_ERROR(RegisterClassEx(&wcex));


    //// Create a window
    DWORD windowStyle = WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION;
    // Calculate desired size of the window
    RECT emptyRect = { 0, 0, WindowWidth, WindowHeight };
    FALSE_WARNING(AdjustWindowRect(&emptyRect, windowStyle, FALSE));
    // Create the window
    FALSE_ERROR(WindowHandle = CreateWindow(ClassName, WindowTitle, windowStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, emptyRect.right - emptyRect.left, emptyRect.bottom - emptyRect.top,
        nullptr, nullptr, InstanceHandle, nullptr));
    FALSE_ERROR(Button = CreateWindow(L"BUTTON", ButtonTitle,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        0, 0, WindowWidth, WindowHeight,
        WindowHandle, nullptr, InstanceHandle, nullptr));


    //// Run
    ShowWindow(WindowHandle, ShowCommand);
    FALSE_WARNING(SetForegroundWindow(WindowHandle));
    FALSE_WARNING(SetFocus(WindowHandle));

    MSG message;
    BOOL result;
    while ((result = GetMessage(&message, NULL, 0, 0)) != 0)
    {
        FALSE_ERROR(result != -1);
        TranslateMessage(&message);
        DispatchMessage(&message);
    }


    //// Cleanup
    if (InstanceHandle)
    {
        FALSE_WARNING(UnregisterClass(ClassName, InstanceHandle));
    }

    return message.wParam;
}



LRESULT CALLBACK WindowProcedure(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
    case WM_COMMAND:
        MessageBox(WindowHandle, L"버튼 눌려짐!", L"테스트", MB_OK);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(WindowHandle, Message, wParam, lParam);
    }
}
