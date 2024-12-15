#pragma once

#include "Common.h"

#include "Constants.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4.h"

namespace MEngine::Math
{
	const double pi = 3.14159265358979323846;

	template<class T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template<class T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template<class T>
	constexpr T Clamp(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}

	template<class T>
	constexpr T Lerp(T a, T b, float t)
	{
		return a + ((b - a) * t);
	}

	template<class T>
	constexpr T Abs(T value)
	{
		return value >= 0 ? value : -value;
	}

	template<class T>
	constexpr T Sqr(T value)
	{
		return value * value;
	}

	constexpr float Dot(Vector3 a, Vector3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	constexpr float MagnitudeSqr(Vector3 a)
	{
		return a.x * a.x + a.y * a.y + a.z * a.z;
	}

	inline float Magnitude(Vector3 a)
	{
		return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	inline float DistanceSqr(Vector3 a, Vector3 b)
	{
		return abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}

	inline float Distance(Vector3 a, Vector3 b)
	{
		return sqrt(abs((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)));
	}

	inline Vector3 Normalize(Vector3 a)
	{
		float magA = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
		return a / magA;
	}

	static Vector3 projectOntoAxis(const Vector3& vector, const Vector3& axis) {
		Vector3 normalizedAxis = Normalize(axis);
		float dotProduct = Dot(vector, normalizedAxis);
		return normalizedAxis * dotProduct;
	}

	static Vector3 getNormalizedAxis(const Quaternion& q)
	{
		Vector3 axis(q.x, q.y, q.z);
		return Normalize(axis);
	}

	static Matrix4 extractRotation(const Matrix4& transform)
	{
		Matrix4 rotation;

		// Extract the rotation part (upper 3x3 matrix)
		rotation._11 = transform._11;
		rotation._12 = transform._12;
		rotation._13 = transform._13;
		rotation._21 = transform._21;
		rotation._22 = transform._22;
		rotation._23 = transform._23;
		rotation._31 = transform._31;
		rotation._32 = transform._32;
		rotation._33 = transform._33;

		// Normalize the rotation matrix columns
		Vector3 col1(rotation._11, rotation._21, rotation._31);
		Vector3 col2(rotation._12, rotation._22, rotation._32);
		Vector3 col3(rotation._13, rotation._23, rotation._33);

		col1 = Normalize(col1);
		col2 = Normalize(col2);
		col3 = Normalize(col3);

		rotation._11 = col1.x; rotation._21 = col1.y; rotation._31 = col1.z;
		rotation._12 = col2.x; rotation._22 = col2.y; rotation._32 = col2.z;
		rotation._13 = col3.x; rotation._23 = col3.y; rotation._33 = col3.z;

		// Set the last row and column to make it a 4x4 matrix again
		rotation._14 = 0.0f;
		rotation._24 = 0.0f;
		rotation._34 = 0.0f;
		rotation._41 = 0.0f;
		rotation._42 = 0.0f;
		rotation._43 = 0.0f;
		rotation._44 = 1.0f;

		return rotation;
	}


	static Vector3 extractRotationAxis(const Matrix4& matrix) {
		// Normalize the rotation matrix columns
		Vector3 col1(matrix._11, matrix._21, matrix._31);
		Vector3 col2(matrix._12, matrix._22, matrix._32);
		Vector3 col3(matrix._13, matrix._23, matrix._33);

		col1 = Normalize(col1);
		col2 = Normalize(col2);
		col3 = Normalize(col3);

		// Calculate the rotation angle (theta)
		float trace = col1.x + col2.y + col3.z;
		float theta = acos((trace - 1.0f) / 2.0f);

		// Calculate the rotation axis
		Vector3 rotationAxis(
			(col3.y - col2.z) / (2.0f * sin(theta)),
			(col1.z - col3.x) / (2.0f * sin(theta)),
			(col2.x - col1.y) / (2.0f * sin(theta))
		);

		return rotationAxis;
	}


	constexpr Vector3 Cross(Vector3 a, Vector3 b)
	{
		float x = a.y * b.z - a.z * b.y;
		float y = a.z * b.x - a.x * b.z;
		float z = a.x * b.y - a.y * b.x;

		return { x, y, z };
	}

	inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		float x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
		float y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
		float z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;

		return { x, y, z };
	}

	inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		float x = v.x * m._11 + v.y * m._21 + v.z * m._31;
		float y = v.x * m._12 + v.y * m._22 + v.z * m._32;
		float z = v.x * m._13 + v.y * m._23 + v.z * m._33;

		return { x, y, z };
	}

	inline Matrix4 Transpose(const Matrix4& m)
	{
		return Matrix4(
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		);
	}

	inline Matrix4 Matrix4::RotationAxis(const Vector3& axis, float rad)
	{
		const Vector3 u = Normalize(axis);
		const float x = u.x;
		const float y = u.y;
		const float z = u.z;
		const float s = sin(rad);
		const float c = cos(rad);

		return {
			c + (x * x * (1.0f - c)),
			x * y * (1.0f - c) + z * s,
			x * z * (1.0f - c) - y * s,
			0.0f,

			x * y * (1.0f - c) - z * s,
			c + (y * y * (1.0f - c)),
			y * z * (1.0f - c) + x * s,
			0.0f,

			x * z * (1.0f - c) + y * s,
			y * z * (1.0f - c) - x * s,
			c + (z * z * (1.0f - c)),
			0.0f,

			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	inline Vector3 GetTranslation(const Matrix4& m)
	{
		return { m._41, m._42, m._43 };
	}

	inline Vector3 GetRight(const Matrix4& m)
	{
		return { m._11, m._12, m._13 };
	}

	inline Vector3 Getup(const Matrix4& m)
	{
		return { m._21, m._22, m._23 };
	}

	inline Vector3 GetLook(const Matrix4& m)
	{
		return { m._31, m._32, m._33 };
	}

	inline float SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis) 
	{ 
		// Normalize the input vectors Vector3 
		Vector3 fromNorm = Math::Normalize(from); 
		Vector3 toNorm = Math::Normalize(to); 
		// Calculate the dot product float 
		float dotProd = Math::Dot(fromNorm, toNorm); 
		// Ensure the dot product is within the valid range for acos 
		dotProd = std::clamp(dotProd, -1.0f, 1.0f); 
		// Calculate the angle in radians 
		float angle = std::acos(dotProd); 
		// Calculate the cross product 
		Vector3 crossProd = Math::Cross(fromNorm, toNorm); 
		// Determine the sign of the angle 
		float sign = (Math::Dot(axis, crossProd) < 0) ? -1.0f : 1.0f; 
		// Return the signed angle 
		return angle * sign; 
	}
}