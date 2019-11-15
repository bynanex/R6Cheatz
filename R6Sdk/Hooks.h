#pragma once
#include "Cheat.h"
#include <string>
#include "math\Vector2D.hpp"
using namespace std;
typedef HRESULT(__stdcall* tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
namespace Hooks
{
	extern HRESULT PresentHooked(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT flags);
	extern LRESULT hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	extern void InitHook();
}

template <typename T> T HookVirtual(void* func, T Hook)
{
	DWORD oldProtect = 0;
	VirtualProtectEx(GetCurrentProcess(), func, 4, PAGE_EXECUTE_READWRITE, &oldProtect);

	T original = *(T*)func;
	*(T*)func = Hook;

	VirtualProtectEx(GetCurrentProcess(), func, 4, oldProtect, &oldProtect);

	return original;
}