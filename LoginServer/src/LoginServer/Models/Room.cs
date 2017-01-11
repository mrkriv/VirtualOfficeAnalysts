using System;

namespace LoginServer.Models
{
    public class Room
    {
        public uint ID { get; set; }
        public string Name { get; set; }
        public string CaseName { get; set; }
        public uint CaseID { get; set; }
        public bool IsOpen { get; set; }
        public Rights Rights { get; set; }
    }
}
