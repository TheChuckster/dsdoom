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
#ifndef _buddyblock_h_
#define _buddyblock_h_


#include <nds/jtypes.h>
#include "z_zone.h"
#include "lprintf.h"

void initBuddyBlocks(int startBlock);
void *blockAlloc( u32 size);

void blockFree( void * mem);

#endif // _buddyblock_h_
