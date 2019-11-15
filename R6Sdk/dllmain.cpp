// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "Cheat.h"
typedef struct
{
	DWORD64 dwEP;
	void* pParam;
}CALL_MYFUNCTION, * PCALL_MYFUNCTION;
typedef DWORD(*_Function)(VOID* p);
DWORD GetModuleSize(DWORD64 base)
{
	IMAGE_DOS_HEADER dos_header = { 0 };
	IMAGE_NT_HEADERS nt_headers = { 0 };
	if (!base)return -1;
	dos_header = *(IMAGE_DOS_HEADER*)base;
	nt_headers = *(IMAGE_NT_HEADERS*)(base + dos_header.e_lfanew);
	return nt_headers.OptionalHeader.SizeOfImage;
}
void WINAPI MyFunctionThread(PCALL_MYFUNCTION pCMF)
{
	if (pCMF != NULL && pCMF->dwEP != NULL)
	{
		_Function Function = (_Function)pCMF->dwEP;
		Function(pCMF->pParam);
	}
}
HANDLE MyCreateThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, LPDWORD lpThreadId)
{
	HMODULE hNtDll = GetModuleHandleA("ntdll.dll");
	if (hNtDll != NULL)
	{
		DWORD dwImageSize = GetModuleSize((DWORD64)hNtDll);
		DbgPrintA("dwImageSize:%d", dwImageSize);
		BYTE* pMemoryData = (BYTE*)hNtDll + dwImageSize - 0x100;

		if (pMemoryData != NULL)
		{
			DWORD dwProtect;
			VirtualProtect(pMemoryData, 0x100, PAGE_EXECUTE_READWRITE, &dwProtect);
			CALL_MYFUNCTION* pCMF = (CALL_MYFUNCTION*)VirtualAlloc(NULL, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			pCMF->dwEP = (DWORD64)(lpStartAddress);
			pCMF->pParam = lpParameter;
			memcpy((LPVOID)pMemoryData, (LPVOID)MyFunctionThread, 0x100);
			HANDLE hHandle = CreateRemoteThread(GetCurrentProcess(), NULL, 0, (LPTHREAD_START_ROUTINE)pMemoryData, pCMF, NULL, lpThreadId);
			return hHandle;
		}
	}
	return 0;
}
void WINAPI InitGame()
{
	DbgPrintA("提示: InitGame!");
	while (TRUE)
	{
		G::GameWnd = FindWindowA(NULL, XorString("Rainbow Six"));
		if (G::GameWnd != NULL) {
			break;
		}
		Sleep(100);
	}
	Sleep(21000); //驱动注入必须加这个
	G::GameWnd = FindWindowA(NULL, XorString("Rainbow Six"));
	OffsetsManger::Get().InstallOffset();
	DbgPrintA("InstallOffset Finish Handle: 0x%08X", G::GameWnd);
	Hooks::InitHook();
	DbgPrintA("成功启动!");
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		G::MoudleBase = hModule;
		G::GameImageBase = (DWORD64)GetModuleHandleA(NULL);
		G::MoudleSize = GetModuleSize((DWORD64)hModule);
		MH_Initialize();
		//InitGame();
		MyCreateThread((LPTHREAD_START_ROUTINE)InitGame, NULL, NULL);
		DbgPrintA("R6注入成功! MoudleBase: 08%08X", G::MoudleBase);
	}
	break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

