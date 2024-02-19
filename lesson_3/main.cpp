#include "utils.h"
#include <stdio.h>

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
            draw_buffer->buffer[draw_buffer->width * row + col] = colors[row % 3];
        }
    }
}


int main(int argc, char** argv)
{
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