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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//----Translate Header titles from hexadecimal----

#define  SOI   0xFFD8             // Start Of Image    Payload: none
#define  SOF0  0xFFC0             // Start Of Frame (Baseline JPEGFileReader)    Payload:variable size
#define  SOF2  0xFFC2             // Start Of Frame (Progressive JPEGFileReader)    Payload:variable size
#define  DHT   0xFFC4             // Define Huffman Table(s)    Payload:variable size
#define  DQT   0xFFDB             // Define Quantization Table(s)    Payload:variable size
#define  DRI   0xFFDD             // Define Restart Interval    Payload:2bytes
#define  SOS   0xFFDA             // Start Of Scan    Payload:variable size
#define  RSTn0 0xFFD0             // Restart    Payload:none
#define  RSTn1 0xFFD1             // Restart    Payload:none
#define  RSTn2 0xFFD2             // Restart    Payload:none
#define  RSTn3 0xFFD3             // Restart    Payload:none
#define  RSTn4 0xFFD4             // Restart    Payload:none
#define  RSTn5 0xFFD5             // Restart    Payload:none
#define  RSTn6 0xFFD6             // Restart    Payload:none
#define  RSTn7 0xFFD7             // Restart    Payload:none
#define  APP0  0xFFE0             // Application-specific    Payload:variable size
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
#define  COM   0xFFFE               // Comment    Payload:variable size
#define  EOI   0xFFD9

//----Initialization----

//---Structures---

	struct huffmanTable // HT structure
	{
		uint8_t DHTnumber;            // DHT segment HT was located in           (4 bits)
		uint8_t HTnumber;             // HT number as indicated by image data    (4 bits)
		uint8_t acdc;                 // AC or DC class of the HT                (1 bit)
		uint8_t codeNumber;           // Number of codes contained within the HT (8 bits)
		uint8_t symbolNumber[16];     // Total number of symbols (classified by code length) of the HT
									  //(16 element array of variable sized elements (max size is a byte))
		uint8_t *symbolPtr[16];       // Array of pointers to symbol arrays
		struct huffmanTable *nextPtr; // Pointer to next HT (HT organized in a linked list)
	};

	typedef struct huffmanTable HTable;
	typedef HTable *HTablePtr;
	HTablePtr startPtr = NULL; // No tables initially

	struct SOSHTable { // chrominance pixel structure
		uint16_t pID;     // chrominance pixel number
		uint8_t *content; // image data byte
		uint8_t *acht;    // AC Huffman table to use (non zero if it exists)
		uint8_t *dcht;    // DC Huffman table to use (non zero if it exists)
		struct SOSHTable *nextSOSPtr;
	};

	typedef struct SOSHTable STable;
	typedef STable *STablePtr;
	STablePtr SstartPtr = NULL; // No tables initially

//---Prototypes---

	// Convert input data stream to byte array (for testing)
	//uint8_t* ImageToByte (uint32_t* jpglengthOut, char *jpglocation);

	// Read bytes from the SD card
	uint8_t read_byte();

	// Determine header command from data segment
	uint8_t commandCheck(uint32_t jpegLength);

	// Store data according to header information
	uint8_t JPEGMethod(uint32_t JPEGCommand) ;

	// Store HT information for a chrominance pixel
	uint8_t ImageDataScan(uint8_t *huffFind[2], uint16_t chrosize, uint16_t count);

