#pragma once
#include "Core/CoreMinimal/sinterface.h"
#include "Core/CoreMinimal/SDefination.h"
#include "../ResourceObjects/ResourceViews.h"
#include "RootSignature.h"

namespace Sakura::Graphics
{
	struct ComputePipelineCreateInfo
	{
		const RootSignature* rootSignature = nullptr;
		ShaderStageCreateInfo shaderStage;
	};
	sinterface ComputePipeline
	{
		virtual ~ComputePipeline() = default;
	protected:
		ComputePipeline() = default;
	};
}