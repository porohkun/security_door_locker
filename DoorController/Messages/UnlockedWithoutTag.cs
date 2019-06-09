namespace DoorController
{
    [StartByte(0x0C)]
    public class UnlockedWithoutTag : StreamMessage
    {
        protected override void Read()
        {
            ShortText = "Unlocked without tag";
            Finished = true;
        }
    }
}