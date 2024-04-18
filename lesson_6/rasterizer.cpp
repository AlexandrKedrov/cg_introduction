#include "rasterizer.h"

namespace Rasterizer
{
    void draw_triangle(MeshUtils::Triangle* triangle, mat4 screen, mat4 screen_inverse, DrawBuffer* draw_buffer)
    {
        //vec3 clip_max = {1, 1, 1};
        //vec3 clip_min = {-1, -1, -1};

        vec3 p0_clipped, p1_clipped, p2_clipped;
        glm_vec3_copy(triangle->p0, p0_clipped);
        glm_vec3_copy(triangle->p1, p1_clipped);
        glm_vec3_copy(triangle->p2, p2_clipped);
        glm_vec3_clamp(p0_clipped, -1.f, 1.f);
        glm_vec3_clamp(p1_clipped, -1.f, 1.f);
        glm_vec3_clamp(p2_clipped, -1.f, 1.f);
        
        vec3 p0_screen, p1_screen, p2_screen;
        glm_mat4_mulv3(screen, p0_clipped, 1.f, p0_screen);
        glm_mat4_mulv3(screen, p1_clipped, 1.f, p1_screen);
        glm_mat4_mulv3(screen, p2_clipped, 1.f, p2_screen);

        vec2 min_screen, max_screen;
        glm_vec2_minv(p0_screen, p1_screen, min_screen);
        glm_vec2_minv(p2_screen, min_screen, min_screen);

        glm_vec2_maxv(p0_screen, p1_screen, max_screen);
        glm_vec2_maxv(p2_screen, max_screen, max_screen);

        for(size_t col = static_cast<size_t>(min_screen[0]);
            col <= static_cast<size_t>(max_screen[0]);
            ++col)
        {
            for(size_t row = static_cast<size_t>(min_screen[1]);
                row <= static_cast<size_t>(max_screen[1]);
                ++row)
            {
                vec3 screen_pos = {static_cast<float>(col), static_cast<float>(row), 0};
                vec3 ndc_pos;
                glm_mat4_mulv3(screen_inverse, screen_pos, 1.f, ndc_pos);
                BarycentricCoords bar_coords;
                if(rasterize_triangle(triangle, ndc_pos, &bar_coords))
                {
                    draw_buffer->buffer[draw_buffer->width * row + col] = 
                    {
                        static_cast<unsigned char>(255.f * bar_coords.s),
                        static_cast<unsigned char>(255.f * bar_coords.t),
                        static_cast<unsigned char>(255.f * bar_coords.v)
                    };
                }
            }
        }
    }

    bool rasterize_triangle(MeshUtils::Triangle* triangle, vec2 point, BarycentricCoords* coords)
    {
        vec2 v1, v2, vp;
        glm_vec2_sub(triangle->p1, triangle->p0, v1);
        glm_vec2_sub(triangle->p2, triangle->p0, v2);
        glm_vec2_sub(triangle->p0, point, vp);

        float det1 = -(vp[0] * v2[1] - vp[1] * v2[0]);
        float det2 = (vp[0] * v1[1] - vp[1] * v1[0]);

        float cross2d = glm_vec2_cross(v1, v2);

        float s = det1 / cross2d;
        float t = det2 / cross2d;
        float v = 1.f - s - t;
        if(s >= 0.f && t >= 0.f && v >= 0.f)
        {
            coords->s = s;
            coords->t = t;
            coords->v = v;
            return true;
        }

        return false;
    }
}