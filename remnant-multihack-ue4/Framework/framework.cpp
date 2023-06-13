#include "pch.h"
#include "framework.h"

#include "Engines/UnrealEngine4/Core.h"

#include "Utils/Logger.h"
#include "Utils/Memory.h"
#include "Utils/Keybinds.h"

#include "../minhook/MinHook.h"
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#include "../imgui/backends/imgui_impl_dx11.h"

#include <d3d11.h>
#include <string>
#include <sstream>

dDrawFunc m_ImGuiDraw = NULL;
dWndProc m_WndProcHandler = NULL;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
ID3D11Device* m_pDevice = NULL;
ID3D11DeviceContext* m_pContext = NULL;
IDXGISwapChain* m_pSwapChain = NULL;
ID3D11RenderTargetView* m_mainRenderTargetView = NULL;
HWND m_Window = NULL;
WNDPROC m_oWndProc = NULL;

bool CheatFramework::Init(HMODULE mod, ETargetEngine targetEngine /*= ETargetEngine::UnrealEngine4*/)
{
	Memory::Module = mod;
	Memory::ProcessBaseAddress = (DWORD64)GetModuleHandle(NULL);

	switch (targetEngine)
	{
		case ETargetEngine::UnrealEngine4:
		{
			if (UnrealEngine4::Initialize() == false)
			{
				LOG_ERROR("Could not initialize UnrealEngine4");
				return false;
			}
			break;
		}
		// TODO add auto-detection of game engine, and specific version of that engine (in case we add support for multiple)
		default:
		{
			LOG_ERROR("Not implemented");
			return false;
		}
	}

	if (HookGL() == false)
	{
		LOG_ERROR("Could not hook the graphics library");
		return false;
	}

	return true;
}

void CheatFramework::RegisterImGuiFunction(dDrawFunc imguiDrawFunc)
{
	m_ImGuiDraw = imguiDrawFunc;
}

void CheatFramework::RegisterWndProcHandler(dWndProc wndProcHandler)
{
	m_WndProcHandler = wndProcHandler;
}

typedef HRESULT(__stdcall* tPresent) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
tPresent oPresent = NULL;
bool HookDxgi(uintptr_t dxgiBase, DWORD dxgiSize)
{
	bool is64Bit = Memory::Is64Bit();
	const char* swapChainVTablePattern = is64Bit ? X64_SWAPCHAIN_VTABLE_PATTERN : X86_SWAPCHAIN_VTABLE_PATTERN;
	const char* swapChainVTableMask = is64Bit ? X64_SWAPCHAIN_VTABLE_MASK : X86_SWAPCHAIN_VTABLE_MASK;

	const int swapChainVTableRIPOffset = is64Bit ? X64_SWAPCHAIN_VTABLE_RIPOFFSET : X86_SWAPCHAIN_VTABLE_RIPOFFSET;
	const int swapChainVTableFullOffset = is64Bit ? X64_SWAPCHAIN_VTABLE_FULLOFFSET : X86_SWAPCHAIN_VTABLE_FULLOFFSET;

	uintptr_t swapChainVTablePatternResult = 0;
	uintptr_t* swapChainVTable = 0;
	MH_STATUS status;

	status = MH_Initialize();
	if (status != MH_OK)
	{
		printf("Could not initialize minhook\n");
		return false;
	}

	if (!Memory::FindPattern(dxgiBase, ((dxgiSize)-strlen(swapChainVTableMask) + 1), (PBYTE)swapChainVTablePattern, swapChainVTableMask, &swapChainVTablePatternResult))
	{
		printf("FATAL ERROR: Unable to find swapchain vtable\n");
		return false;
	}

	printf("SwapChain pattern found @ [%tx]\n", swapChainVTablePatternResult);
	if (is64Bit)
	{
		int ripOffset = *(int*)(swapChainVTablePatternResult + swapChainVTableRIPOffset);
		printf("RIP-offset: %x (%d)\n", ripOffset, ripOffset);
		swapChainVTable = (uintptr_t*)((unsigned char*)swapChainVTablePatternResult + swapChainVTableFullOffset + ripOffset);
	}
	else
	{
		int vtableAddr = *(int*)(swapChainVTablePatternResult + swapChainVTableRIPOffset);
		swapChainVTable = (uintptr_t*)vtableAddr;
	}

	printf("Got VTable @ [%tx]\n", swapChainVTable);
	printf("Hooking present (%tx)\n", swapChainVTable[8]);
	status = MH_CreateHookEx((LPVOID)swapChainVTable[8], &hkPresent, &oPresent);
	if (status != MH_OK)
	{
		printf("Could not create Present hook (%d)\n", status);
		return false;
	}
	status = MH_EnableHook((LPVOID)swapChainVTable[8]);
	if (status != MH_OK)
	{
		printf("Could not enable Present hook (%d)\n", status);
		return false;
	}
	printf("Hooked Present [%tx] -> [%tx]\n", oPresent, hkPresent);
	return true;
}

