#pragma once
#include "SakuraEngine/Core/CoreMinimal/SECS.h"
#include "SakuraEngine/Core/Math/Math.h"

namespace Sakura::World::Scene
{
	using namespace Sakura::Math;
	struct Location
	{
		Vector3f location;
	};

	struct Scale
	{
		Vector3f scale;
	};

	struct Rotation
	{
		Vector4f rotation;
	};

	inline Rotation EulerToQuad(Vector3f euler)
	{
		Rotation rot{ 0, 0, 0, 0 };
		return rot;
	}
}