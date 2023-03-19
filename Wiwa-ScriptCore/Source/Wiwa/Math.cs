using System;

namespace Wiwa
{
    public static class Math
    {
        public static Vector3 forward = new Vector3(0f, 0f, 1f);
        public static Vector3 right = new Vector3(1f, 0f, 0f);
        public static Vector3 up = new Vector3(0f, 1f, 0f);
        public static readonly float Rad2Deg = 360 / (PI * 2);
        public static readonly float Deg2Rad = (PI * 2) / 360;
        #region Trigonometry
        public static T Clamp<T>(this T val, T min, T max) where T : IComparable<T>
        {
            if (val.CompareTo(min) < 0) return min;
            else if (val.CompareTo(max) > 0) return max;
            else return val;
        }
        public static double Radians(double degrees)
        {
            return degrees * System.Math.PI / 180f;
        }
        public static double Cos(double value)
        {
            return System.Math.Cos(value);
        }
        public static float Cos(float value)
        {
            return (float)Cos((double)value);
        }
        public static double Sin(double value)
        {
            return System.Math.Sin(value);
        }
        public static float Sin(float value)
        {
            return (float)Sin((double)value);
        }
        public static double Sqrt(double value)
        {
            return System.Math.Sqrt(value);
        }
        public static float Acos(float value)
        {
            return (float)System.Math.Acos((double)value);
        }
        public static double Atan2(double x, double y)
        {
            return System.Math.Atan2(x, y);
        }
        public static Vector3 Normalize(Vector3 A)
        {
            double distance = System.Math.Sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
            return new Vector3((float)(A.y / distance), (float)(A.y / distance), (float)(A.z / distance));
        }
        public static Vector3 CrossProduct(Vector3 v1, Vector3 v2)
        {
            float x, y, z;
            x = v1.y * v2.z - v2.y * v1.z;
            y = (v1.x * v2.z - v2.x * v1.z) * -1;
            z = v1.x * v2.y - v2.x * v1.y;

            return new Vector3(x, y, z);
        }
        public static float Dot(Vector3 v1, Vector3 v2)
        {
            return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
        }
        #endregion

        #region Transforms
        //X Pitch
        //Y Roll
        //Z Yaw
        public static Vector3 CalculateUp(ref Transform3D t3d)
        {
            Vector3 rotrad;
            rotrad.x = Math.DegToRad(t3d.Rotation.x);
            rotrad.y = Math.DegToRad(t3d.Rotation.y);
            rotrad.z = Math.DegToRad(t3d.Rotation.z);

            Vector3 up;
            up.x = Sin(rotrad.x) * Sin(rotrad.z);
            up.y = Cos(rotrad.x);
            up.z = Sin(rotrad.x) * Cos(rotrad.z);

            up.x = Math.RadToDeg(up.x);
            up.y = Math.RadToDeg(up.y);
            up.z = Math.RadToDeg(up.z);

            return up.Normalized();
        }

        public static Vector3 CalculateForward(ref Transform3D t3d)
        {
            Vector3 rotrad;
            rotrad.x = Math.DegToRad(t3d.Rotation.x);
            rotrad.y = Math.DegToRad(t3d.Rotation.y);
            rotrad.z = Math.DegToRad(t3d.Rotation.z);

            Vector3 forward;
            forward.x = Cos(rotrad.x) * Sin(rotrad.y);
            forward.y = -Sin(rotrad.x);
            forward.z = Cos(rotrad.x) * Cos(rotrad.y);

            forward.x = Math.RadToDeg(forward.x);
            forward.y = Math.RadToDeg(forward.y);
            forward.z = Math.RadToDeg(forward.z);

            return forward.Normalized();
        }
        public static Vector3 CalculateRight(ref Transform3D t3d)
        {
            Vector3 rotrad;
            rotrad.x = Math.DegToRad(t3d.Rotation.x);
            rotrad.y = Math.DegToRad(t3d.Rotation.y);
            rotrad.z = Math.DegToRad(t3d.Rotation.z);

            Vector3 right;
            right.x = Cos(rotrad.z);
            right.y = 0f;
            right.z = -Sin(rotrad.z);

            right.x = Math.RadToDeg(right.x);
            right.y = Math.RadToDeg(right.y);
            right.z = Math.RadToDeg(right.z);

            return right.Normalized();
        }
        public static void Translate(ref Transform3D transform, Vector3 pos)
        {
            // Translate the transform based on the pos vector and the rotation of the object
            transform.LocalPosition +=
            CalculateForward(ref transform) * pos.z + CalculateRight(ref transform) * pos.x + CalculateUp(ref transform) * pos.y;
        }
        public static Vector3 ToEuler(Quaternion q)
        {
            Vector3 angles;

            // roll (x-axis rotation)
            double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
            double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
            angles.y = (float)Math.Atan2(sinr_cosp, cosr_cosp);

            // pitch (y-axis rotation)
            double sinp = Math.Sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
            double cosp = Math.Sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
            angles.x = (float)(2 * Math.Atan2(sinp, cosp) - System.Math.PI / 2);

            // yaw (z-axis rotation)
            double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
            double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
            angles.z = (float)Math.Atan2(siny_cosp, cosy_cosp);

            return angles;
        }
        #endregion

