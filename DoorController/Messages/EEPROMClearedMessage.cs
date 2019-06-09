namespace DoorController
{
    [StartByte(0x09)]
    public class EEPROMClearedMessage : StreamMessage
    {
        protected override void Read()
        {
            ShortText = "EEPROM cleared";
            Finished = true;
        }
    }
}