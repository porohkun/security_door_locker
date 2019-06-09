using DoorController;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConnectApp
{
    public class MessageLevelChecker: INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void NotifyPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }


        private bool _show = false;

        public MessageLevel Level { get; private set; }
        public bool Show
        {
            get => _show;
            set
            {
                if (_show != value)
                {
                    _show = value;
                    NotifyPropertyChanged(nameof(Show));
                }
            }
        }

        public MessageLevelChecker(MessageLevel level, PropertyChangedEventHandler callback)
        {
            Level = level;
            PropertyChanged += callback;
        }

        public override string ToString()
        {
            return Level.ToString();
        }
    }
}
