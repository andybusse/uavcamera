using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Net;
using System.Diagnostics;
using System.IO;

namespace ImagingMethod
{
    class DiscreteCosineTransform
    {
        const int   SOI = 0xFFD8,               //Start Of Image    Payload: none
                    SOF0 = 0xFFC0,              //Start Of Frame (Baseline DiscreteCosineTransform)    Payload:variable size
                    SOF2 = 0xFFC2,              //Start Of Frame (Progressive DiscreteCosineTransform)    Payload:variable size
                    DHT = 0xFFC4,               //Define Huffman Table(s)    Payload:variable size
                    DQT = 0xFFDB,               //Define Quantization Table(s)    Payload:variable size
                    DRI = 0xFFDD,               //Define Restart Interval    Payload:2bytes
                    SOS = 0xFFDA,               //Start Of Scan    Payload:variable size
                    RSTn0 = 0xFFD0,             //Restart    Payload:none
                    RSTn1 = 0xFFD1,             //Restart    Payload:none
                    RSTn2 = 0xFFD2,             //Restart    Payload:none
                    RSTn3 = 0xFFD3,             //Restart    Payload:none
                    RSTn4 = 0xFFD4,             //Restart    Payload:none
                    RSTn5 = 0xFFD5,             //Restart    Payload:none
                    RSTn6 = 0xFFD6,             //Restart    Payload:none
                    RSTn7 = 0xFFD7,             //Restart    Payload:none
                    APPn = 0xFFE0,              //Application-specific    Payload:variable size
                    COM = 0xFFFE,               //Comment    Payload:variable size
                    EOI = 0xFFD9;               //End Of Image    Payload:none
        public void ImageToByte(Image img)//This one works well so far
        {

            MemoryStream Ms1 = new MemoryStream();
            byte[] dataOfJPEG = new byte[img.Width * img.Height];
            img.Save(Ms1, System.Drawing.Imaging.ImageFormat.Jpeg);
            dataOfJPEG = Ms1.GetBuffer();
            commandCheck(dataOfJPEG);
            //to check for a command JPEGCommand
        }
        private UInt16 twoBytesToUint16(byte higherByte,byte lowerByte)
        {
            byte[] twoByteArray=new byte[2];
            UInt16 IntInfo = 0xffff;
            twoByteArray[1] = higherByte;
            twoByteArray[0] = lowerByte;
            IntInfo = BitConverter.ToUInt16(twoByteArray, 0);
            return IntInfo;
        }

