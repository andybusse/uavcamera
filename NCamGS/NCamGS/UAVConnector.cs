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

        public void Connect(string portName, int portNumber, byte dataStream)
        {
            try
            {
                dataPort.Connect(portName, portNumber);
                //MessageBox.Show("Connection to Port Name" + portName + "Port Number" + portNumber + "is complete!");
                Console.WriteLine("Data port connection to Port Name {0}  Port Number {1} is complete!", portName, portNumber);

                // send selection byte to select datastream 0
                byte[] dataStreamSelection = { dataStream };
                dataPort.Send(dataStreamSelection);
                Console.WriteLine("Data stream selection completed.");
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
            dataPort.Receive(sizeByte, 1, SocketFlags.None);
            int numBytes = sizeByte[0];
            byte[] recBytes = new byte[numBytes];
            dataPort.Receive(recBytes, numBytes, SocketFlags.None);
            return recBytes;
        }

    }
}
