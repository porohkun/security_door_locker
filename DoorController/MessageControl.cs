using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Windows;
using System.Windows.Controls;

namespace DoorController
{
    public class MessageControl : ContentControl
    {
        public static readonly DependencyProperty MessageProperty = DependencyProperty.Register(
            nameof(Message),
            typeof(IMessage),
            typeof(MessageControl),
            new PropertyMetadata() { PropertyChangedCallback = MessageChanged });
        public static readonly DependencyProperty FinishedProperty = DependencyProperty.Register(nameof(Finished), typeof(bool), typeof(MessageControl), new PropertyMetadata(false));
        public static readonly DependencyProperty TimeProperty = DependencyProperty.Register(nameof(Time), typeof(DateTime), typeof(MessageControl), new PropertyMetadata(DateTime.MinValue));
        public static readonly DependencyProperty LevelProperty = DependencyProperty.Register(nameof(Level), typeof(MessageLevel), typeof(MessageControl), new PropertyMetadata(MessageLevel.Unknown));
        public static readonly DependencyProperty ShortTextProperty = DependencyProperty.Register(nameof(ShortText), typeof(string), typeof(MessageControl), new PropertyMetadata(""));
        public static readonly DependencyProperty InnerTextProperty = DependencyProperty.Register(nameof(InnerText), typeof(string), typeof(MessageControl), new PropertyMetadata(""));
        public static readonly DependencyProperty FullTextProperty = DependencyProperty.Register(nameof(FullText), typeof(string), typeof(MessageControl), new PropertyMetadata(""));

        public IMessage Message
        {
            get => (IMessage)GetValue(MessageProperty);
            set => SetValue(MessageProperty, value);
        }
        public bool Finished
        {
            get => (bool)GetValue(FinishedProperty);
            set => SetValue(FinishedProperty, value);
        }
        public DateTime Time
        {
            get => (DateTime)GetValue(TimeProperty);
            set => SetValue(TimeProperty, value);
        }
        public MessageLevel Level
        {
            get => (MessageLevel)GetValue(LevelProperty);
            set => SetValue(LevelProperty, value);
        }
        public string ShortText
        {
            get => (string)GetValue(ShortTextProperty);
            set => SetValue(ShortTextProperty, value);
        }
        public string InnerText
        {
            get => (string)GetValue(InnerTextProperty);
            set => SetValue(InnerTextProperty, value);
        }
        public string FullText
        {
            get => (string)GetValue(FullTextProperty);
            set => SetValue(FullTextProperty, value);
        }

        private static void MessageChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var control = d as MessageControl;
            var value = e.NewValue as IMessage;

            control.Finished = value.Finished;
            control.Time = value.TimeUtc;
            control.Level = value.Level;
            control.ShortText = value.ShortText;
            control.InnerText = value.InnerText;
            control.FullText = value.FullText;
        }
    }
}
