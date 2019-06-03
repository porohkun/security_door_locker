namespace DoorController
{
    [StartByte(2)]
    public class ExitStateMessage : StreamMessage
    {
        public DoorState State { get; private set; }

        protected override void Read()
        {
            State = (DoorState)ReadByte();
            ShortText = $"Door state exit: {State}";
            Finished = true;
        }
    }
}
