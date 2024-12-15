#pragma once

namespace MEngine::Math
{
    struct Matrix4;

    //------------------------------------------------------------------------------
    // Quaternion
    struct Quaternion
    {
        float x, y, z, w;
        Quaternion() = default;
        constexpr Quaternion(float ix, float iy, float iz, float iw) noexcept : x(ix), y(iy), z(iz), w(iw) {}

        Quaternion(const Quaternion&) = default;
        Quaternion& operator=(const Quaternion&) = default;

        Quaternion(Quaternion&&) = default;
        Quaternion& operator=(Quaternion&&) = default;

        bool operator == (const Quaternion& q) const { return x == q.x && y == q.y && z == q.z && w == q.w; }
        bool operator != (const Quaternion& q) const { return x != q.x || y != q.y || z != q.z || w != q.w; }

        Quaternion operator+(const Quaternion& rhs) const { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        Quaternion operator-(const Quaternion& rhs) const { return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }

        Quaternion operator*(float s) const { return Quaternion(x * s, y * s, z * s, w * s); }
        Quaternion operator/(float s) const { return Quaternion(x / s, y / s, z / s, w / s); }



        Quaternion operator*(const Quaternion& q) const 
        {
            return Quaternion(
                w * q.x + x * q.w + y * q.z - z * q.y,  // x component
                w * q.y - x * q.z + y * q.w + z * q.x,  // y component
                w * q.z + x * q.y - y * q.x + z * q.w,  // z component
                w * q.w - x * q.x - y * q.y - z * q.z   // w component
            );
        }

        


        static float getAngle(const Quaternion& q) 
        {
            // Ensure the quaternion is normalized
            float magnitude = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
            float wNormalized = q.w / magnitude;

            // Calculate the angle in radians
            float angle = 2.0f * std::acos(wNormalized);

            return angle;
        }     

        static const Quaternion Identity;
        static const Quaternion Zero;

        void Conjugate() noexcept;
        void Invers() noexcept;
        float Magnitude() const noexcept;
        float MagnitudesSquared() const noexcept;
        void Normalize() noexcept;
        float Dot(const Quaternion& q) const noexcept;

        static  Quaternion Conjugate(const Quaternion& q);
        static float Magnitude(const Quaternion& q);
        static Quaternion Normalize(const Quaternion& q);       

        static Quaternion CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;
        static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
        static Quaternion CreateFromRotationMatrix(const Matrix4& m)noexcept;

        static Quaternion Lerp(const Quaternion& q0, const Quaternion& q1, float t);
        static Quaternion slerp(const Quaternion& q0, const Quaternion& q1, float t);
    };
}