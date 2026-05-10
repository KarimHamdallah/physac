#include <Renderer.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600



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

    // Start
    physac::particle p;
    p.pos = { 100.0f, 100.0f };
    p.velocity = { 0.0f, 0.0f };
    p.mass = 3.0f;
    p.raduis = 10.0f * p.mass;

    physac::particle anchor;
    anchor.pos = { WINDOW_WIDTH * 0.5f, 20.0f };
    anchor.raduis = 5.0f;

    physac::particle bob;
    bob.pos = { WINDOW_WIDTH * 0.5f, 220.0f };
    bob.raduis = 10.0f;


    while (!WindowShouldClose())
    {
        // Frame limiter
        currentTime = GetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        if (deltaTime < TARGET_DT)
            deltaTime = TARGET_DT;



        // Update
        p.AddGravityForce(9.8f * PIXELS_PER_METER);
        physac::Vec2 wind_force = physac::Vec2(10.0f * PIXELS_PER_METER, 0.0f);
        physac::Vec2 drag = physac::GenerateDragForce(p, 0.001f);
        p.AddForce(wind_force);
        p.AddForce(drag);
        p.Integrate(deltaTime);



        // spring physics
        bob.AddGravityForce(9.8f * PIXELS_PER_METER);
        bob.AddForce(physac::GenerateSpringForce(bob, anchor.pos, 200.0f, 20.0f));
        bob.Integrate(deltaTime);


        // collision
        if (p.pos.x - p.raduis <= 0)
        {
            p.pos.x = p.raduis;
            p.velocity.x *= -RESTITUTION;
        }
        else if (p.pos.x + p.raduis >= WINDOW_WIDTH)
        {
            p.pos.x = WINDOW_WIDTH - p.raduis;
            p.velocity.x *= -RESTITUTION;
        }

        if (p.pos.y - p.raduis <= 0)
        {
            p.pos.y = p.raduis;
            p.velocity.y *= -RESTITUTION;
        }
        else if (p.pos.y + p.raduis >= WINDOW_HEIGHT)
        {
            p.pos.y = WINDOW_HEIGHT - p.raduis;
            p.velocity.y *= -RESTITUTION;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        Renderer::draw_particle(p.pos, p.raduis, RED);
        // render spring
        DrawLine(anchor.pos.x, anchor.pos.y, bob.pos.x, bob.pos.y, WHITE);
        Renderer::draw_particle(anchor.pos, anchor.raduis, WHITE);
        Renderer::draw_particle(bob.pos, bob.raduis, GREEN);
        EndDrawing();

        while ((float)(GetTime() - currentTime) < TARGET_DT) {} // busy wait loop (Delay)
    }

    CloseWindow();
    return 0;
}
