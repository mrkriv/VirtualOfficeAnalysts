using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LoginServer
{
    public class UserSession
    {
        public static Dictionary<uint, UserSession> OpenSessions = new Dictionary<uint, UserSession>();

        public readonly uint UIN;
        public readonly byte[] Key;
        public readonly uint UserId;

        public static UserSession Open(uint userid)
        {
            var ses = new UserSession(userid);
            OpenSessions.Add(ses.UIN, ses);
            
            return ses;
        }

        private UserSession(uint userid)
        {
            Key = Guid.NewGuid().ToByteArray();
            UIN = BitConverter.ToUInt32(Key, 2);
            UserId = userid;
        }

        public void Close()
        {
            OpenSessions.Remove(UIN);
        }
    }
}