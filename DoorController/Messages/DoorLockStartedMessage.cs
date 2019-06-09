namespace DoorController
{
    [StartByte(0x0A)]
    public class DoorLockStartedMessage : StreamMessage
    {
        protected override void Read()
        {
            ShortText = "DoorLock Started";
            Finished = true;
        }
    }
}