        public static float PI = 3.14159265358979323846f;

        public static float DegToRad(float deg)
        {
            return deg * PI / 180.0f;
        }

        public static float RadToDeg(float rad)
        {
            return rad * 180.0f / PI;
        }
    }
    public struct Quaternion
    {
        public float x, y, z, w;
        public const float kEpsilon = 0.000001F;
        public float LengthSquared
        {
            get
            {
                return x * x + y * y + z * z + w * w;
            }
        }
        public static Quaternion identity
        {
            get
            {
                return new Quaternion(0f, 0f, 0f, 1f);
            }
        }
        public Vector3 xyz
        {
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
            }
            get
            {
                return new Vector3(x, y, z);
            }
        }
        public float Length
        {
            get
            {
                return (float)System.Math.Sqrt(x * x + y * y + z * z + w * w);
            }
        }
        public Quaternion(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        public Quaternion(float pitch, float yaw, float roll)
        {
            float cr = (float)Math.Cos(roll * 0.5);
            float sr = (float)Math.Sin(roll * 0.5);
            float cp = (float)Math.Cos(pitch * 0.5);
            float sp = (float)Math.Sin(pitch * 0.5);
            float cy = (float)Math.Cos(yaw * 0.5);
            float sy = (float)Math.Sin(yaw * 0.5);

            w = cr * cp * cy + sr * sp * sy;
            x = sr * cp * cy - cr * sp * sy;
            y = cr * sp * cy + sr * cp * sy;
            z = cr * cp * sy - sr * sp * cy;
        }
        public Quaternion(Vector3 v, float w)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = w;
        }
        public Quaternion(Vector3 euler)
        {
            float cr = (float)Math.Cos(euler.y * 0.5);
            float sr = (float)Math.Sin(euler.y * 0.5);
            float cp = (float)Math.Cos(euler.x * 0.5);
            float sp = (float)Math.Sin(euler.x * 0.5);
            float cy = (float)Math.Cos(euler.z * 0.5);
            float sy = (float)Math.Sin(euler.z * 0.5);

            w = cr * cp * cy + sr * sp * sy;
            x = sr * cp * cy - cr * sp * sy;
            y = cr * sp * cy + sr * cp * sy;
            z = cr * cp * sy - sr * sp * cy;
        }
        public static Vector3 operator *(Quaternion rotation, Vector3 point)
        {
            float num = rotation.x * 2f;
            float num2 = rotation.y * 2f;
            float num3 = rotation.z * 2f;
            float num4 = rotation.x * num;
            float num5 = rotation.y * num2;
            float num6 = rotation.z * num3;
            float num7 = rotation.x * num2;
            float num8 = rotation.x * num3;
            float num9 = rotation.y * num3;
            float num10 = rotation.w * num;
            float num11 = rotation.w * num2;
            float num12 = rotation.w * num3;
            Vector3 result;
            result.x = (1f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
            result.z = (num7 + num12) * point.x + (1f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
            result.y = (num8 - num11) * point.x + (num9 + num10) * point.y + (1f - (num4 + num5)) * point.z;
            return result;
        }
        public static Quaternion LookRotation(Vector3 forward, Vector3 up)
        {
            forward.Normalize();

            Vector3 vector = Vector3.Normalize(forward);
            Vector3 vector2 = Vector3.Normalize(Math.CrossProduct(up, vector));
            Vector3 vector3 = Math.CrossProduct(vector, vector2);
            var m00 = vector2.x;
            var m01 = vector2.y;
            var m02 = vector2.z;
            var m10 = vector3.x;
            var m11 = vector3.y;
            var m12 = vector3.z;
            var m20 = vector.x;
            var m21 = vector.y;
            var m22 = vector.z;


            float num8 = (m00 + m11) + m22;
            var quaternion = new Quaternion();
            if (num8 > 0f)
            {
                var num = (float)Math.Sqrt(num8 + 1f);
                quaternion.w = num * 0.5f;
                num = 0.5f / num;
                quaternion.x = (m12 - m21) * num;
                quaternion.y = (m20 - m02) * num;
                quaternion.z = (m01 - m10) * num;
                return quaternion;
            }
            if ((m00 >= m11) && (m00 >= m22))
            {
                var num7 = (float)Math.Sqrt(((1f + m00) - m11) - m22);
                var num4 = 0.5f / num7;
                quaternion.x = 0.5f * num7;
                quaternion.y = (m01 + m10) * num4;
                quaternion.z = (m02 + m20) * num4;
                quaternion.w = (m12 - m21) * num4;
                return quaternion;
            }
            if (m11 > m22)
            {
                var num6 = (float)Math.Sqrt(((1f + m11) - m00) - m22);
                var num3 = 0.5f / num6;
                quaternion.x = (m10 + m01) * num3;
                quaternion.y = 0.5f * num6;
                quaternion.z = (m21 + m12) * num3;
                quaternion.w = (m20 - m02) * num3;
                return quaternion;
            }
            var num5 = (float)Math.Sqrt(((1f + m22) - m00) - m11);
            var num2 = 0.5f / num5;
            quaternion.x = (m20 + m02) * num2;
            quaternion.y = (m21 + m12) * num2;
            quaternion.z = 0.5f * num5;
            quaternion.w = (m01 - m10) * num2;
            return quaternion;
        }
        public static float Dot(Quaternion a, Quaternion b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }
        public static bool IsEqualUsingDot(float dot)
        {
            // Returns false in the presence of NaN values.
            return dot > 1.0f - kEpsilon;
        }
        public static float Angle(Quaternion a, Quaternion b)
        {
            float dot = System.Math.Min(System.Math.Abs(Dot(a, b)), 1.0f);
            return IsEqualUsingDot(dot) ? 0.0f : Math.Acos(dot) * 2.0F * Math.Rad2Deg;
        }
        public static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta)
        {
            float angle = Quaternion.Angle(from, to);
            if (angle == 0.0f) return to;
            return SlerpUnclamped(ref from, ref to, System.Math.Min(1.0f, maxDegreesDelta / angle));
        }
        public static Quaternion SlerpUnclamped(ref Quaternion a, ref Quaternion b, float t)
        {
            // if either input is zero, return the other.
            if (a.LengthSquared == 0.0f)
            {
                if (b.LengthSquared == 0.0f)
                {
                    return identity;
                }
                return b;
            }
            else if (b.LengthSquared == 0.0f)
            {
                return a;
            }


            float cosHalfAngle = a.w * b.w + Math.Dot(a.xyz, b.xyz);

            if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
            {
                // angle = 0.0f, so just return one input.
                return a;
            }
            else if (cosHalfAngle < 0.0f)
            {
                b.xyz = -b.xyz;
                b.w = -b.w;
                cosHalfAngle = -cosHalfAngle;
            }

            float blendA;
            float blendB;
            if (cosHalfAngle < 0.99f)
            {
                // do proper slerp for big angles
                float halfAngle = (float)System.Math.Acos(cosHalfAngle);
                float sinHalfAngle = (float)System.Math.Sin(halfAngle);
                float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
                blendA = (float)System.Math.Sin(halfAngle * (1.0f - t)) * oneOverSinHalfAngle;
                blendB = (float)System.Math.Sin(halfAngle * t) * oneOverSinHalfAngle;
            }
            else
            {
                // do lerp if angle is really small.
                blendA = 1.0f - t;
                blendB = t;
            }

            Quaternion result = new Quaternion(blendA * a.xyz + blendB * b.xyz, blendA * a.w + blendB * b.w);
            if (result.LengthSquared > 0.0f)
                return Normalize(result);
            else
                return identity;
        }
        public static void Normalize(ref Quaternion q, out Quaternion result)
        {
            float scale = 1.0f / q.Length;
            result = new Quaternion(q.xyz * scale, q.w * scale);
        }
        public static Quaternion Normalize(Quaternion q)
        {
            Quaternion result;
            Normalize(ref q, out result);
            return result;
        }
    }

