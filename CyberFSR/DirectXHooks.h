#pragma once
#include "pch.h"

typedef void(__fastcall* SETCOMPUTEROOTSIGNATURE)(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* pRootSignature);

extern ID3D12CommandList* myCommandList;

extern ankerl::unordered_dense::map<ID3D12GraphicsCommandList*, ID3D12RootSignature*> commandListVector;

extern std::mutex rootSigMutex;

void HookSetComputeRootSignature(ID3D12GraphicsCommandList* InCmdList);
