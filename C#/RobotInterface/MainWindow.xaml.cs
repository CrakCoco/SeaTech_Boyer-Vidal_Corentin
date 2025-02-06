using System.IO.Ports;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ExtendedSerialPort_NS;

namespace CocoC_
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ExtendedSerialPort serialPort1;
        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();
        }

        private void SendMessage()

        {
            serialPort1.WriteLine(textBoxEmission.Text);
            if (textBoxEmission.Text.Length != 0)
            {
                textBoxReception.Text = textBoxReception.Text + "Reçu : " + textBoxEmission.Text + "\n";
                textBoxEmission.Text = "";
            }
        }
        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (buttonEnvoyer.Background == Brushes.RoyalBlue)
            {
                buttonEnvoyer.Background = Brushes.Beige;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
            }

            SendMessage();
        }

        private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }
    }
}