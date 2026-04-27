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

    physac::particle p;
    p.pos = { 100.0f, 100.0f };
    p.velocity = { 0.0f, 0.0f };
    p.raduis = 10.0f;


    while (!WindowShouldClose())
    {
        // Frame limiter
        currentTime = GetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        if (deltaTime < TARGET_DT)
            deltaTime = TARGET_DT;



        // Update

        p.acceleration = { 2.0f * PIXELS_PER_METER, GRAVITY * PIXELS_PER_METER };
        // Euler integration
        p.velocity += p.acceleration * deltaTime;
        p.pos += p.velocity * deltaTime;


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
        EndDrawing();

        while ((float)(GetTime() - currentTime) < TARGET_DT) {} // busy wait loop (Delay)
    }

    CloseWindow();
    return 0;
}
