using System;

namespace DoorController
{
    public class StartByteAttribute : Attribute
    {
        public byte StartByte { get; private set; }

        public StartByteAttribute(byte b)
        {
            StartByte = b;
        }
    }
}
