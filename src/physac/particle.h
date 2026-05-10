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
		Vec2 sumforces = 0.0f;

		/*
		Forces
		*/

		void ClearForces()
		{
			sumforces = Vec2(0.0f, 0.0f);
		}

		void AddForce(const Vec2& force)
		{
			sumforces += force;
		}

		void AddGravityForce(float gravityscale)
		{
			Vec2 force = Vec2(0.0f, mass * gravityscale);
			sumforces += force;
		}

		/*
		Euler integration: more stable in constant time step cases
		*/

		void Integrate(float dt)
		{
			// find acceleration based on forces
			acceleration = sumforces / mass;

			// integration
			velocity += acceleration * dt;
			pos += velocity * dt;

			ClearForces();
		}
	};

	inline Vec2 GenerateDragForce(const particle& particle, float k)
	{
		Vec2 dragForce = Vec2(0.0f, 0.0f);

		if (particle.velocity.mag_sqrd() > 0)
		{
			// Calculate the drag direction (inverse of velocity unit vector)
			Vec2 dragDirection = normalize_vec2(particle.velocity) * -1.0f;

			// Calculate the drag magnitude, k * |v|^2
			float dragMagnitude = k * particle.velocity.mag_sqrd();

			// Generate the final drag force with direction and magnitude
			dragForce = dragDirection * dragMagnitude;
		}

		return dragForce;
	}

	inline Vec2 GenerateFrictionForce(const particle& particle, float k)
	{
		Vec2 frictionForce = Vec2(0.0f, 0.0f);

		if (particle.velocity.mag_sqrd() > 0)
		{
			Vec2 dragDirection = normalize_vec2(particle.velocity) * -1.0f;
			float dragMagnitude = k;
			frictionForce = dragDirection * dragMagnitude;
		}

		return frictionForce;
	}

	Vec2 GenerateSpringForce(const particle& particle, Vec2 anchor, float restLength, float k)
	{
		Vec2 d = particle.pos - anchor;
		Vec2 springDirection = normalize_vec2(d);

		float displacement = d.mag() - restLength;
		float springMagnitude = -k * displacement;

		Vec2 springForce = springDirection * springMagnitude;
		return springForce;
	}

	Vec2 GenerateSpringForce(const particle& p1, const particle& p2, float restLength, float k)
	{
		Vec2 d = p1.pos - p2.pos;
		Vec2 springDirection = normalize_vec2(d);

		float displacement = d.mag() - restLength;
		float springMagnitude = -k * displacement;

		Vec2 springForce = springDirection * springMagnitude;
		return springForce;
	}
}
