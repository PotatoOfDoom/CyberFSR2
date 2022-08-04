#include "pch.h"
#include "DebugOverlay.h"

DebugOverlay::DebugOverlay(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
}

DebugOverlay::DebugOverlay(VkDevice InDevice, VkCommandBuffer InCmdList)
{
}

DebugOverlay::~DebugOverlay()
{
}

void DebugOverlay::Render(ID3D12GraphicsCommandList* cmdList)
{
}

void DebugOverlay::Render(VkCommandBuffer cmdList)
{
}
