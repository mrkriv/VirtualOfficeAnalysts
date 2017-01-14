namespace LoginServer.Models
{
    public class Vector2D
    {
        public float X { get; set; }
        public float Y { get; set; }

        public Vector2D(float x, float y)
        {
            X = x;
            Y = y;
        }

        public override string ToString()
        {
            return string.Format("{0:0.0} {1:0.0}", X, Y);
        }
    }
}