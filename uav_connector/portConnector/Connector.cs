using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Configuration;
using System.Net.Sockets;
using System.Collections.Specialized;
using System.Collections;
using System.Threading;

namespace portConnector
{
    class Connector
    {
        static int requestCounter;
        static ArrayList hostData = new ArrayList();
        static StringCollection hostNames = new StringCollection();

        Socket UAV = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        public byte[] dataFromUAV()
        {
            byte[] uavData = new byte[8];
           // byte[] testMessage = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0 };
            byte[] testMessage = {0};
            try
            {
                int sendByte = UAV.Send(testMessage, testMessage.Length, SocketFlags.None);
                Console.WriteLine("Sent {0} bytes.", sendByte);
                int size = 8;

                while (true)
                {
                    
                    int byteCount = UAV.Receive(uavData, size, SocketFlags.None);
                   //Console.WriteLine( Encoding.Unicode.GetChars(uavData));
                   //Console.WriteLine(Encoding.UTF8.GetString(uavData));
                    for(int countIn=0; countIn<uavData.Length;countIn++)
                    Console.WriteLine("{0}", uavData[countIn]);
                }
            }
            catch(SocketException uavMessage)
            {
                Console.WriteLine("{0} Error code:{1}.", uavMessage.Message, uavMessage.ErrorCode);
            }
            
            return uavData;
        }

        public void ConnectToPort()
        {
            const Int32 portNumber = 8802;
            const string portName = "localhost";
            try
            {
                UAV.Connect("localhost", portNumber);
                Console.WriteLine("Connection to Port Name:{0}; Port Number{1} is complete!",portName,portNumber);
            }
            catch (SocketException uavMessage)
            {
                Console.WriteLine("{0} Error code:{1}.", uavMessage.Message, uavMessage.ErrorCode);
            }
            
        }


    }
}
