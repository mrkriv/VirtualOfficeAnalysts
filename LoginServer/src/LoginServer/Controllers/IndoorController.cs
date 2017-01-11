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
            Response.Cookies.Append("session", roomID.ToString());

            db.Execute("UPDATE voa.process SET isload=b'1' WHERE pid={0};", pid);
            return true;
        }

    }
}