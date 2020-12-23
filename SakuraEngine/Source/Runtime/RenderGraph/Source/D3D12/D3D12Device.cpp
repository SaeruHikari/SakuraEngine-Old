#include "RenderGraphD3D12/RenderGraphD3D12.h"
#include "System/Log.h"

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <tchar.h>
#include <wrl.h>
using namespace Microsoft;
using namespace Microsoft::WRL;

#include <dxgi1_6.h>

#include <DirectXMath.h>
using namespace DirectX;
//for d3d12

#include <d3d12shader.h>
#include <d3dcompiler.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#include "RenderGraphD3D12/D3D12Device.h"