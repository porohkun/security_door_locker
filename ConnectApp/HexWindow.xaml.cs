using DoorController;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
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
using System.Windows.Shapes;

namespace ConnectApp
{
    /// <summary>
    /// Interaction logic for HexWindow.xaml
    /// </summary>
    public partial class HexWindow : Window, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void NotifyPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public Controller DoorController { get; }
        public ObservableCollection<ByteRow> Rows { get; } = new ObservableCollection<ByteRow>();
        private ByteRow _lastRow;

        public HexWindow(Controller controller)
        {
            DoorController = controller;
            controller.DebugByteReceived += Controller_DebugByteReceived;
            InitializeComponent();
            DataContext = this;
        }

        public HexWindow()
        {
            InitializeComponent();
            DataContext = this;
        }

        private void Controller_DebugByteReceived(byte b)
        {
            if (_lastRow == null || _lastRow.Length == 16)
            {
                _lastRow = new ByteRow();
                Rows.Add(_lastRow);
            }
            _lastRow.AddByte(b);
        }
    }

    public class ByteRow : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void NotifyPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private string _rowString;

        public string Row
        {
            get => _rowString;
            set
            {
                if (_rowString != value)
                {
                    _rowString = value;
                    NotifyPropertyChanged(nameof(Row));
                }
            }
        }


        public string Byte00 => Length > 0 ? ((_row[0] < 16 ? "0" : "") + Convert.ToString(_row[0], 16).ToUpperInvariant()) : "";
        public string Byte01 => Length > 1 ? ((_row[1] < 16 ? "0" : "") + Convert.ToString(_row[1], 16).ToUpperInvariant()) : "";
        public string Byte02 => Length > 2 ? ((_row[2] < 16 ? "0" : "") + Convert.ToString(_row[2], 16).ToUpperInvariant()) : "";
        public string Byte03 => Length > 3 ? ((_row[3] < 16 ? "0" : "") + Convert.ToString(_row[3], 16).ToUpperInvariant()) : "";
        public string Byte04 => Length > 4 ? ((_row[4] < 16 ? "0" : "") + Convert.ToString(_row[4], 16).ToUpperInvariant()) : "";
        public string Byte05 => Length > 5 ? ((_row[5] < 16 ? "0" : "") + Convert.ToString(_row[5], 16).ToUpperInvariant()) : "";
        public string Byte06 => Length > 6 ? ((_row[6] < 16 ? "0" : "") + Convert.ToString(_row[6], 16).ToUpperInvariant()) : "";
        public string Byte07 => Length > 7 ? ((_row[7] < 16 ? "0" : "") + Convert.ToString(_row[7], 16).ToUpperInvariant()) : "";
        public string Byte08 => Length > 8 ? ((_row[8] < 16 ? "0" : "") + Convert.ToString(_row[8], 16).ToUpperInvariant()) : "";
        public string Byte09 => Length > 9 ? ((_row[9] < 16 ? "0" : "") + Convert.ToString(_row[9], 16).ToUpperInvariant()) : "";
        public string Byte10 => Length > 10 ? ((_row[10] < 16 ? "0" : "") + Convert.ToString(_row[10], 16).ToUpperInvariant()) : "";
        public string Byte11 => Length > 11 ? ((_row[11] < 16 ? "0" : "") + Convert.ToString(_row[11], 16).ToUpperInvariant()) : "";
        public string Byte12 => Length > 12 ? ((_row[12] < 16 ? "0" : "") + Convert.ToString(_row[12], 16).ToUpperInvariant()) : "";
        public string Byte13 => Length > 13 ? ((_row[13] < 16 ? "0" : "") + Convert.ToString(_row[13], 16).ToUpperInvariant()) : "";
        public string Byte14 => Length > 14 ? ((_row[14] < 16 ? "0" : "") + Convert.ToString(_row[14], 16).ToUpperInvariant()) : "";
        public string Byte15 => Length > 15 ? ((_row[15] < 16 ? "0" : "") + Convert.ToString(_row[15], 16).ToUpperInvariant()) : "";

        public int Length { get; private set; }
        private byte[] _row;

        public ByteRow()
        {
            _row = new byte[16];
        }

        public ByteRow(byte[] row) : base()
        {
            _row = row;
            for (int i = 0; i < row.Length; i++)
                AddByte(row[i]);
        }

        public void AddByte(byte b)
        {
            Row += (char)b;
            _row[Length] = b;
            var propName = $"Byte{(Length < 10 ? "0" : "")}{Length}";
            Length++;
            NotifyPropertyChanged(propName);
        }
    }
}
