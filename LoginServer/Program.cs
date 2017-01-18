using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using System.Globalization;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Builder;

namespace LoginServer
{
    public class Program
    {
        public static void Main(string[] args)
        {
            System.Globalization.CultureInfo.CurrentCulture = new CultureInfo("en-US");
            
            var host = new WebHostBuilder()
                .UseKestrel()
                .UseUrls("http://192.168.1.44:5000")
                .UseContentRoot(Directory.GetCurrentDirectory())
                .UseIISIntegration()
                .UseStartup<Startup>()
                .Build();

            host.Run();
        }
    }
}
