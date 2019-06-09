namespace DoorController
{
    [StartByte(0x08)]
    public class TagAuthMessage : StreamMessage
    {
        public byte[] TagUID { get; private set; }
        public int Index { get; private set; }
        public bool UidOnly { get; private set; }
        public bool Success { get; private set; }

        protected override void Read()
        {
            TagUID = ReadBytes(ReadByte());
            Index = ReadByte();
            Index = Index == 255 ? -1 : Index;
            UidOnly = ReadBool();
            Success = ReadBool();
            ShortText = $"Tag [{TagUID.ToHexString()}] auth as {(Index == 0 ? "Master" : $"slot {Index}")}: {(Success ? "Success" : "Failed")}";
            Finished = true;
        }
    }
}