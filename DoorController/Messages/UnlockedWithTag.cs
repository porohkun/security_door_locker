namespace DoorController
{
    [StartByte(0x0B)]
    public class UnlockedWithTag : StreamMessage
    {
        public byte[] TagUID { get; private set; }
        public bool Success { get; private set; }

        protected override void Read()
        {
            TagUID = ReadBytes(ReadByte());
            Success = ReadBool();
            ShortText = $"Unlocked with tag [{TagUID.ToHexString()}]: {(Success ? "Success" : "Failed")}";
            Finished = true;
        }
    }
}