#include <windows.h>
#include <iostream>

#include "Core/Core.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    using namespace hvrt;
    Window window(hInst, L"window1", 1280, 720);

    while(window.open)
    {
    }
    
    return 0;
}
