namespace DoorController
{
    [StartByte(1)]
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
