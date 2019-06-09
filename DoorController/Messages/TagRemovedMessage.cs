namespace DoorController
{
    [StartByte(0x07)]
    public class TagRemovedMessage : StreamMessage
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
            ShortText = $"Tag [{TagUID.ToHexString()}] removed from {(Index == 0 ? "Master" : $"slot {Index}")}: {(Success ? "Success" : "Failed")}";
            Finished = true;
        }
    }
}