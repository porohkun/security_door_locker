using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
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
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void NotifyPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public string[] PortsList { get; } = new string[] { "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10", "COM11", "COM12" };
        public string SelectedPort { get; set; } = "COM3";
        public string ConnectStatus => _status.ToString();
        public bool IsConnected => _status == ConnectionStatus.Connected;
        public bool IsDisconnected => _status == ConnectionStatus.Disconnected;
        public string LogString
        {
            get => _logString;
            private set
            {
                if (_logString != value)
                {
                    _logString = value;
                    NotifyPropertyChanged(nameof(LogString));
                }
            }
        }

        private ConnectionStatus __status = ConnectionStatus.Disconnected;
        private ConnectionStatus _status
        {
            get => __status;
            set
            {
                if (__status != value)
                {
                    __status = value;
                    NotifyPropertyChanged(nameof(ConnectStatus));
                    NotifyPropertyChanged(nameof(IsConnected));
                    NotifyPropertyChanged(nameof(IsDisconnected));
                }
            }
        }
        private string _logString;

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        //private AdvancedSerialClient _serialClient;

        BackgroundWorker _bw = new BackgroundWorker();

        private void ConnectButton_Click(object sender, RoutedEventArgs e)
        {
            _status = ConnectionStatus.Connect;
            LogString += "begin connection...\r\n";
            _bw = new BackgroundWorker();
            _bw.DoWork += _bw_DoWork;
            _bw.ProgressChanged += _bw_ProgressChanged;
            _bw.RunWorkerCompleted += _bw_RunWorkerCompleted;
            _bw.WorkerReportsProgress = true;
            _bw.WorkerSupportsCancellation = true;
            _bw.RunWorkerAsync(SelectedPort);
            //_serialClient = new AdvancedSerialClient();

            //_serialClient.Open(SelectedPort, 115200);
            //_serialClient.MessageReceived += _serialClient_MessageReceived;

            //var message = new SerialProtocol.AdvancedSerialMessage();
            //message.ID = 0;
            //_serialClient.Send(message);
        }

        private void _bw_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                using (var sp = new SerialPort())
                {
                    sp.PortName = (string)e.Argument;
                    sp.BaudRate = 115200;
                    sp.Open();
                    _bw.ReportProgress(0);
                    while (!_bw.CancellationPending)
                    {
                        string a = sp.ReadExisting();
                        _bw.ReportProgress(1, a);
                        Thread.Sleep(200);
                    }
                }
            }
            catch (Exception ex)
            {
                _bw.ReportProgress(2, ex);
            }
        }

        private void _bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            switch (e.ProgressPercentage)
            {
                case 0:
                    _status = ConnectionStatus.Connected;
                    LogString += "connected.\r\n";
                    break;
                case 1:
                    LogString += (string)e.UserState;
                    break;
                case 2:
                    var ex = (Exception)e.UserState;
                    LogString += $"\r\n\r\nException: {ex.GetType().Name}\r\n{ex.Message}\r\n\r\nStack trace:\r\n{ex.StackTrace}\r\n";
                    break;
            }
        }

        private void _bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            _status = ConnectionStatus.Disconnected;
            LogString += "disconnected.\r\n";
        }

        //private void _serialClient_MessageReceived(SerialProtocol.AdvancedSerialMessage Message)
        //{

        //}

        private void DisconnectButton_Click(object sender, RoutedEventArgs e)
        {
            LogString += "disconnecting...\r\n";
            _bw.CancelAsync();
        }

        private void ClearButton_Click(object sender, RoutedEventArgs e)
        {
            LogString = "";
        }
    }
}
