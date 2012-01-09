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

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "jpeghuffmanlib.h"
#include <stdio.h>

#define DEBUG





//void initHuffmanSendBuffer();
//BOOL bufferHuffmanBits(uint16_t huffmanBitsIn, int huffmanBitsLengthIn);
//BOOL attemptHuffmanSend();




//uint16_t* huffmanBits;
//uint8_t* huffmanCodesOut;
//int* huffmanBitsLength;



uint8_t encodedData[100];
int encodedDataIndex = 0;


int main(void){

	DLOG("PC_DEBUG set\n");
	uint8_t* huffmanCodes[16];
	uint8_t numCodes[16] = { 0, 2, 1, 3, 3, 2, 5, 2, 4, 4, 4, 5, 3, 3, 2, 7 };

	uint8_t huff2[] = { 0x01, 0x02 };
    huffmanCodes[1] = huff2;
	uint8_t huff3[] = { 0x03 };
    huffmanCodes[2] = huff3;
	uint8_t huff4[] =  { 0x04, 0x05, 0x11 };
    huffmanCodes[3] = huff4;
	uint8_t huff5[] = { 0x00, 0x06, 0x21 };
    huffmanCodes[4] = huff5;
	uint8_t huff6[] = { 0x12, 0x31 };
    huffmanCodes[5] = huff6;
	uint8_t huff7[] = { 0x07, 0x13, 0x22, 0x41, 0x51 };
    huffmanCodes[6] = huff7;
	uint8_t huff8[] = { 0x14, 0x61 };
    huffmanCodes[7] = huff8;
	uint8_t huff9[] = { 0x08, 0x32, 0x71, 0x81 };
    huffmanCodes[8] = huff9;
	uint8_t huff10[] = { 0x15, 0x23, 0x91, 0xA1 };
    huffmanCodes[9] = huff10;
	uint8_t huff11[] = { 0x42, 0xB1, 0xC1, 0xF0 };
    huffmanCodes[10] = huff11;
	uint8_t huff12[] = { 0x16, 0x33, 0x52, 0xD1, 0xF1 };
    huffmanCodes[11] = huff12;
	uint8_t huff13[] = { 0x09, 0x24, 0xE1 };
    huffmanCodes[12] = huff13;
	uint8_t huff14[] = { 0x17, 0x34, 0x43 };
    huffmanCodes[13] = huff14;
	uint8_t huff15[] = { 0x62, 0x25 };
    huffmanCodes[14] = huff15;
	uint8_t huff16[] = { 0x72, 0x53, 0x18, 0x44, 0x82, 0x92, 0xD2 };
    huffmanCodes[15] = huff16;

	int totalCodes = 0;
	for (int i = 0; i < 16; i++)
	{
		totalCodes += numCodes[i];
	}

	HuffmanBitStringTable huffTable = huffmanBitStringTable_build(huffmanCodes, numCodes);


	// let's try to encode something and then decode it
	uint8_t testDataIn[] = { 0x00, 0x01, 0x03, 0x07, 0x08, 0x04, 0x00, 0x92, 0x00, 0x00, 0x00, 0x00};



	// encoding it is a bit odd, since we are not going to have to deal with byte boundries
	// worst case is 16 bits long codeword which is two bytes

	//////////////////////////////////////////////////////////////////////////
	// ENCODING																//
	//////////////////////////////////////////////////////////////////////////

	// we will try to send a byte for every loop iteration seen below, but if we have any left over we will put it in this the two leftover byte variables
	BOOL bitQueueArray[16 + 8];
	FixedLengthBitQueue bitQueue = fixedLengthBitQueue_new(16 + 8, bitQueueArray);

	for (int i = 0; i < 12; i++)
	{
		// find the corresponding huffman bitstring


		uint16_t bitStringToSend;
		uint8_t bitStringToSendLength;
		huffmanBitStringTable_encode(&huffTable, testDataIn[i], &bitStringToSend, &bitStringToSendLength);

		// now we want to send bits one by one for each code so we enqueue them on the bitqueue
		// we need to pick each bit out
		uint16_t mask = (0x01 << (bitStringToSendLength - 1));

		for(int bitIndex = 0; bitIndex < bitStringToSendLength; bitIndex++) {
			// for each bit
			// first we find the bit value
			BOOL bit = FALSE;
			if(bitStringToSend & (mask >> bitIndex))
				bit = TRUE;

			// now we try to add it to the queue and then try to send the queue, if we cannot add it something has gone wrong with the code,
			// since we should be flushing the queue before we have a chance to run out of room
			BOOL added = FALSE;
			added = fixedLengthBitQueue_enqueue(&bitQueue, bit);
			assert(added == TRUE); // should always be able to be added

			attemptSend(&bitQueue);

		}


	}

	sendAndTerminate(&bitQueue);


	//////////////////////////////////////////////////////////////////////////
	// DECODING																//
	//////////////////////////////////////////////////////////////////////////

	// we will need a bit queue at least 15 + 8 bits long (to cope with having a 15 bits of a 16 bit long bitstring
	// in the queue and then having another byte added)

	BOOL decoderBitQueueArray[15 + 8];
	FixedLengthBitQueue decoderBitQueue = fixedLengthBitQueue_new(15 + 8, decoderBitQueueArray);
	HuffmanDecoderState decoderState = huffmanDecoder_new(&huffTable, &decoderBitQueue);

	for (int k = 0; k < encodedDataIndex; k++)
	{
		uint16_t mask = (0x01 << 7);
		for(int bitIndex = 0; bitIndex < 8; bitIndex++) {
			if(encodedData[k] & (mask >> bitIndex)) {
				fixedLengthBitQueue_enqueue(&decoderBitQueue, TRUE);
			} else {
				fixedLengthBitQueue_enqueue(&decoderBitQueue, FALSE);
			}
			uint8_t codeOut = 0x00;
			BOOL codeFound = FALSE;
			codeFound = huffmanDeocder_attemptDecodeSingleCode(&decoderState, &codeOut);
			if (codeFound == TRUE) {
				DLOG("Code %X found!\n", codeOut);
			}


		}
	}

	// we read in new bits one by one and try to decode them as we go
	// we should repeat until the end of the stream:
	//	read in one bit
	//	attempt to decode
	//	if successful then emit the code then continue
	//	if unsuccessful then continue

	// to make it as flexible as possible we should probably have a function which attempts to decode a single code and then
	// call this whenever we want

	}




