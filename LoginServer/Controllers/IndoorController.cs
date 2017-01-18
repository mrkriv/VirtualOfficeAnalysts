using System;
using System.Collections.Generic;
using System.Linq;
using LoginServer.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Options;
using MySql.Data.MySqlClient;
using System.Net;

namespace LoginServer.Controllers
{
    [Route("Room")]
    public class IndoorController : ControllerBase
    {
        public IndoorController(IOptions<Config> config) : base(config) { }

        [HttpPost("Bind")]
        public Shell<bool> Bind([FromBody]Shell<uint> roomID)
        {
            uint pid = db.Get<uint>("SELECT pid FROM voa.process WHERE roomID={0}", roomID);

            var session = RoomSession.Open(roomID, pid);
            Response.Cookies.Append("session", session.ToString());

            db.Execute("UPDATE voa.process SET isload=b'1' WHERE pid={0};", pid);
            return true;
        }

        [HttpGet("GetObjects")]
        public ArrayShell<RoomObject> GetObjects()
        {
            var session = GetSession();
            if (session == null)
                return new RoomObject[] { };

            return db.GetArray<uint, string, float, float, float, float, float, RoomObject>(tuple =>
                new RoomObject()
                {
                    UIN = tuple.Item1,
                    Content = tuple.Item2,
                    Position = new Vector3D(tuple.Item3, tuple.Item4, tuple.Item5),
                    Scale = new Vector2D(tuple.Item6, tuple.Item7),
                },
                "SELECT uin, content, position_x, position_y, position_z, scale_w, scale_h FROM room_objects WHERE roomid = {0}",
                session.RoomId);
        }

        [HttpPost("AddObject")]
        public Shell<uint> AddObject([FromBody]Shell<RoomObject> obj)
        {
            return db.Get<uint>(
                "INSERT INTO room_objects (roomid, content, position_x, position_y, position_z, scale_w, scale_h) VALUES ({0}, '{1}', '{2}', '{3}', '{4}', '{5}', '{6}');" +
                "SELECT LAST_INSERT_ID();",
                GetSession().RoomId,
                obj.Value.Content,
                obj.Value.Position.X,
                obj.Value.Position.Y,
                obj.Value.Position.Z,
                obj.Value.Scale.X,
                obj.Value.Scale.Y);
        }

        [HttpPost("UpdateObjectTransform")]
        public void UpdateObjectTransform([FromBody]Shell<RoomObject> obj)
        {
            db.Execute(
                "UPDATE room_objects SET position_x={2}, position_y={3}, position_z={4}, scale_w={5}, scale_h={6} WHERE uin={1} and roomid={0};",
                GetSession().RoomId,
                obj.Value.UIN,
                obj.Value.Position.X,
                obj.Value.Position.Y,
                obj.Value.Position.Z,
                obj.Value.Scale.X,
                obj.Value.Scale.Y);
        }

        [HttpPost("UpdateObjectContent")]
        public void UpdateObjectContent([FromBody]Shell<RoomObject> obj)
        {
            db.Execute(
                "UPDATE room_objects SET content={2} WHERE uin={1} and roomid={0};",
                GetSession().RoomId, 
                obj.Value.UIN, 
                obj.Value.Content);
        }

        protected bool CheckLogin()
        {
            bool result = false;

            if (HttpContext.Request.Cookies.ContainsKey("session"))
            {
                uint uin = uint.Parse(HttpContext.Request.Cookies["session"]);
                result = RoomSession.OpenSessions.ContainsKey(uin);
            }

            Response.StatusCode = result ? 200 : 403;

            return result;
        }

        protected RoomSession GetSession()
        {
            if (!CheckLogin())
                return null;

            uint uin = uint.Parse(HttpContext.Request.Cookies["session"]);
            return RoomSession.OpenSessions[uin];
        }
    }
}