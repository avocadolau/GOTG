namespace Wiwa
{
    public static class Mat4x4Values
    {
        // Matrix4x4.zero is of questionable usefulness considering C# sets everything to 0 by default, however:
        //  1. it's consistent with other Math structs in Unity such as Vector2, Vector3 and Vector4,
        //  2. "Matrix4x4.zero" is arguably more readable than "new Matrix4x4()",
        //  3. it's already in the API ..
        static readonly Matrix4x4 zeroMatrix = new Matrix4x4(new Vector4(0, 0, 0, 0),
            new Vector4(0, 0, 0, 0),
            new Vector4(0, 0, 0, 0),
            new Vector4(0, 0, 0, 0));

        // Returns a matrix with all elements set to zero (RO).
        public static Matrix4x4 zero { get { return zeroMatrix; } }

        static readonly Matrix4x4 identityMatrix = new Matrix4x4(new Vector4(1, 0, 0, 0),
            new Vector4(0, 1, 0, 0),
            new Vector4(0, 0, 1, 0),
            new Vector4(0, 0, 0, 1));

        // Returns the identity matrix (RO).
        public static Matrix4x4 identity { get { return identityMatrix; } }
    }
}