HuffmanBitStringTable huffmanBitStringTable_build(uint8_t** huffmanCodes, uint8_t* numCodes)
{
	HuffmanBitStringTable huffBitStringTable;
	huffBitStringTable.bitStrings_0_7 = malloc(8 * sizeof(uint8_t*));
	assert(huffBitStringTable.bitStrings_0_7 != NULL);
	huffBitStringTable.bitStrings_7_15 = malloc(8 * sizeof(uint16_t));
	assert(huffBitStringTable.bitStrings_7_15 != NULL);
	//huffBitStringTable.numBitStrings = malloc(16 * sizeof(int));
	//assert(huffBitStringTable.numBitStrings != NULL);
	huffBitStringTable.numBitStrings = numCodes;
	//huffBitStringTable.codes = malloc(16 * sizeof(uint8_t*));
	//assert(huffBitStringTable.codes != NULL);
	huffBitStringTable.codes = huffmanCodes;


	// we begin with a bitstring of 0
	uint16_t currentBitstring = 0;
	for(int lengthlevel = 0; lengthlevel < 16; lengthlevel++) {
		// allocate memory for the bitstrings
		if (numCodes[lengthlevel] > 0) {
			if (lengthlevel < 8) {
				huffBitStringTable.bitStrings_0_7[lengthlevel] = malloc(numCodes[lengthlevel] * sizeof(uint8_t));
				assert(huffBitStringTable.bitStrings_0_7[lengthlevel] != NULL);
			} else {
				huffBitStringTable.bitStrings_7_15[lengthlevel - 8] = malloc(numCodes[lengthlevel] * sizeof(uint16_t));
				assert(huffBitStringTable.bitStrings_7_15[lengthlevel - 8] != NULL);
			}
			//huffBitStringTable.codes[lengthlevel] = malloc(numCodes[lengthlevel] * sizeof(uint8_t));
			//assert(huffBitStringTable.codes[lengthlevel] != NULL);
		}

		// for each code in the current bitstring length level
		for(int codeindex = 0; codeindex < numCodes[lengthlevel]; codeindex++) {
			if (lengthlevel < 8)
			{
				huffBitStringTable.bitStrings_0_7[lengthlevel][codeindex] = (uint8_t)currentBitstring;
				DLOG("Code\t %X \t assigned to bits\t %u \tat length\t %u \n", huffBitStringTable.codes[lengthlevel][codeindex], huffBitStringTable.bitStrings_0_7[lengthlevel][codeindex], lengthlevel + 1);
				DLOG("Bitstring: ");
				printBitString(huffBitStringTable.bitStrings_0_7[lengthlevel][codeindex], lengthlevel + 1);
				DLOG("\n");
			} else {
				huffBitStringTable.bitStrings_7_15[lengthlevel - 8][codeindex] = currentBitstring;
				DLOG("Code\t %X \t assigned to bits\t %u \tat length\t %u \n", huffBitStringTable.codes[lengthlevel][codeindex], huffBitStringTable.bitStrings_7_15[lengthlevel - 8][codeindex], lengthlevel + 1);
				DLOG("Bitstring: ");
				printBitString(huffBitStringTable.bitStrings_7_15[lengthlevel - 8][codeindex], lengthlevel + 1);
				DLOG("\n");
			}
			currentBitstring++; // increment by one to get the next huffman bitstring
		}

		// if we are at the end of the level then we multiply the bits by 2
		currentBitstring *= 2;
	}

	DLOG("Finished!\n");
	return huffBitStringTable;
}

