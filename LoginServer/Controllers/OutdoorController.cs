using System;
using System.Collections.Generic;
using System.Linq;
using LoginServer.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;
using System.Diagnostics;
using Microsoft.Extensions.Options;

namespace LoginServer.Controllers
{
    [Route("api")]
    public class OutdoorController : ControllerBase
    {
        public OutdoorController(IOptions<Config> config) : base(config) { }

        protected override void OnForstLoad()
        {
            base.OnForstLoad();

            db.Execute(
                "Truncate table voa.process;" +
                "Truncate table voa.process_user;");
        }

        [HttpPost("Login")]
        public Shell<bool> Login([FromBody]LoginPassword input)
        {
            if (CheckLogin())
                return true;

            uint userid = db.Get<uint>("SELECT userid FROM user where (login='{0}' and password='{1}');", out bool success, input.Login, input.Password);
            if (!success)
                return false;

            var session = UserSession.Open(userid);
            Response.Cookies.Append("session", session.UIN.ToString());
            return true;
        }

        [HttpPost("RegNew")]
        public Shell<string> RegNew([FromBody]LoginPassword input)
        {
            if(string.IsNullOrEmpty(input.Login) || string.IsNullOrEmpty(input.Password) || string.IsNullOrEmpty(input.Name))
                return "Invalid data";

            if(input.Password.Length <= 5)
                return "Password too short";

            if(!db.Execute("INSERT INTO user (login, password, name) VALUES ('{0}', '{1}', '{2}');",
                input.Login,
                input.Password,
                input.Name))
                return "Login match";

            Login(input);
            return "true";
        }

        [HttpGet("GetName")]
        public Shell<string> GetName()
        {
            var session = GetSession();
            if (session == null)
                return "";

            return db.Get<string>("SELECT name FROM user where userid={0};", session.UserId);
        }

        [HttpGet("Islogin")]
        public Shell<bool> Islogin()
        {
            return CheckLogin();
        }

        [HttpGet("unlogin")]
        public void Unlogin()
        {
            GetSession()?.Close();
        }

        [HttpPost("GetRoomPort")]
        public Shell<uint> GetRoomPort([FromBody]Shell<uint> roomID)
        {
            var session = GetSession();
            if (session == null)
                return 0;

            uint port = db.Get<uint>("SELECT port FROM process WHERE roomid = {0};", roomID);

            if (port != 0 && RoomSession.OpenSessions.ContainsKey(roomID))
            {
                var roomSession = RoomSession.OpenSessions[roomID];
                if (!Process.GetProcesses().Any(p => p.Id == roomSession.PID))
                {
                    db.Execute("DELETE FROM process WHERE roomid={0};", roomID);
                    RoomSession.OpenSessions.Remove(roomID);
                    port = 0;
                }
            }

            if (port == 0)
            {
                port = db.Get<uint>("SELECT MIN(port) FROM port WHERE port NOT IN (SELECT port FROM process)");

                Process p = new Process();
                p.StartInfo.FileName = config.RoomServer.FileName;
                p.StartInfo.WorkingDirectory = config.RoomServer.WorkingDirectory;
                p.StartInfo.Arguments = string.Format(config.RoomServer.Arguments, port, roomID);
                p.Start();

                db.Execute("INSERT INTO voa.process (pid, port, roomid) VALUES ({0}, {1}, {2})", (uint)p.Id, port, roomID);
            }

            return port;
        }

        [HttpPost("TryEnterRoom")]
        public Shell<bool> TryEnterRoom([FromBody]Shell<uint> roomID)
        {
            var session = GetSession();
            if (session == null)
                return false;

            if (db.Get<UInt64>("SELECT isload FROM process WHERE roomid = {0};", roomID.Value) == 0)
                return false;

           // db.Execute(
           //     "INSERT INTO voa.process_user (pid, userid) VALUES ((SELECT pid FROM process WHERE roomid = {0}), {1})", roomID.Value, session.UserId);
            return true;
        }

        [HttpGet("GetRooms")]
        public ArrayShell<Room> GetRooms()
        {
            var session = GetSession();
            if (session == null)
                return null;

            return db.GetArray<uint, string, Int16, uint, string, bool, Room>(
                tuple => new Room()
                {
                    ID = tuple.Item1,
                    Name = tuple.Item2,
                    Rights = (Rights)tuple.Item3,
                    CaseID = tuple.Item4,
                    CaseName = tuple.Item5,
                    IsOpen = tuple.Item6
                },
                "SELECT * FROM " +
                "(SELECT room.roomid, room.name, room_user.rights, @caseID:= room.caseid from room " +
                "LEFT JOIN room_user on room_user.roomid = room.roomid WHERE room_user.userid = 1) T1," +
                "(SELECT voa.case.name, !voa.case.isclose FROM voa.case WHERE caseid = @caseID) T2; ",
                session.UserId);
        }

        [HttpGet("GetCases")]
        public ArrayShell<Case> GetCases()
        {
            var session = GetSession();
            if (session == null)
                return null;

            return db.GetArray<uint, string, string, DateTime, Case>(
                tuple => new Case()
                {
                    ID = tuple.Item1,
                    Name = tuple.Item2,
                    Description = tuple.Item3,
                    OpenDate = tuple.Item4
                },
                "SELECT caseid, name, description, opendate FROM voa.case", session.UserId);
        }

        [HttpPost("CreateCase")]
        public Shell<bool> CreateCase([FromBody]Case input)
        {
            var session = GetSession();
            if (session == null)
                return false;

            db.Execute(
                "INSERT INTO voa.case (name, description, opendate) VALUES ('{0}', '{1}', '{2}');" +
                "SET @caseID := LAST_INSERT_ID();" +
                "INSERT INTO voa.case_user(userid, caseid, case_user.right) VALUES({3}, @caseID, 2);",
                input.Name, input.Description, DateTime.Now.ToString(mySqlDateFormat), session.UserId);

            return true;
        }

        [HttpPost("CreateRoom")]
        public Shell<bool> CreateRoom([FromBody]Room input)
        {
            var session = GetSession();
            if (session == null)
                return false;

            db.Execute(
                "INSERT INTO room (name, caseid) VALUES ((SELECT name FROM voa.case WHERE case.caseid = {0}), {0});" +
                "SET @roomID:= LAST_INSERT_ID();" +
                "INSERT INTO room_user(userid, roomid) VALUES({1}, @roomID);",
                input.CaseID, session.UserId);

            return true;
        }

        protected bool CheckLogin()
        {
            bool result = false;

            if (HttpContext.Request.Cookies.ContainsKey("session"))
            {
                uint uin = uint.Parse(HttpContext.Request.Cookies["session"]);
                result = UserSession.OpenSessions.ContainsKey(uin);
            }

            Response.StatusCode = result ? 200 : 403;

            return result;
        }

        protected UserSession GetSession()
        {
            if (!CheckLogin())
                return null;

            uint uin = uint.Parse(HttpContext.Request.Cookies["session"]);
            return UserSession.OpenSessions[uin];
        }
    }
}