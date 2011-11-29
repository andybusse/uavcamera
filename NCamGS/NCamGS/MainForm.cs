using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;
using System.Threading;
using System.Net.Sockets;

namespace NCamGS
{
    public partial class MainForm : Form
    {
        
        int count = 0;
        private static int uavDataLength = 512;
        //Connector streamPort = new Connector();


        UAVConnector uavConn = new UAVConnector();

       

        byte[] uavDataPrevious = new byte[uavDataLength];
        private const byte
            TAKE_PICTURE = 0x00,// send to console port payload[0].sendbytes 0
            PICTURE_TAKEN = 0x01,//wait for picture taken (from data port) so do payload[0].membytes[0] then set payload[0].membytes[0] 0
            // it will be [ID][MSB][LSB]
            SEND_DOWNLOAD_REQUEST = 0x02, // send to console port send download request
            //[ID][MSB][LSB]
            DOWNLOAD_INFO_COMMAND = 0x03, // from cam to ground
            //[3][MSB][LSB]
            TAKEN_IMAGE_DATA = 0x04;// image data
            //[4][MSB][LSB]
        //byte[]readyForNextCommand =new byte[2];
        //byte[]stopSendingDataCommand=new byte[2];
        //byte[]notReadyCommand=new byte[2];
        //bool pictureTaken=false;
        

        int filePathCount = 0;
        string[] jpegList;
        int jpegOnlyCount = 0;
        bool stopCommand = false,takeNewPicture=false;


        string fileDirectory = null;


        public MainForm()
        {
            InitializeComponent();
            MyInitialize();
        }
        private void MyInitialize()
        {

            uavConn.Connect(0);
            progressBar.Minimum = 1;
            progressBar.Step = 1;
            progressBar.Value = 1;
            fileDirectory= System.IO.Directory.GetCurrentDirectory();
            filePathTextBox.Text = System.IO.Directory.GetCurrentDirectory();
            jpegList = Directory.GetFiles(fileDirectory, "*.jpg");
            uavConn.SendTextToUAV("payload.broadcast_bytes 255 0");
            ResolutionComboBox.SelectedIndex = 3;

        }
        /*[STAThread] //
        static void Main()
        {

            Application.Run(new MainForm());

        }*/

