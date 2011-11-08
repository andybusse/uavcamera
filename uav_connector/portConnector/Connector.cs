using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Configuration;
using System.Net.Sockets;
using System.IO;
//using System.Drawing; //uncomment this if working in window application
//using System.Windows.Forms;  //uncomment this if working in window application


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
        public Image imageFromByte(byte[] imageData) //uncomment this in window application
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
                    for (int oneCycleCount = 0; oneCycleCount < dataLength; oneCycleCount++, allDataCount++)
                    {
                        //Check for start of File)
                        if (uavData[oneCycleCount] == 0xFF)
                        {
                            if (isStartOfFile == false)
                            {
                                if (oneCycleCount != dataLength - 1)
                                {
                                    if (uavData[oneCycleCount + 1] == 0xD8)// If the command is the start of file then allDataCount++ 
                                                                            // If not then overwrite the data
                                    {
                                        isStartOfFile = true;
                                    }
                                }
                                else
                                {
                                    if (uavData[oneCycleCount] == 0xFF)
                                    {
                                        imageData[allDataCount] = (byte)((int)uavData[oneCycleCount] % 160); //save 0xFF 
                                        sendByte=UAV.Receive(uavData, dataLength, SocketFlags.None);    //do another cycle
                                        allDataCount++;
                                        if ((int)uavData[allDataCount]%160 == 0xD8)                 //if start of file
                                        {          
                                            isStartOfFile = true;
                                        }
                                    }

                                }
                            }
                            //check for end of file
                            if (isStartOfFile == true && isEndOfFile == false)
                            {
                                if (oneCycleCount != dataLength - 1)
                                {
                                    if (uavData[oneCycleCount + 1] == 0xD9)
                                    {
                                        isEndOfFile = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    imageData[allDataCount] = (byte)((int)uavData[oneCycleCount] % 160);//save 0xFF
                                    allDataCount++;
                                    sendByte=UAV.Receive(uavData, dataLength, SocketFlags.None); //do another cycle
                                    if ((int)uavData[allDataCount]%160 == 0xD9)
                                    {
                                        isEndOfFile = true; //set end of file to be true
                                    }
                                }
                            }
                        }

                        //getting data
                        //if using the real data don't comment the line below
                        //if (isStartOfFile == true && isEndOfFile == false)
                        {
                            imageData[allDataCount] = (byte)((int)uavData[oneCycleCount] % 160);
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
