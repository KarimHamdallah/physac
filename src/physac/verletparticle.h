#pragma once
#include <physac/math/vec2.h>

namespace physac
{
	struct verletparticle
	{
		float x = 0.0f;      float y = 0.0f;
		float prev_x = 0.0f; float prev_y = 0.0f;
		float acc_x = 0.0f;  float acc_y = 0.0f;
		float mass = 1.0f;
		float radius = 5.0f;

		/*
		verlet integration
		*/

		void Integrate(float dt)
		{
			float new_x = 2 * x - prev_x + acc_x * dt * dt;
			float new_y = 2 * y - prev_y + acc_y * dt * dt;
			prev_x = x;
			prev_y = y;
			x = new_x;
			y = new_y;
		}
	};
}
