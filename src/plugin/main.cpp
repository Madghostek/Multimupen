//define exports etc
#include <windows.h>
#include <cstdio>
#include "window.hpp"
#include "../types.hpp"
#include "config.hpp"
#define exp extern "C" __declspec(dllexport)

int NUMBER_OF_CONTROLS = 1;
BUTTONS table;

exp void GetDllInfo(PLUGIN_INFO * PluginInfo)
{
    PluginInfo->Type = 4;
    PluginInfo->Version = 1;
    sprintf(PluginInfo->Name,"Receiver Plugin");
    PluginInfo->NormalMemory  = FALSE;
    PluginInfo->MemoryBswaped = FALSE;
}

exp void DllAbout(HWND hwnd){
    MessageBox(hwnd,"TAS input receiver plugin.\n\n Author: Madghostek","About",64);
}
exp void DllConfig(HWND hwnd){
    ConfigMain();
}
exp void DllTest(HWND hwnd)
{
    MessageBox(hwnd,"Yoooooo","Yo",64);
}

exp void InitiateControllers(HWND hMainWindow, CONTROL Controls[4]){
    Controls[0].Present = 1;
}
//poll input, place data into Keys->value
exp void GetKeys(int Control, BUTTONS * Keys )
{
	if(Control >= 0 && Control < NUMBER_OF_CONTROLS)// && Controller[Control].bActive)
		Keys->Value = Value;
	else
		Keys->Value = 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        WindowMain(hinstDLL);
    }
    return TRUE;
}
