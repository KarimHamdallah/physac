#include <physac/math/vec2.h>
#include <vector>

namespace physac
{
	enum class ShapeType { CIRCLE, POLYGON, BOX };

	class Shape
	{
	public:
		virtual ~Shape() = default;
		virtual ShapeType GetType() const = 0;
		virtual Shape* Clone() const = 0;
		virtual float GetMomentOfInertia() = 0;
	};

	class CircleShape : public Shape
	{
	public:
		CircleShape(float r)
			: radius(r)
		{

		}
		
		virtual ~CircleShape()
		{

		}

		virtual ShapeType GetType() const override
		{
			return ShapeType::CIRCLE;
		}

		virtual Shape* Clone() const override
		{
			return new CircleShape(radius);
		}

		virtual float GetMomentOfInertia() override 
		{
			return 0.5f * (radius * radius);
		}

	public:
		float radius = 0.0f;
	};

	class PolygonShape : public Shape
	{
	public:
		PolygonShape(const std::vector<Vec2>& vertices)
		{

		}

		virtual ~PolygonShape()
		{

		}

		virtual ShapeType GetType() const override
		{
			return ShapeType::POLYGON;
		}

		virtual Shape* Clone() const override
		{
			return nullptr; // TODO
		}

		virtual float GetMomentOfInertia() override
		{
			return 0.0f; // TODO
		}

	public:
		std::vector<Vec2> vertices;
		std::vector<Vec2> transformed_vertices;
	};

	class BoxShape : public Shape
	{
	public:
		BoxShape(float w, float h)
			: width(w), height(h)
		{
			vertices.resize(4);
			transformed_vertices.resize(4);

			// load box verices - local space
			vertices[0] ={ w * -0.5f, h * -0.5f }; // bottom left
			vertices[1] ={ w * 0.5f, h * -0.5f }; // bottom right
			vertices[2] ={ w * 0.5f, h * 0.5f }; // top right
			vertices[3] ={ w * -0.5f, h * 0.5f }; // top left
		}

		virtual ~BoxShape()
		{

		}

		virtual ShapeType GetType() const override
		{
			return ShapeType::BOX;
		}

		virtual Shape* Clone() const override
		{
			return new BoxShape(width, height);
		}

		virtual float GetMomentOfInertia() override
		{
			return 0.083333f * (width * width + height * height);
		}

	public:
		float width = 0.0f;
		float height = 0.0f;
		std::vector<Vec2> vertices;
		std::vector<Vec2> transformed_vertices;
	};


	class body
	{
	public:
		body() {}
		body(const Shape& shape, Vec2 position, float mass)
			: position(position)
		{
			this->shape = shape.Clone();

			this->mass = mass;

			if (mass != 0)
				inv_mass = 1.0f / mass;
			else
				inv_mass = 0.0f;

			inertia = this->shape->GetMomentOfInertia() * mass;

			if (inertia != 0.0f)
				inv_inertia = 1.0f / inertia;
			else
				inv_inertia = 0.0f;
		}

		~body()
		{
			delete shape;
		}

		void AddForce(const Vec2& force)
		{
			sum_forces += force;
		}

		void AddTorque(float torque)
		{
			sum_torque += torque;
		}

		void Integrate(float dt)
		{
			// linear motion
			acceleration = sum_forces * inv_mass;

			velocity += acceleration * dt;
			position += velocity * dt;

			ClearForces();

			// angular motion
			angular_acceleration = sum_torque * inv_inertia;

			angular_velocity += angular_acceleration * dt;
			rotation += angular_velocity * dt;

			ClearTorque();
			
			// transform vertices from local space to world space
			if (shape->GetType() == ShapeType::BOX)
			{
				BoxShape* box_shape = (BoxShape*)shape;

				for (size_t i = 0; i < box_shape->vertices.size(); i++)
				{
					Vec2 v = box_shape->vertices[i];
					// rotate
					v = physac::rotate_vec2(v, rotation);
					// translate
					v = physac::translate_vec2(v, position);

					box_shape->transformed_vertices[i] = v;
				}
			}
			else if (shape->GetType() == ShapeType::POLYGON)
			{
				PolygonShape* polygon_shape = (PolygonShape*)shape;

				for (size_t i = 0; i < polygon_shape->vertices.size(); i++)
				{
					Vec2 v = polygon_shape->vertices[i];
					// rotate
					v = physac::rotate_vec2(v, rotation);
					// translate
					v = physac::translate_vec2(v, position);

					polygon_shape->transformed_vertices[i] = v;
				}
			}
		}

		void ClearForces()
		{
			sum_forces = { 0.0f, 0.0f };
		}

		void ClearTorque()
		{
			sum_torque = 0.0f;
		}

	public:
		Vec2 position = { 0.0f, 0.0f };
		Vec2 velocity = { 0.0f, 0.0f };
		Vec2 acceleration = { 0.0f, 0.0f };

		float rotation = 0.0f;
		float angular_velocity = 0.0f;
		float angular_acceleration = 0.0f;

		Vec2 sum_forces = { 0.0f, 0.0f };
		float sum_torque = 0.0f;

		// TODO:: angular motion

		float mass = 1.0f;
		float inv_mass = 1.0f;

		float inertia = 1.0f;
		float inv_inertia = 1.0f;

		Shape* shape = nullptr;
	};
}