        private void paintButton_Click(object sender, EventArgs e)
        {
            takeNewPicture = true;
            doCommand();

        }
        private void doCommand()
        {
            statusLabel.Text = "Starting";
            progressBar.Value = 1;
            string fileName = string.Format("uavPictureAt{0:yyyy-MM-dd_hh-mm-ss-tt}.jpg", DateTime.Now);
            FileStream fileStream;

                fileStream = new FileStream(filePathTextBox.Text+"\\"+fileName, FileMode.Create);
            BinaryWriter opFile = new BinaryWriter(fileStream);


            uavConn.SendTextToUAV("da 20 payload[0].mem_bytes[0]");

            stopCommand = false;
            byte[] zeroToken = { 0 }; // send 0 to receive data
            uavConn.SendCommand(zeroToken);

            uint imageID = 0;

            while (stopCommand == false)
            {
                Application.DoEvents();
                Console.Write(".");
                byte[] packet = uavConn.GetDataBytes();
                int packetSize = packet.Length;
                if (packetSize > 0)
                {
                    if (packet[0] == 1 && packetSize == 3)
                    {
                        // got PICTURE_TAKEN
                        statusLabel.Text = "Found PICTURE_TAKEN";
                        Console.WriteLine("Found PICTURE_TAKEN");
                        imageID = (uint)packet[1] + (uint)(packet[2] << 8);
                        break;
                    }
                }
            }

            byte[] imageDownloadRequest = { 2, (byte)imageID, (byte)(imageID >> 8) };
            uavConn.SendCommand(imageDownloadRequest);

            uint lastPacketNum = 500;

            uint totalPackets = 500;

            bool startImage = false;
            long numBytes = 0;
            while (stopCommand==false)
            {
                Application.DoEvents();
                // add cancel code check

                Console.Write(".");
                byte[] packet = uavConn.GetDataBytes();
                int packetSize = packet.Length;
                //progressbar maximum

                if (packet[0] == 3) // is a IMAGE_DOWNLOAD_INFO packet 
                {
                    statusLabel.Text = "Found IMAGE_DOWNLOAD_INFO";
                    Console.WriteLine("Found IMAGE_DOWNLOAD_INFO");
                    totalPackets = (uint)packet[1] + (uint)(packet[2] << 8);
                    progressBar.Maximum = (int)totalPackets;
                }
                else if (packet[0] == 4) // is a IMAGE_DATA packet
                {
                    statusLabel.Text = "Found IMAGE_DATA";
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
                    statusLabel.Text = "Writing";
                    Console.WriteLine("Writing.");
                    for (int i = 3; i < packetSize; i++)
                    {
                        opFile.Write(packet[i]);
                        numBytes++;
                    }
                    progressBar.PerformStep();
                    if (packetNum == totalPackets - 1)
                        break;
                    lastPacketNum = packetNum;

                }
            }
            if (stopCommand == false)
            {
                fileStream.Close();
                opFile.Close();
                progressBar.Value = 1;

                statusLabel.Text = "Done!";
                Console.WriteLine("Done!");


                


                updateDirectory(fileName);

                Array.Resize(ref jpegList, jpegList.Length + 1);
                jpegList[jpegList.Length - 1] = fileDirectory + "\\" + fileName;
                jpegList = Directory.GetFiles(fileDirectory, "*.jpg");


                Image img = Image.FromFile(fileDirectory + "\\" + fileName);
                pictureBox.Image = img;
                statusLabel.Text = null;
            }
            if (stopCommand == true)
            {

                fileStream.Close();
                opFile.Close();
                File.Delete(filePathTextBox.Text + "\\" + fileName);
                statusLabel.Text = "Stop";
            }
           // Console.ReadLine();

            /*
           // CameraCommand newCommand = new CameraCommand();
            byte[] command = new byte[3];
            byte[] uavDataCurrent = new byte[uavDataLength];
            byte[] imageData = new byte[250];
            byte[] allImageData = new byte[30000];
            byte[] zeroToken = { 0 }; // send 0 to receive data
            string fileName=string.Format("uavPictureAt{0:yyyy-MM-dd_hh-mm-ss-tt}.jpg", DateTime.Now);
            

           // uavConnector.SendTextToUAV("da 20 payload[0].mem_bytes[0]");

            uavConnector.StartStream(0);

            byte[] currentCommand = { 0 };

            uavConnector.SendCommand(zeroToken);

          //  uavConnector.SendTextToUAV("payload[0].mem_bytes[0] 0");

            //while (currentCommand.Length > 0 && currentCommand[0] != PICTURE_TAKEN)
            while (true)
            {
               // currentCommand = uavConnector.ReceiveFromStream();
                currentCommand = uavConnector.ReceiveFromStream();
            }
            
            //uavConnector.SendTextToUAV("payload[0].mem_byte[0] 0");

            uint imageID = (uint)currentCommand[1] + (uint)(currentCommand[2] << 8);

            command[0] = SEND_DOWNLOAD_REQUEST;
            command[1] = currentCommand[1];
            command[2] = currentCommand[2];

            uavConnector.SendCommand(command);

            while (currentCommand[0] != DOWNLOAD_INFO_COMMAND)
            {
                currentCommand = uavConnector.ReceiveFromStream();
            }

            uint numberOfPacket = (uint)command[1] + (uint)(command[2] << 8);
            int allImageDataCount=0;

            while (currentCommand[0] != TAKEN_IMAGE_DATA)
            {
                currentCommand = uavConnector.ReceiveFromStream();
            }

            uint prevPacketNumber = 500;

            while(currentCommand[0] == TAKEN_IMAGE_DATA)
            {
                uint currentPacketNum = (uint)command[1] + (uint)(command[2] << 8);

                if (currentPacketNum == prevPacketNumber + 1)
                {

                    for (int countData = 3; countData < currentCommand.Length; countData++, allImageDataCount++)
                    {
                        allImageData[allImageDataCount] = imageData[countData];
                    }
                    if (currentPacketNum >= numberOfPacket - 1)
                    {
                        break;
                    }
                }
                else if(currentPacketNum != prevPacketNumber)
                {
                    throw new Exception("ERROR: Skipped a packet.");
                }
                prevPacketNumber = currentPacketNum;
                currentCommand = uavConnector.ReceiveFromStream();
            }
            Image imageFromUAV = uavConnector.GetImage(allImageData);
            pictureBox.Image = imageFromUAV;

            uavConnector.SaveImage(allImageData, fileName, fileDirectory);
            //uavDataPrevious = uavDataCurrent;
             * 
             * */
            /*
            while (!endOfCommand)
            {
                
                uavDataCurrent= streamPort.StartReceiving();
                switch (uavDataCurrent[COMMAND_BYTE_LOCATION])
                {
                    case PICTURE_TAKEN:
                        pictureTaken = true;
                        numberOfPacket = (int)uavDataCurrent[1]*0xFF+(int)uavDataCurrent[2];
                        imageID = (int)uavDataCurrent[3];
                        break;
                    case IMAGE_DATA:
                        if (pictureTaken == true)
                        {

                            if (uavDataCurrent != uavDataPrevious && pictureTaken == true)
                            {
                                packetID = (int)uavDataCurrent[PACKET_ID_LOCATION];
                                dataLength = (int)uavDataCurrent[LENGTH_BYTE_LOCATION_1] * 0xFF + uavDataCurrent[LENGTH_BYTE_LOCATION_2];
                                // IMPORTANT: Make the second byte(first byte is command byte) of the data a data length byte in the PAYLOAD MODULE
                                //This for loop check whether the data is at the end of file or not //I set 0xD9 to be the end of file code
                                //also it will check for data length cycle.
                                //IMPORTANT: the eachByteCheck start at 1 so make sure that the data start at second byte !! (or modify the code)
                                for (int eachByteCheck = START_OF_IMAGE_DATA; eachByteCheck < dataLength&&!endOfCommand; eachByteCheck++, count++)
                                {
                                    imageData[count] = uavDataCurrent[eachByteCheck];

                                    //Check for end of file
                                    if (imageData[count] == COMMAND_HEADER || nextIsCommandByte)
                                    {
                                        if (((eachByteCheck != dataLength - 1) && imageData[count + 1] == END_OF_FILE) || nextIsCommandByte && imageData[count] == END_OF_FILE)
                                        {
                                            endOfCommand = true;
                                        }
                                        else if (eachByteCheck == dataLength - 1)
                                        {
                                            nextIsCommandByte = true;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
             * */
            //consolePort.SendCommand(stopSendingDataCommand, STOP_SENDING_COMMAND_LENGTH); 

        }
        private void receivingCase(byte[] receivedData)
        {
            byte command = 0;
            switch(command)
            {
                case PICTURE_TAKEN:
                break;
                case DOWNLOAD_INFO_COMMAND:
                break;
                case TAKEN_IMAGE_DATA:
                break;
            }
        }
        private void sendingCase()
        { 
            byte command=0;
        switch(command)
        {
            case TAKE_PICTURE:

                break;
            case SEND_DOWNLOAD_REQUEST:

                break;
                
        }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

        }

