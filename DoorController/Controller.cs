using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Reflection;
using System.Threading;

namespace DoorController
{
    public class Controller : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void NotifyPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private static Dictionary<byte, Type> _messageTypes = new Dictionary<byte, Type>();

        static Controller()
        {
            foreach (var type in Assembly.GetAssembly(typeof(IMessage)).GetTypes().Where(t => t.IsClass && !t.IsAbstract))
            {
                var attr = type.GetCustomAttribute<StartByteAttribute>();
                if (attr != null)
                    _messageTypes.Add(attr.StartByte, type);
            }
        }

        public event Action<byte> DebugByteReceived;
        public string PortName
        {
            get => _portName;
            set
            {
                if (_portName != value)
                {
                    _portName = value;
                    NotifyPropertyChanged(nameof(PortName));
                }
            }
        }
        public int BaudRate
        {
            get => _baudRate;
            set
            {
                if (_baudRate != value)
                {
                    _baudRate = value;
                    NotifyPropertyChanged(nameof(BaudRate));
                }
            }
        }
        public bool Connected
        {
            get => _connected;
            private set
            {
                if (_connected != value)
                {
                    _connected = value;
                    NotifyPropertyChanged(nameof(Connected));
                }
            }
        }
        public ObservableCollection<IMessage> MessageLog { get; } = new ObservableCollection<IMessage>()
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
        };

        private string _portName;
        private int _baudRate = 115200;
        private bool _connected;
        private BackgroundWorker _bw;
        private bool _debug;

        public Controller(bool debug)
        {
            _debug = debug;
            _bw = new BackgroundWorker();
            _bw.DoWork += _bw_DoWork;
            _bw.ProgressChanged += _bw_ProgressChanged;
            _bw.RunWorkerCompleted += _bw_RunWorkerCompleted;
            _bw.WorkerReportsProgress = true;
            _bw.WorkerSupportsCancellation = true;
        }

        public void Start()
        {
            _bw.RunWorkerAsync(new object[] { PortName, BaudRate });
            Connected = true;
        }

        public void Stop()
        {
            _bw.CancelAsync();
        }

        private void _bw_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                ReportProgress(BwReports.MessageReceived, new SystemMessage(MessageLevel.Info, "BackgroundWorker started"));
                using (var sp = new SerialPort())
                {
                    IMessage currentMessage = null;
                    IEnumerator readCoroutine = null;
                    sp.PortName = (string)((object[])e.Argument)[0];
                    sp.BaudRate = (int)((object[])e.Argument)[1];
                    sp.Open();
                    ReportProgress(BwReports.Connected);
                    while (!_bw.CancellationPending)
                    {
                        if (sp.BytesToRead == 0)
                        {
                            Thread.Sleep(50);
                            continue;
                        }
                        //System.Windows.MessageBox.Show(sp.ReadExisting());
                        //continue;
                        if (currentMessage == null)
                        {
                            var b = ReadByte(sp);
                            if (_messageTypes.ContainsKey(b))
                            {
                                currentMessage = Activator.CreateInstance(_messageTypes[b]) as IMessage;
                                readCoroutine = ReadCoroutine(currentMessage, sp);
                            }
                            else
                                ReportException(new KeyNotFoundException($"Unknown message type {b}"));
                        }
                        else
                        {
                            while (!_bw.CancellationPending && readCoroutine != null && sp.BytesToRead > 0)
                                try
                                {
                                    if (readCoroutine.MoveNext())
                                        if (currentMessage.Finished)
                                        {
                                            ReportProgress(BwReports.MessageReceived, currentMessage);
                                            currentMessage = null;
                                            readCoroutine = null;
                                        }
                                }
                                catch (Exception ex)
                                {
                                    ReportException(new InvalidOperationException($"Cant read {currentMessage.GetType().Name} message", ex));
                                }
                        }
                    }
                }
                _bw.ReportProgress((int)BwReports.Disconnected);
            }
            catch (Exception ex)
            {
                ReportException(ex);
            }
        }

        private IEnumerator<bool> ReadCoroutine(IMessage currentMessage, SerialPort sp)
        {
            var level = (MessageLevel)ReadByte(sp);
            yield return false;
            var length = ReadByte(sp) << 8;
            yield return false;
            length += ReadByte(sp);
            var buffer = new byte[length];
            for (int i = 0; i < length; i++)
            {
                yield return false;
                buffer[i] = ReadByte(sp);
            }
            currentMessage.Read(level, buffer);
            yield return true;
        }

        private byte ReadByte(SerialPort sp)
        {
            var b = (byte)sp.ReadByte();
            if (_debug)
                ReportDebug(b);
            return b;
        }

        private void ReportProgress(BwReports report, IMessage message = null)
        {
            _bw.ReportProgress((int)report, message);
        }

        private void ReportException(Exception exception)
        {
            _bw.ReportProgress((int)BwReports.Exception, exception);
        }

        private void ReportDebug(byte b)
        {
            _bw.ReportProgress((int)BwReports.Debug, b);
        }

        private void _bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            switch ((BwReports)e.ProgressPercentage)
            {
                case BwReports.Connected:
                    MessageLog.Add(new ConnectedMessage("Connected"));
                    break;
                case BwReports.Disconnected:
                    MessageLog.Add(new DisconnectedMessage("Disconnected"));
                    break;
                case BwReports.MessageReceived:
                    MessageLog.Add((IMessage)e.UserState);
                    break;
                case BwReports.Exception:
                    MessageLog.Add(new SystemMessage((Exception)e.UserState));
                    break;
                case BwReports.Debug:
                    DebugByteReceived?.Invoke((byte)e.UserState);
                    break;
            }
        }

        private void _bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            MessageLog.Add(new SystemMessage(MessageLevel.Info, "BackgroundWorker stopped"));
            Connected = false;
        }

        private enum BwReports
        {
            Connected,
            Disconnected,
            MessageReceived,
            Exception,
            Debug
        }
    }
}
