#include "pch.h"
#include "Config.h"
#include "CyberFsr.h"
#include "DirectXHooks.h"
#include "Util.h"

NvParameter* CyberFsrContext::AllocateParameter()
{
	Parameters.push_back(std::make_unique<NvParameter>());
	return Parameters.back().get();
}

void CyberFsrContext::DeleteParameter(NvParameter* parameter)
{
	auto it = std::find_if(Parameters.begin(), Parameters.end(),
		[parameter](const auto& p) { return p.get() == parameter; });
	Parameters.erase(it);
}

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
