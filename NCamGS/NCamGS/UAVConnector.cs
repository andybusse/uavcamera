/* Copyright 2011 Michael Hodgson, Piyabhum Sornpaisarn, Andrew Busse, John Charlesworth, Paramithi Svastisinha

    This file is part of uavcamera.

    uavcamera is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    uavcamera is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uavcamera.  If not, see <http://www.gnu.org/licenses/>.

*/

﻿//#define SIMULATE

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
        Random random = new Random();

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
#if SIMULATE
                        if (random.Next(100) != 1)
                        {
#endif
                            int sendByte = consolePort.Send(toUAVByte, toUAVChar.Length, SocketFlags.None);
#if SIMULATE
                        }
                        else
                        {
                            Console.WriteLine("NOT SENDING!");
                        }
#endif
                    }
                    catch (SocketException ex)
                    {
                        Console.WriteLine("ERROR: " + ex.Message);
                    }
                    for (int ii = 0; ii < 600; ii++)
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

                    Console.WriteLine("No acknowledgement, sadface " + i);   
                }
            }



            //Port.Receive(fromUAVByte);
            //fromUAV = System.Text.Encoding.ASCII.GetString(fromUAVByte);
        }

        public void Close()
        {
            dataPort.Close();
        }

        ~UAVConnector()
        {
            this.Close();
        }

    }
}