void huffmanBitStringTable_encode(HuffmanBitStringTable* huffTable, uint8_t codeToEncode, uint16_t* bitStringOut, uint8_t* bitStringLengthOut) {
	int bitStringLengthLevel = 0;
	int huffmanCodeIndex = 0;
	BOOL codeFound = FALSE;
	for(; bitStringLengthLevel < 16; bitStringLengthLevel++) {
		huffmanCodeIndex = 0;
		for(; huffmanCodeIndex < huffTable->numBitStrings[bitStringLengthLevel]; huffmanCodeIndex++) {
			if (codeToEncode == huffTable->codes[bitStringLengthLevel][huffmanCodeIndex]) {
				codeFound = TRUE;
				break;
			}
		}
		if (codeFound == TRUE)
			break;
	}

	assert(codeFound == TRUE); // if this is not true then no corresponding bitstring was found for this code which indicates something is wrong!

	#ifdef DEBUG
		if (bitStringLengthLevel < 8)
		{
			DLOG("Encoding code %X with bitstring %u of length %u ", huffTable->codes[bitStringLengthLevel][huffmanCodeIndex], huffTable->bitStrings_0_7[bitStringLengthLevel][huffmanCodeIndex], bitStringLengthLevel + 1);
			printBitString(huffTable->bitStrings_0_7[bitStringLengthLevel][huffmanCodeIndex], bitStringLengthLevel + 1);
			DLOG("\n");
		} else {
			DLOG("Encoding code %X with bitstring %u of length %u ", huffTable->codes[bitStringLengthLevel][huffmanCodeIndex], huffTable->bitStrings_7_15[bitStringLengthLevel - 8][huffmanCodeIndex], bitStringLengthLevel + 1);
			printBitString(huffTable->bitStrings_7_15[bitStringLengthLevel - 8][huffmanCodeIndex], bitStringLengthLevel + 1);
			DLOG("\n");
		}
	#endif

	if (bitStringLengthLevel < 8)
	{
		*bitStringOut = (uint8_t)huffTable->bitStrings_0_7[bitStringLengthLevel][huffmanCodeIndex];
	} else {
		*bitStringOut = huffTable->bitStrings_7_15[bitStringLengthLevel - 8][huffmanCodeIndex];
	}
	*bitStringLengthOut = bitStringLengthLevel + 1;
}

