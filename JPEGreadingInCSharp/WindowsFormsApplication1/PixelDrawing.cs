
using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using System.Net;
using System.Diagnostics;


namespace ImagingMethod
{

    public class ImagingMethod : Form
    {
        SolidBrush myBrush, bT = new SolidBrush(Color.Black);
        Image imageFromFile; //image for testing
        byte[] imageByte; //byte of image pure JPEG dataOfJPEG
        DiscreteCosineTransform dct = new DiscreteCosineTransform();

        public ImagingMethod()
        {
            InitializeComponent();
            MyIni();
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();

            this.ClientSize = new System.Drawing.Size(290, 260);

            this.ResumeLayout(false);
        }
        private void MyIni()
        {
            myBrush = new SolidBrush(Color.FromArgb(255, 60, 60));
            imageFromFile = Image.FromFile("D:\\Pics\\Avatars\\Okami.jpg");
            imageByte = new byte[imageFromFile.Width * imageFromFile.Height + 18];
            dct.ImageToByte(imageFromFile);

        }
        static void Main()
        {
            Application.Run(new ImagingMethod());
        }
        protected override void OnPaint(PaintEventArgs pea)
        {
            for (int countYOnScreen = 0; countYOnScreen < imageFromFile.Height + 1; countYOnScreen++)
            {
                for (int countXOnScreen = 0; countXOnScreen < imageFromFile.Width + 1; countXOnScreen++)
                {
                    myBrush.Color = Color.ForestGreen;
                    Sketch(countXOnScreen, countYOnScreen);
                }
            }
        }
        private void Sketch(int xCoordinate, int yCoordinate)
        {
            Graphics g = Graphics.FromHwnd(this.Handle);  // <=> g = CreateGraphics();

            g.FillRectangle(myBrush, xCoordinate, yCoordinate, 1, 1);  // passe-partout
            g.Dispose();

        }
        private Color getcColor(int red, int green, int blue)
        {
            Color color = Color.Empty;
            color = Color.FromArgb(red, green, blue);
            return color;
        }


 


    }
}

/*
Professional Windows GUI Programming Using C#
by Jay Glynn, Csaba Torok, Richard Conway, Wahid Choudhury, 
   Zach Greenvoss, Shripad Kulkarni, Neil Whitlow

Publisher: Peer Information
ISBN: 1861007663
*/
// Change the path(s) if needed. If you have VS.NET write:
// "..\\..\\Altamira5.bmp" or @"..\..\Altamira5.bmp"
// otherwise
// "Altamira5.bmp"
// and compile with:
// csc ImagingMethod.cs


/*
private void SketchDBuf()
{
    int hh = 3, w = 260, h = 220;

    Graphics g;
    Bitmap bm = new Bitmap(w + 2 * hh, h + 2 * hh);
    g = Graphics.FromImage(bm);  // buffer graphics

    g.FillRectangle(myBrush, hh, hh, w, h);  // passe-partout
    g.DrawRectangle(new Pen(Color.Gray, 2 * hh), hh, hh, w, h);  // frame
    g.DrawImage(im, hh + 30, hh + 32, 200, 145);  // image
    g.DrawString("ALTAMIRA", f, bT, 180, 190);  // text

    g = Graphics.FromHwnd(this.Handle);  // real graphics
    g.DrawImage(bm, 1, 1);

    g.Dispose();
}
*/


/*
        private void Sketch()
        {
            Graphics g = Graphics.FromHwnd(this.Handle);  // <=> g = CreateGraphics();

                    g.FillRectangle(myBrush, countXOnScreen, countYOnScreen, 1, 1);  // passe-partout
                    g.DrawRectangle(p, 4, 4, 260, 220);  // frame
                    g.DrawImage(im, 33, 35, 200, 145);  // image
                     g.DrawString("ALTAMIRA", f, bT, 180, 190);  // text
                    g.Dispose();


        }
*/
/*
        private void GetYCbCrFromPixel(YCbCrPixelLayout layout, byte[] currentPreviewBuffer, int xFramePos, int yFramePos, out byte yCoordinate, out int cr, out int cb)
        {
            // Find the bytes corresponding to the pixel location in the frame.
            int yBufferIndex = layout.YOffset + yFramePos * layout.YPitch + xFramePos * layout.YXPitch;
            int crBufferIndex = layout.CrOffset + (yFramePos / 2) * layout.CrPitch + (xFramePos / 2) * layout.CrXPitch;
            int cbBufferIndex = layout.CbOffset + (yFramePos / 2) * layout.CbPitch + (xFramePos / 2) * layout.CbXPitch;

            // The luminance value is always positive.
            yCoordinate = currentPreviewBuffer[yBufferIndex];

            // The preview buffer contains an unsigned value between 255 and 0.
            // The buffer value is cast from a byte to an integer.
             cr = currentPreviewBuffer[crBufferIndex];
            
            // Convert to a signed value between 127 and -128.
            cr -= 128;

            // The preview buffer contains an unsigned value between 255 and 0.
            // The buffer value is cast from a byte to an integer.
            cb = currentPreviewBuffer[cbBufferIndex];

            // Convert to a signed value between 127 and -128.
            cb -= 128;  
            
        
        }
*/