using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;
using System.Windows.Media;
using System.Windows.Media.Imaging;
//using System.Windows.Controls; for using text block


namespace ImageDisplay
{
    class ConvertImageToByte
    {
        public byte[] ImageToByte(Image img)
        {
            ImageConverter converter = new ImageConverter();
            return (byte[])converter.ConvertTo(img, typeof(byte[]));
        }
        public byte[] ImageToByte2(Image img)
        {
            byte[] byteArray = new byte[0];
            using (MemoryStream stream = new MemoryStream())
            {
                img.Save(stream, System.Drawing.Imaging.ImageFormat.Jpeg);
                stream.Close();

                byteArray = stream.ToArray();

            }
            return byteArray;
        }
        public byte[] ImageToByte3 (Image img)//This one works well so far
        {
            MemoryStream Ms1 = new MemoryStream() ;
            byte[] data=new byte[50000];
            img.Save(Ms1, System.Drawing.Imaging.ImageFormat.Jpeg);
            data = Ms1.GetBuffer();
            return data;
        }
        public byte[] ImageToPixel_JFIF(Image img)
        {
            byte[] pixelData = new byte[50000];
            byte[] APP0_marker = new byte[2];//Always equals 0xFFE0
            byte[] Length = new byte[2];//Length of segment excluding APP0 marker
            byte[] Identifier={63,47,32,31,15,0};//Always equals "JFIF" (with zero following) (0x4A46494600)
            byte[] Version = new byte[2];//First byte is major version (currently 0x01), Second byte is minor version (currently 0x02)
            byte[] Density_units = new byte[1];//	Units for pixel2D density fields
/*
    0 - No units, aspect ratio only specified
    1 - Pixels per inch
    2 - Pixels per centimetre
*/
            byte[] X_Density_units = new byte[2];//	Integer horizontal pixel2D density
            byte[] Y_density_units = new byte[2];//	Integer vertical pixel2D density
            byte[] Thumbnail_width = new byte[1];//Horizontal size of embedded JFIF thumbnail in pixels
            byte[] Thumbnail_height = new byte[1];//	Vertical size of embedded JFIF thumbnail in pixels
            byte[] Thumbnail_data = new byte[3 * Thumbnail_height.Length * Thumbnail_width.Length];//Uncompressed 24 bit RGB raster thumbnail



            return pixelData;
        }
        public byte[] ImageToPixel_JFXX(Image img)
        {
            byte[] pixelData = new byte[50000];
            byte[] APP0_marker = new byte[2];//Always equals 0xFFE0
            byte[] Length = new byte[2];//Length of segment excluding APP0 marker
            byte[] Identifier = { 63, 47, 32, 31, 15, 0 };//	Always equals "JFXX" (with zero following) (0x4A46585800)
            byte[] Thumbnail_format = new byte[1];//	Units for pixel2D density fields
            /*
	            Specifies what data format is used for the thumbnail:

                0x10 - JPEG format
                0x11 - 1 byte per pixel2D palettised format
                0x13 - 3 byte per pixel2D RGB format

            */
            byte[] Thumbnail_width = new byte[1];//Horizontal size of embedded JFIF thumbnail in pixels
            byte[] Thumbnail_height = new byte[1];//	Vertical size of embedded JFIF thumbnail in pixels
            byte[] Thumbnail_palette = new byte[768];
            byte[] Thumbnail_data_3_byte_per_pixel = new byte[3 * Thumbnail_height.Length * Thumbnail_width.Length];//Uncompressed 24 bit RGB raster thumbnail 
            byte[] Thumbnail_data_1_byte_per_pixel = new byte[ Thumbnail_height.Length * Thumbnail_width.Length];


            return pixelData;
        }
        /*
        public BitmapSource BitmapToJpegEncoder()
        {
            int width = 128;
            int height = width;
            int stride = width / 8;
            byte[] pixels = new byte[height * stride];
            
            // Define the image palette
            BitmapPalette myPalette = BitmapPalettes.Halftone256;

            // Creates a new empty image with the pre-defined palette
            BitmapSource image = BitmapSource.Create(
                width,
                height,
                96,
                96,
                PixelFormats.Indexed1,
                myPalette,
                pixels,
                stride);

            FileStream stream = new FileStream("new.jpg", FileMode.Create);
            JpegBitmapEncoder encoder = new JpegBitmapEncoder();
            //TextBlock myTextBlock = new TextBlock();
            //myTextBlock.Text = "Codec Author is: " + encoder.CodecInfo.Author.ToString();
            encoder.FlipHorizontal = true;
            encoder.FlipVertical = false;
            encoder.QualityLevel = 30;
            encoder.Rotation = Rotation.Rotate90;
            encoder.Frames.Add(BitmapFrame.Create(image));
            encoder.Save(stream);
            return image;
        }
         * /
        public void decodeJPEG()
        {
            // Open a Stream and decode a JPEG image
            Stream imageStreamSource = new FileStream("tulipfarm.jpg", FileMode.Open, FileAccess.Read, FileShare.Read);
            JpegBitmapDecoder decoder = new JpegBitmapDecoder(imageStreamSource, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default);
            BitmapSource bitmapSource = decoder.Frames[0];
/*
            // Draw the Image
            Image myImage;
            myImage.Source = bitmapSource;
            myImage.Stretch = Stretch.None;
            myImage.Margin = new Thickness(20);

        } * */
        
    }
}
