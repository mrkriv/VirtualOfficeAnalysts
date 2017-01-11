using System;
using System.Collections.Generic;

namespace LoginServer.Models
{
    public class ArrayShell<T>
    {
        public T[] Array;

        public ArrayShell()
        {
            Array = new T[0];
        }

        public ArrayShell(T[] array)
        {
            Array = array;
        }

        public ArrayShell(List<T> list)
        {
            Array = list.ToArray();
        }

        public static implicit operator ArrayShell<T>(T[] array)
        {
            return new ArrayShell<T>(array);
        }

        public static implicit operator ArrayShell<T>(List<T> list)
        {
            return new ArrayShell<T>(list);
        }
    }
}