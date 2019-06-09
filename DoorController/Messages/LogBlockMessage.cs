namespace DoorController
{
    [StartByte(0x05)]
    public class LogBlockMessage : StreamMessage
    {
        public byte Block { get; private set; }
        public string Action { get; private set; }
        public byte[] Data { get; private set; }
        public bool Success { get; private set; }

        protected override void Read()
        {
            Block = ReadByte();
            Action = ReadString(ReadByte());
            Data = ReadBytes(ReadByte());
            Success = ReadBool();
            ShortText = $"{Action}. Block {Block}: {(Success ? "Success" : "Failed")}";
            if (Success)
                InnerText = $"Data: [{Data.ToHexString()}]";
            Finished = true;
        }
    }
}