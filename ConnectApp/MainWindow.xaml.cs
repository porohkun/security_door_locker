using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ConnectApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public string[] PortsList { get; } = new string[] { "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10", "COM11", "COM12" };
        public string SelectedPort { get; set; } = "COM3";
        public string ConnectStatus => _status.ToString();
        public bool IsConnected => _status == ConnectionStatus.Connected;
        public bool IsDisconnected => _status == ConnectionStatus.Disconnected;
        public string LogString { get; private set; }

        private ConnectionStatus _status = ConnectionStatus.Disconnected;

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        private AdvancedSerialClient _serialClient;

        private void ConnectButton_Click(object sender, RoutedEventArgs e)
        {
            _serialClient = new AdvancedSerialClient();

            _serialClient.Open(SelectedPort, 115200);

            var message = new SerialProtocol.AdvancedSerialMessage();
            message.ID = 0;
            _serialClient.Send(message);
        }

        private void DisconnectButton_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
