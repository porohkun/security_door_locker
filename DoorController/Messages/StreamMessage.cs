using System;
using System.IO;
using System.Text;

namespace DoorController
{
    public abstract class StreamMessage : IMessage
    {
        public bool Finished { get; protected set; }
        public DateTime TimeUtc { get; private set; }
        public MessageLevel Level { get; private set; }

        public string ShortText { get; protected set; }
        public string InnerText { get; protected set; }
        public string FullText => this.GetFullText();

        protected int _length;
        private MemoryStream _stream;

        public void Read(MessageLevel level, byte[] buffer)
        {
            TimeUtc = DateTime.UtcNow;
            Level = level;
            _length = buffer.Length;
            using (_stream = new MemoryStream(buffer))
                Read();
        }

        protected abstract void Read();

        protected bool CanRead()
        {
            return _stream.Position < _stream.Length;
        }

        protected bool ReadBool()
        {
            return _stream.ReadByte() != 0;
        }

        protected byte ReadByte()
        {
            return (byte)_stream.ReadByte();
        }

        protected byte[] ReadBytes(int length)
        {
            var result = new byte[length];
            for (int i = 0; i < length; i++)
                result[i] = (byte)_stream.ReadByte();
            return result;
        }

        protected int ReadInt16()
        {
            return (byte)_stream.ReadByte() << 8
                + (byte)_stream.ReadByte();
        }

        protected int ReadInt32()
        {
            return (byte)_stream.ReadByte() << 24
                + (byte)_stream.ReadByte() << 16
                + (byte)_stream.ReadByte() << 8
                + (byte)_stream.ReadByte();
        }

        protected string ReadString(int length)
        {
            var sb = new StringBuilder();
            for (int i = 0; i < length; i++)
                sb.Append((char)ReadByte());
            return sb.ToString();
        }
    }
}
