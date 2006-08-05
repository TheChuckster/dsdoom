#ifndef _buddyblock_h_
#define _buddyblock_h_


#include <nds/jtypes.h>
#include "z_zone.h"
#include "lprintf.h"

void initBuddyBlocks(int startBlock);
void *blockAlloc( u32 size);

void blockFree( void * mem);

#endif // _buddyblock_h_
