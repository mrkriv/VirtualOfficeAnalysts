using System;
using System.Collections.Generic;

namespace LoginServer.Models
{
    public class Case
    {
        public uint ID { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public DateTime OpenDate { get; set; }
        public bool IsOpen { get; set; }
    }
}