bool CheatFramework::HookGL()
{
	HANDLE procHandle = GetCurrentProcess();
	DWORD errorCode = 0;


	int numTries = 0;
	MODULEINFO dxgiModuleInfo;
	uintptr_t dxgiBase = NULL;
	while (numTries < 10)
	{
		numTries++;
		dxgiBase = (uintptr_t)GetModuleHandle(TEXT("dxgi.dll"));
		if (dxgiBase == NULL)
		{
			Sleep(200);
			continue;
		}
		else
		{
			if (GetModuleInformation(procHandle, (HMODULE)dxgiBase, &dxgiModuleInfo, sizeof(MODULEINFO)) == 0)
			{
				LOG_ERROR("Failed to get module information for dxgi.dll");
				Utils::Strings::PrintLastError();
				return false;
			}
			break;
		}
	}

	if (dxgiBase != NULL)
		return HookDxgi(dxgiBase, dxgiModuleInfo.SizeOfImage);

	numTries = 0;
	// TODO scan for opengl or vulkan or whatever next
	LOG_ERROR("DirectX was not found and other graphics libraries are not yet supported.");
	return false;
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	KeyBinds::WndProc(uMsg, wParam, lParam);

	if (m_WndProcHandler != NULL)
	{
		if (m_WndProcHandler(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	return CallWindowProc(m_oWndProc, hWnd, uMsg, wParam, lParam);
}

bool FinalizeInitializationWithSwapChain(IDXGISwapChain* pSwapChain)
{
	if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&m_pDevice)))
	{
		m_pDevice->GetImmediateContext(&m_pContext);
		DXGI_SWAP_CHAIN_DESC sd;
		pSwapChain->GetDesc(&sd);
		m_Window = sd.OutputWindow;
		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_mainRenderTargetView);
		pBackBuffer->Release();
		m_oWndProc = (WNDPROC)SetWindowLongPtr(m_Window, GWLP_WNDPROC, (LONG_PTR)WndProc);
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(m_Window);
		ImGui_ImplDX11_Init(m_pDevice, m_pContext);
	}
	else
	{
		LOG_ERROR("Could not get Device from SwapChain");
		return false;
	}
	return true;
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (m_pSwapChain == NULL)
	{
		m_pSwapChain = pSwapChain;
		if (FinalizeInitializationWithSwapChain(pSwapChain) == false)
		{
			LOG_ERROR("Could not finalize d3d hook");
			m_pSwapChain = NULL;
			// TODO unhook or something, i dunno
			return oPresent(pSwapChain, SyncInterval, Flags);
		}
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImDrawList* overlayDrawList = ImGui::GetForegroundDrawList();
	std::ostringstream oss;
	oss << "Cheat Framework Initialized\n<Delete> to toggle Menu";
	overlayDrawList->AddText(ImVec2(10.f, 10.f), ImColor(1.f, 0.f, 1.f), oss.str().c_str());

	if (m_ImGuiDraw != NULL)
	{
		m_ImGuiDraw(overlayDrawList);
	}

	ImGui::EndFrame();
	ImGui::Render();
	m_pContext->OMSetRenderTargets(1, &m_mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}