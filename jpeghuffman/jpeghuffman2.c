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

#include <stdint.h>

#include <assert.h>

#define BOOL int
#define FALSE 0
#define TRUE 1



//void initHuffmanSendBuffer();
//BOOL bufferHuffmanBits(uint16_t huffmanBitsIn, int huffmanBitsLengthIn);
//BOOL attemptHuffmanSend();



typedef struct FixedLengthBitQueue_t {
	int fixedMaxSize;
	int headIndex;
	int currentSize;
	BOOL* items;	
} FixedLengthBitQueue;

FixedLengthBitQueue fixedLengthBitQueue_new(int queueLength, BOOL* arrayToUse);
BOOL fixedLengthBitQueue_enqueue(FixedLengthBitQueue* queue, BOOL item);
BOOL fixedLengthBitQueue_dequeue(FixedLengthBitQueue* queue, BOOL* bitOut);

BOOL attemptSend(FixedLengthBitQueue* queue);
void sendAndTerminate(FixedLengthBitQueue* queue);

void printBitString(uint16_t bitString, int bitStringLength);
void printByte(uint8_t byteToPrint);

uint16_t* huffmanBits;
uint8_t* huffmanCodesOut;
int* huffmanBitsLength;

int main(void){
	printf("PC_DEBUG set\n");
	uint8_t* huffmanCodes[16];
	int numCodes[16] = { 0, 2, 1, 3, 3, 2, 5, 2, 4, 4, 4, 5, 3, 3, 2, 7 };
	
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
	
	// for ease of representation we should use uint8_ts since we don't want them to be signed (the codes don't have signs!)
	
	huffmanBits = malloc(totalCodes * sizeof(uint16_t));
	huffmanCodesOut = malloc(totalCodes * sizeof(uint8_t));
	huffmanBitsLength = malloc(totalCodes * sizeof(int));
	
	// we begin with a bitstring of 0
	uint16_t currentBitstring = 0;
	int currentCodeNo = 0;
	for (int lengthlevel = 0; lengthlevel < 16; lengthlevel++) {
		
		// for each code in the current bistring length level
		for(int codeindex = 0; codeindex < numCodes[lengthlevel]; codeindex++) {
			huffmanBits[currentCodeNo] = currentBitstring;
			huffmanCodesOut[currentCodeNo] = huffmanCodes[lengthlevel][codeindex];
			huffmanBitsLength[currentCodeNo] = lengthlevel + 1;
			
			printf("Code\t %X \t assigned to bits\t %u \tat length\t %u \n", huffmanCodesOut[currentCodeNo], huffmanBits[currentCodeNo], huffmanBitsLength[currentCodeNo]);
			printf("Bitstring: ");
			printBitString(huffmanBits[currentCodeNo], huffmanBitsLength[currentCodeNo]);
			printf("\n");
			
			currentCodeNo++;
			currentBitstring++; // increment by one to get the next huffman bitstring
		}
		
		// if we are at the end of the level then we multiply the bits by 2
		currentBitstring *= 2;
	}
	
	printf("Finished!\n");


	#ifdef PC_DEBUG
	// let's try to encode something and then decode it
	uint8_t testDataIn[] = { 0x00, 0x01, 0x03, 0x07, 0x08, 0x04, 0x00, 0x92, 0x00, 0x00, 0x00, 0x00};
	
	uint8_t encodedData[100];
	
	// encoding it is a bit odd, since we are not going to have to deal with byte boundries
	// worst case is 16 bits long codeword which is two bytes
	
	// ENCODING
	
	// we will try to send a byte for every loop iteration seen below, but if we have any left over we will put it in this the two leftover byte variables
	BOOL bitQueueArray[16+8];
	FixedLengthBitQueue bitQueue = fixedLengthBitQueue_new(16+8, bitQueueArray);
	
	for (int i = 0; i < 12; i++)
	{
		// find the corresponding huffman bitstring
		int codeIndex = 0;
		for (; codeIndex < totalCodes; codeIndex++)
		{
			if (testDataIn[i] == huffmanCodesOut[codeIndex])
				break;
		}
		printf("Encoding code %X with bitstring %u of length %u ", huffmanCodesOut[codeIndex], huffmanBits[codeIndex], huffmanBitsLength[codeIndex]);
		printBitString(huffmanBits[codeIndex], huffmanBitsLength[codeIndex]);
		printf("\n");
		
		
		// now we want to send bits one by one for each code so we enqueue them on the bitqueue
		// we need to pick each bit out
		uint16_t mask = (0x01 << (huffmanBitsLength[codeIndex] - 1));
		for(int bitIndex = 0; bitIndex < huffmanBitsLength[codeIndex]; bitIndex++) {
			// for each bit
			// first we find the bit value
			BOOL bit = FALSE;
			if(huffmanBits[codeIndex] & (mask >> bitIndex)) 
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

	// we assume the smallest amount we can read in at a time is a byte
	uint8_t lowbyte = 0;
	uint8_t highbyte = 0;
	
	#endif

}



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
		printf("Byte to send hex: %X bits: ", byteToSend);
		printByte(byteToSend);
		printf("\n");
		
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
	printf("Sending and terminating with byte hex: %X bits: ", byteToSend);
	printByte(byteToSend);
	printf("\n");
	
}

void printBitString(uint16_t bitString, int bitStringLength) {			
	uint16_t mask = (0x01 << (bitStringLength - 1));
	for(int bitIndex = 0; bitIndex < bitStringLength; bitIndex++) {
		if(bitString & (mask >> bitIndex)) {
			printf("1");
		} else {
			printf("0");
		}
	}
}	

void printByte(uint8_t byteToPrint) {
	uint16_t mask = (0x01 << 7);
	for(int bitIndex = 0; bitIndex < 8; bitIndex++) {
		if(byteToPrint & (mask >> bitIndex)) {
			printf("1");
		} else {
			printf("0");
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
			printf("Bits %u with length %u added to send buffer at position %d\n", huffmanBitsIn, huffmanBitsLengthIn, posIndex);
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
		// we could probably optimise all this by putting huffmanBitsLengthSendBuffer[huffmanSendBufferHeadIndex] etc in tempoary variables
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
