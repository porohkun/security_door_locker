namespace DoorController
{
    [StartByte(0x01)]
    public class InitStateMessage : StreamMessage
    {
        public DoorState State { get; private set; }

        protected override void Read()
        {
            State = (DoorState)ReadByte();
            ShortText = $"Door state init: {State}";
            Finished = true;
        }
    }
}
