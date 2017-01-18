using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LoginServer.Models
{
    public class Shell<T>
    {
        public T Value { get; set; }

        public Shell() { }
        public Shell(T value)
        {
            Value = value;
        }

        public static implicit operator Shell<T>(T value)
        {
            return new Shell<T>(value);
        }

        public static implicit operator T(Shell<T> self)
        {
            return self.Value;
        }

        public override bool Equals(object obj)
        {
            return Value.Equals(obj);
        }

        public override int GetHashCode()
        {
            return Value.GetHashCode();
        }

        public override string ToString()
        {
            return Value != null ? Value.ToString() : "NULL";
        }
    }
}
