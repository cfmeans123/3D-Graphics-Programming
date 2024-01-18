#include "Windows.h"
//#include <MEngine\inc\MEngine.h> get this working


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    App myApp;

    myApp.Run(2.0f);

    return(0);
}