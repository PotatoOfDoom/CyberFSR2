#include "pch.h"
#include "Util.h"
#include "DirectXHooks.h"
#include <tlhelp32.h>

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

#define INITIAL_THREAD_CAPACITY 128

HANDLE g_hHeap = NULL;

#define THREAD_ACCESS \
    (THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION | THREAD_SET_CONTEXT)

typedef struct _FROZEN_THREADS
{
	LPDWORD pItems;         // Data heap
	UINT    capacity;       // Size of allocated data heap, items
	UINT    size;           // Actual number of data items
} FROZEN_THREADS, * PFROZEN_THREADS;

static BOOL EnumerateThreads(PFROZEN_THREADS pThreads)
{
	BOOL succeeded = FALSE;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 te;
		te.dwSize = sizeof(THREADENTRY32);
		if (Thread32First(hSnapshot, &te))
		{
			succeeded = TRUE;
			do
			{
				if (te.dwSize >= (FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(DWORD))
					&& te.th32OwnerProcessID == GetCurrentProcessId()
					&& te.th32ThreadID != GetCurrentThreadId())
				{
					if (pThreads->pItems == NULL)
					{
						pThreads->capacity = INITIAL_THREAD_CAPACITY;
						pThreads->pItems
							= (LPDWORD)HeapAlloc(g_hHeap, 0, pThreads->capacity * sizeof(DWORD));
						if (pThreads->pItems == NULL)
						{
							succeeded = FALSE;
							break;
						}
					}
					else if (pThreads->size >= pThreads->capacity)
					{
						pThreads->capacity *= 2;
						LPDWORD p = (LPDWORD)HeapReAlloc(
							g_hHeap, 0, pThreads->pItems, pThreads->capacity * sizeof(DWORD));
						if (p == NULL)
						{
							succeeded = FALSE;
							break;
						}

						pThreads->pItems = p;
					}
					pThreads->pItems[pThreads->size++] = te.th32ThreadID;
				}

				te.dwSize = sizeof(THREADENTRY32);
			} while (Thread32Next(hSnapshot, &te));

			if (succeeded && GetLastError() != ERROR_NO_MORE_FILES)
				succeeded = FALSE;

			if (!succeeded && pThreads->pItems != NULL)
			{
				HeapFree(g_hHeap, 0, pThreads->pItems);
				pThreads->pItems = NULL;
			}
		}
		CloseHandle(hSnapshot);
	}

	return succeeded;
}

static bool Freeze(PFROZEN_THREADS pThreads)
{

	pThreads->pItems = NULL;
	pThreads->capacity = 0;
	pThreads->size = 0;
	if (!EnumerateThreads(pThreads))
	{
		return false;
	}
	else if (pThreads->pItems != NULL)
	{
		UINT i;
		for (i = 0; i < pThreads->size; ++i)
		{
			HANDLE hThread = OpenThread(THREAD_ACCESS, FALSE, pThreads->pItems[i]);
			if (hThread != NULL)
			{
				SuspendThread(hThread);
				CloseHandle(hThread);
			}
		}
	}

	return true;
}

static VOID Unfreeze(PFROZEN_THREADS pThreads)
{
	if (pThreads->pItems != NULL)
	{
		UINT i;
		for (i = 0; i < pThreads->size; ++i)
		{
			HANDLE hThread = OpenThread(THREAD_ACCESS, FALSE, pThreads->pItems[i]);
			if (hThread != NULL)
			{
				ResumeThread(hThread);
				CloseHandle(hThread);
			}
		}

		HeapFree(g_hHeap, 0, pThreads->pItems);
	}
}


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

		if (g_hHeap == NULL)
		{
			g_hHeap = HeapCreate(0, 0, 0);
		}

		DWORD oldProtect;
		FROZEN_THREADS threads;
		Freeze(&threads);
		VirtualProtect(computeRootSigFuncVTable, sizeof(void*), PAGE_READWRITE, &oldProtect);
		*computeRootSigFuncVTable = &hSetComputeRootSignature;
		VirtualProtect(computeRootSigFuncVTable, sizeof(void*), oldProtect, nullptr);
		Unfreeze(&threads);
	}
}