#pragma once
#include <raylib.h>
#include <physac/particle.h>
#include <physac/verletparticle.h>

namespace Renderer
{

	inline void draw_vec2(const physac::Vec2& start, const physac::Vec2& end, Color color)
	{
		DrawCircle(start.x, start.y, 5, color);
		DrawCircle(end.x, end.y, 5, color);
		DrawLine(start.x, start.y, end.x, end.y, color);
	}

	inline void draw_particle(const physac::Vec2& center, float raduis, Color color)
	{
		DrawCircle(center.x, center.y, raduis, color);
	}
}
