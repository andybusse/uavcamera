using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using System.Windows.Forms;

namespace NCamGS
{
    class Program
    {
        static MainForm mainForm = new MainForm();
        static void Main(string[] args)
        {
            mainForm.Show();

            while (true)
            {
                Application.DoEvents();
            }
        }
    }
}
