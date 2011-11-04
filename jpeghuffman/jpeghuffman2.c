/*
 * jpeghuffman2.c
 *
 * Created: 04/11/2011 18:07:23
 *  Author: mh23g08
 using the observations from http://www.impulseadventure.com/photo/jpeg-decoder.html we can simplify construction of the huffman lookup tables
 specifically:

"Reading DHT Tables source code

Interpreting the DHT table in the JPEG JFIF file stream is not as intuitive as it may appear. It was designed to be stored in the most space-efficient manner, which makes reading it much harder. On paper, it is a fairly simple process to fill out the binary tree with the huffman codes as they appear in the DHT marker segment, but this doesn't translate well to the coded form.

The following example source code is completely un-optimized, as it is intended to show the basic mechanism of reading the DHT table and decoding the variable length JPEG huffman codes. It is based upon my experiences in developing the JPEGsnoop JPEG decoder from scratch.

Instead, one will observe that the codes that appear within the same bitlength will be incrementing by one. Going to the next bitlength requires the next code value to be doubled before continuing the count by one. This method is much more apparent when observing the resulting binary values that the codes correspond to.

Note that a two-dimensional array is used here to retain all of the variable-length codes. The first array index is used to reference the DHT table that we are processing (e.g. AC, DC, Luminance, Chrominance, etc.) The second index is simply to cover all of the possible code strings (max 162)."
from http://www.impulseadventure.com/photo/jpeg-decoder.html accessed 04/11/2011
 */ 

/*
 * jpeghuffman.c
 *
 * Created: 01/11/2011 17:06:17
 *  Author: mh23g08
 */ 

#define PC_DEBUG

#ifdef PC_DEBUG
#include <stdio.h>
#else
#include <avr/io.h>
#endif

#include <stdlib.h>

#define BOOL int
#define FALSE 0
#define TRUE 1



int main(void)
{
	printf("PC_DEBUG set\n");
	unsigned char* huffmanCodes[16];
	int numCodes[16] = { 0, 2, 1, 3, 3, 2, 5, 2, 4, 4, 4, 5, 3, 3, 2, 7 };
	
	unsigned char huff2[] = { 0x01, 0x02 };
    huffmanCodes[1] = huff2;
	unsigned char huff3[] = { 0x03 };
    huffmanCodes[2] = huff3;
	unsigned char huff4[] =  { 0x04, 0x05, 0x11 };
    huffmanCodes[3] = huff4;
	unsigned char huff5[] = { 0x00, 0x06, 0x21 };
    huffmanCodes[4] = huff5;
	unsigned char huff6[] = { 0x12, 0x31 };
    huffmanCodes[5] = huff6;
	unsigned char huff7[] = { 0x07, 0x13, 0x22, 0x41, 0x51 };
    huffmanCodes[6] = huff7;
	unsigned char huff8[] = { 0x14, 0x61 };
    huffmanCodes[7] = huff8;
	unsigned char huff9[] = { 0x08, 0x32, 0x71, 0x81 };
    huffmanCodes[8] = huff9;
	unsigned char huff10[] = { 0x15, 0x23, 0x91, 0xA1 };
    huffmanCodes[9] = huff10;
	unsigned char huff11[] = { 0x42, 0xB1, 0xC1, 0xF0 };
    huffmanCodes[10] = huff11;
	unsigned char huff12[] = { 0x16, 0x33, 0x52, 0xD1, 0xF1 };
    huffmanCodes[11] = huff12;
	unsigned char huff13[] = { 0x09, 0x24, 0xE1 };
    huffmanCodes[12] = huff13;
	unsigned char huff14[] = { 0x17, 0x34, 0x43 };
    huffmanCodes[13] = huff14;
	unsigned char huff15[] = { 0x62, 0x25 };
    huffmanCodes[14] = huff15;
	unsigned char huff16[] = { 0x72, 0x53, 0x18, 0x44, 0x82, 0x92, 0xD2 };
    huffmanCodes[15] = huff16;
	
	int totalCodes = 0;
	for (int i = 0; i < 16; i++)
	{
		totalCodes += numCodes[i];
	}
	
	// for ease of representation we should use unsigned chars since we don't want them to be signed (the codes don't have signs!)
	
	unsigned int huffmanbits[totalCodes];
	unsigned char huffmancodesout[totalCodes];
	
	// we begin with a bitstring of 0
	unsigned int currentBitstring = 0;
	int currentCodeNo = 0;
	for (int lengthlevel = 0; lengthlevel < 16; lengthlevel++) {
		
		// for each code in the current bistring length level
		for(int codeindex = 0; codeindex < numCodes[lengthlevel]; codeindex++) {
			huffmanbits[currentCodeNo] = currentBitstring;
			huffmancodesout[currentCodeNo] = huffmanCodes[lengthlevel][codeindex];
			
			printf("Code\t %X \t assigned to bits\t %u \n", huffmancodesout[currentCodeNo], huffmanbits[currentCodeNo]);
			
			currentCodeNo++;
			currentBitstring++; // increment by one to get the next huffman bitstring
		}
		
		// if we are at the end of the level then we multiply the bits by 2
		currentBitstring *= 2;
	}
	
	printf("Finished!");

}
