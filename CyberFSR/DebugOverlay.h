#pragma once
class DebugOverlay
{
public:
	DebugOverlay(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
	DebugOverlay(VkDevice InDevice, VkCommandBuffer InCmdList);
	~DebugOverlay();
	void Render(ID3D12GraphicsCommandList* cmdList);
	void Render(VkCommandBuffer cmdList);
};