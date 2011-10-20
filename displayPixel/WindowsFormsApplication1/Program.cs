
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
// csc Altamira.cs

using System;
using System.Drawing;
using System.Windows.Forms;

namespace Altamira
{
    public class Altamira1 : Form
    {
        Pen p;
        SolidBrush b, bT = new SolidBrush(Color.Black);
        string path = "C:\\Users\\peak\\Desktop\\bob.jpg";  // change the path if needed
        Image im;
        Font f;

        public Altamira1()
        {
            InitializeComponent();
            MyIni();
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();

            this.ClientSize = new System.Drawing.Size(290, 260);
            this.Text = "Altamira";

            this.ResumeLayout(false);
        }
        private void MyIni()
        {
            Color cP = Color.Gray;
            Color cB = Color.LightGray;

            p = new Pen(cP, 6);
            b = new SolidBrush(cB);
            im = Image.FromFile(path);
            f = new Font(new FontFamily("Times New Roman"), 10);
        }
        static void Main()
        {
            Application.Run(new Altamira1());
        }
        protected override void OnPaint(PaintEventArgs pea)
        {
            Sketch();
            //SketchDBuf();
        }
        private void Sketch()
        {
            Graphics g = Graphics.FromHwnd(this.Handle);  // <=> g = CreateGraphics();

            g.FillRectangle(b, 4, 4, 260, 220);  // passe-partout
            g.DrawRectangle(p, 4, 4, 260, 220);  // frame
            g.DrawImage(im, 33, 35, 200, 145);  // image
            g.DrawString("ALTAMIRA", f, bT, 180, 190);  // text

            g.Dispose();
        }
        private void SketchDBuf()
        {
            int hh = 3, w = 260, h = 220;

            Graphics g;
            Bitmap bm = new Bitmap(w + 2 * hh, h + 2 * hh);
            g = Graphics.FromImage(bm);  // buffer graphics

            g.FillRectangle(b, hh, hh, w, h);  // passe-partout
            g.DrawRectangle(new Pen(Color.Gray, 2 * hh), hh, hh, w, h);  // frame
            g.DrawImage(im, hh + 30, hh + 32, 200, 145);  // image
            g.DrawString("ALTAMIRA", f, bT, 180, 190);  // text

            g = Graphics.FromHwnd(this.Handle);  // real graphics
            g.DrawImage(bm, 1, 1);

            g.Dispose();
        }
    }
}
