#include "pch.h"
#include "Util.h"
#include "DirectXHooks.h"

/*
Cyberpunk doesn't reset the ComputeRootSignature after running DLSS.
This is fine for DLSS itself because they only use CUDA stuff but breaks everything for implementations that use compute shaders

The only solution to bypass this problem in a safe manner is to hook the CreateCommandQueue function of the CommandLists VTable we get in NVSDK_NGX_D3D12_CreateFeature and after that hook the SetComputeRootSignature function of every created CommandQueue.
This allows us to keep track of every ComputeRootSignature and match the RootSignature with the CommandList we get in NVSDK_NGX_D3D12_EvaluateFeature to restore it after FSR2 completes.
*/

SETCOMPUTEROOTSIGNATURE oSetComputeRootSignature = nullptr;

ID3D12CommandList* myCommandList = nullptr;

std::unordered_map<ID3D12GraphicsCommandList*, ID3D12RootSignature*> commandListVector;

std::mutex rootSigMutex;

void hSetComputeRootSignature(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* pRootSignature)
{
	rootSigMutex.lock();
	commandListVector[commandList] = pRootSignature;
	rootSigMutex.unlock();

	return oSetComputeRootSignature(commandList, pRootSignature);
}

void HookSetComputeRootSignature(ID3D12GraphicsCommandList* InCmdList)
{
	constexpr int offset = 0xE8 / sizeof(void*);

	void** cmdListVTable = *reinterpret_cast<void***>(InCmdList);
	const auto computeRootSigFuncVTable = reinterpret_cast<SETCOMPUTEROOTSIGNATURE*>(cmdListVTable + offset);

	if (oSetComputeRootSignature == nullptr)
	{
		oSetComputeRootSignature = *computeRootSigFuncVTable;
		DWORD oldProtect;
		VirtualProtect(computeRootSigFuncVTable, sizeof(void*), PAGE_READWRITE, &oldProtect);
		*computeRootSigFuncVTable = &hSetComputeRootSignature;
		VirtualProtect(computeRootSigFuncVTable, sizeof(void*), oldProtect, nullptr);
	}
}