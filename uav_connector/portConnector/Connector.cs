using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Configuration;
using System.Net.Sockets;
using System.IO;
//using System.Drawing;
//using System.Windows.Forms;


namespace portConnector
{
    class Connector
    {
        //static int requestCounter;
        //This socket code is using to connect to the UAV
        Socket UAV = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        //This byte count is using for counting the increment byte 
        int byteCount;

        //UpdatingPopUp updatePage = new UpdatingPopUp();
        /*
        public Image imageFromByte(byte[] imageData)
        {
            Image image = null;
            MemoryStream ms = new MemoryStream(imageData);
            Image returnImage = Image.FromStream(ms);
            return image;
        }
         */

        private void sendCommand(byte[] command)
        {
            try
            {
                int sendByte = UAV.Send(command, command.Length, SocketFlags.None);
            }
            catch
            {
                //MessageBox.Show("Unable to send command to the port. \n Could be because: \n 1. Command is not correct\n 2.Can not connect to the port");
                Console.WriteLine("Unable to send command to the port. \n Could be because: \n 1. Command is not correct\n 2.Can not connect to the port");
            }

        }
        public byte[] byte_stream_from_port()
        {
            int allDataCount = 0,  dataLength = 8;
            byte[] address = new byte[1];
            byte[] uavData = new byte[8]; //take one byte at a time
            byte[] imageData = new byte[100000];
            // byte[] testMessage = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0 };
            byte[] testMessage = { 0 }; // send 0 to receive data
            bool isEndOfFile = false;
            bool isStartOfFile = false;
            try
            {
                int sendByte = UAV.Send(testMessage, testMessage.Length, SocketFlags.None); //send 0
                //Console.WriteLine("Sent {0} bytes.", sendByte);
                //UAV.Receive(uavData, 3, SocketFlags.None);
                do
                {
                    byteCount = UAV.Receive(uavData, dataLength, SocketFlags.None);
                    for (int oneClockDataCount = 0; oneClockDataCount < dataLength; oneClockDataCount++, allDataCount++)
                    {
                        //Check for start of File)
                        if (uavData[oneClockDataCount] == 0xFF)
                        {
                            if (isStartOfFile == false)
                            {
                                if (oneClockDataCount != dataLength - 1)
                                {
                                    if (uavData[oneClockDataCount + 1] == 0xD8)
                                    {
                                        isStartOfFile = true;
                                    }
                                }
                                else
                                {
                                    if (uavData[oneClockDataCount] == 0xFF)
                                    {
                                        imageData[allDataCount] = (byte)((int)uavData[oneClockDataCount] % 160);
                                        sendByte=UAV.Receive(uavData, dataLength, SocketFlags.None);
                                        if ((int)uavData[allDataCount]%160 == 0xD8)
                                        {
                                            allDataCount++;
                                            isStartOfFile = true;
                                        }
                                    }

                                }
                            }
                            //check for end of file
                            if (isStartOfFile == true && isEndOfFile == false)
                            {
                                if (oneClockDataCount != dataLength - 1)
                                {
                                    if (uavData[oneClockDataCount + 1] == 0xD9)
                                    {
                                        isEndOfFile = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    imageData[allDataCount] = (byte)((int)uavData[oneClockDataCount] % 160);
                                    
                                    sendByte=UAV.Receive(uavData, dataLength, SocketFlags.None);
                                    if ((int)uavData[allDataCount]%160 == 0xD9)
                                    {
                                        allDataCount++;
                                        isEndOfFile = true;
                                    }
                                }
                            }
                        }

                        //getting data
                        //if using the real data don't comment the line below
                        //if (isStartOfFile == true && isEndOfFile == false)
                        {
                            imageData[allDataCount] = (byte)((int)uavData[oneClockDataCount] % 160);
                            Console.WriteLine(imageData[allDataCount]);
                            allDataCount++;
                        }
                    }
                } while (isEndOfFile == false);
                //while(count<1000);
                //check for end of the image 
                //MessageBox.Show("The image have been saved");
                
                try
                {
                    Console.WriteLine("The image have been saved");
                    File.WriteAllBytes(@"H:\.das\Desktop\image1.txt", imageData);
                    Console.ReadLine();
                }
                catch (Exception e)
                {
                   // MessageBox.Show(e.ToString());
                    Console.WriteLine(e.ToString());
                }
            }
            catch (SocketException uavMessage)
            {
                //MessageBox.Show(uavMessage.Message + uavMessage.ErrorCode);
                Console.WriteLine(uavMessage.Message + uavMessage.ErrorCode);
            }
            return uavData;
        }

        public void ConnectToPort(Int32 portNumber, string portName)
        {
            try
            {
                UAV.Connect("localhost", portNumber);
                //MessageBox.Show("Connection to Port Name" + portName + "Port Number" + portNumber + "is complete!");
                Console.WriteLine("Connection to Port Name{0}  Port Number {1} is complete!", portName,portNumber);
            }
            catch
            {
                Console.WriteLine("Error code\n Unable to connect to UAV:\n Please check that:\n1.The UAV is connected \n2.The program gcs has opened \n3. The program has connect to the UAV\n4.The program has send stream dat\n5. The program as to run testbyte on da");
                //MessageBox.Show("Error code\n Unable to connect to UAV:\n Please check that:\n1.The UAV is connected \n2.The program gcs has opened \n3. The program has connect to the UAV\n4.The program has send stream dat\n5. The program as to run testbyte on da");
            }

        }


    }
}
