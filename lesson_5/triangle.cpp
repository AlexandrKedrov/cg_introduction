#include "triangle.h"
#include <string>

namespace MeshUtils
{
    void read_from_obj(std::istream& input, Mesh* mesh)
    {
        std::string line;
        while(std::getline(input, line))
        {
            uint32_t ind = 0;
            while(ind < line.size() && line[ind] == ' ')
            {
                ++ind;
            }
            if(ind == line.size())
            {
                continue;
            }

            if(line[ind] == 'v')
            {
                if(ind + 1 < line.size() && line[ind + 1] == ' ')
                {
                    // Читаем вершину
                    char* addr,* end_addr;
                    addr = &line[ind + 1]; 
                    float x = std::strtof(addr, &end_addr);
                    if(addr == end_addr)
                    {
                        continue;
                    }
                    addr = end_addr;
                    float y = std::strtof(addr, &end_addr);
                    if(addr == end_addr)
                    {
                        continue;
                    }
                    addr = end_addr;
                    float z = std::strtof(addr, &end_addr);
                    if(addr == end_addr)
                    {
                        continue;
                    }

                    mesh->vertices.push_back(x);
                    mesh->vertices.push_back(y);
                    mesh->vertices.push_back(z);
                }
            }
            else if(line[ind] == 'f')
            {
                if(ind + 1 < line.size() && line[ind + 1] == ' ')
                {
                    // Читаем индексы
                    char* addr,* end_addr;
                    addr = &line[ind + 1]; 
                    long i1 = std::strtol(addr, &end_addr, 10);
                    if(addr == end_addr)
                    {
                        continue;
                    }
                    addr = end_addr;
                    long i2 = std::strtol(addr, &end_addr, 10);
                    if(addr == end_addr)
                    {
                        continue;
                    }
                    addr = end_addr;
                    long i3 = std::strtol(addr, &end_addr, 10);
                    if(addr == end_addr)
                    {
                        continue;
                    }
                    mesh->indices.push_back(i1 - 1);
                    mesh->indices.push_back(i2 - 1);
                    mesh->indices.push_back(i3 - 1);
                }
            }
            else
            {
                continue;
            }
        }
    }

    bool trace_triangle(vec3 camera_pos, vec3 ray, Triangle* triangle, HitInfo* info)
    {
        mat4 look_at;
        vec3 center;
        vec3 up = {0.f, 1.f, 0.f};
        glm_vec3_sub(camera_pos, ray, center);
        glm_lookat(camera_pos, center, up, look_at);

        vec3 p0_transformed, p1_transformed, p2_transformed;
        glm_mat4_mulv3(look_at, triangle->p0, 1.f, p0_transformed);
        glm_mat4_mulv3(look_at, triangle->p1, 1.f, p1_transformed);
        glm_mat4_mulv3(look_at, triangle->p2, 1.f, p2_transformed);

        vec3 v1, v2;
        glm_vec3_sub(p1_transformed, p0_transformed, v1);
        glm_vec3_sub(p2_transformed, p0_transformed, v2);

        vec3 cross, normal;
        glm_vec3_cross(v1, v2, cross);
        glm_vec3_normalize_to(cross, normal);
        if(normal[2] > 0)
        {
            glm_vec3_negate(normal);
        }

        float d = -glm_dot(normal, p0_transformed);
        if(d < 0)
        {
            return false;
        }

        float new_z = -d / normal[2];

        float det1 = -(p0_transformed[0] * v2[1] - p0_transformed[1] * v2[0]);
        float det2 = (p0_transformed[0] * v1[1] - p0_transformed[1] * v1[0]);

        float s = det1 / cross[2];
        float t = det2 / cross[2];
        if(s >= 0 && t >= 0 && 1.f - s - t >= 0)
        {
            mat4 look_at_inverse;
            glm_mat4_inv(look_at, look_at_inverse);
            vec3 newPos = {0.f, 0.f, new_z};
            glm_mat4_mulv3(look_at_inverse, newPos, 1.f, info->pos);
            glm_mat4_mulv3(look_at_inverse, normal, 0.f, info->normal);
            return true;
        }

        return false;
    }
}