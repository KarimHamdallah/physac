#pragma once
#include <physac/math/vec2.h>

namespace physac
{
	struct particle
	{
		Vec2  pos = {};
		Vec2  velocity = {};
		Vec2  acceleration = {};
		float mass = 1.0f;
		float raduis = 0.0f;
	};
}
