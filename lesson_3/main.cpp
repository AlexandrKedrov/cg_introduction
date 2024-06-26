#include "utils.h"
#include <stdio.h>
#include <cglm/cglm.h>
#include <stdlib.h>
#include <limits>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


bool trace_sphere(vec3 ray, vec3 sphere_pos, float radius, vec3 hit)
{
    float distance_to_sphere = glm_vec3_norm(sphere_pos);
    float cosA = glm_dot(ray, sphere_pos) / distance_to_sphere;

    if(cosA < 0)
    {
        return false;
    }

    vec3 cross;
    glm_cross(ray, sphere_pos, cross);
    float h = glm_vec3_norm(cross);

    if(h > radius)
    {
        return false;
    }

    float d = sqrtf(radius * radius - h * h);
    glm_vec3_scale(ray, cosA * distance_to_sphere - d, hit);
    return true;
}


void fill_image(DrawBuffer* draw_buffer)
{
    const Color colors[] {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255}
    };

    mat4 projection;
    glm_perspective_rh_no(
        glm_rad(45.f),
        SCREEN_WIDTH / static_cast<float>(SCREEN_HEIGHT),
        1e-2, 1e3,
        projection 
    );

    mat4 screen;
    glm_mat4_identity(screen);
    screen[0][0] = SCREEN_WIDTH * 0.5f;
    screen[3][0] = SCREEN_WIDTH * 0.5f;
    screen[1][1] = -SCREEN_HEIGHT * 0.5f;
    screen[3][1] = SCREEN_HEIGHT * 0.5f;

    mat4 screen_projection;
    mat4 screen_projection_inverse;
    glm_mat4_mul(screen, projection, screen_projection);
    glm_mat4_inv(screen_projection, screen_projection_inverse);

    struct Sphere {
        vec3 pos;
        Color color;
    };

    constexpr int sphere_count = 1000;

    Sphere spheres[sphere_count];

    float minX = -20.f;
    float maxX = 20.f;
    float minY = -20.f;
    float maxY = 20.f;
    float maxZ = -100.f;
    float minZ = -140.f;

    for(int i = 0; i < sphere_count; ++i)
    {
        spheres[i].pos[0] = rand() / static_cast<float>(RAND_MAX)  * (maxX - minX) + minX;
        spheres[i].pos[1] = rand() / static_cast<float>(RAND_MAX)  * (maxY - minY) + minY;
        spheres[i].pos[2] = rand() / static_cast<float>(RAND_MAX)  * (maxZ - minZ) + minZ;
        spheres[i].color = {
            static_cast<unsigned char>(rand() % 256),
            static_cast<unsigned char>(rand() % 256),
            static_cast<unsigned char>(rand() % 256),
            0
        };
    }

    float radius = 5.f;

    for(int row = 0; row < draw_buffer->height; ++row)
    {
        for(int col = 0; col < draw_buffer->width; ++col)
        {
            vec4 screen_point = {col + 0.5f, row + 0.5f, -1.f, 1.f};
            vec4 ray;
            glm_mat4_mulv(screen_projection_inverse, screen_point, ray);
            glm_vec4_divs(ray, ray[3], ray);
            glm_normalize(ray);

            float min_distance = std::numeric_limits<float>::max();

            vec3 hit;
            draw_buffer->buffer[draw_buffer->width * row + col] = {0, 0, 0, 0};
            for(int i = 0; i < sphere_count; ++i)
            {
                if(trace_sphere(ray, spheres[i].pos, radius, hit))
                {
                    float distance = glm_vec3_norm2(hit);
                    if(distance < min_distance)
                    {
                        draw_buffer->buffer[draw_buffer->width * row + col] = spheres[i].color;
                        min_distance = distance;
                    }
                }
            }
        }
    }
}


int main(int argc, char** argv)
{
    mat4 projection;
    glm_perspective_rh_no(
        glm_rad(80.f),
        SCREEN_WIDTH / static_cast<float>(SCREEN_HEIGHT),
        1e-2, 1e2,
        projection 
    );

    mat4 projection_inverse;
    glm_mat4_inv(projection, projection_inverse);

    for(unsigned int i = 0; i < 8; ++i)
    {
        float x = (i & 1) == 0 ? -1.f : 1.f;
        float y = (i & 2) == 0 ? -1.f : 1.f;
        float z = (i & 4) == 0 ? -1.f : 1.f;
        vec4 ndc_point = {x, y, z, 1.f};
        vec4 world_point;
        glm_mat4_mulv(projection_inverse, ndc_point, world_point);
        glm_vec4_divs(world_point, world_point[3], world_point);
        printf("%f %f %f -> %f %f %f\n", ndc_point[0], ndc_point[1], ndc_point[2],
                                         world_point[0], world_point[1], world_point[2]);
    }

    mat4 screen;
    glm_mat4_identity(screen);
    screen[0][0] = SCREEN_WIDTH * 0.5f;
    screen[3][0] = SCREEN_WIDTH * 0.5f;
    screen[1][1] = -SCREEN_HEIGHT * 0.5f;
    screen[3][1] = SCREEN_HEIGHT * 0.5f;

    mat4 screen_projection;
    mat4 screen_projection_inverse;
    glm_mat4_mul(screen, projection, screen_projection);
    glm_mat4_inv(screen_projection, screen_projection_inverse);

    for(unsigned int i = 0; i < SCREEN_HEIGHT; ++i)
    {
        for(unsigned int j = 0; j < SCREEN_WIDTH; ++j)
        {
            vec4 screen_point = {j + 0.5f, i + 0.5f, -1.f, 1.f};
            vec4 ray;
            glm_mat4_mulv(screen_projection_inverse, screen_point, ray);
            glm_vec4_divs(ray, ray[3], ray);
            glm_normalize(ray);
        }
    }

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow(
                    "lesson_3",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH, SCREEN_HEIGHT,
                    SDL_WINDOW_SHOWN
                    );
    if (window == NULL) {
        printf("could not create window: %s\n", SDL_GetError());
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    DrawBuffer draw_buffer;
    draw_buffer_create(SCREEN_WIDTH, SCREEN_HEIGHT, renderer, &draw_buffer);

    // Рисование на экране
    draw_buffer_lock(&draw_buffer);
    fill_image(&draw_buffer);
    draw_buffer_unlock(&draw_buffer);
    draw_buffer_show(&draw_buffer);
    
    SDL_RenderPresent(renderer);

    bool isRunning = true;
    while(isRunning)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            
            default:
                break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}