        private void updateDirectory(string fileName)
        {
            fileDirectory =filePathTextBox.Text;
            
            fileName = fileDirectory +"\\"+ fileName;
            jpegList = Directory.GetFiles(fileDirectory, "*.jpg");
            if (takeNewPicture == true)
            {
                Array.Resize(ref jpegList, jpegList.Length + 1);
                jpegList[jpegList.Length - 1] = fileName;
                takeNewPicture = false;
                filePathCount = jpegList.Length;
            }
            jpegOnlyCount = jpegList.Length;
            if(jpegOnlyCount!=0)
            {
                for (filePathCount = 0; jpegList[filePathCount] != fileName; filePathCount++)
                {

                }
            }
            if (jpegOnlyCount == 0)
            {
                rightButton.Hide();
                leftButton.Hide();
                pictureBox.Image = null;
            }
            else
            {
                rightButton.Show();
                leftButton.Show();
            }
        }
        private void mnuOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileOpen = new OpenFileDialog();
            
            
            fileOpen.Title = "Select file to open:";
            fileOpen.Filter ="(*.JPG)|*.JPG;|All files (*.*)|*.*";
            

            if (fileOpen.ShowDialog() == DialogResult.OK)
            {
                updateDirectory(fileOpen.FileName);
                if (jpegList.Length != 0)
                {
                    
                    pictureBox.Image = Image.FromFile(jpegList[filePathCount]);
                }
                else
                {
                    pictureBox.Image = Image.FromFile(jpegList[0]);
                }
            }
            pictureBox.SizeMode = PictureBoxSizeMode.StretchImage;
        }

        private void commandRecTimer_Tick(object sender, EventArgs e)
        {

        }

        private void leftButton_Click(object sender, EventArgs e)
        {
            if (filePathCount == 0)
            {
                leftButton.Hide();
                try
                {
                    pictureBox.Image = Image.FromFile(jpegList[filePathCount]);
                }
                catch
                {
                    leftButton.Hide();
                }
            }
            else
            {
                rightButton.Show();

                pictureBox.Image = Image.FromFile(jpegList[--filePathCount]);
            }
        }

        private void rightButton_Click(object sender, EventArgs e)
        {
            if (filePathCount < jpegOnlyCount - 2)
            {
                leftButton.Show();
                try
                {
                    pictureBox.Image = Image.FromFile(jpegList[++filePathCount]);
                }
                catch//(OutOfMemoryException ex)
                { 
                    
                }
            }
            else rightButton.Hide();
        }
            
        

