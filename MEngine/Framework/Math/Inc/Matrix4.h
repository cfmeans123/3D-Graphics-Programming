#pragma once

namespace MEngine::Math
{
	struct Matrix4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			std::array<float, 16> v;
		};

		constexpr Matrix4() noexcept
			: Matrix4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			)
		{

		}

		constexpr Matrix4(
			float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44
		) noexcept
			: _11(_11), _12(_12), _13(_13), _14(_14)
			, _21(_21), _22(_22), _23(_23), _24(_24)
			, _31(_31), _32(_32), _33(_33), _34(_34)
			, _41(_41), _42(_42), _43(_43), _44(_44)
		{}

		const static Matrix4 Zero;
		const static Matrix4 Identity;

		static Matrix4 Translation(float x, float y, float z)
		{
			return Matrix4
			(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				x, y, z, 1.0f
			);
		}

		static Matrix4 Translation(const Vector3& v)
		{
			return Matrix4
			(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				v.x, v.y, v.z, 1.0f
			);
		}

		static Matrix4 RotationAxis(const Vector3& axis, float rad);

		static Matrix4 RotationX(float rad)
		{
			return Matrix4
			(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, cosf(rad), sinf(rad), 0.0f,
				0.0f, -sinf(rad), cosf(rad), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}

		static Matrix4 RotationY(float rad)
		{
			return Matrix4
			(
				cosf(rad), 0.0f, -sinf(rad), 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				sinf(rad), 0.0f, cosf(rad), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 RotationZ(float rad)
		{
			return Matrix4
			(
				cosf(rad), sinf(rad), 0.0f, 0.0f,
				-sinf(rad), cosf(rad), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 ComposeRotation(const Math::Vector3& rotationAxis, float angle) 
		{ 
			// Decompose the axis into its components 
			float x = rotationAxis.x;
			float y = rotationAxis.y;
			float z = rotationAxis.z; 
			// Create individual rotation matrices 
			Math::Matrix4 rotateX = Math::Matrix4::RotationX(angle * x);
			Math::Matrix4 rotateY = Math::Matrix4::RotationY(angle * y);
			Math::Matrix4 rotateZ = Math::Matrix4::RotationZ(angle * z); 
			// Combine the rotations 
			return rotateX * rotateY * rotateZ; 
		}

		static Math::Vector3 GetPosition(const Math::Matrix4& q)
		{ 			
			return Math::Vector3(q._14, q._24, q._34); 
		}


		static Matrix4 MatrixRotationQuaternion(const Math::Quaternion& q)
		{
			return Matrix4
			(
				1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
				(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
				(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
				0.0f,

				(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
				1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
				(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
				0.0f,

				(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
				(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
				1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
				0.0f,

				0.0f,
				0.0f,
				0.0f,
				1.0f
			);
		}

		static Matrix4 Scaling(float s)
		{
			return Matrix4
			(
				s, 0.0f, 0.0f, 0.0f,
				0.0f, s, 0.0f, 0.0f,
				0.0f, 0.0f, s, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 Scaling(float sx, float sy, float sz)
		{
			return Matrix4
			(
				sx, 0.0f, 0.0f, 0.0f,
				0.0f, sy, 0.0f, 0.0f,
				0.0f, 0.0f, sz, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 Scaling(const Vector3& s)
		{
			return Matrix4
			(
				s.x, 0.0f, 0.0f, 0.0f,
				0.0f, s.y, 0.0f, 0.0f,
				0.0f, 0.0f, s.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}


		Vector4 multiplyMatrixByVector(const Vector4& vector) const
		{
			Vector4 result(0.0f);
			result.x = _11 * vector.x + _12 * vector.y + _13 * vector.z + _14 * vector.w;
			result.y = _21 * vector.x + _22 * vector.y + _23 * vector.z + _24 * vector.w;
			result.z = _31 * vector.x + _32 * vector.y + _33 * vector.z + _34 * vector.w;
			result.w = _41 * vector.x + _42 * vector.y + _43 * vector.z + _44 * vector.w;
			return result;
		}

		Matrix4 Inverse() const 
		{ 
			float inv[16], det;
			int i; inv[0] = _22 * _33 * _44 - _22 * _34 * _43 - _32 * _23 * _44 + _32 * _24 * _43 + _42 * _23 * _34 - _42 * _24 * _33;
			inv[4] = -_21 * _33 * _44 + _21 * _34 * _43 + _31 * _23 * _44 - _31 * _24 * _43 - _41 * _23 * _34 + _41 * _24 * _33;
			inv[8] = _21 * _32 * _44 - _21 * _34 * _42 - _31 * _22 * _44 + _31 * _24 * _42 + _41 * _22 * _34 - _41 * _24 * _32;
			inv[12] = -_21 * _32 * _43 + _21 * _33 * _42 + _31 * _22 * _43 - _31 * _23 * _42 - _41 * _22 * _33 + _41 * _23 * _32;
			inv[1] = -_12 * _33 * _44 + _12 * _34 * _43 + _32 * _13 * _44 - _32 * _14 * _43 - _42 * _13 * _34 + _42 * _14 * _33;
			inv[5] = _11 * _33 * _44 - _11 * _34 * _43 - _31 * _13 * _44 + _31 * _14 * _43 + _41 * _13 * _34 - _41 * _14 * _33;
			inv[9] = -_11 * _32 * _44 + _11 * _34 * _42 + _31 * _12 * _44 - _31 * _14 * _42 - _41 * _12 * _34 + _41 * _14 * _32;
			inv[13] = _11 * _32 * _43 - _11 * _33 * _42 - _31 * _12 * _43 + _31 * _13 * _42 + _41 * _12 * _33 - _41 * _13 * _32;
			inv[2] = _12 * _23 * _44 - _12 * _24 * _43 - _22 * _13 * _44 + _22 * _14 * _43 + _42 * _13 * _24 - _42 * _14 * _23;
			inv[6] = -_11 * _23 * _44 + _11 * _24 * _43 + _21 * _13 * _44 - _21 * _14 * _43 - _41 * _13 * _24 + _41 * _14 * _23;
			inv[10] = _11 * _22 * _44 - _11 * _24 * _42 - _21 * _12 * _44 + _21 * _14 * _42 + _41 * _12 * _24 - _41 * _14 * _22;
			inv[14] = -_11 * _22 * _43 + _11 * _23 * _42 + _21 * _12 * _43 - _21 * _13 * _42 - _41 * _12 * _23 + _41 * _13 * _22;
			inv[3] = -_12 * _23 * _34 + _12 * _24 * _33 + _22 * _13 * _34 - _22 * _14 * _33 - _32 * _13 * _24 + _32 * _14 * _23;
			inv[7] = _11 * _23 * _34 - _11 * _24 * _33 - _21 * _13 * _34 + _21 * _14 * _33 + _31 * _13 * _24 - _31 * _14 * _23;
			inv[11] = -_11 * _22 * _34 + _11 * _24 * _32 + _21 * _12 * _34 - _21 * _14 * _32 - _31 * _12 * _24 + _31 * _14 * _22;
			inv[15] = _11 * _22 * _33 - _11 * _23 * _32 - _21 * _12 * _33 + _21 * _13 * _32 + _31 * _12 * _23 - _31 * _13 * _22;
			det = _11 * inv[0] + _12 * inv[4] + _13 * inv[8] + _14 * inv[12]; 
			if (det == 0)
			{
				throw std::runtime_error("Matrix is singular and cannot be inverted");
			}
			det = 1.0 / det;
			Matrix4 invOut;
			for (i = 0; i < 16; i++)
			{
				invOut.v[i] = inv[i] * det;
			}
			return invOut; 
		}

		constexpr Matrix4 operator-() const
		{
			return Matrix4(
				-_11, -_12, -_13, -_14,
				-_21, -_22, -_23, -_24,
				-_31, -_32, -_33, -_34,
				-_41, -_42, -_43, -_44);
		}
		constexpr Matrix4 operator+(const Matrix4& rhs) const
		{
			return Matrix4(
				_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
				_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
				_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
				_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44);
		}
		constexpr Matrix4 operator-(const Matrix4& rhs) const
		{
			return Matrix4(
				_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
				_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
				_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
				_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44);
		}
		constexpr Matrix4 operator*(const Matrix4& rhs) const
		{
			return Matrix4(
				(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
				(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
				(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
				(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

				(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
				(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
				(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
				(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

				(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
				(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
				(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
				(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

				(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
				(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
				(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
				(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44));
		}
		constexpr Matrix4 operator*(float s) const
		{
			return Matrix4(
				_11 * s, _12 * s, _13 * s, _14 * s,
				_21 * s, _22 * s, _23 * s, _24 * s,
				_31 * s, _32 * s, _33 * s, _34 * s,
				_41 * s, _42 * s, _43 * s, _44 * s);
		}
		constexpr Matrix4 operator/(float s) const
		{
			return Matrix4(
				_11 / s, _12 / s, _13 / s, _14 / s,
				_21 / s, _22 / s, _23 / s, _24 / s,
				_31 / s, _32 / s, _33 / s, _34 / s,
				_41 / s, _42 / s, _43 / s, _44 / s);
		}
		constexpr Matrix4 operator+=(const Matrix4& rhs)
		{
			_11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
			_21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
			_31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
			_41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;
			return *this;
		}
	};
}