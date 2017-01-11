using System;

namespace LoginServer.Models
{
    public enum Rights : short
    {
        Guest = 0,
        User = 1,
        Admin = 2,
    }
}