        private void filePathButton_Click(object sender, EventArgs e)
        {
            DialogResult result = folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
               // filePathTextBox.Text = folderBrowserDialog1.SelectedPath;
                updateDirectory(folderBrowserDialog1.SelectedPath);
            }

        }

        private void saveMnu_Click(object sender, EventArgs e)
        {
            if (filePathTextBox.Text == null)
            {
                saveFileDialog1.InitialDirectory = Convert.ToString(Environment.SpecialFolder.MyDocuments);
                
            }
            else
            {
                saveFileDialog1.InitialDirectory = filePathTextBox.Text;
            }
            saveFileDialog1.Filter = "(*.jpg)|*.jpg|(*.bmp)|*.bmp|All Files (*.*)|*.*";
            saveFileDialog1.FilterIndex = 1;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    updateDirectory(saveFileDialog1.FileName);
                    pictureBox.Image.Save(saveFileDialog1.FileName);
                    Array.Resize(ref jpegList, jpegList.Length + 1);
                    jpegList[jpegList.Length-1] = saveFileDialog1.FileName;
                    jpegOnlyCount++;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result;
            result = MessageBox.Show("Are you sure you want to exit?", "Exit", MessageBoxButtons.YesNo);
            if (result == DialogResult.No)
            {

            }
            if (result == DialogResult.Yes)
            {
                Application.Exit();
            }
        }

        private void deleteButton_Click(object sender, EventArgs e)
        {
            DialogResult result=new DialogResult();

            if (pictureBox.Image != null)
            {
                result = MessageBox.Show("Are you sure you want to delete this file?", "Delete photo", MessageBoxButtons.YesNo);
            }
            if (result == DialogResult.No)
            {

            }
            if (result == DialogResult.Yes)
            {
                pictureBox.Image.Dispose();
                if (filePathCount < jpegOnlyCount-1)
                {
                    pictureBox.Image = Image.FromFile(jpegList[filePathCount + 1]);
                }
                else if (jpegOnlyCount == 1)
                {
                    pictureBox.Image = Image.FromFile(jpegList[0]);
                }
                else
                {
                    pictureBox.Image = Image.FromFile(jpegList[filePathCount - 1]);
                }

                try
                {

                    File.Delete(jpegList[filePathCount]);
                    if (jpegList.Length != 0)
                    {
                        pictureBox.Image = Image.FromFile(jpegList[jpegOnlyCount - 1]);
                    }
                    else
                    {
                        pictureBox.Image = null;
                    }

                    updateDirectory(fileDirectory);

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

            /*
            pictureBox.Image= null;
            string f = jpegList[filePathCount];
            Array.Clear(jpegList, filePathCount, 1);
            File.Delete(f);
            pictureBox.Image=Image.FromFile(jpegList[++filePathCount]);
            */
            /*
            pictureBox.Image = null;
            File.Delete(jpegList[filePathCount]);
            if (filePathCount != 0)
                pictureBox.Image = Image.FromFile(jpegList[--filePathCount]);
            else if (filePathCount == 0)
                pictureBox.Image = Image.FromFile(jpegList[--filePathCount]);
            else if (jpegOnlyCount == 1)
                pictureBox.Image = null;
            jpegOnlyCount--;
             * */
        }

        private void pictureBox_Click(object sender, EventArgs e)
        {
            if (filePathCount < jpegOnlyCount - 2)
            {
                leftButton.Show();
                try
                {
                    pictureBox.Image = Image.FromFile(jpegList[++filePathCount]);
                }
                catch //(OutOfMemoryException ex)
                { 
                
                }
            }
            else rightButton.Hide();
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            stopCommand = true;
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            uavConn.Connect(0);

            uavConn.SendTextToUAV("da 20 payload[0].mem_bytes[0]");
            uavConn.SendTextToUAV("payload.broadcast_bytes 255 0");
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            uavConn.Close();
        }

        private void ResolutionComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (ResolutionComboBox.SelectedIndex)
            {
                case 0:
                    uavConn.SendTextToUAV("payload[0].send_bytes 5 0x07 0x07 0x01");
                    break;
                case 1:
                    uavConn.SendTextToUAV("payload[0].send_bytes 5 0x07 0x07 0x03");
                    break;
                case 2:
                    uavConn.SendTextToUAV("payload[0].send_bytes 5 0x07 0x07 0x05");
                    break;
                case 3:
                    uavConn.SendTextToUAV("payload[0].send_bytes 5 0x07 0x07 0x07");
                    break;
                default:
                    break;
            }
        }





















    }
}