        private void commandCheck(byte[] dataOfJPEG)
        {

            UInt16 JPEGCommand = 0x0000; //the data start with 0xFF therefore the double data is not convertible ( the max value of doule is 0x7FFF
            for (int countCheckForCommand = 0; countCheckForCommand < dataOfJPEG.Length - 1; countCheckForCommand++)
            {
                if (dataOfJPEG[countCheckForCommand] == 0xff)
                {
                    JPEGCommand=this.twoBytesToUint16(dataOfJPEG[countCheckForCommand], dataOfJPEG[countCheckForCommand + 1]);
                    countCheckForCommand = this.JPEGMethod(JPEGCommand, dataOfJPEG, countCheckForCommand);

                }
            }
        
        }
        public int JPEGMethod(int JPEGCode, byte[] dataOfJPEG, int countCheckforCommand) //JPEG method check for the JPEG command and also return a correct new countYOnScreen
        {


            switch (JPEGCode)
            {
                case SOI:
                    //Start Of Image    Payload: none
                    //Mitch Work

                    Console.WriteLine("start of the image");
                    break;
                case SOF0:
                    //Start Of Frame (Baseline DiscreteCosineTransform)    Payload:variable size
                    //Mitch Work
                    break;
                case SOF2:
                    //Start Of Frame (Progressive DiscreteCosineTransform)    Payload:variable size
                    //Mitch Work
                    break;
                case DHT:
                    //Define Huffman Table(s)    Payload:variable size
                    //Michael Work
                    break;
                case DQT:
                    double[,] g = new double[8, 8];//sub Image array
                    double[,] G = new double[8, 8];//two-dimensional DCT
                    double[,] Q = new double[8, 8];//A typical quantization matrix, as specified in the original JPEG Standard
                    int[,] B = new int[8, 8];//The quantized DCT coefficients are computed with
                    int quantizationLength = 0x00;
                    int numberOfQuantizationTable = 0x00;
                    quantizationLength = getLength(dataOfJPEG, countCheckforCommand);
                    countCheckforCommand = countCheckforCommand + 2;
                    
                    g = subImage(JPEGCode, dataOfJPEG, countCheckforCommand); //define g
                    G = DCT(g); //define G
                    Q = this.quantizationMatrixForStandardJPEG();//define Q
                    B = this.quantizedDCTcoefficient(G, Q);
                    break;
                case DRI:
                    //Define Restart Interval    Payload:2bytes

                    break;
                case SOS:
                    //Start Of Scan    Payload:variable size
                    //Peak Work
                    break;

                case RSTn0:
                case RSTn1:
                case RSTn2:
                case RSTn3:
                case RSTn4:
                case RSTn5:
                case RSTn6:
                case RSTn7:
                    //Restart    Payload:none

                    break;
                case APPn:
                    //Application-specific    Payload:variable size
                    //Mitch Work
                    break;
                case COM:
                    //Comment    Payload:variable size

                    break;
                case EOI:
                    //Mitch Work
                    //End Of Image    Payload:none

                    break;
            }
            return countCheckforCommand;
        }
        private double alpha(double w)
        {
            double alpha;
            if (w == 0)
            {
                alpha = System.Math.Sqrt(0.125);

            }
            else
            {
                alpha = System.Math.Sqrt(0.25);

            }
            return alpha;
        }
        private double[,] g(double[] subImageData)
        {
            int countData = 0;
            double[,] g=new double[8,8];

            for (int countY = 0; countY < 8; countY++)
            {
                for (int countX = 0; countX < 8; countX++,countData++)
                {
                    g[countX, countY] = subImageData[countData];
                }
            
            }

            return g;
        }
        private double[,] DCT(double[,] g)//input g output G
        {
            
        double[,] G=new double[8,8];
        for (int u = 0; u < 8; u++)
        {
            for (int v = 0; v < 8; v++)
            {
                for (int x = 0; x < 8; x++)
                {
                    for (int y = 0; y < 8; y++)
                    {
                        G[u, v] += alpha(u) * alpha(v) * g[x, y] *
                            System.Math.Cos(System.Math.PI / 8 * ((x + 1 / 2) * u)) *
                            System.Math.Cos(System.Math.PI / 8 * ((y + 1 / 2) * v));
                    }
                }
            }
        }

        return G; 
        }
        private double[,] quantizationMatrixForStandardJPEG()
        {
            double[,] Q = new double[8, 8] 
            {
            {16,11,10,16,24,40,51,61},
            {12,12,14,19,26,58,60,55},
            {14,13,16,24,40,57,69,56},
            {14,17,22,29,51,87,80,62},
            {18,22,37,56,68,109,103,77},
            {24,35,55,64,81,104,113,92},
            {49,64,78,87,103,121,120,101},
            {72,92,95,98,112,100,103,99}
            };

            return Q;
        
        }
        private int[,] quantizedDCTcoefficient(double[,] G, double[,] Q)
        {
            int[,] B = new int[8, 8];
            for (int k = 0; k < 8; k++)
            {
                for (int j = 0; j < 8; j++)
                {
                    B[j,k] =(int) System.Math.Round(G[j,k] / Q[j,k]);
                }
            }
            return B;
        }
        private Double[,] InverseDCT(Double[,] input)
        {
            double sqrtOfLength = System.Math.Sqrt(input.Length);


            int N = input.GetLength(0);

            Double[,] coefficientsMatrix = this.quantizationMatrixForStandardJPEG();
            Double[,] output = new Double[N, N];

            for (int x = 0; x <= N - 1; x++)
            {
                for (int y = 0; y <= N - 1; y++)
                {
                    double sum = 0.0;
                    for (int u = 0; u <= N - 1; u++)
                    {
                        for (int v = 0; v <= N - 1; v++)
                        {
                            sum += coefficientsMatrix[u, v] * input[u, v] * System.Math.Cos(((2.0 * x + 1.0) / (2.0 * N)) * u * System.Math.PI) * System.Math.Cos(((2.0 * y + 1.0) / (2.0 * N)) * v * System.Math.PI);
                        }
                    };
                    output[x, y] = System.Math.Round(sum);
                }
            }
            return output;
        }
        private int getLength(byte[] dataOFJPEG, int countCheckforCommand)
        {
            int length = 0;


            return length;
        }
        private double[,] subImage(int JPEGCode, byte[] dataOfJPEG, int countCheckforCommand)
        {
            double[,] quantizedData = new double[8, 8];//8x8 and a bit more 
            
            //Define Quantization Table(s)    Payload:variable size
            //quantizationMatrixForStandardJPEG = DCT.quantizationMatrixForStandardJPEG();
            countCheckforCommand = countCheckforCommand + 2; //in order to get the fist byte of quantization table not the command data
            int countXAxis;
            for (int countYAxis = 0; countYAxis < 8; countYAxis++)
            {
                countXAxis = 0;
                do
                {
                        quantizedData[countYAxis, countXAxis] = dataOfJPEG[countCheckforCommand];
                        countXAxis++;
                    countCheckforCommand++;
                } while (countXAxis < 8);
            }
            
            return quantizedData;
        }
    }
}
