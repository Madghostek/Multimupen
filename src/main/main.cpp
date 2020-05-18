#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <vector>
#include "../types.hpp"
#include <thread>
#include <time.h>
HWND hDebugwnd;
std::vector <HWND> hMupenwnd;
BOOL stopThread = FALSE;

//winapi controls
HWND hCheckKeyup, hBtnRefresh, hBtnConfig, hCheckForce, hTextPlugininfo , hTextStatus;
//types from mupen api
typedef void (*Type_GetDllInfo)(PLUGIN_INFO *PluginInfo);
typedef void (*Type_DllConfig)(HWND hParent);
typedef void( *Type_old_initiateController)(HWND,CONTROL[4]);
typedef void(*Type_RomOpen)(void);
typedef void(*Type_GetKeys)(int, BUTTONS *buttons);
Type_GetDllInfo GetDllInfo;
Type_DllConfig DllConfig;
Type_old_initiateController Init;
Type_RomOpen RomOpen;
Type_GetKeys GetKeys;

CONTROL ctrl[4];

BOOL CALLBACK CheckIfMupen(HWND hwnd,LPARAM lparam)
{
    char cname[MAX_PATH];
    GetClassName(hwnd, cname, sizeof(cname));
    if (strcmp(cname,"myWindowClass")==0)  //cool class name from tutorial btw
    {
        hMupenwnd.push_back(hwnd);
    }
    return TRUE;
}

