#pragma once
#include <physac/math/vec2.h>

namespace physac
{
	struct verletparticle
	{
		Vec2 pos = { 0.0f, 0.0f };
		Vec2 prev_pos = { 0.0f, 0.0f };
		Vec2 acc = { 0.0f, 0.0f };
		Vec2 sum_forces = { 0.0f, 0.0f };
		float mass = 1.0f;
		float radius = 5.0f;
		bool pinned = false;

		// for cloth simulation
		bool is_selected = false;
		bool is_valid = true;


		void ClearForces()
		{
			sum_forces = { 0.0f, 0.0f };
		}

		void AddForce(const Vec2& force)
		{
			sum_forces += force;
		}

		/*
		verlet integration
		*/

		void Integrate(float dt, float damping = 1.0f)
		{
			if (pinned)
				return;

			acc = sum_forces / mass;

			Vec2 velocity = pos - prev_pos;
			float new_x = pos.x + velocity.x + acc.x * dt * dt;
			float new_y = pos.y + velocity.y + acc.y * dt * dt;
			prev_pos = { pos.x, pos.y };
			pos = { new_x, new_y };

			ClearForces();
		}

		void KeepInsideWindow(uint32_t window_width, uint32_t window_height)
		{
			Vec2 velocity = pos - prev_pos;

			if (pos.x < 0) // left border
			{
				pos.x = 0;
				prev_pos.x = pos.x;
				//prev_pos.x = pos.x + velocity.x;
			}
			else if (pos.x >= window_width) // right border
			{
				pos.x = window_width;
				prev_pos.x = pos.x;
				//prev_pos.x = pos.x + velocity.x;
			}

			if (pos.y < 0) // upper border
			{
				pos.y = 0;
				prev_pos.y = pos.y;
				//prev_pos.y = pos.y + velocity.y;
			}
			else if (pos.y >= window_height) // bottom border
			{
				pos.y = window_height;
				prev_pos.y = pos.y;
				//prev_pos.y = pos.y + velocity.y;
			}
		}
	};

	struct Stick
	{
		Stick() {}
		~Stick() {}

		Stick(verletparticle& particle1, verletparticle& particle2, float length)
			: p1(&particle1), p2(&particle2), restlength(length)
		{}

		void Update()
		{
			if (is_breaked)
				return;

			Vec2 d = p2->pos - p1->pos;
			Vec2 direction = normalize_vec2(d);
			float distance = d.mag();
			float displacement = distance - restlength;

			if (!p1->pinned && !p2->pinned)
			{
				p1->pos += direction * displacement * 0.5f;
				p2->pos -= direction * displacement * 0.5f;
			}
			else if (p1->pinned && !p2->pinned)
			{
				p2->pos -= direction * displacement;
			}
			else if (!p1->pinned && p2->pinned)
			{
				p1->pos += direction * displacement;
			}
		}

		verletparticle* p1 = nullptr;
		verletparticle* p2 = nullptr;
		float restlength = 0.0f;
		bool is_selected = false;
		bool is_breaked = false;
	};
}