    public struct Vector4
    {
        public float x, y, z, w;

        public Vector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
    }
    public struct Vector3
    {
        public float x, y, z;
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        public static Vector3 operator *(float a, Vector3 b)
        {
            return new Vector3(a * b.x, a * b.y, a * b.z);
        }
        public static Vector3 operator *(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
        }
        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }
        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
        }
        public static Vector3 operator *(Vector3 a, float k)
        {
            return new Vector3(a.x * k, a.y * k, a.z * k);
        }
        public static Vector3 operator -(Vector3 v)
        {
            return new Vector3(-v.x, -v.y, -v.z);
        }

        public float Magnitude()
        {
            return (float)System.Math.Sqrt(x * x + y * y + z * z);
        }

        public Vector3 Normalized()
        {
            float mag = Magnitude();

            return new Vector3(x / mag, y / mag, z / mag);
        }

        public void Normalize()
        {
            float mag = Magnitude();

            x /= mag;
            y /= mag;
            z /= mag;
        }
        public static float Magnitude(Vector3 v)
        {
            return (float)System.Math.Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        }
        public static Vector3 Normalize(Vector3 v)
        {
            float mag = Magnitude(v);

            return new Vector3(v.x / mag, v.y / mag, v.z / mag);
        }
    }
    public struct Vector2
    {
        public float x, y;

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    }


}
