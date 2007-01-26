/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
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
 * DESCRIPTION:  Head up display
 *
 *-----------------------------------------------------------------------------*/

#ifndef __HU_STUFF_H__
#define __HU_STUFF_H__

#include "d_event.h"

/*
 * Globally visible constants.
 */
#define HU_FONTSTART    '!'     /* the first font characters */
#define HU_FONTEND      (0x7f) /*jff 2/16/98 '_' the last font characters */

/* Calculate # of glyphs in font. */
#define HU_FONTSIZE     (HU_FONTEND - HU_FONTSTART + 1)

#define HU_BROADCAST    5

/*#define HU_MSGREFRESH   KEYD_ENTER                                phares */
#define HU_MSGX         0
#define HU_MSGY         0
#define HU_MSGWIDTH     64      /* in characters */
#define HU_MSGHEIGHT    1       /* in lines */

#define HU_MSGTIMEOUT   (4*TICRATE)

//
// Locally used constants, shortcuts.
// Jefklak 21/11/06 - moved defines to header
//
// Ty 03/28/98 -
// These four shortcuts modifed to reflect char ** of mapnamesx[]
#define HU_TITLE  (*mapnames[(gameepisode-1)*9+gamemap-1])
#define HU_TITLE2 (*mapnames2[gamemap-1])
#define HU_TITLEP (*mapnamesp[gamemap-1])
#define HU_TITLET (*mapnamest[gamemap-1])
#define HU_TITLEHEIGHT  1
#define HU_TITLEX 0
//jff 2/16/98 change 167 to ST_Y-1
// CPhipps - changed to ST_TY
// proff - changed to 200-ST_HEIGHT for stretching
#define HU_TITLEY ((200-ST_HEIGHT) - 1 - SHORT(hu_font[0].height))

//jff 2/16/98 add coord text widget coordinates
// proff - changed to SCREENWIDTH to 320 for stretching
#define HU_COORDX (320 - 13*SHORT(hu_font2['A'-HU_FONTSTART].width))
//jff 3/3/98 split coord widget into three lines in upper right of screen
#define HU_COORDX_Y (1 + 0*SHORT(hu_font['A'-HU_FONTSTART].height))
#define HU_COORDY_Y (2 + 1*SHORT(hu_font['A'-HU_FONTSTART].height))
#define HU_COORDZ_Y (3 + 2*SHORT(hu_font['A'-HU_FONTSTART].height))

//jff 2/16/98 add ammo, health, armor widgets, 2/22/98 less gap
#define HU_GAPY 8
#define HU_HUDHEIGHT (6*HU_GAPY)
#define HU_HUDX 2
#define HU_HUDY (200-HU_HUDHEIGHT-1)
#define HU_MONSECX (HU_HUDX)
#define HU_MONSECY (HU_HUDY+0*HU_GAPY)
#define HU_KEYSX   (HU_HUDX)
//jff 3/7/98 add offset for graphic key widget
#define HU_KEYSGX  (HU_HUDX+4*SHORT(hu_font2['A'-HU_FONTSTART].width))
#define HU_KEYSY   (HU_HUDY+1*HU_GAPY)
#define HU_WEAPX   (HU_HUDX)
#define HU_WEAPY   (HU_HUDY+2*HU_GAPY)
#define HU_AMMOX   (HU_HUDX)
#define HU_AMMOY   (HU_HUDY+3*HU_GAPY)
#define HU_HEALTHX (HU_HUDX)
#define HU_HEALTHY (HU_HUDY+4*HU_GAPY)
#define HU_ARMORX  (HU_HUDX)
#define HU_ARMORY  (HU_HUDY+5*HU_GAPY)

//jff 3/4/98 distributed HUD positions
#define HU_HUDX_LL 2
#define HU_HUDY_LL (200-2*HU_GAPY-1)
// proff/nicolas 09/20/98: Changed for high-res
#define HU_HUDX_LR (320-120)
#define HU_HUDY_LR (200-2*HU_GAPY-1)
// proff/nicolas 09/20/98: Changed for high-res
#define HU_HUDX_UR (320-96)
#define HU_HUDY_UR 2
#define HU_MONSECX_D (HU_HUDX_LL)
#define HU_MONSECY_D (HU_HUDY_LL+0*HU_GAPY)
#define HU_KEYSX_D   (HU_HUDX_LL)
#define HU_KEYSGX_D  (HU_HUDX_LL+4*SHORT(hu_font2['A'-HU_FONTSTART].width))
#define HU_KEYSY_D   (HU_HUDY_LL+1*HU_GAPY)
#define HU_WEAPX_D   (HU_HUDX_LR)
#define HU_WEAPY_D   (HU_HUDY_LR+0*HU_GAPY)
#define HU_AMMOX_D   (HU_HUDX_LR)
#define HU_AMMOY_D   (HU_HUDY_LR+1*HU_GAPY)
#define HU_HEALTHX_D (HU_HUDX_UR)
#define HU_HEALTHY_D (HU_HUDY_UR+0*HU_GAPY)
#define HU_ARMORX_D  (HU_HUDX_UR)
#define HU_ARMORY_D  (HU_HUDY_UR+1*HU_GAPY)

//#define HU_INPUTTOGGLE  't' // not used                           // phares
#define HU_INPUTX HU_MSGX
#define HU_INPUTY (HU_MSGY + HU_MSGHEIGHT*(SHORT(hu_font[0].height) +1))
#define HU_INPUTWIDTH 64
#define HU_INPUTHEIGHT  1

#define key_alt KEYD_RALT
#define key_shift KEYD_RSHIFT
// END

/*
 * Heads up text
 */
void HU_Init(void);
void HU_Start(void);

boolean HU_Responder(event_t* ev);

void HU_Ticker(void);
void HU_Drawer(void);
char HU_dequeueChatChar(void);
void HU_Erase(void);

/* killough 5/2/98: moved from m_misc.c: */

/* jff 2/16/98 hud supported automap colors added */
extern int hudcolor_titl;   /* color range of automap level title   */
extern int hudcolor_xyco;   /* color range of new coords on automap */
/* jff 2/16/98 hud text colors, controls added */
extern int hudcolor_mesg;   /* color range of scrolling messages    */
extern int hudcolor_chat;   /* color range of chat lines            */
/* jff 2/26/98 hud message list color and background enable */
extern int hudcolor_list;   /* color of list of past messages                  */
extern int hud_list_bgon;   /* solid window background for list of messages    */
extern int hud_msg_lines;   /* number of message lines in window up to 16      */
extern int hud_distributed; /* whether hud is all in lower left or distributed */
/* jff 2/23/98 hud is currently displayed */
extern int hud_displayed;   /* hud is displayed */
/* jff 2/18/98 hud/status control */
extern int hud_active;      /* hud mode 0=off, 1=small, 2=full          */
extern int hud_nosecrets;   /* status does not list secrets/items/kills */

#endif
