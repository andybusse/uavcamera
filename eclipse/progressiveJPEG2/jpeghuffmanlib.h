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
 * jpeghuffmanlib.h
 *
 * Created: 08/11/2011 19:52:20
 *  Author: mh23g08
 */ 


#ifndef JPEGHUFFMANLIB_H_
#define JPEGHUFFMANLIB_H_

#define BOOL int
#define FALSE 0
#define TRUE 1

#define PLATFORM_PC
#define DEBUG

#ifdef DEBUG
	#ifdef PLATFORM_PC
		#define DLOG(...)  printf(__VA_ARGS__)
	#elif defined PLATFORM_PURE_AVR
		#define DLOG(...)
	#elif PLATFORM_ARDUINO
		#define DLOG(...)  
	#else
		#define DLOG(...)
	#endif
#else
	#define DLOG(...)
#endif

typedef struct FixedLengthBitQueue_t {
	int fixedMaxSize;
	int headIndex;
	int currentSize;
	BOOL* items; // items[0] does NOT always correspond to the first item in the queue, use fixedLengthBitQueue_peekIndex(...) if you want to peek at an item!
} FixedLengthBitQueue;

FixedLengthBitQueue fixedLengthBitQueue_new(int queueLength, BOOL* arrayToUse);
BOOL fixedLengthBitQueue_enqueue(FixedLengthBitQueue* queue, BOOL item);
BOOL fixedLengthBitQueue_dequeue(FixedLengthBitQueue* queue, BOOL* bitOut);
void fixedLengthBitQueue_peekIndex(FixedLengthBitQueue* queue, int peekIndex, BOOL* bitOut);

BOOL attemptSend(FixedLengthBitQueue* queue);
void sendAndTerminate(FixedLengthBitQueue* queue);


typedef struct HuffmanBitStringTable_t {
	uint8_t** bitStrings_0_7; // aagggghhh!
	uint16_t** bitStrings_7_15; // we only need two bytes for bitstrings above length 8
	uint8_t* numBitStrings;
	uint8_t** codes;
} HuffmanBitStringTable;


typedef struct HuffmanDecoderState_t {
	uint16_t currentBitString;
	int numCheckedBits;
	FixedLengthBitQueue* queue;
	HuffmanBitStringTable* bitStringsTable;
} HuffmanDecoderState;


HuffmanDecoderState huffmanDecoder_new(HuffmanBitStringTable* bitStringsTable, FixedLengthBitQueue* queue);
BOOL huffmanDeocder_attemptDecodeSingleCode(HuffmanDecoderState* decoderState, uint8_t* codeOut);

//BOOL matchHuffmanBitString(HuffmanBitStringTable huffTable, int startfrom, uint16_t bitString, uint16_t bitStringLength, uint8_t* foundCodeOut);

HuffmanBitStringTable huffmanBitStringTable_build(uint8_t** huffmanCodes, uint8_t* numCodes);
void huffmanBitStringTable_encode(HuffmanBitStringTable* huffTable, uint8_t codeToEncode, uint16_t* bitStringOut, uint8_t* bitStringLengthOut);

void printBitString(uint16_t bitString, uint8_t length);
void printByte(uint8_t byteToPrint);




#endif /* JPEGHUFFMANLIB_H_ */