//---General variables---

	uint8_t *jpegData = NULL;   // pointer to stored read byte
	uint8_t currentByte; // temporary variable for a read byte (combine with above?)
	uint32_t jpegLength; // length of jpeg

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
	uint8_t mcuheight;           // height of an MCU
	uint8_t mcuwidth;            // width of an MCU
	uint8_t *sampfactor[3];      // sampling factor information

	//--DHT Header--

	uint16_t DHTlength;
	uint16_t DHTcount = 0;      // Defines number of DHT segments read.
	uint16_t HTcount = 0;       // Defines number of HT created.

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
	uint16_t SOScount = 0;

	//--EOI Header--
	uint8_t end_of_image = 0;

	//--APP0 Header--
	// uint64_t fileIdentifierMark = 0;
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

	FILE *jpegFile; // input stream

		int main(void)
		{
			//Initialize input stream
			jpegFile = fopen("D:\\JPEGtestpics\\Leaves2.jpg", "rb");
			if (jpegFile == NULL)
			{
				printf("Failed to open jpeg.\n");
				return 0;
			} // end if : error checking

  			// Get file length
  			fseek(jpegFile, 0, SEEK_END);
  			jpegLength = ftell(jpegFile);
  			fseek(jpegFile, 0, SEEK_SET);

			uint8_t didItWork = commandCheck(jpegLength);

			//----CODE VERIFICATION----

			// Successful run
			if (didItWork == 1)
			{
				printf("Code ran successfully.\n\n");
			} // end if : code run confirmation
			else
			{
				printf("Something went wrong.\n");
			} // end else : error output

			// HEADER LENGTHS
			printf("JPEG image contains %d bytes.\n", jpegLength);
			printf("Length of SOF segment: %d bytes.\n", sof0Length);
			printf("Length of DQT segment: %d bytes.\n", QTlength);
			printf("Length of SOS segment: %d bytes.\n", SOSlength);
			printf("Length of APP0 segment: %d bytes.\n", APP0length);

			// SOF INFORMATION
			printf("\n---SOF INFORMATION---\n");
			printf("Data precision: %d bits/sample.\n", sof0precision);
			printf("Image height: %d pixels.\n", height);
			printf("Image width: %d pixels.\n", width);
			printf("Components per pixel: %d.\n", numberOfComponents);

			uint8_t i;
			for(i = 0; i < numberOfComponents; i++)
			{
				printf("\nComponent %d uses the following sampling factors:\n", i + 1);
				printf("%d bit(s) vertical.\n", sampfactor[1][i] % 16);
				printf("%d bit(s) horizontal.\n", sampfactor[1][i] / 16);
			} // end for : output of all component sampling factors

			printf("\nChroma subsampling height: %d.\n", chroheight);
			printf("Chroma subsampling width: %d.\n", chrowidth);
			printf("Chroma subsampling size: %d.\n", chrosize);
			printf("MCU width: %d.\n", mcuwidth);
			printf("MCU height: %d.\n", mcuheight);

			// SOS INFORMATION
			printf("\n---SOS INFORMATION---\n");
			printf("Number of image components: %d\n", numberOfComponents);
			for(i = 0; i < numberOfComponents; i++)
			{
				printf("\nComponent %d : AC Table: %d\n", i + 1, (scanComponents[1][i]) % 16);
				printf("              DC Table: %d\n", (scanComponents[1][i]) / 16);
			}

			// DHT INFORMATION
			printf("\n---DHT INFORMATION---\n");
			printf("Number of DHT segments read: %d.\n", DHTcount);
			printf("Number of HT's read: %d.\n", HTcount);

			// HT DATA
			if( startPtr == NULL)
			{
				printf("\nThere are no Huffman tables.\n\n");
			} // end outer if : error output
			else
			{
				printf("\nThe Huffman tables are:\n\n");

				// while there are still HT's
				while(startPtr != NULL)
				{
					printf("Huffman table number: %d\n", startPtr->HTnumber + 1 );
					printf("DHT segment: %d\n", startPtr->DHTnumber + 1 );

					if(startPtr->acdc == 1)
					{
						printf("Table is AC.\n");
					} // end if : AC confirmation
					else
					{
						printf("Table is DC.\n");
					} // end else : DC confirmation

					printf("Table contains %d codes.\n", startPtr->codeNumber + 1);

					uint8_t i;
					uint8_t j;

					for(i = 0; i < 16; i++)
					{
						printf("Number of symbols of length %2d: %d\n",i + 1,startPtr->symbolNumber[i]);
					} // end for : AC confirmation

					for(i = 0; i < 16; i++)
					{
						printf("Codes of length %2d: ", i + 1);
						for(j = 0; j < startPtr->symbolNumber[i]; j++)
						{
							if(((j + 1) % 11 == 0))
							{
								printf("\n                    ");
							} // end inner if : indentation
							printf("%02X ", startPtr->symbolPtr[i][j]);
						} // end inner for : code output for set length

						printf("\n");
					} // end outer for : code output for all lengths
					startPtr = startPtr->nextPtr;

					printf("\n---\n");
				} // end while : current HT information output
			} // end outer else : all HT information output

			return 0;
		} // end main code
