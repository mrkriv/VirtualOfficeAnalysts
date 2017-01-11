using System;
using System.Collections.Generic;
using System.Linq;
using LoginServer.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;
using Microsoft.Extensions.Options;

namespace LoginServer.Controllers
{
    public abstract class ControllerBase : Controller
    {
        private static bool isFirst = true;
        protected const string mySqlDateFormat = "yyyy-MM-dd HH:mm:ss";
        protected DataBaseInterface db;
        protected Config config;

        public ControllerBase(IOptions<Config> _config)
        {
            config = _config.Value;

            db = new DataBaseInterface(
                config.DataBase.Host,
                config.DataBase.Port,
                config.DataBase.Database,
                config.DataBase.Login,
                config.DataBase.Password
            );

            if (isFirst)
            {
                OnForstLoad();
                isFirst = false;
            }
        }

        virtual protected void OnForstLoad()
        {
        }
    }
}