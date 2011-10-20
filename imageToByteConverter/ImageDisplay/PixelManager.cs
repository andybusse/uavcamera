using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace ImageDisplay
{
    class PixelManager
    {
       
        

        public void doPixelManger(byte[] pixel1D,UInt16 width,UInt16 height)
        {
            this.getPixel2D(pixel1D,width,height);

        }


        private byte[,] getPixel2D(byte[] pixel1D, UInt16 width, UInt16 height)
        {
            byte[,] pixel2D = new byte[width, height];


            for (int heightCount = 0; heightCount < height; heightCount++)
            {

                for (int widthCount = 0; widthCount < width; widthCount++)
                {

                    pixel2D[widthCount, heightCount] = pixel1D[widthCount + heightCount];//**when connect to real UAV this will change to stream **
                
                }
            
            }

            return pixel2D;
        }
        private byte[] pixelSquareMethod(byte[,] pixel2D,int PixelWidth,int PixelHeight)
        {
           // Stack stackPixel = new Stack();
            int pixelCount = 0;
            byte[] finalPixelArrange = new byte[PixelWidth* PixelHeight];
            int orderMax=1;

            finalPixelArrange[0] = pixel2D[PixelWidth, PixelHeight];
            //stackPixel.Push(finalPixelArrange[0]);

            while (pixelCount < PixelHeight * PixelWidth)
            {

                for (int count = 0, order = 0; order < orderMax;orderMax++,order++ )//order is implement here because the number of step=(3^order)
                {
                    finalPixelArrange[pixelCount * 3 + count+order] = pixel2D[PixelWidth, PixelHeight];//Wrong Should not be here
                    count++;
                    finalPixelArrange[pixelCount * 3 + count+order] = pixel2D[PixelWidth / 2, PixelHeight];
                    count++;
                    finalPixelArrange[pixelCount * 3 + count+order] = pixel2D[PixelWidth, PixelHeight / 2];
                    count++;
                    finalPixelArrange[pixelCount * 3 + count+order] = pixel2D[PixelWidth / 2, PixelHeight / 2];
                    count++;
                }
                PixelHeight = PixelHeight / 2;
                PixelWidth = PixelWidth / 2;
                pixelCount++;
            }

            return finalPixelArrange;
        }
        private void displayPixelOnScreen(byte[] squareMethodByte)
        { 
        
        
        
        }
    }
}
