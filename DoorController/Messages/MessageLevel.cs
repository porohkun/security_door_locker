using System;

namespace DoorController
{
    [Flags]
    public enum MessageLevel : byte
    {
        Unknown = 0,
        Debug = 1 << 0,
        Info = 1 << 1,
        Warning = 1 << 2,
        Error = 1 << 3,
        Special = 1 << 4
    }
}