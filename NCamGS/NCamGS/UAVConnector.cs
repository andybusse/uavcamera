using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net.Configuration;
using System.IO;

namespace NCamGS
{
    class UAVConnector
    {
        Socket dataPort = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        Socket consolePort = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        public void Connect(byte dataStream)
        {
            try
            {
                dataPort.Connect("localhost", 8802);
                //MessageBox.Show("Connection to Port Name" + portName + "Port Number" + portNumber + "is complete!");
                Console.WriteLine("Data port connection to Port Name {0}  Port Number {1} is complete!", "localhost", 8802);

                // send selection byte to select datastream 0
                byte[] dataStreamSelection = { dataStream };
                dataPort.Send(dataStreamSelection);
                Console.WriteLine("Data stream selection completed.");

                consolePort.Connect("localhost", 8800);
                Console.WriteLine("Data port connection to Port Name {0} Port Number {1} is complete!", "localhost", 8800);
            }
            catch
            {
                Console.WriteLine("Error code\n Unable to connect to UAV:\n Please check that:\n1.The UAV is connected \n2.The program gcs has opened \n3. The program has connect to the UAV\n4.The program has send stream dat\n5. The program as to run testbyte on da");
                //MessageBox.Show("Error code\n Unable to connect to UAV:\n Please check that:\n1.The UAV is connected \n2.The program gcs has opened \n3. The program has connect to the UAV\n4.The program has send stream dat\n5. The program as to run testbyte on da");
            }
        }

        public byte[] GetDataBytes()
        {
            //byte[] getByteSendBytes = new byte[numBytes]; // you need to send 0's to get bytes out for some reason (NOPE, it does not)
            //for(int i = 0; i < numBytes; i++) {
            //    getByteSendBytes[i] = 0;
            //}
            //dataPort.Send(getByteSendBytes);
            byte[] sizeByte = { 0 };
            try
            {
                dataPort.Receive(sizeByte, 1, SocketFlags.None);
            }
            catch
            {
                
            }
            int numBytes = sizeByte[0];
            byte[] recBytes = new byte[numBytes];
            try
            {
                dataPort.Receive(recBytes, numBytes, SocketFlags.None);
            }
            catch
            { 
                
            }
            return recBytes;
        }

        public void SendTextToUAV(string textToUAV)
        {
            char[] toUAVChar = new char[512];
            byte[] toUAVByte = new byte[512];
            byte[] fromUAVByte = new byte[1000];
            byte[] oneByteArray = new byte[1];
            textToUAV += "\n";
            toUAVChar = textToUAV.ToCharArray();
            toUAVByte = System.Text.Encoding.ASCII.GetBytes(toUAVChar);
            try
            {
                int sendByte = consolePort.Send(toUAVByte, toUAVChar.Length, SocketFlags.None);
            }
            catch (SocketException ex)
            {
                Console.WriteLine("ERROR: " + ex.Message);
            }
        }
        public void SendCommand(byte[] command, bool ack)
        {
            // use to send commmand to the uav
            string toUAV;
            //string fromUAV;
            char[] toUAVChar = new char[512];
            byte[] toUAVByte = new byte[512];
            byte[] fromUAVByte = new byte[1000];
            byte[] oneByteArray = new byte[1];

            toUAV = "payload[0].send_bytes";

            for (int commandByteCount = 0; commandByteCount < command.Length; commandByteCount++)
            {
                oneByteArray[0] = command[commandByteCount];
                toUAV += " 0x" + BitConverter.ToString(oneByteArray);
                //toUAV += " 0x" +System.Text.Encoding.ASCII.GetString(oneByteArray); 
            }

            toUAV += "\n";
            toUAVChar = toUAV.ToCharArray();
            toUAVByte = System.Text.Encoding.ASCII.GetBytes(toUAVChar);
            if (ack) // if the command is an ack it doesn't want acks
            {
                try
                {
                    int sendByte = consolePort.Send(toUAVByte, toUAVChar.Length, SocketFlags.None);
                }
                catch (SocketException ex)
                {
                    Console.WriteLine("ERROR: " + ex.Message);
                }
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    try
                    {
                        int sendByte = consolePort.Send(toUAVByte, toUAVChar.Length, SocketFlags.None);
                    }
                    catch (SocketException ex)
                    {
                        Console.WriteLine("ERROR: " + ex.Message);
                    }
                    for (int ii = 0; ii < 250; ii++)
                    {
                       byte[] packet = this.GetDataBytes();
                        int packetSize = packet.Length;
                        if (packetSize > 0)
                        {
                            if (packet[0] == 8 && packet[1] == command[0])
                            {
                                Console.WriteLine("Command aknowledged sir!");
                                return;
                            }
                        } 
                    }
                    
                }
                Console.WriteLine("No acknowledgement, sadface");
            }



            //Port.Receive(fromUAVByte);
            //fromUAV = System.Text.Encoding.ASCII.GetString(fromUAVByte);
        }

        public void Close()
        {
            dataPort.Close();
        }

    }
}
