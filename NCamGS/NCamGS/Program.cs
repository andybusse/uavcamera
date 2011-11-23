using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace NCamGS
{
    class Program
    {
        static void Main(string[] args)
        {
            UAVConnector uavConn = new UAVConnector();
            uavConn.Connect("localhost", 8802, 0);

            FileStream fileStream = new FileStream("output.jpg", FileMode.Create);
            BinaryWriter opFile = new BinaryWriter(fileStream);

            uint lastPacketNum = 500;

            bool startImage = false;
            long numBytes = 0;
            while (true)
            {
                Console.WriteLine("Getting data...");
                byte[] packet = uavConn.GetDataBytes();
                int packetSize = packet.Length;
                uint packetNum = (uint)packet[0] + (uint)(packet[1] << 8);

                Console.Write("#" + packetNum + " ");

                for(int i = 2; i < packetSize; i++)
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
                for (int i = 2; i < packetSize; i++)
                {
                    opFile.Write(packet[i]);
                    numBytes++;
                }

                if (packetNum == 1157)
                    break;
                lastPacketNum = packetNum;
            }

            Console.WriteLine("Done!");
            opFile.Close();

            Console.ReadLine();
        }
    }
}
