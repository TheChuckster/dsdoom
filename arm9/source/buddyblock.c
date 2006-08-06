/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *  dsdoom based on 
 *
 *  PrBoom a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *
 *  Copyright (C) 2006 by
 *  Dave Murphy
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *      buddy block allocator used for sgIP_malloc
 *
 *
 *-----------------------------------------------------------------------------
 */
#include "buddyblock.h"

typedef struct tag_blockheader_t {
	u8 size;
	u8 free;
	u16 tag;
	struct tag_blockheader_t *nextblock;
	struct tag_blockheader_t *prevblock;
} blockheader_t;

#define MAX_BLOCK 7
#define SMALL_BLOCK 512

blockheader_t *buckets[MAX_BLOCK];
int blocksize[MAX_BLOCK];
void *memBase;
void *startMem;
int topBlock;

void initBuddyBlocks(int startBlock) {

	if (startBlock > MAX_BLOCK)
		I_Error ("buddyblock: inital pool too large\n");

	int i;
	int blockbytes = SMALL_BLOCK;
		
	for ( i=0; i<MAX_BLOCK; i++ ) {
		buckets[i] = 0;
		blocksize[i] = blockbytes;
		blockbytes *= 2; 
	}
//	startMem = malloc(blocksize[startBlock] + blocksize[startBlock] - 1);
//	blockheader_t *alignMem = (blockheader_t*)(((u32)startMem + blocksize[startBlock] - 1) & -blocksize[startBlock]);
	startMem = malloc(blocksize[startBlock]);
	blockheader_t *alignMem = startMem;
	alignMem->size = startBlock;
	alignMem->free = 1;
	alignMem->tag = 0xbeef;
	alignMem->nextblock = 0;
	alignMem->prevblock = 0;
	buckets[startBlock] = alignMem;
	
	memBase = alignMem; 
	topBlock = startBlock;
}

void *blockAlloc( u32 size) {

	int bucket = 0;
	blockheader_t *block1, *block2;

	while ( buckets[bucket] == 0 && bucket < topBlock ) bucket++;
	while ( blocksize[bucket] < (size + sizeof(blockheader_t)) && bucket < topBlock) bucket++;	

	if ( buckets[bucket] == 0 ) {
		
		int i;
		
		for ( i=0; i<MAX_BLOCK; i++ ) {
			int count = 0;
			
			blockheader_t fakeblock, *block = &fakeblock;
			block->nextblock = buckets[i];
			while ( block->nextblock ) count++;
			iprintf("%d blocks of %d bytes\n", count, blocksize[i]);
		}
		I_Error ("buddyblock: Failure trying to allocate %u bytes\n",size);
		return 0;
	}
	while (1) {
		if ( size < (blocksize[bucket]/2) - sizeof(blockheader_t) && bucket >0) {
		
			block1 = buckets[bucket];
			buckets[bucket] = block1->nextblock;
			
			if ( buckets[bucket] )
				buckets[bucket]->prevblock = 0;

			bucket--;
			block2 = (blockheader_t *)(((char *)block1) + blocksize[bucket]);	

			block1->nextblock = block2;
			block1->free = 1;
			block1->prevblock = 0;
			block1->size = bucket;
			block1->tag = 0xbeef;

			block2->prevblock = block1;
			block2->free = 1;
			block2->size = 	bucket;
			block2->nextblock = buckets[bucket];
			block2->tag = 0xbeef;
			
			buckets[bucket] = block1;
			
			if ( 0 != block2->nextblock )
				block2->nextblock->prevblock = block2;

		} else {
			block1 = buckets[bucket];
			buckets[bucket] = block1->nextblock;
			if ( buckets[bucket] )
				buckets[bucket]->prevblock = 0;

			block1->free = 0;
			block1->tag = 0xdead;
			return (void*)(block1+1);			
		}
	}
}

void blockFree( void * mem) {

	if ( mem == 0 ) return;

	blockheader_t *buddy;

	blockheader_t *block = (blockheader_t *)((u32)mem - sizeof(blockheader_t));
	
	if ( block->tag != 0xdead ) 
		I_Error ("buddyblock: freeing corrupted block\naddress = %p\nsize=%d\ntag=%04x\n", block, block->size,block->tag);

	while(1) {
		if ( block->size < topBlock ) {
			u32 offset = (u32)block - (u32)memBase;

			if ( (offset / blocksize[block->size]) & 1)
				buddy = (blockheader_t *)(((u32)block) - blocksize[block->size]);
			else
				buddy = (blockheader_t *)(((u32)block) + blocksize[block->size]);
		
			if ( buddy->free && buddy->size == block->size) {
				if ( buddy == buckets[buddy->size] ) {
					buckets[buddy->size] = buddy->nextblock;
					if ( buckets[buddy->size] )
						buckets[buddy->size]->prevblock = 0;
				} else {
					blockheader_t *prev = buddy->prevblock;
					blockheader_t *next = buddy->nextblock;
					prev->nextblock = next;
					if (next)
						next->prevblock = prev; 
				}
				
				if (block > buddy ) {
					blockheader_t *temp = block;
					block = buddy;
					buddy = temp;
				}
				block->size++;
			} else {
				break;
			}
		} else {
			break;
		}
	}
	block->nextblock = buckets[block->size];
	block->tag = 0xbeef;
	block->free = 1;
	buckets[block->size] = block;
	
	if ( block->nextblock )
		block->nextblock->prevblock = block;
	
}
