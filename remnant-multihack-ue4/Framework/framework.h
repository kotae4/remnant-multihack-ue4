#pragma once

#include "../imgui/imgui.h"
#include "../minhook/MinHook.h"
#include <dxgi.h>

enum class ETargetEngine
{
	Auto,
	UnrealEngine4,
	Unity
};

typedef void(__cdecl* dDrawFunc)(ImDrawList* overlayDrawList);
typedef bool(__cdecl* dWndProc)(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class CheatFramework
{
public:
	static bool Init(HMODULE mod, ETargetEngine targetEngine = ETargetEngine::UnrealEngine4);
	static void RegisterImGuiFunction(dDrawFunc imguiDrawFunc);
	static void RegisterWndProcHandler(dWndProc wndProcHandler);
private:
	static bool HookGL();
	
};

// TODO figure out better place to put this and the dxgi patterns
template <typename T>
inline MH_STATUS MH_CreateHookEx(LPVOID pTarget, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

// dxgi.dll
#define X64_SWAPCHAIN_VTABLE_PATTERN "\x48\x8B\xD3\xE8\x00\x00\x00\x00\x90\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x07"
#define X64_SWAPCHAIN_VTABLE_MASK "xxxx????xxxx????xxx"
#define X64_SWAPCHAIN_VTABLE_RIPOFFSET 12
#define X64_SWAPCHAIN_VTABLE_FULLOFFSET 16
// TODO verify these are correct (low priority, 32 bit programs are ancient at this point)
#define X86_SWAPCHAIN_VTABLE_PATTERN "\x89\x7D\xEC\xFF\x75\x10\xFF\x75\x0C\xE8\x00\x00\x00\x00\x83\x65\xFC\x00\xC7\x07\x00\x00\x00\x00"
#define X86_SWAPCHAIN_VTABLE_MASK "xxxxxxxxxx????xxxxxx????"
#define X86_SWAPCHAIN_VTABLE_RIPOFFSET 20
#define X86_SWAPCHAIN_VTABLE_FULLOFFSET 24