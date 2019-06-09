namespace DoorController
{
    public enum MifareKeyType : byte
    {
        KeyA = 0,
        KeyB = 1
    }
    [StartByte(0x04)]
    public class AuthBlockMessage : StreamMessage
    {
        public byte[] TagUID { get; private set; }
        public byte Block { get; private set; }
        public MifareKeyType KeyType { get; private set; }
        public byte[] Key { get; private set; }
        public bool Success { get; private set; }

        protected override void Read()
        {
            TagUID = ReadBytes(ReadByte());
            Block = ReadByte();
            KeyType = (MifareKeyType)ReadByte();
            Key = ReadBytes(ReadByte());
            Success = ReadBool();
            ShortText = $"Auth block {Block} with {KeyType} {(Success ? "Success" : "Failed")}";
            InnerText = $"Tag: [{TagUID.ToHexString()}] Key: [{Key.ToHexString()}]";
            Finished = true;
        }
    }
}