void RefreshMupens(BOOL msg)
{
    hMupenwnd.clear();
    EnumWindows(CheckIfMupen,0);
    char buf[32];
    sprintf(buf,"Disconnected");
    if (hMupenwnd.size()!=0)
    {
        sprintf(buf,"Found %i mupen instance(s)",hMupenwnd.size());
        MessageBox(0,buf,"Info",64);
        sprintf(buf,"%i connected",hMupenwnd.size());
    }
    else if(msg)
    {
        MessageBoxA(0,"Mupen not found.","Error",0x30);
    }
    SendMessage(hTextStatus,WM_SETTEXT,0,(LPARAM)buf);
}
//Sends information to all mupen windows
void SendData(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int cycle = 0; //debug
    BUTTONS Data;
    GetKeys(0,&Data);

    //prepare data to send with WM_COPYDATA
    COPYDATASTRUCT cds;
    cds.lpData = &Data.Value;
    cds.dwData = 1;
    cds.cbData = sizeof(DWORD);
    printf("---\n");
    for (HWND handle : hMupenwnd)
    {
        printf("window #%i, key: %i, uMsg: %i\n",cycle++,wParam,uMsg);
        if (!IsWindow(handle))
        {
            MessageBox(0,"Mupen window not found. Did you close it?","Error",0x30L);
            hMupenwnd.clear();
            return;
        }
        hDebugwnd = GetWindow(handle,GW_CHILD); //its the plugin window that reads data because I'm not recompiling mupen
        if (!hDebugwnd)
        {
            MessageBox(0,"Couldn't communicate with receiver plugin, make sure it's selected in mupen settings.","Error",0x30L);
            hMupenwnd.clear();
            return;
        }
        switch (uMsg)
        {
            //If this is done inside receiver then mupen reads garbage key data because windows is buggy and sucks.
            //PostMessage fixes hotkeys problem
            //Note: order of COPYDATA and KEYDOWN/UP is important
            case WM_KEYUP:
            case WM_KEYDOWN: SendMessage(hDebugwnd,WM_COPYDATA,(WPARAM)hwnd,(LPARAM)(PVOID)&cds); PostMessage(handle,uMsg,wParam,lParam);
            break;
            case WM_COMMAND: PostMessage(handle,WM_COMMAND,1063,0);
            break;
            case WM_COPYDATA: SendMessage(hDebugwnd,WM_COPYDATA,(WPARAM)hwnd,(LPARAM)(PVOID)&cds);
            break;
        }
        hDebugwnd = NULL;
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
        {
            //hwnd is not constant so no switch case?
            if ((HWND)lParam==hBtnRefresh) RefreshMupens(1);
            else if ((HWND)lParam==hBtnConfig) DllConfig(hwnd);
        }
        break;
        //useless
        //case WM_LBUTTONDOWN: SetCapture(hwnd); break;
        //case WM_LBUTTONUP: ReleaseCapture(); break;
        //case WM_MOUSEMOVE:
        break;
        case WM_KEYUP:
        {
            SendData(hwnd, uMsg, wParam, lParam);
        }
        break;
        case WM_KEYDOWN:
        {
            if (!IsDlgButtonChecked(hwnd, CHK_REP) || !(lParam&0x40000000)) //disable holding keys, reduces desync probability
            {
                SendData(hwnd, WM_KEYDOWN, wParam, lParam);
            }
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
        default:
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return 0;
}
//I don't care if this is bad I cba to implement joystick detection
//update: its bad and desyncs like crazy but I'll leave it for now
void loop(HWND hwnd)
{
    while (!stopThread)
    {
        if (IsDlgButtonChecked(hwnd, CHK_FORCE) && hMupenwnd.size()) //if checked send joystick info
        {
            SendData(hwnd,WM_COPYDATA,0,0);
            Sleep(10);
        }
        else
        {
            Sleep(200); //so it doesn't completely lag the system
        }
    }
    return;
}

void SetFont(std::vector <HWND> hwnds,HFONT font)
{
    for (HWND handle : hwnds)
    {
        SendMessage(handle, WM_SETFONT, (WPARAM)font, FALSE);
    }
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //Load TASinput
    HINSTANCE hTAS=LoadLibraryA(".\\TASDI.dll");
    if (hTAS==NULL)
    {
        MessageBoxA(0,"TASDI.dll not found","Error",0x30L);
        return -1;
    }

    PLUGIN_INFO PluginInfo;
    //find functions in DLL
    GetDllInfo = (Type_GetDllInfo)GetProcAddress(hTAS, "GetDllInfo");
    RomOpen = (Type_RomOpen)GetProcAddress(hTAS,"RomOpen");
    Init = (Type_old_initiateController)GetProcAddress(hTAS,"InitiateControllers");
    DllConfig = (Type_DllConfig)GetProcAddress(hTAS, "DllConfig");
    GetKeys = (Type_GetKeys)GetProcAddress(hTAS, "GetKeys");

    GetDllInfo(&PluginInfo);

    InitCommonControls();
    const char CLASS_NAME[]  = "InputSenderClass";
    MSG msg;

    //generated (it could be done with .rc too)
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof wcex);
    wcex.cbSize         = sizeof wcex;
    wcex.hbrBackground  = (HBRUSH)(COLOR_3DFACE + 1);
    wcex.lpszMenuName   = 0;


    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WindowProc;
    wcex.hInstance      = hInst;
    wcex.hIcon          = LoadIcon(hInst, "POKEY");
    wcex.hIconSm        = LoadIcon(hInst, "POKEY");
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName  = CLASS_NAME;
    RegisterClassEx(&wcex);

    HFONT hfont0 = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    HWND hwnd = CreateWindowEx(WS_EX_LEFT, CLASS_NAME, "Multimupen", WS_CAPTION | WS_GROUP | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 290, 133, 0, 0, hInst, 0);
    hCheckKeyup = CreateWindowEx(0, WC_BUTTON, "Disable key repeat", WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 119, 42, 138, 13, hwnd, (HMENU)CHK_REP, hInst, 0);
    hBtnRefresh = CreateWindowEx(0, WC_BUTTON, "Refresh", WS_VISIBLE | WS_CHILD | WS_TABSTOP, 119, 11, 66, 23, hwnd, (HMENU)40001, hInst, 0);
    hBtnConfig = CreateWindowEx(0, WC_BUTTON, "Input plugin config", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_MULTILINE | 0x00000001, 11, 11, 90, 39, hwnd, (HMENU)40000, hInst, 0);
    hCheckForce = CreateWindowEx(0, WC_BUTTON, "Force input update (experimental)", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_MULTILINE | 0x00000003, 119, 59, 150, 28, hwnd, (HMENU)CHK_FORCE, hInst, 0);
    hTextStatus = CreateWindowEx(0, WC_STATIC, "Disconnected", WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT | SS_CENTERIMAGE, 192, 16, 68, 15, hwnd, (HMENU)TXT_STATUS, hInst, 0);
    char txt[200] = "Using: ";
    sprintf(txt + strlen(txt),PluginInfo.Name);
    hTextPlugininfo = CreateWindowEx(0, WC_STATIC, txt, WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 11, 55, 83, 31, hwnd, (HMENU)0, hInst, 0);
    //generated end
    std::vector<HWND> controls{hCheckKeyup,hCheckForce,hBtnRefresh,hBtnConfig,hTextStatus,hTextPlugininfo};
    SetFont(controls,hfont0);
    if (hwnd == NULL)
    {
        MessageBoxA(0,"Window creation failed???","Error",0x30L);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    RefreshMupens(0);

    Init(hwnd,ctrl); //initialize controllers
    RomOpen(); //tasinput starts running here
    std::thread thrd(loop,hwnd); //force input
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    stopThread = TRUE;
    thrd.join();
    return msg.wParam;
}
