#include "pch.h"
#include "Config.h"
#include "CyberFsr.h"
#include "DirectXHooks.h"
#include "Util.h"

FeatureContext* CyberFsrContext::CreateContext()
{
	auto handleId = rand();
	Contexts[handleId] = std::make_unique<FeatureContext>();
	Contexts[handleId]->Handle.Id = handleId;
	return Contexts[handleId].get();
}

void CyberFsrContext::DeleteContext(NVSDK_NGX_Handle* handle)
{
	auto handleId = handle->Id;

	auto it = std::find_if(Contexts.begin(), Contexts.end(),
		[&handleId](const auto& p) { return p.first == handleId; });
	Contexts.erase(it);
}

CyberFsrContext::CyberFsrContext()
{
	MyConfig = std::make_unique<Config>("nvngx.ini");
}
