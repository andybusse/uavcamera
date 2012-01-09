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

/*
 * gettingJPEGdataInC.c
 *
 * Created: 08/11/2011 20:45:26
 *  Author: ps26g08 & ps6g08
 * Last edition date: 15/11/2011
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//----Translate Header titles from hexadecimal----

#define  SOI   0xFFD8               //Start Of Image    Payload: none
#define  SOF0  0xFFC0              //Start Of Frame (Baseline JPEGFileReader)    Payload:variable size
#define  SOF2  0xFFC2              //Start Of Frame (Progressive JPEGFileReader)    Payload:variable size
#define  DHT   0xFFC4               //Define Huffman Table(s)    Payload:variable size
#define  DQT   0xFFDB               //Define Quantization Table(s)    Payload:variable size
#define  DRI   0xFFDD               //Define Restart Interval    Payload:2bytes
#define  SOS   0xFFDA               //Start Of Scan    Payload:variable size
#define  RSTn0 0xFFD0             //Restart    Payload:none
#define  RSTn1 0xFFD1             //Restart    Payload:none
#define  RSTn2 0xFFD2             //Restart    Payload:none
#define  RSTn3 0xFFD3             //Restart    Payload:none
#define  RSTn4 0xFFD4             //Restart    Payload:none
#define  RSTn5 0xFFD5             //Restart    Payload:none
#define  RSTn6 0xFFD6             //Restart    Payload:none
#define  RSTn7 0xFFD7             //Restart    Payload:none
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
#define  APP10 0xFFEA
#define  APP11 0xFFEB
#define  APP12 0xFFEC
#define  APP13 0xFFED
#define  APP14 0xFFEE
#define  APP15 0xFFEF                 
#define  COM   0xFFFE               //Comment    Payload:variable size
#define  EOI   0xFFD9   

//----Initialization----

//---Structures---

	struct huffmanTable // HT structure
	{
		uint8_t DHTnumber;            // DHT segment HT was located in           (4 bits)
		uint8_t HTnumber;             // HT number as indicated by image data    (4 bits)
		uint8_t acdc;                 // AC or DC class of the HT                (1 bit)
		uint8_t codeNumber;           // Number of codes contained within the HT (8 bits)
		uint8_t symbolNumber[16];     // Total number of symbols (classified by code length) of the HT (16 element array of variable sized elements (max size is a byte))
		uint8_t *symbolPtr[16];       // Array of pointers to symbol arrays
		struct huffmanTable *nextPtr; // Pointer to next HT (HT organized in a linked list)
	};
	
	typedef struct huffmanTable HTable;
	typedef HTable *HTablePtr;

//---Prototypes---

	// Convert input data stream to byte array
	void ImageToByte(uint8_t *dataOfJPEG, uint16_t jpglength, char *jpglocation);

	// Determine header command from data segment
	void commandCheck(uint8_t *dataOfJPEG);

	// Store data according to header information
	void JPEGMethod(uint16_t JPEGCommand, uint8_t *dataOfJPEG ) ;

	// Store HT information for a chrominance pixel
	uint8_t ImageDataScan(uint

//---General variables---
	uint16_t max_bytes_from_cam = 512;
	uint8_t bytes_at_once_from_uav = 8;  //unused at the moment
	static uint8_t dimension_of_dqt = 8; //unused at the moment
	uint16_t countCheckForCommand = 0;
	char *jpglocation = "";
		
//---Variables and structures by Header initialized in---

	//--SOF0 Header--
    uint16_t sof0Length;         // length of sof0 data segment
    uint8_t sof0precision;       // data precision (in bits/sample)
    uint16_t height;             // image height
    uint16_t width;              // image width
    uint8_t numberOfComponents;  // describes image colour type. Usually:
								  	// 1 = Grey scaled
									// 3 = color YcbCr or YIQ
									// 4 = color CMYK
    uint8_t chroheight;		     // height of chrominance pixel
    uint8_t chrowidth;			 // width of chrominance pixel
	uint16_t chrosize;   		 // size of chrominance pixel
	uint8_t *sampfactor[3];      // sampling factor information
	
	//--DHT Header--
	
	uint16_t DHTcount = 0;      // Defines number of DHT segments read.
	uint16_t HTcount = 0;       // Defines number of HT created.
	HTablePtr startPtr = NULL; // No tables initially

	//--DQT Header--
    uint16_t QTlength = 0x00;
    uint8_t QTnumber = 0x00;
    uint8_t QTInformation = 0;
    uint8_t QTprecision = 0;
	uint8_t all_dqt_table[5][16][16];
    uint8_t QTcount = 0;
			 
	//--SOS Header--
    uint16_t SOSlength;
    uint8_t SOSNumberOfComponentScan;
	uint8_t *scanComponents[2];
	uint16_t SOScount;
	
	struct SOSHTable {
		uint8_t *acht; // AC Huffman table to use (non zero if it exists)
		uint8_t *dcht; // DC Huffman table to use (non zero if it exists)
		struct SOSHTable *nextSOSPtr;
	};
	
	typedef struct SOSHTable STable;
	typedef STable *STablePtr;
	STablePtr SstartPtr = NULL; // No tables initially
			
	//--EOI Header--
	uint8_t end_of_image = 0;

	//--APP0 Header--
	uint64_t fileIdentifierMark = 0;
    uint16_t APP0length;
    uint8_t majorRevisionNumber;
    uint8_t minorRevisionNumber;
    uint8_t unitForXYDensities;
    uint16_t xDensity;
    uint16_t yDensity;
    uint8_t thumbnailWidth;
    uint8_t thumbnailHeight;
	uint8_t *bytesToBeRead;
	
//----MAIN FUNCTION----

		int main(void)
		{
			return 0;
		}
	
//----SUB-FUNCTIONS----

//---ImageToByte function---

		void ImageToByte (uint8_t *dataOfJPEG, uint16_t jpglength, char *jpglocation)
		{
			// Initialization
			FILE *jpegFile; // input stream

			//Read file as a binary file
			jpegFile = fopen(jpglocation, "rb");
			if (jpegFile == NULL)
			{
				fprintf(stderr, "Unable to open file %s", jpegFile); //Error message: change for AVR?
				return;
			}

			// Get file length
			fseek(jpegFile, 0, SEEK_END);
			jpglength = ftell(jpegFile); // read as binary file: divide by 8 to convert to bytes?
			fseek(jpegFile, 0, SEEK_SET);

			// Allocate memory: saved as array of bytes
			dataOfJPEG = malloc(sizeof(uint8_t)*jpglength);
			if (dataOfJPEG == NULL)
			{
				fprintf(stderr, "Memory error!"); //Error message: change for AVR?
				fclose(jpegFile);
			}

		   // Read data from input stream to byte array
		   fread(dataOfJPEG,sizeof(uint8_t),jpglength,jpegFile);
		   fclose(jpegFile);
		}

//---commandCheck function---

		void commandCheck(uint8_t *dataOfJPEG)
		{
			uint16_t JPEGCommand = 0x0000; //the data start with 0xFF therefore the double data is not convertible ( the max value of double is 0x7FFF)
			uint8_t next_byte_is_a_command = 0; // was a bool
			do
			{
				//dataOfJPEG= one_chunk_data();
				for( int count_data_in = 0; count_data_in < max_bytes_from_cam; count_data_in++ )
				{
					if ((dataOfJPEG[count_data_in] == 0xff && count_data_in<max_bytes_from_cam-1) || next_byte_is_a_command == 1)
					{
						if(next_byte_is_a_command == 0)
						{
							JPEGCommand = (0xFF << 8) + dataOfJPEG[count_data_in+1];
						} // end if
						else if (next_byte_is_a_command == 1) //if 0xFF is the last byte
						{
							JPEGCommand = (0xFF << 8) + dataOfJPEG[count_data_in]; 
						} // end else if
						
						JPEGMethod(JPEGCommand, dataOfJPEG);
						next_byte_is_a_command = 0;
					} // end if
					else if(dataOfJPEG[count_data_in] == 0xff &&count_data_in>=max_bytes_from_cam-1)
					{
						next_byte_is_a_command = 1;
					} // end else if
				} // end for
			} while (end_of_image == 0); // end while
		} // end function

		
//---JPEGMethod function---

		void JPEGMethod(uint16_t JPEGCommand, uint8_t *dataOfJPEG ) 
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
					
					for(uint8_t i = 0; i < 3; i++)
					{
						sampfactor[i] = malloc(sizeof (uint8_t)*numberOfComponents);
						if(sampfactor[i] == NULL)
						{
							// error prevention/notification if there is no space available goes here
							//return_; //(?)
							break;
						} // end if
					} // end for
					
					for (uint8_t i = 0; i < numberOfComponents; i++)
					{
						countCheckForCommand = countCheckForCommand + 1;  // Now reading: component ID:
																				// 1 = Y
																				// 2 = Cb
																				// 3 = Cr
																				// 4 = I
																				// 5 = Q
						sampfactor[0][i] = dataOfJPEG[countCheckForCommand];
                        

						countCheckForCommand = countCheckForCommand + 1;  // Now reading: sampling factors :
																				// bit 0-3: vertical
																				// bit 4-7: horizontal
						sampfactor[1][i] = dataOfJPEG[countCheckForCommand];

						countCheckForCommand = countCheckForCommand + 1;  // Now reading: Quantization table number
						sampfactor[2][i] = dataOfJPEG[countCheckForCommand];
					} // end for

					//if (numberOfComponents == 1)
					//{
					//    //Monochrome case: should not have to do anything, all are  considered Y
					//}
					//else
					if (numberOfComponents == 3) //Colour case
					{
						// YCbCr case
						if ((sampfactor[0][0] == 1) && (sampfactor[0][1] == 2) && (sampfactor[0][2] == 3))
						{
							chroheight = sampfactor[1][1] % 16; // Obtain first 4 bits of the byte as int
							chrowidth = sampfactor[1][1] / 16;  // Obtain last 4 bits of the byte as int
							chrosize = chrowidth * chroheight;
						}
						// YIQ case
						else if ((sampfactor[0][0] == 1) && (sampfactor[0][1] == 4) && (sampfactor[0][2] == 5))
						{
							// currently unhandled
							// return_; ?
							break;
						}
					}
					//else if (numberOfComponents == 4)
					//{
					//		// CMYK functionality not yet handled
					//}

					countCheckForCommand++;
					break;

				case SOF2:
					//Start Of Frame (Progressive JPEGFileReader)    Payload:variable size

					break;

				case DHT:

					// Define Huffman Table(s)    Payload:variable size

					//Marker Identifier 2 bytes 0xff, 0xc4 to identify DHT marker
					countCheckForCommand = countCheckForCommand + 2; // Now reading: Huffman table segment length

					//Length 2 bytes: This specifies the length of the DHT segment (2 bytes)
					uint16_t DHTlength = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];

					countCheckForCommand = countCheckForCommand + 2; 
					uint16_t bytesRead = 2;   // number of bytes read in DHT segment (same type as countCheckForCommand)

					while (bytesRead < DHTlength && dataOfJPEG[countCheckForCommand] != 0xff) // while there is still data to read 
																								// (i.e. multiple HT tables to read)
					{
						//----Step 1: EXTRACT BINARY DATA----
					
						uint8_t numberOfSymbols[16]; // Identifies number of symbols per word length
						uint8_t numberOfCodes = 0;   // Number of codes per symbol if available
						
						// Now reading: Huffman table information (to decompose)
					
						//HT information 1 byte bit 0..3 : number of HT (0..3, otherwise error)
						//bit 4 : type of HT, 0 = DC table, 1 = AC table
						//bit 5..7 : not used, must be 0
						uint8_t DHTInformation = dataOfJPEG[countCheckForCommand];
						uint8_t numberOfHT = DHTInformation % 16;   // First 4 bits give number of Huffman table
						uint8_t HTisAC = (DHTInformation / 16) % 2; // 5th bit gives AC/DC information

						//----Step 2: EXTRACT SYMBOL COUNT: PER CODE LENGTH AND TOTAL----
						for (int i = 0; i < 16 && dataOfJPEG[countCheckForCommand] != 0xff; i++) // for codes of length 1...16 (constant)
						{
							numberOfSymbols[i] = dataOfJPEG[++countCheckForCommand]; // Now reading: Number of symbols with codes of length i
							bytesRead++;
							numberOfCodes += dataOfJPEG[countCheckForCommand]; // increment total number of symbols
						} // end for
					
						//----Step 3: IF HT EXISTS, BEGIN CONSTRUCTION OF HT STRUCTURE----
						if(numberOfCodes > 0)
						{						
							HTablePtr newPtr;      // pointer to a new table
							HTablePtr previousPtr; // pointer to the previous table
							HTablePtr currentPtr;  // pointer to the current table
						
							newPtr = malloc( sizeof(HTable) ); // create pointer to blank HT
						
							if(newPtr != NULL) // if space is available
							{
								newPtr->DHTnumber = DHTcount;           // place DHTcount value in DHTnumber of HT
								newPtr->HTnumber = numberOfHT;          // place HTcount value in HTnumber of HT
								newPtr->acdc = HTisAC;                  // place HTisAC value in acdc of HT
								newPtr->codeNumber = numberOfCodes;     // place numberOfCodes value in codeNumber of HT
							
								for(int i = 0; i < 16; i++)
								{
									newPtr->symbolNumber[i] = numberOfSymbols[i]; // place numberOfSymbols ARRAY values in symbolNumber ARRAY (possible?) of HT
								}
							
								//----Step 4: OBTAIN SYMBOLS MATRIX----
								// Create a Huffman table structure with all the previous information and symbols
								for (int i = 0; i < 16 && dataOfJPEG[countCheckForCommand] != 0xff; i++) // for codes of length 1...16 (constant)
								{
									if(numberOfSymbols[i] != 0)
									{
										newPtr->symbolPtr[i] = malloc(sizeof (uint8_t)*numberOfSymbols[i]); // Check if we can allocate enough memory 
																											// to an array of size numberOfSymbols[i]
										if(newPtr->symbolPtr[i] != NULL) // There is enough space.
										{
											for (int j = 0; j < numberOfSymbols[i]; j++) // for each symbol with codes of length i
											{
												// use pointer arithmetic to place symbols, not placeholder array
												newPtr->symbolPtr[i][j] = dataOfJPEG[++countCheckForCommand]; // Now reading: Symbols
												bytesRead++;
											} // end inner for
										}
										else // Not enough space
										{
											// error prevention/notification if there is no space available goes here
											//return_; //(?)
											break;
										} // end else
									} // end outer if
									else
									{
										newPtr->symbolPtr[i] = NULL;
									}
								} // end outer for
							
								newPtr->nextPtr = NULL; // HT is placed last in linked list
							
								//----Step 5: UPDATE LINKED LINK POINTERS----
							
								previousPtr = NULL;    // Initialize previous HT pointer
								currentPtr = startPtr; // Initialize current HT pointer
							
								while(currentPtr != NULL) // Go to the end of the linked list
								{
									previousPtr = currentPtr;
									currentPtr = currentPtr -> nextPtr;
								} // end inner while
							
								if (previousPtr == NULL) // This is the first HT found
								{
									newPtr->nextPtr = startPtr; // No HT's follow yet (NULL)
									startPtr = newPtr;          // Initialize starting pointer
								} // end if		
								else // All subsequent HTs found
								{
									previousPtr->nextPtr = newPtr; // Update previous HT pointer to new HT pointer
									newPtr->nextPtr = currentPtr;  // HT is last in list (NULL)
								} // end else
														
								HTcount++;
							} // end inner if
							else // no space available
							{
								// error prevention/notification if there is no space available goes here
								// return _; //(?)
								break;
							} // end else
						} // end outer if
						countCheckForCommand++;
						bytesRead++;
					} // end while : no more data to read in DHT segment

					DHTcount++; // One DHT segment has been read

					break;

				case DQT:

					//Marker Identifier 2 bytes 0xff, 0xdb identifies DQT
					countCheckForCommand = countCheckForCommand + 2;
					//Length 2 bytes This gives the length of QT.
					QTlength = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];
					countCheckForCommand = countCheckForCommand + 2;

					//QT information 1 byte bit 0..3: number of QT (0..3, otherwise error)
					//bit 4..7: precision of QT, 0 = 8 bit, otherwise 16 bit
					QTInformation = dataOfJPEG[countCheckForCommand] % 8;
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

					uint8_t count_y=0,count_x=0;
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
                    
					//Start Of Scan    Payload:variable size
					countCheckForCommand = countCheckForCommand + 2; // Now reading: Length
					SOSlength = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1]; // = 6+2*(number of components in scan)
																												// does not include compressed image data

					countCheckForCommand = countCheckForCommand + 2; // Now reading: Number of components in scan (axis of colour space)
					SOSNumberOfComponentScan = dataOfJPEG[countCheckForCommand]; // between 1 and 4:
																					// 1 : grey-scaled
																					// 3 : YCbCr or YIQ
																					// 4 : CMYK (not supported here)
					
					scanComponents[1] = malloc(sizeof (uint8_t)*SOSNumberOfComponentScan); // Storage matrix for component ID and associated Huffman table
					scanComponents[2] = malloc(sizeof (uint8_t)*SOSNumberOfComponentScan); // Storage matrix for component ID and associated Huffman table
					
					if((scanComponents[1] != NULL) && (scanComponents[2] != NULL)) // There is enough space.
					{
						for(uint8_t i; i < SOSNumberOfComponentScan; i++)
						{							
								countCheckForCommand++; // Now reading: component ID
								scanComponents[1][i] = dataOfJPEG[countCheckForCommand]; // Contains component ID:
																							// 0 = Y
																							// 1 = Cb
																							// 2 = Cr
																							// 3 = I
																							// 4 = Q
								countCheckForCommand++; // Now reading: Associated Huffman table
								scanComponents[2][i] = dataOfJPEG[countCheckForCommand]; // Contains Huffman table to use:
																							// bit 0...3 : AC table (0...3)
																							// bit 4...7 : DC table (0...3)
						}						
					}
					else
					{
						// error prevention/notification if there is no space available goes here
						//return_; //(?)
						break;
					}
					
					countCheckForCommand = countCheckForCommand + 3; // skips ignorable bytes

					// Start treatment of compressed data

					// Variable initialization
					uint8_t compCheck;
					uint8_t EOIcheck = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];

					if (numberOfComponents == 1) //monochrome image
					{
						// code sending monochrome data to appropriate Huffman table
					}
					else if(numberOfComponents == 3)// colour image: 
					{
						if((sampfactor[0][0] == 1) && (sampfactor[0][1] == 2) && (sampfactor[0][2] == 3)) // YCbCr
						{
							// Deal with compressed image data
							while (EOIcheck != EOI)
							{
								STablePtr newSPtr;      // pointer to a new table
								STablePtr previousSPtr; // pointer to the previous table
								STablePtr currentSPtr;  // pointer to the current table
								
								newSPtr = malloc( sizeof(STable) ); // create pointer to blank HT data repository
						
								if(newSPtr != NULL) // if space is available
								{
									newSPtr->acht = malloc(sizeof (uint8_t)*(chrosize + 2));
									newSPtr->dcht = malloc(sizeof (uint8_t)*(chrosize + 2));
	
									if ((newSPtr->acht != NULL) && (newSPtr->dcht != NULL))
									{
										for (uint8_t i = 0; i < chrosize + 2; i++)
										{
											compCheck = dataOfJPEG[countCheckForCommand]; // byte under test
											newSPtr->acht[i] = ImageDataScan(scanComponents, compCheck, chrosize, i) % 16; // Obtain first 4 bits of the byte
											newSPtr->dcht[i] = ImageDataScan(scanComponents, compCheck, chrosize, i) / 16; // Obtain last 4 bits of the byte
											countCheckForCommand++; // next byte

											// check for EOI
											EOIcheck = (dataOfJPEG[countCheckForCommand]<<8)+ dataOfJPEG[countCheckForCommand + 1];
											if (EOIcheck == EOI)
											{
												break;
											} // end if
										} // end for
									} // end if
									else
									{
										// error prevention/notification if there is no space available goes here
										//return_; //(?)
										break;
									} // end if else
	
									newSPtr->nextSOSPtr = NULL; // HT data is placed last in linked list
							
									//----UPDATE LINKED LINK POINTERS----
							
									previousSPtr = NULL;    // Initialize previous HT data pointer
									currentSPtr = SstartPtr; // Initialize current HT data pointer
							
									while(currentSPtr != NULL) // Go to the end of the linked list
									{
										previousSPtr = currentSPtr;
										currentSPtr = currentSPtr -> nextSOSPtr;
									} // end inner while
							
									if (previousSPtr == NULL) // This is the first HT found
									{
										newSPtr->nextSOSPtr = SstartPtr; // No HT data follows yet (NULL)
										SstartPtr = newSPtr;          // Initialize starting pointer
									} // end if		
									else // All subsequent HTs found
									{
										previousSPtr->nextSOSPtr = newSPtr; // Update previous HT data pointer to new HT data pointer
										newSPtr->nextSOSPtr = currentSPtr;  // HT data is last in list (NULL)
									} // end else
									
									SOScount++;
								} // end if
								else // no space available
								{
									// error prevention/notification if there is no space available goes here
									// return _; //(?)
									break;
								} // end else
								
								/* Huffman table operations go here */
							} // end while
						} // end if
						else if((sampfactor[0][0] == 1) && (sampfactor[0][1] == 4) && (sampfactor[0][2] == 5)) // YIQ 
						{
							//not yet handled
							break;
						}											
					} // end colour case (else)

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
					
					bytesToBeRead = malloc(sizeof(uint8_t)*(APP0length - 16));   // number of bytes read in APP0 segment (same type as countCheckForCommand)

					if(bytesToBeRead != NULL)
					{
						for (uint8_t count = 0; dataOfJPEG[countCheckForCommand]!=0xFF; count++)
						{
							bytesToBeRead[count] = dataOfJPEG[countCheckForCommand];
							countCheckForCommand++;
						}
					}
					else
					{
						// error prevention/notification if there is no space available goes here
						//return_; //(?)
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
					end_of_image = 1; // was a bool
					break;
				default:
					countCheckForCommand++;
					break;
			}
		}
		
//---ImageDataScan class---

        uint8_t ImageDataScan(uint8_t *huffFind[2], uint8_t compbyte, uint16_t chrosize, uint8_t count)
        {
            uint8_t hTable = 0;

            if ((huffFind[0][0] == 1) && (huffFind[1][0] == 2) && (huffFind[2][0] == 3))
            {   
                //YCbCr treatment
                if (count < chrosize)       // luma (Y) values until last two bytes
                {
                    hTable = huffFind[0][1];
                }
                else if (count == chrosize) // Cb case
                {
                    hTable = huffFind[1][1];
                }
                else                        // Cr case
                {
                    hTable = huffFind[2][1];
                }
            }
            else if ((huffFind[0][0] == 1) && (huffFind[1][0] == 4) && (huffFind[2][0] == 5))
            {
                //YIQ treatment
            }
            return hTable;
        }