/*
//---ImageToByte function---
		uint8_t* ImageToByte (uint32_t* jpglengthOut, char *jpglocation)
		{
			// Initialization
			FILE *jpegFile; // input stream
			uint32_t jpglength;

			//Read file as a binary file
			jpegFile = fopen(jpglocation, "rb");
			if (jpegFile == NULL)
			{
				fprintf(stderr, "Unable to open file %s", jpglocation); //Error message: change for AVR?
			}

			// Get file length
			fseek(jpegFile, 0, SEEK_END);
			jpglength = ftell(jpegFile); // read as binary file.
			fseek(jpegFile, 0, SEEK_SET);

			// Allocate memory: saved as array of bytes
			uint8_t* dataOfJPEG = malloc(sizeof(uint8_t)*jpglength);
			if (dataOfJPEG == NULL)
			{
				fprintf(stderr, "Memory error!"); //Error message: change for AVR?
				fclose(jpegFile);
			}

		   // Read data from input stream to byte array
		   fread(dataOfJPEG,sizeof(uint8_t),jpglength,jpegFile);

		   fclose(jpegFile);

		   *jpglengthOut = jpglength;

		   // return byte array pointer
		   return dataOfJPEG;
		}
*/
		uint8_t read_byte()
		{
			jpegData = malloc(sizeof(uint8_t));
			if(jpegData == NULL)
			{
				// no space available
				return 0;
			} // end if : error confirmation

			fread(jpegData,sizeof(uint8_t),1,jpegFile);
			return *jpegData;
		} // end function : read_byte

//---commandCheck function---

		uint8_t commandCheck(uint32_t jpegLength)
		{
			uint32_t count_data_in;
			uint32_t JPEGCommand = 0x0000; //the data starts with 0xFF.
										   //Therefore the double data is not convertible
										   //( the max value of double is 0x7FFF)
			currentByte = read_byte();
			for( count_data_in = 0; count_data_in < jpegLength; count_data_in++ )
			{
				if (currentByte == 0xff) // if header is found
				{
					currentByte = read_byte(); // checks 2nd part of header
											   // also "de-header"s 0xff
					JPEGCommand = (0xFF << 8) + currentByte;             // get next header byte
					uint8_t didJPEGMethodWork = JPEGMethod(JPEGCommand); //
					if(didJPEGMethodWork == 0)
					{
						break;
					} // end inner if: error break
					//End of file treatment
					if(end_of_image == 1)
					{
						return 1;
					} // end if: image data extraction
				} // end outer if : header detection response
				else // header not found
				{
					currentByte = read_byte();
				}

				//End of file treatment
				if(end_of_image == 1)
				{
					return 1;
				} // end if: image data extraction
				else if(count_data_in == jpegLength - 1)
				{
					return 0; // Error
				} // end else if : error confirmation
			} // end for : reading image data

			return 0;
		} // end function : commandCheck

