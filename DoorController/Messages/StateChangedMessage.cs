namespace DoorController
{
    public enum DoorState : byte
    {
        Locked = 0x00,
        Unlocking = 0x01,
        Opened = 0x02,
        ListenForMaster = 0x03,
        ListenForEmpty = 0x04
    }

    [StartByte(0x03)]
    public class StateChangedMessage : StreamMessage
    {
        public DoorState FromState { get; private set; }
        public DoorState ToState { get; private set; }

        protected override void Read()
        {
            FromState = (DoorState)ReadByte();
            ToState = (DoorState)ReadByte();
            ShortText = $"Door state: {FromState} -> {ToState}";
            Finished = true;
        }
    }
}
