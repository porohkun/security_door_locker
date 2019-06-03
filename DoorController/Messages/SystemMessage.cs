using System;

namespace DoorController
{
    public class SystemMessage : IMessage
    {
        public bool Finished { get; } = true;
        public DateTime Time { get; private set; }
        public MessageLevel Level { get; private set; }

        public string ShortText { get; private set; }
        public string InnerText { get; private set; }
        public string FullText => this.GetFullText();

        public SystemMessage(MessageLevel type, string message)
        {
            Time = DateTime.UtcNow;
            Level = type;
            ShortText = message;
        }

        public SystemMessage(MessageLevel type, string message, string innerMessage)
        {
            Time = DateTime.UtcNow;
            Level = type;
            ShortText = message;
            InnerText = innerMessage;
        }

        public SystemMessage(Exception e)
        {
            Level = MessageLevel.Error;
            ShortText = $"Exception: {e.GetType().Name}\r\n{e.Message}\r\nStack trace:\r\n{e.StackTrace}";
            if (e.InnerException != null)
                InnerText = $"Inner Exception: {e.InnerException.GetType().Name}\r\n{e.InnerException.Message}\r\nStack trace:\r\n{e.InnerException.StackTrace}";
        }

        public void Read(MessageLevel level, byte[] buffer)
        {
            throw new NotImplementedException();
        }
    }
}
