#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl/wrappers/corewrappers.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <limits>
#include <string>
#include <cctype>
#include <algorithm>
#include <filesystem>
#include <stdexcept>

#include <vulkan/vulkan.hpp>

#include <ffx-fsr2-api/ffx_fsr2.h>
#include <ffx-fsr2-api/dx12/ffx_fsr2_dx12.h>
#include <ffx-fsr2-api/vk/ffx_fsr2_vk.h>

#define NV_WINDOWS
#define NVSDK_NGX
#define NGX_ENABLE_DEPRECATED_GET_PARAMETERS
#include <nvsdk_ngx.h>
#include <nvsdk_ngx_vk.h>

#include "SimpleIni.h"