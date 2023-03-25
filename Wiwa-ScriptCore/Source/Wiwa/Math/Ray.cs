namespace Wiwa
{
    public partial struct Ray
    {
        private Vector3 m_Origin;
        private Vector3 m_Direction;

        // Creates a ray starting at /origin/ along /direction/.

        public Ray(Vector3 origin, Vector3 direction)
        {
            m_Origin = origin;
            m_Direction = direction.normalized;
        }

        // The origin point of the ray.
        public Vector3 origin
        {
            get { return m_Origin; }
            set { m_Origin = value; }
        }

        // The direction of the ray.
        public Vector3 direction
        {
            get { return m_Direction; }
            set { m_Direction = value.normalized; }
        }

        // Returns a point at /distance/ units along the ray.
        public Vector3 GetPoint(float distance)
        {
            return m_Origin + m_Direction * distance;
        }
    }
}