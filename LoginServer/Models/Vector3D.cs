namespace LoginServer.Models
{
    public class Vector3D
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }

        public Vector3D(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public override string ToString()
        {
            return string.Format("{0:0.0} {1:0.0} {1:0.0}", X, Y, Z);
        }
    }
}