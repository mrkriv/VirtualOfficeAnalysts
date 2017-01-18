using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LoginServer.Models
{
    public class Config
    {
        public CRoomServer RoomServer { get; set; }
        public CDataBase DataBase { get; set; }

        public class CRoomServer
        {
            public string FileName { get; set; }
            public string Arguments { get; set; }
            public string WorkingDirectory { get; set; }
        }

        public class CDataBase
        {
            public string Host { get; set; }
            public uint Port { get; set; }
            public string Database { get; set; }
            public string Login { get; set; }
            public string Password { get; set; }
        }
    }
}