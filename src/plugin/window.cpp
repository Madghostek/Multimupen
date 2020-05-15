//debug window that clogs up the screen
#include <windows.h>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <time.h>
#include "window.hpp"
#include "config.hpp"
HWND sender,label;
DWORD Value;
int id,msgCount = 0;

typedef struct {
    BOOL showDebug;
} CONFIG;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    break;
    case WM_CREATE:
    {
        label = CreateWindow("STATIC","Plugin loaded",WS_VISIBLE|WS_CHILD,5,5,100,20,hwnd,NULL,GetModuleHandle(0),NULL);
    }
    break;
    case WM_COPYDATA:
        {
            msgCount++;
            char txt[10];
            sprintf(txt,"count: %i",msgCount);
            SendMessage(label,WM_SETTEXT,0,(LPARAM)txt);
            COPYDATASTRUCT* data = (COPYDATASTRUCT*)lParam;
            Value = *(DWORD*)(data->lpData);
            if (!IsWindow(sender)) sender = FindWindow("InputSenderClass",0);
            char buf [30];
            sprintf(buf,"before, id: %i",id);
            //MessageBox(0,buf,"receiver",0);
            SendMessage(sender,ID_READY,0,0);
            if (IsWindow(sender))
            {
                sprintf(buf,"mupen exists, id: %i",id);
                //MessageBox(0,buf,"receiver",0);
            }
            else
            {
                sprintf(buf,"error id: %i",id);
                //MessageBox(0,buf,"receiver",0);
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
            return DefWindowProc(hwnd, uMsg,wParam,lParam);
        }
    }
    return 0;
}

void ReadConfig(CONFIG*cfg)
{
    std::fstream cFile(".Receiver.cfg");
    if (!cFile.is_open())
    {
        cFile.open(".Receiver.cfg", std::fstream::out);
        cFile << "Show=0";
        cfg->showDebug = 0;
    }
    else{
        std::string show;
        getline (cFile, show);
        std::string val = show.substr(show.find("=")+1,1);
        cfg->showDebug = val == "1";
        cFile.close();
    }
}

DWORD WindowMain(HINSTANCE h)
{
    CONFIG cfg;
    ReadConfig(&cfg);
    const char CLASS_NAME[]  = "ReceiverClass";
    //MSG msg;
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = h;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "debug",    // Window text
        WS_CHILD|WS_VISIBLE,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, 200,50, // Size and position
        FindWindow("myWindowClass",0), //parent
        NULL,       // Menu
        h,  // Instance handle (main thread)
        NULL        // Additional application data
    );
    std::srand(time(0));
    id=std::rand()%1000;
    ShowWindow(hwnd,cfg.showDebug ? SW_SHOW : SW_HIDE);
    //this will be handled by mupen!
    //while(GetMessage(&msg, NULL, 0, 0) > 0)
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}
    //return msg.wParam;
    return 0;
}
