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
using System.IO.Ports;
using System.Security.Cryptography.X509Certificates;
using static System.Net.Mime.MediaTypeNames;

namespace RoverNav
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        byte[] data_bytes = new byte[9];
        byte[] buffer = new byte[1];
        string[] input_text = new string[8];
        int error_count = 0;
        public MainWindow()
        {
            InitializeComponent();

            COMPortSelector.Init();
            COMPortSelector.SetDataReceivedHandle(aDataReceivedHandler);
        }

        private static void aDataReceivedHandler(
                    object sender,
                    SerialDataReceivedEventArgs e)
        {
            //Reception processing
        }

        private void SerialPortComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void Connect_Click(object sender, RoutedEventArgs e)
        {
            COMPortSelector.PushConnectButton();
        }

        void CollectInput()
        {
            input_text[0] = AngleBox0.Text;
            input_text[1] = DistanceBox0.Text;
            input_text[2] = AngleBox1.Text;
            input_text[3] = DistanceBox1.Text;
            input_text[4] = AngleBox2.Text;
            input_text[5] = DistanceBox2.Text;
            input_text[6] = AngleBox3.Text;
            input_text[7] = DistanceBox3.Text;
        }                   

        private void ParseInputs(String[] InputBoxInfo)
        {
            int value;

            for (int i = 0; i < input_text.Length; i++)
            {
                char n;
                if (i % 2 == 1)
                {
                    n = 'd';
                }
                else
                {
                    n = 'a';
                }
                if (int.TryParse(input_text[i], out value))
                {

                    if (n == 'a')
                    {
                        value += 90;
                    }
                    if (n == 'a' && value <= 180 && value >= 0)
                    {
                        data_bytes[i] = (byte)(value);
                    }
                    else if (n == 'd' && value <= 250 && value >= 0)
                    {
                        data_bytes[i] = (byte)(value);
                    }
                    else
                    {
                        StatusLabel.Content = "Distances are 0to250, Angles -90to90";
                        error_count++;
                    }
                }
                else
                {
                    StatusLabel.Content = "Invalid Input";
                    error_count++;
                }
            }
        }

        private void clearData(string[] input_text)
        {
            for (int i = 0; i < input_text.Length; i++)
            {
                input_text[i] = string.Empty;
            }
        }
        private void UploadButton_Click(object sender, RoutedEventArgs e)
        {
            CollectInput();
            ParseInputs(input_text);
            data_bytes[data_bytes.Length - 1] = (byte)5;

            if (error_count == 0)
            {
                if (COMPortSelector.IsConnected())
                {
                    COMPortSelector.port.Write(data_bytes, 0, data_bytes.Length);
                    StatusLabel.Content = "Success!";
                    //clearData(input_text);

                }
                else
                {
                    StatusLabel.Content = "Connect to COM port";
                }
            }

            error_count = 0;
            for(int i = 0; i < data_bytes.Length; i ++)
            {
                data_bytes[i] = (byte)0;
            }
        }
    }
}
