#include <Renderer.h>
#include <iostream>
#include <vector>
#include <format>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

using namespace physac;

/////////// Timer

#define TARGET_FPS 60
#define TARGET_DT (1.0f / TARGET_FPS)

float currentTime, deltaTime, previousTime = 0.0f;
/////////////////

const float PIXELS_PER_METER = 50.0f;
const float GRAVITY = 9.8f;
const float RESTITUTION = 0.9f;

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "physac");
    SetTargetFPS(60);

    // Start


    int points_count = 10;
    float rest_length = 30.0f;
    float drag = 0.002f;
    float spring = 350.0f;

    std::vector<particle> points;
    points.resize(points_count);
    for (size_t i = 0; i < points_count; i++)
    {
        particle& p = points[i];

        p.pos = Vec2(WINDOW_WIDTH * 0.5f, 50.0f + i * rest_length);
        p.mass = 2.0f;
        p.radius = 5.0f;

        if (i == 0) // anchor
            p.is_pinned = true;
    }

    Vector2 mouse_pos = { 0.0f, 0.0f };
    Vector2 mouse_delta = { 0.0f, 0.0f };

    while (!WindowShouldClose())
    {
        // Frame limiter
        currentTime = GetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        // Update
        for (size_t i = 0; i < points.size(); i++)
        {
            particle& p = points[i];


            // weight force
            Vec2 gravity = Vec2(0.0f, p.mass * 9.8f * PIXELS_PER_METER);
            p.AddForce(gravity);

            // drag
            Vec2 drag_force = physac::GenerateDragForce(p, drag);
            p.AddForce(drag_force);

            // spring
            if (i < points.size() - 1)
            {
                Vec2 spring_force = physac::GenerateSpringForce(points[i], points[i + 1], rest_length, spring);
                points[i].AddForce(spring_force);
                points[i + 1].AddForce(spring_force * -1.0f);
            }

            p.Integrate(deltaTime);
        }

        if (IsKeyDown(KEY_SPACE))
        {
            for (size_t i = 0; i < points.size(); i++)
            {
                particle& p = points[i];
                Vec2 wind_force = Vec2(30.0f * PIXELS_PER_METER, 0.0f);
                p.AddForce(wind_force);
            }

        }


        // mouse influence
        mouse_pos = GetMousePosition();
        mouse_delta = GetMouseDelta();

        // Draw
        BeginDrawing();
        ClearBackground(CLITERAL(Color) { 50, 50, 50, 255 });
        
        for (size_t i = 0; i < points.size(); i++)
        {
            if (i < points.size() - 1)
            {
                particle& p1 = points[i];
                particle& p2 = points[i + 1];
                DrawLine(p1.pos.x, p1.pos.y, p2.pos.x, p2.pos.y, YELLOW);
            }
        }

        for (size_t i = 0; i < points.size(); i++)
        {
            particle& p = points[i];
            DrawCircle(p.pos.x, p.pos.y, p.radius, i == 0 ? RED : WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
