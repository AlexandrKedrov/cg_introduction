#pragma once
#include <SDL.h>

struct Color
{
    unsigned char color[4];
};

struct DrawBuffer
{
    Color* buffer;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    int width;
    int height;
    int pitch;
};


inline void draw_buffer_create(int width, int  height, SDL_Renderer* renderer, DrawBuffer* out)
{
    out->width = width;
    out->height = height;
    out->renderer = renderer;
    out->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, width, height);
    out->buffer = NULL;
    out->pitch = NULL;
}


inline void draw_buffer_lock(DrawBuffer* draw_buffer)
{
    SDL_LockTexture(draw_buffer->texture, NULL, reinterpret_cast<void**>(&draw_buffer->buffer), &draw_buffer->pitch);
}

inline void draw_buffer_unlock(DrawBuffer* draw_buffer)
{
    SDL_UnlockTexture(draw_buffer->texture);
    draw_buffer->buffer = NULL;
    draw_buffer->pitch = NULL;
}

inline void draw_buffer_show(DrawBuffer* draw_buffer)
{
    SDL_RenderCopy(draw_buffer->renderer, draw_buffer->texture, NULL, NULL);
}

inline void draw_buffer_destroy(DrawBuffer* in)
{
    SDL_DestroyTexture(in->texture);
}