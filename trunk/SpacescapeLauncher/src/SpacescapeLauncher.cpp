// SpacescapeLauncher.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SpacescapeLauncher.h"
#include <process.h>

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	_execl("app\\win\\release\\Spacescape.exe", "app\\win\\release\\Spacescape.exe", NULL);
}