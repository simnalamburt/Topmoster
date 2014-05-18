#include "pch.h"
#include "ErrorReporter.h"



HINSTANCE InstanceHandle;

LPCWSTR ClassName = L"TOPMOSTER";

HWND WindowHandle;
LPCWSTR WindowTitle = L"Topmoster";
LONG WindowWidth = 200;
LONG WindowHeight = 30;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HWND ButtonHandle;
LPCWSTR ButtonWaiting = L"저를 누르세요";
LPCWSTR ButtonSelecting = L"윈도우를 고르세요";
wchar_t ButtonSelected[32] = L"";

enum class Status { Waiting, Selecting, Selected };
Status CurrentStatus;



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
    FALSE_ERROR(ButtonHandle = CreateWindow(L"BUTTON", ButtonWaiting,
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
        switch (CurrentStatus)
        {
        case Status::Waiting:
            EnableWindow(ButtonHandle, FALSE);
            SetWindowText(ButtonHandle, ButtonSelecting);
            CurrentStatus = Status::Selecting;
            return 0;
        case Status::Selecting:
            EnableWindow(ButtonHandle, TRUE);
            SetWindowText(ButtonHandle, ButtonSelected);
            CurrentStatus = Status::Selected;
            InvalidateRect(WindowHandle, nullptr, TRUE);
            return 0;
        }
        break;
    case WM_PAINT:
        switch (CurrentStatus)
        {
        case Status::Selecting:
            auto border = 3;
            auto hwnd = WindowHandle;

            //window rectangle (screen coords)
            RECT rect;
            GetWindowRect(hwnd, &rect);

            //client rectangle (screen coords)
            RECT rectc;
            GetClientRect(hwnd, &rectc);
            ClientToScreen(hwnd, (POINT *)&rectc.left);
            ClientToScreen(hwnd, (POINT *)&rectc.right);
            //MapWindowPoints(hwnd, 0, (POINT *)&rectc, 2);

            int x1 = rect.left, y1 = rect.top;
            OffsetRect(&rect, -x1, -y1);
            OffsetRect(&rectc, -x1, -y1);

            if (rect.bottom - border * 2 < 0)
                border = 1;

            if (rect.right - border * 2 < 0)
                border = 1;

            auto hdc = GetWindowDC(hwnd);
            if (hdc == 0) break;

            //top edge
            //border = rectc.top-rect.top;
            RECT rect2 = { 0, 0, rect.right, border };
            InvertRect(hdc, &rect2);

            //left edge
            //border = rectc.left-rect.left;
            SetRect(&rect2, 0, border, border, rect.bottom);
            InvertRect(hdc, &rect2);

            //right edge
            //border = rect.right-rectc.right;
            SetRect(&rect2, border, rect.bottom - border, rect.right, rect.bottom);
            InvertRect(hdc, &rect2);

            //bottom edge
            //border = rect.bottom-rectc.bottom;
            SetRect(&rect2, rect.right - border, border, rect.right, rect.bottom - border);
            InvertRect(hdc, &rect2);


            ReleaseDC(hwnd, hdc);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(WindowHandle, Message, wParam, lParam);
}
