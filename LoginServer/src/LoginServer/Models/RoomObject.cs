using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LoginServer.Models
{
    public class RoomObject
    {
        public uint UIN { get; set; }
        public Vector3D Position { get; set; }
        public Vector2D Scale { get; set; }
        public string Content { get; set; }
    }
}