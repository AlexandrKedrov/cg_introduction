#pragma once
#include <string.h>

struct Vector3
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float data[3];
	};
};

void vector3_create(float x, float y, float z, Vector3* out)
{
	out->x = x;
	out->y = y;
	out->z = z;
}

Vector3 vector3_sub(const Vector3* a, const Vector3* b)
{
	Vector3 out;
	out.x = a->x - b->x;
	out.y = a->y - b->y;
	out.z = a->z - b->z;

	return out;
}

Vector3 vector3_add(const Vector3* a, const Vector3* b)
{
	Vector3 out;
	out.x = a->x + b->x;
	out.y = a->y + b->y;
	out.z = a->z + b->z;

	return out;
}

float vector3_dot(const Vector3* a, const Vector3* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vector3 vector3_cross(const Vector3* a, const Vector3* b)
{
	Vector3 vec;
	vec.x = a->y * b->z - a->z * b->y;
	vec.y = a->z * b->x - a->x * b->z;
	vec.z = a->x * b->y - a->y * b->x;

	return vec;
}

struct Vector4
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};
};

void vector4_create(float x, float y, float z, float w, Vector4* out)
{
	out->x = x;
	out->y = y;
	out->z = z;
	out->w = w;
}

void vector4_create_vector(float x, float y, float z, Vector4* out)
{
	out->x = x;
	out->y = y;
	out->z = z;
	out->w = 0.f;
}

void vector4_create_vector(const Vector3* in, Vector4* out)
{
	out->x = in->x;
	out->y = in->y;
	out->z = in->z;
	out->w = 0.f;
}

void vector4_create_point(float x, float y, float z, Vector4* out)
{
	out->x = x;
	out->y = y;
	out->z = z;
	out->w = 1.f;
}

void vector4_create_point(const Vector3* in, Vector4* out)
{
	out->x = in->x;
	out->y = in->y;
	out->z = in->z;
	out->w = 1.f;
}

Vector4 vector4_sub(const Vector4* a, const Vector4* b)
{
	Vector4 out;
	out.x = a->x - b->x;
	out.y = a->y - b->y;
	out.z = a->z - b->z;
	out.w = a->w - b->w;

	return out;
}

Vector4 vector4_add(const Vector4* a, const Vector4* b)
{
	Vector4 out;
	out.x = a->x + b->x;
	out.y = a->y + b->y;
	out.z = a->z + b->z;
	out.w = a->w + b->w;

	return out;
}

float vector4_dot(const Vector4* a, const Vector4* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

Vector3* vector4_as_vector3(Vector4* in)
{
	return reinterpret_cast<Vector3*>(&in->x);
}

const Vector3* vector4_as_vector3(const Vector4* in)
{
	return reinterpret_cast<const Vector3*>(&in->x);
}

struct Matrix
{
	union
	{
        Vector4 rows[4];
        struct
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
    };
};

void matrix_create_identity(Matrix* mat)
{
	memset(mat, 0, sizeof(Matrix));
	mat->m00 = 1.f;
	mat->m11 = 1.f;
	mat->m22 = 1.f;
	mat->m33 = 1.f;
};

Vector4 matrix_mul_vector4(const Matrix* mat, const Vector4* vec)
{
	Vector4 out;
	out.x = vector4_dot(&mat->rows[0], vec);
	out.y = vector4_dot(&mat->rows[1], vec);
	out.z = vector4_dot(&mat->rows[2], vec);
	out.w = vector4_dot(&mat->rows[3], vec);

	return out;
};

Vector3 matrix_mul_vector3(const Matrix* mat, const Vector3* vec)
{
	Vector3 out;
	out.x = vector3_dot(vector4_as_vector3(&mat->rows[0]), vec);
	out.y = vector3_dot(vector4_as_vector3(&mat->rows[1]), vec);
	out.z = vector3_dot(vector4_as_vector3(&mat->rows[2]), vec);

	return out;
};

Matrix matrix_mul_matrix(const Matrix* mat1, const Matrix* mat2)
{
	Matrix out;
	out.m00 = mat1->m00 * mat2->m00 + mat1->m01 * mat2->m10 + mat1->m02 * mat2->m20 + mat1->m03 * mat2->m30;
	out.m10 = mat1->m10 * mat2->m00 + mat1->m11 * mat2->m10 + mat1->m12 * mat2->m20 + mat1->m13 * mat2->m30;
	out.m20 = mat1->m20 * mat2->m00 + mat1->m21 * mat2->m10 + mat1->m22 * mat2->m20 + mat1->m23 * mat2->m30;
	out.m30 = mat1->m30 * mat2->m00 + mat1->m31 * mat2->m10 + mat1->m32 * mat2->m20 + mat1->m33 * mat2->m30;

	out.m01 = mat1->m00 * mat2->m01 + mat1->m01 * mat2->m11 + mat1->m02 * mat2->m21 + mat1->m03 * mat2->m31;
	out.m11 = mat1->m10 * mat2->m01 + mat1->m11 * mat2->m11 + mat1->m12 * mat2->m21 + mat1->m13 * mat2->m31;
	out.m21 = mat1->m20 * mat2->m01 + mat1->m21 * mat2->m11 + mat1->m22 * mat2->m21 + mat1->m23 * mat2->m31;
	out.m31 = mat1->m30 * mat2->m01 + mat1->m31 * mat2->m11 + mat1->m32 * mat2->m21 + mat1->m33 * mat2->m31;

	out.m02 = mat1->m00 * mat2->m02 + mat1->m01 * mat2->m12 + mat1->m02 * mat2->m22 + mat1->m02 * mat2->m32;
	out.m12 = mat1->m10 * mat2->m02 + mat1->m11 * mat2->m12 + mat1->m12 * mat2->m22 + mat1->m12 * mat2->m32;
	out.m22 = mat1->m20 * mat2->m02 + mat1->m21 * mat2->m12 + mat1->m22 * mat2->m22 + mat1->m22 * mat2->m32;
	out.m32 = mat1->m30 * mat2->m02 + mat1->m31 * mat2->m12 + mat1->m32 * mat2->m22 + mat1->m32 * mat2->m32;

	out.m03 = mat1->m00 * mat2->m03 + mat1->m01 * mat2->m13 + mat1->m02 * mat2->m23 + mat1->m02 * mat2->m33;
	out.m13 = mat1->m10 * mat2->m03 + mat1->m11 * mat2->m13 + mat1->m12 * mat2->m23 + mat1->m12 * mat2->m33;
	out.m23 = mat1->m20 * mat2->m03 + mat1->m21 * mat2->m13 + mat1->m22 * mat2->m23 + mat1->m22 * mat2->m33;
	out.m33 = mat1->m30 * mat2->m03 + mat1->m31 * mat2->m13 + mat1->m32 * mat2->m23 + mat1->m32 * mat2->m33;

	return out;
}