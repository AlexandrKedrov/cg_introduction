#pragma once

#include "../lesson_5/triangle.h"
#include "../lesson_3/utils.h"

#include <cglm/cglm.h>

namespace Rasterizer
{

    struct BarycentricCoords
    {
        float s, t, v;
    };

    void draw_triangle(MeshUtils::Triangle* triangle, mat4 screen, mat4 screen_inverse, DrawBuffer* draw_buffer);
    bool rasterize_triangle(MeshUtils::Triangle* triangle, vec2 point, BarycentricCoords* coords);
}