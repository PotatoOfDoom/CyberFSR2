#include "pch.h"
#include "Config.h"
#include "CyberFsr.h"
#include "DirectXHooks.h"
#include "Util.h"


void CyberFsrContext::DeleteParameter(NVSDK_NGX_Parameter* parameter)
{
	auto it = std::find(Parameters.begin(), Parameters.end(), parameter);
	Parameters.erase(it);
}

FeatureContext* CyberFsrContext::CreateContext()
{
	auto dCtx = new FeatureContext();
	dCtx->Handle.Id = rand();
	Contexts[dCtx->Handle.Id] = dCtx;

	return dCtx;
}

void CyberFsrContext::DeleteContext(NVSDK_NGX_Handle* handle)
{
	auto handleId = handle->Id;

	auto it = std::find_if(Contexts.begin(), Contexts.end(),
		[&handleId](const auto& p) { return p.first == handleId; });
	Contexts.erase(it);
}
