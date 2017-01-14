using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LoginServer
{
    public class RoomSession
    {
        public static Dictionary<uint, RoomSession> OpenSessions = new Dictionary<uint, RoomSession>();
        
        public readonly uint RoomId;
        public readonly uint PID;

        public static RoomSession Open(uint roomid, uint pid)
        {
            var ses = new RoomSession(roomid, pid);
            OpenSessions.Add(roomid, ses);

            return ses;
        }

        private RoomSession(uint roomid, uint pid)
        {
            RoomId = roomid;
            PID = pid;
        }

        public void Close()
        {
            OpenSessions.Remove(RoomId);
        }

        public override string ToString()
        {
            return RoomId.ToString();
        }
    }
}