HuffmanDecoderState huffmanDecoder_new(HuffmanBitStringTable* bitStringsTable, FixedLengthBitQueue* queue) {
	HuffmanDecoderState huff;
	huff.currentBitString = 0;
	huff.numCheckedBits = 0;
	huff.queue = queue;
	huff.bitStringsTable = bitStringsTable;
	return huff;
}

// returns true when has decoded a code (and puts it in codeOut), returns false when no code found in queue
// remember - will only try to read ONE code every time it is called, so if you enqueue more than one bit at a time
// in the queue you should do something like while(decodeSingleHuffmanCode(queue, &codeOut) == TRUE) { blah }
// because there may be more than one code contained in that set of multiple bits
// NB: you should NOT dequeue items from the decoders queue in between calls of this function since this could cause it to fail oddly.
// this function is designed to be a efficient so might be confusing. We could search through the queue all the way every time we call
// this function, but then we would be wasting time if we have already searched through it to no avail already, so this function stores the
// the current searched bitstring and bistring length to allow searches to continue from the previous search position when the function is
// called again
BOOL huffmanDeocder_attemptDecodeSingleCode(HuffmanDecoderState* decoderState, uint8_t* codeOut) {

	assert(decoderState->numCheckedBits < decoderState->queue->currentSize);
	assert(decoderState->numCheckedBits < 16); // otherwise we have not found a code in time

	// peek through the queue if/until we find a match, then dequeue those items (if a match is found)

	for (int i = decoderState->numCheckedBits; i < decoderState->queue->currentSize; i++)
	{
		//decoderState->numCheckedBits++;
		decoderState->currentBitString = decoderState->currentBitString << 1; // doesn't matter if we do this at the beginning since the beginning value of 0 won't change from shift
		BOOL currentBit = FALSE;
		fixedLengthBitQueue_peekIndex(decoderState->queue, i, &currentBit);
		if(currentBit == TRUE)
			decoderState->currentBitString = decoderState->currentBitString | 0x01;

		// now for the time consuming bit: we must check against the huffman bitstring list to find a match
		// we need only check against bitstrings of length equal to the number of bits we have read out of the queue (otherwise we
		// should have found our match already)
		for (int j = 0; j < decoderState->bitStringsTable->numBitStrings[i]; j++)
		{
			// check to see if the bitstrings match
			uint16_t bitStringToMatch;
			if (i < 8) {
				bitStringToMatch = (uint16_t)decoderState->bitStringsTable->bitStrings_0_7[i][j];
			} else {
				bitStringToMatch = decoderState->bitStringsTable->bitStrings_7_15[i - 8][j];
			}
			if (bitStringToMatch == decoderState->currentBitString)
			{
				// we have found our matching bitstring!
				*codeOut = decoderState->bitStringsTable->codes[i][j];
				// reset the decoder state
				decoderState->numCheckedBits = 0;
				decoderState->currentBitString = 0x00;
				for (int k = 0; k < i + 1; k++)
				{
					BOOL dequeueSucc = FALSE;
					BOOL dequeueOut = FALSE;
					dequeueSucc = fixedLengthBitQueue_dequeue(decoderState->queue, &dequeueOut) == TRUE;
					assert(dequeueSucc == TRUE);
				}
				return TRUE;
			}

		}

		decoderState->numCheckedBits++;
	}

	return FALSE;

}

/*BOOL matchHuffmanBitString(HuffmanBitStringTable* huffTable, int startfrom, uint16_t bitString, uint16_t bitStringLength, uint8_t* foundCodeOut) {
	assert(startfrom < bitStringLength);
	for (int i = startfrom; i < bitStringLength; i++)
	{
		 for (int j = 0; j < huffTable->numBitStrings[i]; j++)
		 {
			 if (i < 8)
			 {
				 if(bitString == (uint16_t)huffTable->bitStrings_0_7[i][j]) {
					 *foundCodeOut = huffTable->codes[i][j];
					 return TRUE;
				 }
			 } else {
				 if(bitString == huffTable->bitStrings_7_15[i - 8][j]) {
					 *foundCodeOut = huffTable->codes[i][j];
					 return TRUE;
				 }
			 }
		 }
	}
	return FALSE;
}*/

