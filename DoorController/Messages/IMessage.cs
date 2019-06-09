using System;
using System.Text;

namespace DoorController
{
    public interface IMessage
    {
        bool Finished { get; }
        DateTime TimeUtc { get; }
        MessageLevel Level { get; }
        string ShortText { get; }
        string InnerText { get; }
        string FullText { get; }
        void Read(MessageLevel level, byte[] buffer);
    }

    public static class IMessageExtension
    {
        public static string GetFullText(this IMessage message)
        {
            return $"{message.ShortText}{(message.InnerText != null ? "\r\n" : "")}{message.InnerText ?? ""}";
        }

        public static string ToHexString(this byte b)
        {
            return (b < 16 ? "0" : "") + Convert.ToString(b, 16).ToUpperInvariant();
        }

        public static string ToHexString(this byte[] array, int bytesInRow = 16)
        {
            var sb = new StringBuilder();

            for (int i = 0; i < array.Length; i++)
            {
                sb.Append(array[i].ToHexString());
                var n = i + 1;
                var endline = n % 16 == 0;
                var endfile = n % array.Length == 0;
                if (!endfile)
                    if (endline)
                        sb.Append("\r\n");
                    else
                        sb.Append(' ');
            }

            return sb.ToString();
        }
    }
}
