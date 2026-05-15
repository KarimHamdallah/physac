#pragma once
#include <raylib.h>
#include <physac/particle.h>
#include <physac/verletparticle.h>
#include <vector>

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

	inline void draw_circle(const physac::Vec2& center, float radius, float angle, Color color)
	{
		DrawCircleLines(center.x, center.y, radius, color);
		Vector2 endLine;
		endLine.x = center.x + cosf(angle) * radius;
		endLine.y = center.y + sinf(angle) * radius;
		DrawLine(center.x, center.y, endLine.x, endLine.y, color);
	}

	inline void draw_polygon(const physac::Vec2& center, const std::vector<physac::Vec2>& vertices, Color color)
	{
		for (size_t i = 0; i < vertices.size(); i++)
		{
			if (i < vertices.size() - 1)
			{
				physac::Vec2 v1 = vertices[i];
				physac::Vec2 v2 = vertices[i + 1];

				DrawLine(v1.x, v1.y, v2.x, v2.y, color);
			}
		}

		physac::Vec2 v_first = vertices[0];
		physac::Vec2 v_last = vertices[vertices.size() - 1];

		DrawLine(v_first.x, v_first.y, v_last.x, v_last.y, color);

		DrawCircle(center.x, center.y, 2.0f, color);
	}
}
