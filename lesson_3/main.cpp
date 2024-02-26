#include "utils.h"
#include <stdio.h>
#include <cglm/cglm.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


void fill_image(DrawBuffer* draw_buffer)
{
    const Color colors[] {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255}
    };

    for(int row = 0; row < draw_buffer->height; ++row)
    {
        for(int col = 0; col < draw_buffer->width; ++col)
        {
            draw_buffer->buffer[draw_buffer->width * row + col] = colors[(row / 5) % 3];
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