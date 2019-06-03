using DoorController;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConnectApp
{
    public class MainWindowExample
    {
        public MainWindowExample() { }

        public ObservableCollection<IMessage> Messages { get; } = new ObservableCollection<IMessage>()
        {
            new SystemMessage(MessageLevel.Unknown, "test Unknown", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Unknown, "test Unknown", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Debug, "test Debug", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Debug, "test Debug", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test Info", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test Info", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Warning, "test Warning", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Warning, "test Warning", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Error, "test Error", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Error, "test Error", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Special, "test Special", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Special, "test Special", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla"),
            new SystemMessage(MessageLevel.Info, "test aaaa", "test aaaa\r\nmultiline\r\nbla bla bla")
        };
    }
}