// provide a queueLength and an array of preallocated memory to use
FixedLengthBitQueue fixedLengthBitQueue_new(int queueLength, BOOL* arrayToUse) {
	FixedLengthBitQueue queue;
	queue.currentSize = 0;
	queue.fixedMaxSize = queueLength;
	queue.headIndex = 0;
	queue.items = arrayToUse;
	return queue;
}

// returns true if the item could be added to the queue, false otherwise
BOOL fixedLengthBitQueue_enqueue(FixedLengthBitQueue* queue, BOOL item) {

	// see if the queue is full
	if (queue->currentSize >= queue->fixedMaxSize)
		return FALSE;

	// if not, find the index at which we want to add our item
	int enqueueIndex = (queue->headIndex + queue->currentSize) % queue->fixedMaxSize;

	// add our item at the enqueueIndex
	queue->items[enqueueIndex] = item;

	// add one to the current size of the queue
	queue->currentSize++;

	return TRUE;
}

// gets bit value at the beginning of the queue and removes it, returns false if queue is empty, true otherwise
BOOL fixedLengthBitQueue_dequeue(FixedLengthBitQueue* queue, BOOL* bitOut) {

	// see if the queue is empty
	if (queue->currentSize <= 0)
		return FALSE;

	// if not, put the item at the head of the queue into butOut and remove it from the queue by advancing the headIndex and decrementing the current size
	*bitOut = queue->items[queue->headIndex];
	queue->headIndex = (queue->headIndex + 1) % queue->fixedMaxSize;
	queue->currentSize--;

	return TRUE;
}

// lets us peek at the value at the queue position peekIndex without removing it from the queue
void fixedLengthBitQueue_peekIndex(FixedLengthBitQueue* queue, int peekIndex, BOOL* bitOut) {
	// peekIndex should be referenced from 0 so we need to convert it into the queue array index
	assert(peekIndex < queue->fixedMaxSize); // should not be larger than the max size of the array
	assert(peekIndex < queue->currentSize); // should refer to an item in the queue that actually exists

	*bitOut = queue->items[(queue->headIndex + peekIndex) % queue->fixedMaxSize];
}

BOOL attemptSend(FixedLengthBitQueue* queue) {
	// if we have at least a byte of data to send
	if(queue->currentSize >= 8) {

		uint8_t byteToSend = 0x00;

		uint16_t mask = (0x01 << 7);
		for(int bitIndex = 0; bitIndex < 8; bitIndex++) {
			BOOL currentBit;
			BOOL dequeued = fixedLengthBitQueue_dequeue(queue, &currentBit);

			assert(dequeued == TRUE); // we know this should be true since we have established above that the current size of the queue is at least 8

			if(currentBit == TRUE)
				byteToSend = byteToSend | (mask >> bitIndex);

		}
		DLOG("Byte to send hex: %X bits: ", byteToSend);
		printByte(byteToSend);
		DLOG("\n");

		encodedData[encodedDataIndex] = byteToSend;
		encodedDataIndex++;

		// let's do some recursion (yay!) to try to send as many bytes as possible
		// to be honest this will probably never do anything in our use (might be best to remove it to optimize)
		attemptSend(queue);
		return TRUE; // we have sent at least one byte so return true here

	} else {
		return FALSE;
	}
}

// send the last remaining bytes in the buffer even if there is less than 8 bits
// and terminate with an EOF (end of file marker) and pad with 0's or whatever we decide to use
// NB: EOF NOT ACTUALLY IMPLEMENTED YET
void sendAndTerminate(FixedLengthBitQueue* queue) {
	while(attemptSend(queue) == TRUE); // attempt to send normally until we can't any more
	// we should now have less than 8 bits left in the queue
	uint8_t byteToSend = 0x00;
	uint16_t mask = (0x01 << 7);
	int numBits = queue->currentSize;
	for(int bitIndex = 0; bitIndex < numBits; bitIndex++) {
		BOOL currentBit;
		BOOL dequeued = fixedLengthBitQueue_dequeue(queue, &currentBit);

		assert(dequeued == TRUE); // we know this should be true since we are looping through the current size of the queue

		if(currentBit == TRUE)
			byteToSend = byteToSend | (mask >> bitIndex);

	}
	DLOG("Sending and terminating with byte hex: %X bits: ", byteToSend);
	printByte(byteToSend);
	DLOG("\n");

}

