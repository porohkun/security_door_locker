namespace DoorController
{
    [StartByte(0x06)]
    public class TagSavedMessage : StreamMessage
    {
        public byte[] TagUID { get; private set; }
        public int Index { get; private set; }
        public bool Success { get; private set; }

        protected override void Read()
        {
            TagUID = ReadBytes(ReadByte());
            Index = ReadByte();
            Index = Index == 255 ? -1 : Index;
            Success = ReadBool();
            ShortText = $"Tag [{TagUID.ToHexString()}] saved to {(Index == 0 ? "Master" : $"slot {Index}")}: {(Success ? "Success" : "Failed")}";
            Finished = true;
        }
    }
}