#pragma once
#include <cglm/cglm.h>
#include <vector>
#include <istream>

struct HitInfo
{
    vec3 pos;
    vec3 normal;
};

namespace MeshUtils
{
    struct Triangle
    {
        vec3 p0, p1, p2;
    };

    struct Mesh
    {
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
    };

    void read_from_obj(std::istream& input, Mesh* mesh);
    bool trace_triangle(vec3 camera_pos, vec3 ray, Triangle* triangle, HitInfo* info);
}