void printBitString(uint16_t bitString, uint8_t length) {
	uint16_t mask = (0x01 << (length - 1));
	for(int bitIndex = 0; bitIndex < length; bitIndex++) {
		if(bitString & (mask >> bitIndex)) {
			DLOG("1");
		} else {
			DLOG("0");
		}
	}
}

void printByte(uint8_t byteToPrint) {
	uint16_t mask = (0x01 << 7);
	for(int bitIndex = 0; bitIndex < 8; bitIndex++) {
		if(byteToPrint & (mask >> bitIndex)) {
			DLOG("1");
		} else {
			DLOG("0");
		}
	}
}

/*
uint8_t currentSendByteBuffer = 0x00;
int currentSendByteBufferLength = 0;
int huffmanSendBufferHeadIndex = 0;
uint16_t huffmanBitsSendBuffer[8];
int huffmanBitsLengthSendBuffer[8];

void initHuffmanSendBuffer() {
	for (int i = 0; i < 8; i++)
	{
		huffmanBitsLengthSendBuffer[i] = 0;
	}
}



BOOL bufferHuffmanBits(uint16_t huffmanBitsIn, int huffmanBitsLengthIn) {
	int posIndex = huffmanSendBufferHeadIndex;
	int i = 0;
	while(TRUE)
	{
		if (huffmanBitsLengthSendBuffer[posIndex] <= 0)
		{
			huffmanBitsSendBuffer[posIndex] = huffmanBitsIn;
			huffmanBitsLengthSendBuffer[posIndex] = huffmanBitsLengthIn;
			DLOG("Bits %u with length %u added to send buffer at position %d\n", huffmanBitsIn, huffmanBitsLengthIn, posIndex);
			break;
		}

		i++;
		if(i >= 8)
			return FALSE;
		posIndex++;
		// loop back round once we reach the end
		if (posIndex >= 8)
			posIndex = 0;
	}

	// try to send as much data as possible
	attemptHuffmanSend();

	return TRUE;
}

// send as much data as possible
BOOL attemptHuffmanSend() {
	// we assume we can only send whole bytes

	// repeat until we cannot send any more
	while(TRUE) {
		// we could probably optimise all this by putting huffmanBitsLengthSendBuffer[huffmanSendBufferHeadIndex] etc in temp variables
		// look at the length of the value at the head of the buffer i.e. value at huffmanSendBufferHeadIndex
		if(huffmanBitsLengthSendBuffer[huffmanSendBufferHeadIndex] <= 0) {
			// if it is 0 then we have run out of things to send so return false
			return FALSE;
		} else {
			// we actually have something
			// NB: the currentSendByteBufferLength should not equal or be above 8 since otherwise it would already have been sent

			// get the number of extra bits needed to fill currentSendByteBuffer
			int extraBits = 8 - currentSendByteBufferLength;

			// rule for our bits and bytes is left to right, i.e. first bit on the left then going to the right
			// we need to add our new bits to rightmost end of this buffer going from left to right
			// i.e. [old bytes|new bytes]

			// so let's get the leftmost extraBits number of bits from our current buffer index and put it into the currentSendByteBuffer so pick out the bits one by one
			// there is probably a more optimal way of doing this then picking the bits out one by one but we can worry about that later if needed
			for (int i = 0; i < extraBits; i++)
			{
				uint16_t currentBit =
				currentSendByteBuffer = currentSendByteBuffer & (huffmanBitsSendBuffer[huffmanSendBufferHeadIndex] & (0x01 << huffmanBitsLengthSendBuffer[huffmanSendBufferHeadIndex] - 1))
			}

		}
	}

}
*/




