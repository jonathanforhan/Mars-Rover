﻿using System;
using System.Windows.Controls;
using System.IO.Ports;
using System.Windows.Threading;
using System.ComponentModel;

using RoverNav;

delegate void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e);
static class COMPortSelector
{
    public static SerialPort port;
    private static int BAUDRATE = 9600;
    private static MainWindow mainWindow;
    private static ComboBox SerialPortComboBox;
    private static Button ConnectButton;
    private static DispatcherTimer _timer;
    private static bool isConnected = false;


    private static DataReceivedHandler data_received_handle_;

    public static void Init()
    {
        mainWindow = (MainWindow)App.Current.MainWindow;
        SerialPortComboBox = mainWindow.SerialPortComboBox;
        ConnectButton = mainWindow.Connect;
        SerialPortComboBox.SelectedIndex = 0;
        SetTimer();
    }
    public static void SetBaudrate(int baudrate)
    {
        BAUDRATE = baudrate;
    }
    public static void PushConnectButton()
    {
        if (IsConnected())
        {
            DisconnectPort();
        }
        else
        {
            ConnectPort();
        }
    }

    public static void ConnectPort()
    {
        if (isConnected) return;

        UpdateSerialPortComboBox();
        string port_name = SerialPortComboBox.Text;
        if (String.IsNullOrEmpty(port_name)) return;
        port = new SerialPort(port_name, BAUDRATE, Parity.None, 8, StopBits.One);
        try
        {
            port.Open();
            port.DtrEnable = true;
            port.RtsEnable = true;
            isConnected = true;
            ConnectButton.Content = "Disconnect";
            Console.WriteLine("Connected.");
            port.DataReceived += new SerialDataReceivedEventHandler(data_received_handle_);
        }
        catch (Exception err)
        {
            Console.WriteLine("Unexpected exception : {0}", err.ToString());
        }
    }
    public static void DisconnectPort()
    {
        if (isConnected)
        {
            port.Close();
            port.Dispose();
            isConnected = false;
            ConnectButton.Content = "Connect";
            Console.WriteLine("Disconnected.");
        }
    }
    public static bool IsConnected()
    {
        return isConnected;
    }

    public static void SetDataReceivedHandle(DataReceivedHandler data_received_handle)
    {
        data_received_handle_ = data_received_handle;
    }

    private static void UpdateSerialPortComboBox()
    {
        //Get the previously selected port
        string prev_selected_port = "";
        if (SerialPortComboBox.SelectedItem != null)
            prev_selected_port = SerialPortComboBox.SelectedItem.ToString();

        //Update port list
        string[] port_list = SerialPort.GetPortNames();
        SerialPortComboBox.Items.Clear();
        foreach (var i in port_list) SerialPortComboBox.Items.Add(i);

        //Reselect the previously selected port
        for (int i = 0; i < SerialPortComboBox.Items.Count; i++)
        {
            if (SerialPortComboBox.Items[i].ToString() == prev_selected_port)
                SerialPortComboBox.SelectedIndex = i;
        }
        //Select 0 if the number of ports is 1 or less
        if (SerialPortComboBox.Items.Count <= 1)
            SerialPortComboBox.SelectedIndex = 0;
    }
    private static void SetTimer()
    {
        _timer = new DispatcherTimer();
        _timer.Interval = new TimeSpan(0, 0, 1);
        _timer.Tick += new EventHandler(OnTimedEvent);
        _timer.Start();
        mainWindow.Closing += new CancelEventHandler(StopTimer);
    }
    private static void OnTimedEvent(Object source, EventArgs e)
    {
        UpdateSerialPortComboBox();
    }
    private static void StopTimer(object sender, CancelEventArgs e)
    {
        _timer.Stop();
    }
}