﻿using System;

namespace DoorController
{
    public class DisconnectedMessage : IMessage
    {
        public bool Finished { get; } = true;
        public DateTime TimeUtc { get; private set; }
        public MessageLevel Level { get; private set; }

        public string ShortText { get; private set; }
        public string InnerText { get; private set; }
        public string FullText => this.GetFullText();

        public DisconnectedMessage(string message)
        {
            ShortText = message;
        }

        public void Read(MessageLevel level, byte[] buffer)
        {
            throw new NotImplementedException();
        }
    }
}
