#pragma once
#include <windows.h>
#include <stdint.h>
#include <assert.h>
#include <psapi.h>
#include "../../dependencies/minhook/MinHook.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <dxgi.h>

#include "../../dependencies/ImGui/imgui.h"
#include "../../dependencies/ImGui/imgui_impl_dx11.h"
#include "../../dependencies/ImGui/imgui_impl_win32.h"

typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
IDXGISwapChainPresent oIDXGISwapChainPresent;

typedef void(APIENTRY* ID3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
ID3D11DrawIndexed oID3D11DrawIndexed;

#if defined _M_X64
typedef uint64_t uintx_t;
#elif defined _M_IX86
typedef uint32_t uintx_t;
#endif

static uintx_t* MethodsTable = NULL;

struct _DirectXVersion {
	int D3D11 = 3;
}DirectXVersion;

WNDCLASSEX WindowClass;
HWND WindowHwnd;

bool InitWindow() {

	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = DefWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = "MJ";
	WindowClass.hIconSm = NULL;
	RegisterClassEx(&WindowClass);
	WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
	if (WindowHwnd == NULL) {
		return false;
	}
	return true;
}

bool DeleteWindow() {
	DestroyWindow(WindowHwnd);
	UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
	if (WindowHwnd != NULL) {
		return false;
	}
	return true;
}

bool CreateHook(uint16_t Index, void** Original, void* Function) {
	assert(Index >= 0 && Original != NULL && Function != NULL);
	void* target = (void*)MethodsTable[Index];
	if (MH_CreateHook(target, Function, Original) != MH_OK || MH_EnableHook(target) != MH_OK) {
		return false;
	}
	return true;
}

void DisableHook(uint16_t Index) {
	assert(Index >= 0);
	MH_DisableHook((void*)MethodsTable[Index]);
}

void DisableAll() {
	MH_DisableHook(MH_ALL_HOOKS);
	free(MethodsTable);
	MethodsTable = NULL;
}

namespace DirectX11 {
	bool Init() {
		if (InitWindow() == false) {
			return false;
		}

		HMODULE D3D11Module = GetModuleHandle("d3d11.dll");
		if (D3D11Module == NULL) {
			DeleteWindow();
			return false;
		}

		void* D3D11CreateDeviceAndSwapChain = GetProcAddress(D3D11Module, "D3D11CreateDeviceAndSwapChain");
		if (D3D11CreateDeviceAndSwapChain == NULL) {
			DeleteWindow();
			return false;
		}

		D3D_FEATURE_LEVEL FeatureLevel;
		const D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 1;
		SwapChainDesc.OutputWindow = WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* SwapChain;
		ID3D11Device* Device;
		ID3D11DeviceContext* Context;

		if (((long(__stdcall*)(
			IDXGIAdapter*,
			D3D_DRIVER_TYPE,
			HMODULE,
			UINT,
			const D3D_FEATURE_LEVEL*,
			UINT,
			UINT,
			const DXGI_SWAP_CHAIN_DESC*,
			IDXGISwapChain**,
			ID3D11Device**,
			D3D_FEATURE_LEVEL*,
			ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, FeatureLevels, 1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, &FeatureLevel, &Context) < 0)
		{
			DeleteWindow();
			return false;
		}

		MethodsTable = (uintx_t*)::calloc(205, sizeof(uintx_t));
		memcpy(MethodsTable, *(uintx_t**)SwapChain, 18 * sizeof(uintx_t));
		memcpy(MethodsTable + 18, *(uintx_t**)Device, 43 * sizeof(uintx_t));
		memcpy(MethodsTable + 18 + 43, *(uintx_t**)Context, 144 * sizeof(uintx_t));

		SwapChain->Release();
		SwapChain = NULL;
		Device->Release();
		Device = NULL;
		Context->Release();
		Context = NULL;
		DeleteWindow();
		return true;
	}

	DWORD WINAPI MainThread(LPVOID lpParameter);
}