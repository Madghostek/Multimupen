//config window
#include <windows.h>
#include <commctrl.h>
#include <fstream>
#include "../types.hpp"
#include "config.hpp"
HWND hwnd,hText,hCheckShowDebug,hBtnSave;

void Save()
{
    MessageBox(0,"You will need to restart mupen to see changes","Info",64);
    std::fstream cFile(".Receiver.cfg", std::fstream::out);
    cFile << "Show=" << IsDlgButtonChecked(hwnd, CHK_DBG);
    cFile.close();
    DestroyWindow(hwnd);
}

LRESULT CALLBACK ConfigProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
        {
            if ((HWND)lParam==hBtnSave) Save();
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
        default:
        {
            return DefWindowProc(hwnd, uMsg,wParam,lParam);
        }
    }
    return 0;
}

DWORD ConfigMain()
{
    InitCommonControls();
    MSG msg;
    HINSTANCE hInst = GetModuleHandle(0);
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof wcex);
    wcex.cbSize         = sizeof wcex;
    wcex.hbrBackground  = (HBRUSH)(COLOR_3DFACE + 1);
    wcex.lpszMenuName   = 0;


    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = ConfigProc;
    wcex.hInstance      = hInst;
    wcex.hIcon          = LoadIcon(0, (LPCTSTR)IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName  = "WndClass";
    RegisterClassEx(&wcex);

    HFONT hfont = CreateFont(-11, 0, 0, 0, 400, FALSE, FALSE, FALSE, 1, 400, 0, 0, 0, ("Ms Shell Dlg"));
    hwnd = CreateWindowEx(WS_EX_LEFT, "WndClass", "Config", WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 212, 91, 0, 0, hInst, 0);
    hText = CreateWindowEx(0, "STATIC", "Nothing important here ", WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 8, 41, 122, 15, hwnd, (HMENU)0, hInst, 0);
    SendMessage(hText, WM_SETFONT, (WPARAM)hfont, FALSE);
    hCheckShowDebug = CreateWindowEx(0, WC_BUTTON, "Show debug window", WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 11, 11, 119, 13, hwnd, (HMENU)CHK_DBG, hInst, 0);
    SendMessage(hCheckShowDebug, WM_SETFONT, (WPARAM)hfont, FALSE);
    hBtnSave = CreateWindowEx(0, WC_BUTTON, "Save", WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000001, 137, 8, 57, 23, hwnd, (HMENU)IDOK, hInst, 0);
    SendMessage(hBtnSave, WM_SETFONT, (WPARAM)hfont, FALSE);

    ShowWindow(hwnd,SW_SHOW);
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
    }
