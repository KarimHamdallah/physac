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

class Cloth
{
public:
    Cloth() {}
    ~Cloth() {}

    Cloth(int width, int height, int spacing, int startx, int starty)
    {
        // points
        points.reserve(width * height);

        for (size_t i = 0; i < height; i++)
        {
            for (size_t j = 0; j < width; j++)
            {
                float xpos = (float)startx + j * spacing;
                float ypos = (float)starty + i * spacing;

                verletparticle p;
                p.pos = { xpos, ypos };
                p.prev_pos = { xpos, ypos };
                p.mass = 1.0f;
                p.radius = point_radius;

                if (i == 0) // first raw
                    p.pinned = true;

                points.push_back(p);
            }
        }

        // sticks
        sticks.reserve(height * (width - 1) + width * (height - 1));

        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {

                verletparticle& p1 = points[x + y * width]; // point

                if (x < width - 1)
                {
                    verletparticle& p2 = points[(x + 1) + y * width]; // right
                    Stick stick1(p1, p2, spacing);
                    sticks.push_back(stick1);
                }
                if (y < height - 1)
                {
                    verletparticle& p3 = points[x + ((y + 1) * width)]; // bottom
                    Stick stick2(p1, p3, spacing);
                    sticks.push_back(stick2);
                }
            }
        }
    }



    void Update()
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            verletparticle& p = points[i];
            p.AddForce(gravity * p.mass);
            p.AddForce(wind);
            p.Integrate(deltaTime);
            p.KeepInsideWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        for (int iter = 0; iter < iterations; iter++)
        {
            for (size_t i = 0; i < sticks.size(); i++)
            {
                Stick& stick = sticks[i];
                stick.Update();
            }
        }

        wind.x = 0.0f;
    }


    void Draw() const
    {
        // sticks
        for (size_t i = 0; i < sticks.size(); i++)
        {
            const Stick& stick = sticks[i];
            if(!stick.is_breaked)
                DrawLine(stick.p1->pos.x, stick.p1->pos.y, stick.p2->pos.x, stick.p2->pos.y, stick.is_selected ? GREEN : WHITE);
        }

        // points
        for (size_t i = 0; i < points.size(); i++)
        {
            const verletparticle& p = points[i];
            if(p.is_valid)
                DrawCircle(p.pos.x, p.pos.y, p.radius, p.pinned ? RED : p.is_selected ? GREEN : WHITE);
        }
    }

public:
    std::vector<verletparticle> points;
    std::vector<Stick> sticks;

    Vec2 gravity = { 0.0f, 9.8f * PIXELS_PER_METER };
    Vec2 wind = { 0.01f * PIXELS_PER_METER, 0.0f };
    float drag = 0.9f;
    float elasticity = 10.0f;
    float point_radius = 2.0f;
    int iterations = 10;
private:
};

inline bool point_to_circle(Vec2 point, Vec2 circle_center, float raduis)
{
    Vec2 d = point - circle_center;
    float distance = d.mag();

    if (distance <= raduis)
        return true;

    return false;
}

enum class MOUSE_INFLUENCE_MODE : uint32_t
{
    TOUCH = 0,
    DRAG,
    CUT
};

std::string ToString(MOUSE_INFLUENCE_MODE mode)
{
    switch (mode)
    {
    case MOUSE_INFLUENCE_MODE::TOUCH: return "TOUCH"; break;
    case MOUSE_INFLUENCE_MODE::DRAG: return "DRAG"; break;
    case MOUSE_INFLUENCE_MODE::CUT: return "CUT"; break;
    default:
        return "NULL";
        break;
    }
}

MOUSE_INFLUENCE_MODE current_mouse_mode = MOUSE_INFLUENCE_MODE::TOUCH;


int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "physac");
    SetTargetFPS(60);

    // Start

    int cloth_width = 120;
    int cloth_height = 50;
    int spacing = 8;
    int start_x = 150;
    int start_y = 50;

    Cloth cloth(cloth_width, cloth_height, spacing, start_x, start_y);

    float mouse_influence_area_radius = 30.0f;
    Vector2 mouse_pos = { 0.0f, 0.0f };
    Vector2 mouse_delta = { 0.0f, 0.0f };

    while (!WindowShouldClose())
    {
        // Frame limiter
        currentTime = GetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        // Update
        cloth.Update();

        // mouse influence
        mouse_pos = GetMousePosition();
        mouse_delta = GetMouseDelta();


        {
            for (size_t i = 0; i < cloth.points.size(); i++)
            {
                verletparticle& p = cloth.points[i];
                if (p.pinned)
                    continue;                
                if (point_to_circle({ p.pos.x, p.pos.y }, { mouse_pos.x, mouse_pos.y }, mouse_influence_area_radius))
                {
                    if (IsMouseButtonDown(0))
                    {

                        switch (current_mouse_mode)
                        {
                        case MOUSE_INFLUENCE_MODE::TOUCH:
                        {
                            p.pos.add({ mouse_delta.x, mouse_delta.y });
                            p.prev_pos = p.pos;
                        }
                        break;
                        case MOUSE_INFLUENCE_MODE::DRAG:
                        {
                            p.prev_pos = p.pos - Vec2(mouse_delta.x, mouse_delta.y);
                        }
                        break;
                        default:
                            break;
                        }
                    }
                    p.is_selected = true;
                }
                else
                    p.is_selected = false;
            }

            for (size_t i = 0; i < cloth.sticks.size(); i++)
            {
                Stick& stick = cloth.sticks[i];
                if (stick.p1->is_selected || stick.p2->is_selected)
                    stick.is_selected = true;
                else
                    stick.is_selected = false;

                if (stick.is_selected && IsMouseButtonDown(0) && current_mouse_mode == MOUSE_INFLUENCE_MODE::CUT)
                {
                    stick.is_breaked = true;
                    stick.p1->is_valid = false;
                    stick.p2->is_valid = false;
                }
            }
        }

        if(IsKeyPressed(KEY_SPACE))
        {
            uint32_t mouse_mode = (uint32_t)current_mouse_mode;
            mouse_mode++;
            mouse_mode %= 3;
            current_mouse_mode = (MOUSE_INFLUENCE_MODE)mouse_mode;
        }

        if (IsKeyDown(KEY_UP))
        {
            mouse_influence_area_radius++;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            mouse_influence_area_radius--;
        }

        // Draw
        BeginDrawing();
        ClearBackground(CLITERAL(Color) { 50, 50, 50, 255 });
        cloth.Draw();

        DrawText(TextFormat(std::format("cursor mode: {}", ToString(current_mouse_mode)).c_str()), 10, 10, 20, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