//---JPEGMethod function---

		uint8_t JPEGMethod(uint32_t JPEGCommand)
		{
			currentByte = 0;

			switch(JPEGCommand)
			{
				/*
				case SOI:
					// Start Of Image    Payload: none
					break;
			 	*/

				case SOF0:
					//Start Of Frame (Baseline JPEGFileReader)    Payload:variable size
					// Data segment reading

					// Now reading: Length data
					// Length data = 8 + 3 * component values
					sof0Length = read_byte();     // take one byte
					sof0Length = sof0Length << 8; // set as most significant byte
					sof0Length += read_byte();    // take least significant byte

					// Now reading: Data precision
					sof0precision = read_byte();

					// Now reading: Image height
					height = read_byte();     // take one byte
					height = height << 8;     // set as most significant byte
					height += read_byte();    // take least significant byte

					// Now reading: Image width
					width = read_byte();     // take one byte
					width = width << 8;      // set as most significant byte
					width += read_byte();    // take least significant byte

					// Now reading: Number of components
					numberOfComponents = read_byte();

					uint8_t i;

					// Sampling factor information initialization: set size
					for(i = 0; i < 3; i++)
					{
						sampfactor[i] = malloc(sizeof(uint8_t) * numberOfComponents);
						if(sampfactor[i] == NULL)
						{
							// no space available
							return 0;
						} // end if : error confirmation
					} // end for : reading sampling factor information

					// Sampling factor information initialization: get data
					for (i = 0; i < numberOfComponents; i++)
					{
						// Now reading: component ID:
							// 1 = Y
							// 2 = Cb
							// 3 = Cr
							// 4 = I
							// 5 = Q
						sampfactor[0][i] = read_byte();

						// Now reading: sampling factors :
							// bit 0-3: vertical
							// bit 4-7: horizontal
						sampfactor[1][i] = read_byte();

						// Now reading: Quantization table number
						sampfactor[2][i] = read_byte();
					} // end for : reading sampling factor information

					//if (numberOfComponents == 1)
					//{
					//    //Monochrome case: should not have to do anything, all are considered Y
					//}
					//else

					// Obtain chrominance pixel data
					if (numberOfComponents == 3) //Colour case
					{
						// YCbCr case
						if ((sampfactor[0][0] == 1) && (sampfactor[0][1] == 2) && (sampfactor[0][2] == 3))
						{
							chroheight = sampfactor[1][0] % 16; // Obtain first 4 bits of the byte as int
							chrowidth = sampfactor[1][0] / 16;  // Obtain last 4 bits of the byte as int
							chrosize = chrowidth * chroheight;

							if((chrowidth == 1 )&&(chroheight == 1)) // 1x1 chroma sub-sampling
							{
								//4:4:4
								mcuwidth = 8;
								mcuheight = 8;
							} // end if : 1x1 chroma subsampling case
							else if((chrowidth == 2 )&&(chroheight == 1))// 2x1 chroma sub-sampling
							{
								//4:2:2
								mcuwidth = 16;
								mcuheight = 8;
							} // end if : 2x1 chroma subsampling case
							else if((chrowidth == 2 )&&(chroheight == 2))// 2x2 chroma sub-sampling
							{
								//4:2:0
								mcuwidth = 16;
								mcuheight = 16;
							} // end if : 2x2 chroma sub-sampling case
							else // error
							{
								return 0;
							} // end else : error
						}
						// YIQ case
						else if ((sampfactor[0][0] == 1) && (sampfactor[0][1] == 4) && (sampfactor[0][2] == 5))
						{
							return 0;
						} // end else if : error
					}
					//else if (numberOfComponents == 4)
					//{
					//		// CMYK functionality not yet handled
					//}

					break;

				/*
				case SOF2:
					//Start Of Frame (Progressive JPEGFileReader)    Payload:variable size
					break;
				*/

				case DHT:

					// Define Huffman Table(s)    Payload:variable size
					// Marker Identifier 2 bytes 0xff, 0xc4 to identify DHT marker

					// Now reading: Huffman table segment length

					//Length 2 bytes: This specifies the length of the DHT segment (2 bytes)
					DHTlength = read_byte();     // take one byte
					DHTlength = DHTlength << 8;  // set as most significant byte
					DHTlength += read_byte();    // take least significant byte

					uint32_t bytesRead = 2;   // number of bytes read in DHT segment (same type as countCheckForCommand)
					currentByte = read_byte();
					while ((bytesRead < DHTlength)&&(currentByte != 0xff)) // while there is still data to read
												                           // (i.e. multiple HT tables to read)
					{
						//----Step 1: EXTRACT BINARY DATA----

						uint8_t numberOfSymbols[16]; // Identifies number of symbols per word length
						uint8_t numberOfCodes = 0;   // Number of codes per symbol if available

						// Now reading: Huffman table information (to decompose)

						//HT information 1 byte bit 0..3 : number of HT (0..3, otherwise error)
						//bit 4 : type of HT, 0 = DC table, 1 = AC table
						//bit 5..7 : not used, must be 0
						uint8_t DHTInformation = currentByte;
						uint8_t numberOfHT = DHTInformation % 16;   // First 4 bits give number of Huffman table
						uint8_t HTisAC = (DHTInformation / 16) % 2; // 5th bit gives AC/DC information
						uint8_t i;

						//----Step 2: EXTRACT SYMBOL COUNT: PER CODE LENGTH AND TOTAL----
						for (i = 0; i < 16 && currentByte != 0xff; i++) // for codes of length 1...16 (constant)
						{
							currentByte = read_byte();
							numberOfSymbols[i] = currentByte; // Now reading: Number of symbols with codes of length i
							bytesRead++;
							numberOfCodes += currentByte; // increment total number of symbols
						} // end for : Obtain number of symbols

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

								for(i = 0; i < 16; i++)
								{
									newPtr->symbolNumber[i] = numberOfSymbols[i]; // place numberOfSymbols ARRAY values in symbolNumber ARRAY (possible?) of HT
								} // end for : allocate symbol memory

								//----Step 4: OBTAIN SYMBOLS MATRIX----
								// Create a Huffman table structure with all the previous information and symbols


								for (i = 0; i < 16 && currentByte != 0xff; i++) // for codes of length 1...16 (constant)
								{
									if(numberOfSymbols[i] != 0)
									{
										newPtr->symbolPtr[i] = malloc(sizeof (uint8_t)*numberOfSymbols[i]); // Check if we can allocate enough memory
																											// to an array of size numberOfSymbols[i]
										uint8_t j;
										if(newPtr->symbolPtr[i] != NULL) // There is enough space.
										{
											for (j = 0; j < numberOfSymbols[i]; j++) // for each symbol with codes of length i
											{
												currentByte = read_byte();
												// use pointer arithmetic to place symbols, not placeholder array
												if(currentByte != 0xff)
												{
													newPtr->symbolPtr[i][j] = currentByte; // Now reading: Symbols
													bytesRead++;
												} // end if : not the end of the image length
												else
												{
													break;
												} // end else : new header found
											} // end inner for : read code
										} // end inner if : read for current length
										else // Not enough space
										{
											break;
										} // end else : memory error
									} // end outer if : non zero number of symbols
									else
									{
										newPtr->symbolPtr[i] = NULL;
									} // end outer else: no symbols of this length
								} // end outer for : end current code length operation

								newPtr->nextPtr = NULL; // HT is placed last in linked list

								//----Step 5: UPDATE LINKED LINK POINTERS----

								previousPtr = NULL;    // Initialize previous HT pointer
								currentPtr = startPtr; // Initialize current HT pointer

								while(currentPtr != NULL)
								{
									previousPtr = currentPtr;
									currentPtr = currentPtr -> nextPtr;
								} // end inner while : set pointer to the end of the linked list

								if (previousPtr == NULL)
								{
									newPtr->nextPtr = startPtr; // No HT's follow yet (NULL)
									startPtr = newPtr;          // Initialize starting pointer
								} // end if : first HT case
								else // All subsequent HTs found
								{
									previousPtr->nextPtr = newPtr; // Update previous HT pointer to new HT pointer
									newPtr->nextPtr = currentPtr;  // HT is last in list (NULL)
								} // end else : second or later HT case

								HTcount++;
							} // end if : HT creation
							else // no space available
							{
								// error prevention/notification if there is no space available goes here
								return 0;
							} // end else : error confirmation
						} // end outer if : existing HT operation

						if(currentByte != 0xff)
						{
							currentByte = read_byte(); // prepare currentByte for next loop
							bytesRead++;
						} // end if : not the end of the image length
						else
						{
							break;
						} // end else : new header found
					} // end while : no more data to read in DHT segment

					DHTcount++; // One DHT segment has been read

					break;

				case DQT:

					//Marker Identifier 2 bytes 0xff, 0xdb identifies DQT

					//Length 2 bytes This gives the length of QT.
					QTlength = read_byte();     // take one byte
					QTlength = QTlength << 8; // set as most significant byte
					QTlength += read_byte();    // take least significant byte

					currentByte = read_byte(); // QT information byte

					//QT information 1 byte bit 0..3: number of QT (0..3, otherwise error)
					//bit 4..7: precision of QT, 0 = 8 bit, otherwise 16 bit
					QTInformation = currentByte % 8;
					if (currentByte / 8 == 0)
					{
						QTnumber = currentByte / 8;
						QTprecision = 8;
					} // end if
					else
					{
						QTprecision = 16;
					} // end else

					uint8_t count_y = 0,count_x = 0;
						while (count_y < QTprecision)
						{
							count_x = 0;
							while (count_x < QTprecision)
							{
								all_dqt_table[QTcount][count_y][count_x] = read_byte();
								count_x++;
							} // end inner while
							count_y++;
						} // end outer while
					QTcount++;
					//Bytes n bytes This gives QT values, n = 64*(precision+1)

					break;

				/*
				case DRI:
					//Define Restart Interval    Payload:2bytes
					break;
				*/

				case SOS:
					//Start Of Scan    Payload:variable size

					// Now reading: Length
					// = 6+2*(number of components in scan)
					// does not include compressed image data
					SOSlength = read_byte();    // take one byte
					SOSlength = SOSlength << 8; // set as most significant byte
					SOSlength += read_byte();   // take least significant byte

					// Now reading: Number of components in scan (axis of colour space)
					SOSNumberOfComponentScan = read_byte(); // between 1 and 4:
																					// 1 : grey-scaled
																					// 3 : YCbCr or YIQ
																					// 4 : CMYK (not supported here)

					// Storage matrices for component ID and associated Huffman table
					scanComponents[0] = malloc(sizeof(uint8_t) * SOSNumberOfComponentScan);
					scanComponents[1] = malloc(sizeof(uint8_t) * SOSNumberOfComponentScan);

					if((scanComponents[0] != NULL) && (scanComponents[1] != NULL)) // There is enough space.
					{
						uint8_t i;
						for(i = 0; i < SOSNumberOfComponentScan; i++)
						{
							// Now reading: component ID
							scanComponents[0][i] = read_byte(); // Contains component ID:
																						// 1 = Y
																						// 2 = Cb
																						// 3 = Cr
																						// 4 = I
																						// 5 = Q
							// Now reading: Associated Huffman table
							scanComponents[1][i] = read_byte(); // Contains Huffman table to use:
																						// bit 0...3 : AC table (0...3)
																						// bit 4...7 : DC table (0...3)
						} // end for : each component
					} // end if : memory successfully allocated
					else
					{
						// error prevention/notification if there is no space available goes here
						return 0;
					} // end else : memory allocation failed

					read_byte(); // skips ignorable byte
					read_byte(); // skips ignorable byte
					read_byte(); // skips ignorable byte

					// TREATMENT OF COMPRESSED DATA

					currentByte = 0;
					/*
					if (numberOfComponents == 1) //monochrome image
					{
						// code sending monochrome data to appropriate Huffman table
					} // end if
					*/
					if(numberOfComponents == 3) // colour image:
					{
						// YCbCr
						if((sampfactor[0][0] == 1) && (sampfactor[0][1] == 2) && (sampfactor[0][2] == 3))
						{
							uint8_t nextByte;
							// Deal with compressed image data
							while(1)
							{
								STablePtr newSPtr;      // pointer to a new Cpixel
								STablePtr previousSPtr; // pointer to the previous Cpixel
								STablePtr currentSPtr;  // pointer to the current Cpixel

								newSPtr = malloc( sizeof(STable) ); // create pointer to blank Cpixel data repository

								if(newSPtr != NULL) // if space is available
								{
									newSPtr->pID = SOScount;
									newSPtr->acht = malloc(sizeof (uint8_t)*(chrosize + 2));
									newSPtr->dcht = malloc(sizeof (uint8_t)*(chrosize + 2));
									newSPtr->content = malloc(sizeof (uint8_t)*(chrosize + 2));

									newSPtr->nextSOSPtr = NULL; // Cpixel data is placed last in linked list

									//----UPDATE STRUCTURE DATA----

									if ((newSPtr->acht != NULL) && (newSPtr->dcht != NULL) && (newSPtr->content != NULL))
									{
										uint16_t i;
										for (i = 0; i < chrosize + 2; i++)
										{
											currentByte = read_byte();

											// Check if end of compressed data
											if(currentByte == 0xff) // special case: 0xff
											{
												nextByte = read_byte();
												if(nextByte != 0x00) // header found/end of data
												{
													free(newSPtr->acht);
													free(newSPtr->dcht);
													free(newSPtr->content);
													if(nextByte == 0xd9)
													{
														end_of_image = 1;
													}
													break; // break while loop when marker found
												} // end if : premature end of image data
												// 0x00 automatically skipped
											} // end if : end special case for 0xff

											newSPtr->content[i] = currentByte; // byte under test
											newSPtr->acht[i] = ImageDataScan(scanComponents, chrosize, i) % 16; // Obtain first 4 bits of the byte
											newSPtr->dcht[i] = ImageDataScan(scanComponents, chrosize, i) / 16; // Obtain last 4 bits of the byte
										} // end for : currently seperating the data by chrominance pixel.
										  //           (may need to change to MCU size)
									} // end if : memory successfully allocated
									else
									{
										// error prevention/notification if there is no space available goes here
										free(newSPtr->acht);
										free(newSPtr->dcht);
										free(newSPtr->content);
										return 0;
									} // end else : failed to allocate memory

									//----UPDATE LINKED LINK POINTERS----

									previousSPtr = NULL;    // Initialize previous HT data pointer
									currentSPtr = SstartPtr; // Initialize current HT data pointer

									while(currentSPtr != NULL) // Go to the end of the linked list
									{
										previousSPtr = currentSPtr;
										currentSPtr = currentSPtr -> nextSOSPtr;
									} // end inner while : add an element at the end of the linked list

									if (previousSPtr == NULL)
									{
										newSPtr->nextSOSPtr = NULL; // No HT data follows yet (NULL)
										SstartPtr = newSPtr;        // Initialize starting pointer
									} // end if : initialize linked list pointers (first element)
									else
									{
										previousSPtr->nextSOSPtr = newSPtr; // Update previous HT data pointer to new HT data pointer
										newSPtr->nextSOSPtr = currentSPtr;  // HT data is last in list (NULL)
									} // end else : continue linked list (subsequent elements)

									SOScount++;
								} // end if : memory allocation successful
								else // no space available
								{
									// error prevention/notification if there is no space available goes here
									return 0;
								} // end else : memory allocation failed

								// Check if end of compressed data
								if(currentByte == 0xff)
								{
									if(nextByte != 0x00)
									{
										if(nextByte == 0xd9)
										{
											end_of_image = 1;
										}
										break; // break while loop when marker found
									} // end if : premature end of image data
									// 0x00 automatically skipped
								} // end if : possible header case
							} // end while : read through image data until header found
						} // end outer if : YCbCr case
						else if((sampfactor[0][0] == 1) && (sampfactor[0][1] == 4) && (sampfactor[0][2] == 5)) // YIQ
						{
							//not yet handled
							return 0;
						} // end else if
					} // end else : colour case
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

					//Length 2 bytes It must be >= 16
					APP0length = read_byte();     // take one byte
					APP0length = APP0length << 8; // set as most significant byte
					APP0length += read_byte();    // take least significant byte

					//File Identifier Mark 5 bytes This identifies JFIF.
					//'JFIF'#0 (0x4a, 0x46, 0x49, 0x46, 0x00)

					uint8_t check1 = read_byte();
					uint8_t check2 = read_byte();
					uint8_t check3 = read_byte();
					uint8_t check4 = read_byte();
					uint8_t check5 = read_byte();

					//I don't know what this does...
					/*
					   fileIdentifierMark = (dataOfJPEG[countCheckForCommand] == 0x4A &&
											dataOfJPEG[countCheckForCommand + 1] == 0x46 &&
											dataOfJPEG[countCheckForCommand + 2] == 0x49 &&
											dataOfJPEG[countCheckForCommand + 3] == 0x46 &&
											dataOfJPEG[countCheckForCommand + 4] == 0x00);
					*/

					//Major revision number 1 byte Should be 1, otherwise error

					majorRevisionNumber = read_byte();

					//Minor revision number 1 byte Should be 0..2, otherwise try to decode anyway
					minorRevisionNumber = read_byte();

					//Units for x/y densities 1 byte 0 = no units, x/y-density specify the aspect ratio instead
					//1 = x/y-density are dots/inch
					//2 = x/y-density are dots/cm
					unitForXYDensities = read_byte();

					//X-density 2 bytes It should be <> 0
					xDensity = read_byte();     // take one byte
					xDensity = xDensity << 8; // set as most significant byte
					xDensity += read_byte();    // take least significant byte

					//Y-density 2 bytes It should be <> 0
					yDensity = read_byte();     // take one byte
					yDensity = yDensity << 8; // set as most significant byte
					yDensity += read_byte();    // take least significant byte

					//Thumbnail width 1 byte -------
					thumbnailWidth = read_byte();

					//Thumbnail height 1 byte -------
					thumbnailHeight = read_byte();

					//Bytes to be read n bytes For thumbnail (RGB 24 bit), n = width*height*3 bytes should be read imm
					//followed by thumbnail height
					//error: no new

					bytesToBeRead = malloc(sizeof(uint8_t)*(APP0length - 16));   // number of bytes read in APP0 segment (same type as countCheckForCommand)
					currentByte = 0;

					if(bytesToBeRead != NULL)
					{
						uint8_t count;
						for (count = 0; currentByte != 0xFF; count++)
						{
							currentByte = read_byte();
							bytesToBeRead[count] = currentByte;
						} // end for
					} // end if
					else
					{
						// error prevention/notification if there is no space available goes here
						return 0; //(?)
					} // end else

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
					// If byte is not a marker that we can handle, increment count and return to for loop:
					// Access the next byte in the data stream
					break;
			}

			return 1;
		}

//---ImageDataScan class---

		uint8_t ImageDataScan(uint8_t *huffFind[2], uint16_t chrosize, uint16_t count)
		{
			uint8_t hTable = 0;

			if ((huffFind[0][0] == 1) && (huffFind[0][1] == 2) && (huffFind[0][2] == 3))
			{
				//YCbCr treatment
				if (count < chrosize)       // luma (Y) values until last two bytes
				{
					hTable = huffFind[1][0];
				}
				else if (count == chrosize) // Cb case
				{
					hTable = huffFind[1][1];
				}
				else                        // Cr case
				{
					hTable = huffFind[1][2];
				}
			}
			else if ((huffFind[0][0] == 1) && (huffFind[0][1] == 4) && (huffFind[0][2] == 5))
			{
				//YIQ treatment
			}

			return hTable;
		}


