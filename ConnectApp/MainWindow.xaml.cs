using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using DoorController;

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
        public ObservableCollection<IMessage> Messages => DoorController.MessageLog;
        public Controller DoorController { get; } = new Controller(true);

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;

            var hexWin = new HexWindow(DoorController);
            hexWin.Show();
        }

        private void ConnectButton_Click(object sender, RoutedEventArgs e)
        {
            DoorController.Start();
        }

        private void DisconnectButton_Click(object sender, RoutedEventArgs e)
        {
            DoorController.Stop();
        }

        private void ClearButton_Click(object sender, RoutedEventArgs e)
        {
            Messages.Clear();
        }
    }
}
