using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;
using System.Windows.Media;


namespace ImageDisplay
{    
    class ConvertByteToImage
    {
        MemoryStream Ms1;
        public byte[,] byteToImage(byte[] data)
        {
            byte[] inClassData = data;
            Ms1 = new MemoryStream(data);
            Image imageFromByte = Image.FromStream(Ms1);
            byte thumbnailWidth = inClassData[20];
            byte thumbnailHeight = inClassData[21];
            byte[] rawPixelData = new byte[data.Length];
            byte[,] TwoD_imagearray=new byte[thumbnailWidth,thumbnailHeight];//data[16] is tw(thumbnail width), data[17] is thumbnail height[]
            for (int byteCount = 0; byteCount < (data.Length-23); byteCount++)
            {
                rawPixelData[byteCount] = data[byteCount +23];//first 3 byte is the byte that is using for jpeg header
                TwoD_imagearray[byteCount / thumbnailWidth, byteCount % thumbnailHeight] = rawPixelData[byteCount];
            }

            return TwoD_imagearray;
        }
        public MemoryStream byteToImageStream()
        {
            return Ms1;
        }
        public void byteToPixel()
        {
            PixelFormat myImageSetting = new PixelFormat();
            myImageSetting = PixelFormats.Rgb24;

        
        }
    }
}
