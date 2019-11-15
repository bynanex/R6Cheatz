#include "pch.h"
#include "Cheat.h"

HINSTANCE G::MoudleBase;
DWORD64 G::GameImageBase;
HWND G::GameWnd;
DWORD64 G::MoudleSize;
CBaseEntity* G::LocalPlayer;
bool G::PressedKeys[256];
Variables Vars;