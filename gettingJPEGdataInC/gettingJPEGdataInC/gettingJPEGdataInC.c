/*
 * gettingJPEGdataInC.c
 *
 * Created: 08/11/2011 20:45:26
 *  Author: ps26g08
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#define  SOI  0xFFD8                 //Start Of Image    Payload: none
#define  SOF0  0xFFC0              //Start Of Frame (Baseline JPEGFileReader)    Payload:variable size
#define  SOF2  0xFFC2              //Start Of Frame (Progressive JPEGFileReader)    Payload:variable size
#define  DHT  0xFFC4               //Define Huffman Table(s)    Payload:variable size
#define  DQT  0xFFDB               //Define Quantization Table(s)    Payload:variable size
#define  DRI  0xFFDD               //Define Restart Interval    Payload:2bytes
#define  SOS  0xFFDA               //Start Of Scan    Payload:variable size
#define  RSTn0  0xFFD0             //Restart    Payload:none
#define  RSTn1  0xFFD1             //Restart    Payload:none
#define  RSTn2  0xFFD2             //Restart    Payload:none
#define  RSTn3  0xFFD3             //Restart    Payload:none
#define  RSTn4  0xFFD4             //Restart    Payload:none
#define  RSTn5  0xFFD5             //Restart    Payload:none
#define  RSTn6  0xFFD6             //Restart    Payload:none
#define  RSTn7  0xFFD7             //Restart    Payload:none
#define  APP0  0xFFE0              //Application-specific    Payload:variable size
#define  APP1  0xFFE1
#define  APP2  0xFFE2
#define  APP3  0xFFE3
#define  APP4  0xFFE4
#define  APP5  0xFFE5
#define  APP6  0xFFE6
#define  APP7  0xFFE7
#define  APP8  0xFFE8
#define  APP9  0xFFE9
#define  APP10  0xFFEA
#define  APP11 0xFFEB
#define  APP12  0xFFEC
#define  APP13  0xFFED
#define  APP14  0xFFEE
#define  APP15  0xFFEF                 
#define  COM  0xFFFE               //Comment    Payload:variable size
#define  EOI  0xFFD9   
	int max_bytes_from_cam=512;
	int bytes_at_once_from_uav=8;
	static uint8_t dimension_of_dqt =8;
	uint8_t all_dqt_table[5][16][16];
	                    // Variable initialization
            uint16_t sof0Length;         // APP0length of sof0 data segment
            uint8_t sof0precision;          // data precision (in bits/sample)
            uint16_t height;             // image height
            uint16_t width;              // image width
            uint8_t numberOfComponents; // describes image colour type. Usually:
            // 1 = Grey scaled
            // 3 = color YcbCr or YIQ
            // 4 = color CMYK
            int chroheight; // height of chrominance pixel
            int chrowidth;  // width of chrominance pixel



            uint16_t QTlength = 0x00;
            uint8_t QTnumber = 0x00;
            uint8_t QTInformation = 0;
            uint8_t QTprecision = 0;
            uint8_t QTcount = 0;
			 
            int SOSlength;
            int SOSNumberOfComponentScan;
            int IgnorableBytes = 3;
            int EOIcheck = 0;
			
			bool end_of_image=false;

			int fileIdentifierMark=0;

            int APP0length;
            int majorRevisionNumber;
            int minorRevisionNumber;
            int unitForXYDensities;
            int xDensity;
            int yDensity;
            int thumbnailWidth;
            int thumbnailHeight;
			
			int countCheckForCommand=0;
			
			int bytesToBeRead[100];
			int sampfactor[512][3];


			

		void JPEGMethod( uint16_t JPEGCommand, uint8_t dataOfJPEG[max_bytes_from_cam] ) 
		{
			//haven't done
			switch(JPEGCommand)
			{
                case SOI:
                    // Start Of Image    Payload: none
                    countCheckForCommand = countCheckForCommand + 2;
                    break;

                case SOF0:
                    //Start Of Frame (Baseline JPEGFileReader)    Payload:variable size


                    // Data segment reading
                    countCheckForCommand = countCheckForCommand + 2; // Now reading: Length data = 8 + 3 * component values
                    sof0Length = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];

                    countCheckForCommand = countCheckForCommand + 2; // Now reading: Data precision
                    sof0precision = dataOfJPEG[countCheckForCommand];

                    countCheckForCommand = countCheckForCommand + 1; // Now reading: Image height
                    height =( dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];

                    countCheckForCommand = countCheckForCommand + 2; // Now reading: Image width
                    width = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];

                    countCheckForCommand = countCheckForCommand + 2; // Now reading: Number of components
                    numberOfComponents = dataOfJPEG[countCheckForCommand];
					
                    //sampfactor = new uint8_t[numberOfComponents, 3]; // Initialization of component data storage array
                    for (int i = 0; i < numberOfComponents; i++)
                    {

                        countCheckForCommand = countCheckForCommand + 1;  // Now reading: component ID:
                                                                          // 1 = Y
                                                                          // 2 = Cb
                                                                          // 3 = Cr
                                                                          // 4 = I
                                                                          // 5 = Q
                        sampfactor[i][0] = dataOfJPEG[countCheckForCommand];
                        

                        countCheckForCommand = countCheckForCommand + 1;  // Now reading: sampling factors :
                                                                          // bit 0-3: vertical
                                                                          // bit 4-7: horizontal
                        sampfactor[i][1] = dataOfJPEG[countCheckForCommand];

                        countCheckForCommand = countCheckForCommand + 1;  // Now reading: Quantization table number
                        sampfactor[i][2] = dataOfJPEG[countCheckForCommand];
                    }

                    //if (numberOfComponents == 1)
                    //{
                    //    //Monochrome case: should not have to do anything
                    //}
                    //else
                    if (numberOfComponents == 3) //Colour case
                    {
                        // YCbCr case
                        if ((sampfactor[0][0] == 1) && (sampfactor[1][0] == 2) && (sampfactor[2][0] == 3))
                        {
							//error:sampfactor need to be in to do this
                            //chroheight = sampfactor[1, 1] % 16; // Obtain first 4 bits of the byte as int
                            //error:sampfactor need to be in to do this
							//chrowidth = sampfactor[1, 1] / 16;  // Obtain last 4 bits of the byte as int
                            //chrosize = chrowidth * chroheight;
                        }
                        // YIQ case
                        else if ((sampfactor[0][0] == 1) && (sampfactor[1][0] == 4) && (sampfactor[2][0] == 5))
                        {

                        }
                    }

                    countCheckForCommand++;
                    break;

                case SOF2:
                    //Start Of Frame (Progressive JPEGFileReader)    Payload:variable size

                    break;

                case DHT:


                    break;

                case DQT:

                    //Marker Identifier 2 bytes 0xff, 0xdb identifies DQT
                    countCheckForCommand = countCheckForCommand + 2;
                    //Length 2 bytes This gives the length of QT.
                    QTlength = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];
                    countCheckForCommand = countCheckForCommand + 2;

                    //QT information 1 byte bit 0..3: number of QT (0..3, otherwise error)
                    //bit 4..7: precision of QT, 0 = 8 bit, otherwise 16 bit
                    QTInformation = dataOfJPEG[countCheckForCommand]%8;
                    if (dataOfJPEG[countCheckForCommand] / 8 == 0)
                    {
                        QTnumber = dataOfJPEG[countCheckForCommand] / 8;
                        QTprecision = 8;
                    }
                    else
                    {
                        QTprecision = 16;
                    }
                    countCheckForCommand++;

                    int count_y=0,count_x=0;
                    	while (count_y<QTprecision)
						{
							count_x=0;
							while (count_x<QTprecision)
							{
								all_dqt_table[QTcount][count_y][count_x]=dataOfJPEG[countCheckForCommand];
								count_x++;
							}
							count_y++;
						}
                    QTcount++;
                    //Bytes n bytes This gives QT values, n = 64*(precision+1)
                    countCheckForCommand = countCheckForCommand + (QTprecision*QTprecision);
                    break;

                case DRI:
                    //Define Restart Interval    Payload:2bytes

                    break;

                case SOS:
                    
                    break;
/*
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
 * */
                case APP0:

                    //Application-specific    Payload:variable size

                    //Marker Identifier 2 bytes 0xff, 0xe0 to identify APP0 marker
                    countCheckForCommand = countCheckForCommand + 2;
                    
                    //Length 2 bytes It must be >= 16
                    APP0length = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];
                    countCheckForCommand = countCheckForCommand + 2;

                    //File Identifier Mark 5 bytes This identifies JFIF.
                    //'JFIF'#0 (0x4a, 0x46, 0x49, 0x46, 0x00)
                    fileIdentifierMark = (dataOfJPEG[countCheckForCommand] == 0x4A &&
                                          dataOfJPEG[countCheckForCommand + 1] == 0x46 &&
                                          dataOfJPEG[countCheckForCommand + 2] == 0x49 &&
                                          dataOfJPEG[countCheckForCommand + 3] == 0x46 &&
                                          dataOfJPEG[countCheckForCommand + 4] == 0x00);
                    countCheckForCommand = countCheckForCommand + 5;
                    //Major revision number 1 byte Should be 1, otherwise error
                    majorRevisionNumber = dataOfJPEG[countCheckForCommand];
                    countCheckForCommand++;

                    //Minor revision number 1 byte Should be 0..2, otherwise try to decode anyway
                    minorRevisionNumber = dataOfJPEG[countCheckForCommand];
                    countCheckForCommand++;

                    //Units for x/y densities 1 byte 0 = no units, x/y-density specify the aspect ratio instead
                    //1 = x/y-density are dots/inch
                    //2 = x/y-density are dots/cm
                    unitForXYDensities = dataOfJPEG[countCheckForCommand];
                    countCheckForCommand++;

                    //X-density 2 bytes It should be <> 0
                    xDensity = (dataOfJPEG[countCheckForCommand]<<8) +dataOfJPEG[countCheckForCommand + 1];
                    countCheckForCommand = countCheckForCommand + 2;

                    //Y-density 2 bytes It should be <> 0
                    yDensity =(dataOfJPEG[countCheckForCommand]<<8) +dataOfJPEG[countCheckForCommand + 1];
                    countCheckForCommand = countCheckForCommand + 2;

                    //Thumbnail width 1 byte -------
                    thumbnailWidth = dataOfJPEG[countCheckForCommand];
                    countCheckForCommand++;

                    //Thumbnail height 1 byte -------
                    thumbnailHeight = dataOfJPEG[countCheckForCommand];
                    countCheckForCommand++;


                    //Bytes to be read n bytes For thumbnail (RGB 24 bit), n = width*height*3 bytes should be read imm
                    //followed by thumbnail height
					//error: no new

                    for (int count = 0; dataOfJPEG[countCheckForCommand]!=0xFF; count++)
                    {
                        bytesToBeRead[count] = dataOfJPEG[countCheckForCommand];
                        countCheckForCommand++;
                    }
                    /*
                     Remarks:
                        If there's no 'JFIF'#0, or the length is < 16, then it is probably not a JFIF segment and should be ignor
                        Normally units=0, x-dens=1, y-dens=1, meaning that the aspect ratio is 1:1 (evenly scaled).
                        JFIF files including thumbnails are very rare, the thumbnail can usually be ignored. If there's no th
                        then width=0 and height=0.If the length doesn't match the thumbnail size, a warning may be prin
                        continue decoding.
                     */


                     
                    break;
                    /*
                case APP1:
                case APP2:
                case APP3:
                case APP4:
                case APP5:
                case APP6:
                case APP7:
                case APP8:
                case APP9:
                case APP10:
                case APP11:
                case APP12:
                case APP13:
                case APP14:
                case APP15:
                    countCheckForCommand++;
                    break;
                     * */
                    /*
                case COM:
                    //Comment    Payload:variable size

                    break;
                     * */
                case EOI:
                    //End Of Image    Payload:none
                    end_of_image = true;
                    break;
                default:
                    countCheckForCommand++;
                    break;

            }
		}


        void commandCheck(uint8_t dataOfJPEG[max_bytes_from_cam])
        {
            uint16_t JPEGCommand = 0x0000; //the data start with 0xFF therefore the double data is not convertible ( the max value of double is 0x7FFF)
            bool next_byte_is_a_command=false;
            do
            {
				//dataOfJPEG= one_chunk_data();
				for(int count_data_in=0;count_data_in<max_bytes_from_cam;count_data_in++)
				{
					if ((dataOfJPEG[count_data_in] == 0xff &&count_data_in<max_bytes_from_cam-1)
																||next_byte_is_a_command==true)
					{
						if(next_byte_is_a_command==false)
						JPEGCommand=(0xFF<<8)+dataOfJPEG[count_data_in+1];
						else if (next_byte_is_a_command==true) //if 0xFF is the last byte
						{
							JPEGCommand=(0xFF<<8)+dataOfJPEG[count_data_in]; 
						}
						
						JPEGMethod(JPEGCommand, dataOfJPEG);
						next_byte_is_a_command=false;
					}
					else if(dataOfJPEG[count_data_in] == 0xff &&count_data_in>=max_bytes_from_cam-1)
					{
						next_byte_is_a_command=true;
					}					
				}
								
            }while (!end_of_image);
        }
		uint8_t one_chunk_data(uint8_t data_in[max_bytes_from_cam]) 
		{
			uint8_t* dataOfJPEG=data_in;
			return dataOfJPEG;
		}
		int function(void)
			{
				uint8_t data_in[10000];
				commandCheck(one_chunk_data(data_in));
			
			
			}
