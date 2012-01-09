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
        
        private static int uavDataLength = 512;

        //Connector streamPort = new Connector();


        UAVConnector uavConn = new UAVConnector();

        Random random = new Random();

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
            uavConn.SendCommand(zeroToken,false);

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
                        byte[] picture_taken_ack = { 8, 1 }; // ack the picture_taken command
                        uavConn.SendCommand(picture_taken_ack,true);

                        statusLabel.Text = "Found PICTURE_TAKEN";
                        Console.WriteLine("Found PICTURE_TAKEN");
                        imageID = (uint)packet[1] + (uint)(packet[2] << 8);
                        break;
                    }
                }
            }

            byte[] imageDownloadRequest = { 2, (byte)imageID, (byte)(imageID >> 8) };
            uavConn.SendCommand(imageDownloadRequest,false);

            
           imageListen(fileName, fileStream, opFile);

        }

        private void imageListen(string fileName, FileStream fileStream, BinaryWriter opFile)
        {
            int num_to_ack_check = 50;
            bool info_ack_flag = false;

            bool[] received_check = new bool[1];
            
            byte[][] image = new byte[1][];

            uint lastPacketNum = 500;

            uint totalPackets = 500;

            bool startImage = false;
            long numBytes = 0;
            while (stopCommand == false)
            {
                Application.DoEvents();
                // add cancel code check

                Console.Write(".");
                byte[] packet = uavConn.GetDataBytes();
                int packetSize = packet.Length;
                //progressbar maximum

                if (packet[0] == 3) // is a IMAGE_DOWNLOAD_INFO packet 
                {
#if SIMULATE
                    if (random.Next(20) != 1) 
                    {
#endif

                    if (!info_ack_flag)
                    {
                        byte[] image_download_info_ack = { 8, 3 }; // ack the image_download_info command
                        uavConn.SendCommand(image_download_info_ack,true);
                        info_ack_flag = true;
                    }
                    statusLabel.Text = "Found IMAGE_DOWNLOAD_INFO";
                    Console.WriteLine("Found IMAGE_DOWNLOAD_INFO");
                    totalPackets = (uint)packet[1] + (uint)(packet[2] << 8);
                    progressBar.Maximum = (int)totalPackets;
                    received_check = new bool[progressBar.Maximum];
                    for (int rc = 0; rc < received_check.Length; rc++)
			        {
                        received_check[rc] = false;
			        }
                    image = new byte[progressBar.Maximum][];
#if SIMULATE
                    }
#endif
                }
                else if (packet[0] == 4) // is a IMAGE_DATA packet
                {
#if SIMULATE
                    if (random.Next(100) != 1)
                    {
#endif
                    statusLabel.Text = "Found IMAGE_DATA";
                    Console.WriteLine("Found IMAGE_DATA");
                    uint packetNum = (uint)packet[1] + (uint)(packet[2] << 8);


                    received_check[packetNum] = true;

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
                        continue;
                    }


                    if ((((packetNum + 1) % num_to_ack_check) == 0) || packetNum == (totalPackets - 1))
                    {
                        int check_num = num_to_ack_check;
                        if (received_check.Length < num_to_ack_check)
                            check_num = received_check.Length;

                        bool isOK = true;
                        for (int pCheck = (int)packetNum; pCheck > (packetNum - check_num); pCheck--)
                        {
                            if (received_check[pCheck] == false)
                            {
                                isOK = false;
                            }
                        }

                        if (isOK)
                        {
                            byte[] image_data_ack = { 8, 4 }; // ack the image data command
                            uavConn.SendCommand(image_data_ack, true);
                            Console.WriteLine("Sending ACK");
                        }
                        else
                        {
                            byte[] image_data_nak = { 9, 4 }; // nak the image data command
                            uavConn.SendCommand(image_data_nak, true);
                            lastPacketNum = packetNum;
                            continue;
                        }
                    }

                    statusLabel.Text = "Writing";
                    Console.WriteLine("Writing.");

                    image[packetNum] = new byte[packetSize - 3];
                    for (int i = 3; i < packetSize; i++)
                    {
                        //opFile.Write(packet[i]);
                        image[packetNum][i-3] = packet[i];
                        numBytes++;
                    }
                    
                    progressBar.PerformStep();
                    if (packetNum == (totalPackets - 1))
                        break;
                    lastPacketNum = packetNum;
#if SIMULATE
                }
#endif 
                }
            }
            if (stopCommand == false)
            {
                for (int i = 0; i < image.Length; i++)
                {
                    for (int j = 0; j < image[i].Length; j++)
                    {
                        opFile.Write(image[i][j]);
                    }
                }
                fileStream.Close();
                opFile.Close();
                progressBar.Value = 1;

                statusLabel.Text = "Done!";
                Console.WriteLine("Done!");





                updateInitialDirectory(fileName);

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

        private void updateInitialDirectory(string fileName)
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
        private void updateDirectory(string fileName)
        {
            bool fileNameIsDirectory = false;
            if (fileName.Substring(fileName.Length - 4, 4).Contains("."))
            {
                fileDirectory = fileName.Substring(0, fileName.LastIndexOf("\\"));
                fileNameIsDirectory = false;

            }
            else
            {
                fileDirectory = fileName;
                fileNameIsDirectory = true;
            }
            filePathTextBox.Text = fileDirectory;
            try
            {
                jpegList = Directory.GetFiles(fileDirectory, "*.jpg");
            }
            catch
            {
            }
            jpegOnlyCount = jpegList.Length;
            if (jpegOnlyCount != 0)
            {
                if (fileNameIsDirectory == false &&( fileName.Substring(fileName.Length - 4, 4).Contains(".jpg") || fileName.Substring(fileName.Length - 4, 4).Contains(".JPG")))
                {
                    try
                    {
                        for (filePathCount = 0; jpegList[filePathCount] != fileName; filePathCount++)
                        {

                        }
                    }
                    catch 
                    {
                        filePathCount--;
                    }
                }
                else
                {
                    filePathCount = 0;
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
                try
                {
                    pictureBox.Image = Image.FromFile(jpegList[filePathCount]);

                    pictureBox.SizeMode = PictureBoxSizeMode.StretchImage;
                }
                catch
                { 
                //do something
                }
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
            fileOpen.Dispose();
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
                try
                {
                    pictureBox.Image = Image.FromFile(jpegList[--filePathCount]);
                }
                catch (OutOfMemoryException)
                {
                    DialogResult result1 = MessageBox.Show("Incomplete JpegFile founded. Do you want to delete it?"
                                                , "Incomplete JpegFile founded!", MessageBoxButtons.YesNo);
                    if (result1 == DialogResult.Yes)
                    {
                        File.Delete(jpegList[filePathCount]);
                        updateDirectory(jpegList[filePathCount]);
                        pictureBox.Image = Image.FromFile(jpegList[filePathCount--]);
                    }
                    
                }
            }
        }

        private void rightButton_Click(object sender, EventArgs e)
        {
            if (filePathCount < jpegOnlyCount - 1)
            {
                leftButton.Show();
                try
                {
                    pictureBox.Image = Image.FromFile(jpegList[++filePathCount]);
                }
                catch(OutOfMemoryException)
                {
                    DialogResult result1 = MessageBox.Show("Incomplete JpegFile founded. Do you want to delete it?"
                                                                ,"Incomplete JpegFile founded!", MessageBoxButtons.YesNo);
                    if (result1 == DialogResult.Yes)
                    {
                        File.Delete(jpegList[filePathCount]);
                        updateDirectory(jpegList[filePathCount]);
                        pictureBox.Image = Image.FromFile(jpegList[filePathCount--]);
                    }
                    
  
  
                }
            }
            else rightButton.Hide();
        }
            
        

        private void filePathButton_Click(object sender, EventArgs e)
        {
            
            DialogResult result = folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
                filePathTextBox.Text = folderBrowserDialog1.SelectedPath;
                updateDirectory(folderBrowserDialog1.SelectedPath);
            }
           folderBrowserDialog1.Dispose();
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
                    //updateInitialDirectory(saveFileDialog1.FileName);
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
            DialogResult result = new DialogResult();

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
                if (filePathCount < jpegOnlyCount - 1)
                {
                    pictureBox.Image = Image.FromFile(jpegList[filePathCount + 1]);
                }
                else if (jpegOnlyCount == 1)
                {
                    pictureBox.Image = Image.FromFile(jpegList[0]);
                    //add default image and We are done!!!
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
        }


        private void pictureBox_Click(object sender, EventArgs e)
        {
            if (filePathCount < jpegOnlyCount - 1)
            {
                leftButton.Show();
                try
                {
                    pictureBox.Image = Image.FromFile(jpegList[++filePathCount]);
                }
                catch (OutOfMemoryException)
                {
                    DialogResult result1 = MessageBox.Show("Incomplete JpegFile founded. Do you want to delete it?"
                                                , "Incomplete JpegFile founded!", MessageBoxButtons.YesNo);
                    if (result1 == DialogResult.Yes)
                    {
                        pictureBox.Dispose();
                        File.Delete(jpegList[filePathCount]);
                        updateDirectory(jpegList[filePathCount]);
                        pictureBox.Image = Image.FromFile(jpegList[filePathCount--]);
                    }
                    
                }
            }
            else rightButton.Hide();
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            stopCommand = true;
            uavConn.SendTextToUAV("payload[0].send_bytes 7");
            if(filePathCount!=0)
            updateDirectory(jpegList[filePathCount]);
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
