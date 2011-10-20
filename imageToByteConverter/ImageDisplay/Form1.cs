using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ImageDisplay
{
    public partial class ImageHandler : Form
    {
        const string bobImageDir = "C:\\Users\\peak\\Documents\\Visual Studio 2010\\Projects\\ImageDisplay\\ImageDisplay\\rsc\\bob.jpg";
        Image bobImage = Image.FromFile(bobImageDir);
        ConvertImageToByte photoConverter = new ConvertImageToByte();
        ConvertByteToImage byteConverter = new ConvertByteToImage();


        public ImageHandler()
        {
            
            InitializeComponent();

            pictureBoxPeak.Image = bobImage;
            


        }

        private void ConvertToByteButton_Click(object sender, EventArgs e)
        {
            byte[] data=new byte[10000];
            
            data=photoConverter.ImageToByte3(bobImage);
            pictureBoxConverted.Image = byteConverter.byteToImage(data);
            //textBoxImageByteConversion.Text = byteConverter.byteToImageStream().ToString();
            //textBoxImageByteConversion.Text = byteConverter.byteToImageStream().ToString();
           
        }





    }
}
