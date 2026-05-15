#include <raylib.h>
#include <physac/body.h>
#include <Renderer.h>
#include <iostream>

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

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

    physac::Vec2 screen_center = { WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f };
    physac::body body(physac::BoxShape(200.0f, 100.0f), screen_center, 1.0f);

    Vector2 mouse_pos = { 0.0f, 0.0f };
    Vector2 mouse_delta = { 0.0f, 0.0f };

    while (!WindowShouldClose())
    {
        // Frame limiter
        currentTime = GetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        // Update
        mouse_pos = GetMousePosition();
        mouse_delta = GetMouseDelta();

        physac::Vec2 weight_force = { 0.0f, 9.8f * PIXELS_PER_METER };
        //body.AddForce(weight_force);

        float torque = 200.0f;
        body.AddTorque(torque);

        body.Integrate(deltaTime);

        // Keep Inside Window

        if (body.shape->GetType() == physac::ShapeType::CIRCLE)
        {
            physac::CircleShape* shape = (physac::CircleShape*)body.shape;
            
            if (body.position.x - shape->radius < 0.0f)
            {
                body.position.x = shape->radius;
                body.velocity.x *= -0.9f;
            }
            if (body.position.x + shape->radius > WINDOW_WIDTH)
            {
                body.position.x = WINDOW_WIDTH - shape->radius;
                body.velocity.x *= -0.9f;
            }
            if (body.position.y - shape->radius < 0.0f)
            {
                body.position.y = shape->radius;
                body.velocity.y *= -0.9f;
            }
            if (body.position.y + shape->radius > WINDOW_HEIGHT)
            {
                body.position.y = WINDOW_HEIGHT - shape->radius;
                body.velocity.y *= -0.9f;
            }
        }
        else if(body.shape->GetType() == physac::ShapeType::BOX)
        {
            physac::BoxShape* shape = (physac::BoxShape*)body.shape;
            physac::Vec2 extent = { shape->width * 0.5f, shape->height * 0.5f };


            if (body.position.x - extent.x < 0.0f)
            {
                body.position.x = extent.x;
                body.velocity.x *= -0.9f;
            }
            if (body.position.x + extent.x > WINDOW_WIDTH)
            {
                body.position.x = WINDOW_WIDTH - extent.x;
                body.velocity.x *= -0.9f;
            }
            if (body.position.y - extent.y < 0.0f)
            {
                body.position.y = extent.y;
                body.velocity.y *= -0.9f;
            }
            if (body.position.y + extent.y > WINDOW_HEIGHT)
            {
                body.position.y = WINDOW_HEIGHT - extent.y;
                body.velocity.y *= -0.9f;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(CLITERAL(Color) { 50, 50, 50, 255 });


        if (body.shape->GetType() == physac::ShapeType::CIRCLE)
        {
            physac::CircleShape* shape = (physac::CircleShape*)body.shape;

            Renderer::draw_circle(body.position, shape->radius, body.rotation, WHITE);
        }
        else if (body.shape->GetType() == physac::ShapeType::BOX)
        {
            physac::BoxShape* shape = (physac::BoxShape*)body.shape;

            Renderer::draw_polygon(body.position, shape->transformed_vertices, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
