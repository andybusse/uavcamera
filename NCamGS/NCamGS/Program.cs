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

﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using System.Windows.Forms;

namespace NCamGS
{
    class Program
    {
        static MainForm mainForm = new MainForm();
        [STAThread]
        static void Main(string[] args)
        {
            mainForm.Show();

            while (true)
            {
                Application.DoEvents();
            }

#if false

            UAVConnector uavConn = new UAVConnector();
            uavConn.Connect(0);

            FileStream fileStream = new FileStream("output.jpg", FileMode.Create);
            BinaryWriter opFile = new BinaryWriter(fileStream);

            uavConn.SendTextToUAV("da 20 payload[0].mem_bytes[0]");


            byte[] zeroToken = { 0 }; // send 0 to receive data
            uavConn.SendCommand(zeroToken);
            
            uint imageID = 0;

            while (true)
            {
                Console.Write(".");
                byte[] packet = uavConn.GetDataBytes();
                int packetSize = packet.Length;

                if (packet[0] == 1)
                {
                    // got PICTURE_TAKEN
                    Console.WriteLine("Found PICTURE_TAKEN");
                    imageID = (uint)packet[1] + (uint)(packet[2] << 8);
                    break;
                }
            }

            byte[] imageDownloadRequest = { 2, (byte)imageID, (byte)(imageID >> 8) };
            uavConn.SendCommand(imageDownloadRequest);

            uint lastPacketNum = 500;

            uint totalPackets = 500;

            bool startImage = false;
            long numBytes = 0;
            while (true)
            {
                Console.Write(".");
                byte[] packet = uavConn.GetDataBytes();
                int packetSize = packet.Length;

                if (packet[0] == 3) // is a IMAGE_DOWNLOAD_INFO packet 
                {
                    Console.WriteLine("Found IMAGE_DOWNLOAD_INFO");
                    totalPackets = (uint)packet[1] + (uint)(packet[2] << 8);
                }
                else if(packet[0] == 4) // is a IMAGE_DATA packet
                {
                    Console.WriteLine("Found IMAGE_DATA");
                    uint packetNum = (uint)packet[1] + (uint)(packet[2] << 8);

                    Console.Write("#" + packetNum + " ");

                    for (int i = 3; i < packetSize; i++)
                    {
                        Console.Write(packet[i] + " ");
                    }
                    Console.WriteLine();



                    if (packetNum == 0)
                        startImage = true;

                    if (startImage == false)
                        continue;

                    if (packetNum == lastPacketNum)
                    {
                        //Console.WriteLine("End of image.");
                        continue;
                    }

                    Console.WriteLine("Writing.");
                    for (int i = 3; i < packetSize; i++)
                    {
                        opFile.Write(packet[i]);
                        numBytes++;
                    }

                    if (packetNum == totalPackets - 1)
                        break;
                    lastPacketNum = packetNum;
                }
            }

            Console.WriteLine("Done!");
            opFile.Close();

            uavConn.Close();

            Console.ReadLine();

#endif
        }
    }
}
