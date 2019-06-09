using System.Text;

namespace DoorController
{
    [StartByte(0x00)]
    public class TextMessage : StreamMessage
    {
        protected override void Read()
        {
            ShortText = ReadString(_length);
            Finished = true;
        }
    }
}
