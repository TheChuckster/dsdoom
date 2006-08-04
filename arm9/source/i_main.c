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
 * DESCRIPTION:
 *      Startup and quit functions. Handles signals, inits the
 *      memory management, then calls D_DoomMain. Also contains
 *      I_Init which does other system-related startup stuff.
 *
 *-----------------------------------------------------------------------------
 */

#include "config.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef WIFI_DEBUG
#include <user_debugger.h>
#endif

#include "doomdef.h"
#include "m_argv.h"
#include "d_main.h"
#include "i_system.h"
#include "i_video.h"
#include "z_zone.h"
#include "lprintf.h"
#include "m_random.h"
#include "doomstat.h"
#include "g_game.h"
#include "m_misc.h"
#include "i_sound.h"
#include "i_main.h"
#include "lprintf.h"
#ifdef USE_SDL
#include "SDL.h"
#endif

/*#include <signal.h>
#include <stdio.h>
#include <stdlib.h>*/

#include "gba_nds_fat.h"
#include <dswifi9.h>
#define		VCOUNT		(*((u16 volatile *) 0x04000006))

#ifdef WIFI_DEBUG
void debug_print_stub(char *string)
{
	printf(string);
}
#endif

//---------------------------------------------------------------------------------
// Dswifi stub functions
void * sgIP_malloc(int size) { return malloc(size); }
void sgIP_free(void * ptr) { free(ptr); }

// sgIP_dbgprint only needed in debug version
void sgIP_dbgprint(char * txt, ...) {	}

// wifi timer function, to update internals of sgIP
void Timer_50ms(void)
{
	Wifi_Timer(50);
}

// notification function to send fifo message to arm7
void arm9_synctoarm7()
{ // send fifo message
	REG_IPC_FIFO_TX=0x87654321;
}

// interrupt handler to receive fifo messages from arm7
void arm9_fifo()
{ // check incoming fifo messages
	u32 value = REG_IPC_FIFO_RX;
	if(value == 0x87654321) Wifi_Sync();
}
//---------------------------------------------------------------------------------

int broken_pipe;

/* Most of the following has been rewritten by Lee Killough
 *
 * I_GetTime
 * killough 4/13/98: Make clock rate adjustable by scale factor
 * cphipps - much made static
 */

int realtic_clock_rate = 100;
static int_64_t I_GetTime_Scale = 1<<24;

static int I_GetTime_Scaled(void)
{
  return (int)( (int_64_t) I_GetTime_RealTime() * I_GetTime_Scale >> 24);
}



static int  I_GetTime_FastDemo(void)
{
  static int fasttic;
  return fasttic++;
}



static int I_GetTime_Error(void)
{
  I_Error("I_GetTime_Error: GetTime() used before initialization");
  return 0;
}



int (*I_GetTime)(void) = I_GetTime_Error;

void I_Init(void)
{
  /* killough 4/14/98: Adjustable speedup based on realtic_clock_rate */
  if (fastdemo)
    I_GetTime = I_GetTime_FastDemo;
  else
    if (realtic_clock_rate != 100)
      {
        I_GetTime_Scale = ((int_64_t) realtic_clock_rate << 24) / 100;
        I_GetTime = I_GetTime_Scaled;
      }
    else
      I_GetTime = I_GetTime_RealTime;

  {
    /* killough 2/21/98: avoid sound initialization if no sound & no music */
    extern boolean nomusicparm, nosfxparm;
    if (!(nomusicparm && nosfxparm))
      I_InitSound();
  }
}

/* cleanup handling -- killough:
 */
static void I_SignalHandler(int s)
{
/*
  char buf[2048];

#ifdef SIGPIPE
  // CPhipps - report but don't crash on SIGPIPE
  if (s == SIGPIPE) {
    fprintf(stderr, "Broken pipe\n");
    broken_pipe = 1;
    return;
  }
#endif
  signal(s,SIG_IGN);  // Ignore future instances of this signal.

  strcpy(buf,"Exiting on signal: ");
  I_SigString(buf+strlen(buf),2000-strlen(buf),s);

  // If corrupted memory could cause crash, dump memory
  // allocation history, which points out probable causes
  if (s==SIGSEGV || s==SIGILL || s==SIGFPE)
    Z_DumpHistory(buf);

  I_Error("I_SignalHandler: %s", buf);*/
}



/* killough 2/22/98: Add support for ENDBOOM, which is PC-specific
 *
 * this converts BIOS color codes to ANSI codes.
 * Its not pretty, but it does the job - rain
 * CPhipps - made static
 */

inline static int convert(int color, int *bold)
{
  if (color > 7) {
    color -= 8;
    *bold = 1;
  }
  switch (color) {
  case 0:
    return 0;
  case 1:
    return 4;
  case 2:
    return 2;
  case 3:
    return 6;
  case 4:
    return 1;
  case 5:
    return 5;
  case 6:
    return 3;
  case 7:
    return 7;
  }
  return 0;
}

/* CPhipps - flags controlling ENDOOM behaviour */
enum {
  endoom_colours = 1,
  endoom_nonasciichars = 2,
  endoom_droplastline = 4
};

unsigned int endoom_mode;

static void PrintVer(void)
{
  char vbuf[200];
  lprintf(LO_INFO,"%s\n",I_GetVersionString(vbuf,200));
}

/* I_EndDoom
 * Prints out ENDOOM or ENDBOOM, using some common sense to decide which.
 * cphipps - moved to l_main.c, made static
 */
static void I_EndDoom(void)
{
  int lump_eb, lump_ed, lump = -1;

  /* CPhipps - ENDOOM/ENDBOOM selection */
  lump_eb = W_CheckNumForName("ENDBOOM");/* jff 4/1/98 sign our work    */
  lump_ed = W_CheckNumForName("ENDOOM"); /* CPhipps - also maybe ENDOOM */

  if (lump_eb == -1)
    lump = lump_ed;
  else if (lump_ed == -1)
    lump = lump_eb;
  else
  { /* Both ENDOOM and ENDBOOM are present */
#define LUMP_IS_NEW(num) (!((lumpinfo[num].source == source_iwad) || (lumpinfo[num].source == source_auto_load)))
    switch ((LUMP_IS_NEW(lump_ed) ? 1 : 0 ) |
      (LUMP_IS_NEW(lump_eb) ? 2 : 0)) {
    case 1:
      lump = lump_ed;
      break;
    case 2:
      lump = lump_eb;
      break;
    default:
      /* Both lumps have equal priority, both present */
      lump = (P_Random(pr_misc) & 1) ? lump_ed : lump_eb;
      break;
    }
  }

  if (lump != -1)
  {
    const char (*endoom)[2] = (void*)W_CacheLumpNum(lump);
    int i, l = W_LumpLength(lump) / 2;

    /* cph - colour ENDOOM by rain */
    int oldbg = -1, oldcolor = -1, bold = 0, oldbold = -1, color = 0;
#ifndef _WIN32
    if (endoom_mode & endoom_nonasciichars)
      /* switch to secondary charset, and set to cp437 (IBM charset) */
      printf("\e)K\016");
#endif /* _WIN32 */

    /* cph - optionally drop the last line, so everything fits on one screen */
    if (endoom_mode & endoom_droplastline)
      l -= 80;
    lprintf(LO_INFO,"\n");
    for (i=0; i<l; i++)
    {
#ifdef _WIN32
      I_ConTextAttr(endoom[i][1]);
#elif defined (DJGPP)
      textattr(endoom[i][1]);
#else
      if (endoom_mode & endoom_colours)
      {
        if (!(i % 80))
        {
          /* reset everything when we start a new line */
          oldbg = -1;
          oldcolor = -1;
          printf("\e[0m\n");
        }
        /* foreground color */
        bold = 0;
        color = endoom[i][1] % 16;
        if (color != oldcolor)
        {
          oldcolor = color;
          color = convert(color, &bold);
          if (oldbold != bold)
          {
            oldbold = bold;
      printf("\e[%cm", bold + '0');
      if (!bold) oldbg = -1;
          }
          /* we buffer everything or output is horrendously slow */
          printf("\e[%dm", color + 30);
          bold = 0;
        }
        /* background color */
        color = endoom[i][1] / 16;
        if (color != oldbg)
        {
          oldbg = color;
          color = convert(color, &bold);
          printf("\e[%dm", color + 40);
        }
      }
#endif
      /* cph - portable ascii printout if requested */
      if (isascii(endoom[i][0]) || (endoom_mode & endoom_nonasciichars))
        lprintf(LO_INFO,"%c",endoom[i][0]);
      else /* Probably a box character, so do #'s */
        lprintf(LO_INFO,"#");
    }
#ifndef _WIN32
    lprintf(LO_INFO,"\b"); /* hack workaround for extra newline at bottom of screen */
    lprintf(LO_INFO,"\r");
    if (endoom_mode & endoom_nonasciichars)
      putchar('\017'); /* restore primary charset */
#endif /* _WIN32 */
    W_UnlockLumpNum(lump);
  }
#ifndef _WIN32
  if (endoom_mode & endoom_colours)
    puts("\e[0m"); /* cph - reset colours */
  PrintVer();
#else /* _WIN32 */
  I_uSleep(3000000); // CPhipps - don't thrash cpu in this loop
#endif /* _WIN32 */
}

static int has_exited;

/* I_SafeExit
 * This function is called instead of exit() by functions that might be called
 * during the exit process (i.e. after exit() has already been called)
 * Prevent infinitely recursive exits -- killough
 */

void I_SafeExit(int rc)
{
  if (!has_exited)    /* If it hasn't exited yet, exit now -- killough */
    {
      has_exited=rc ? 2 : 1;
      exit(rc);
    }
}

void I_Quit (void)
{
  if (!has_exited)
    has_exited=1;   /* Prevent infinitely recursive exits -- killough */

  if (has_exited == 1) {
    I_EndDoom();
    if (demorecording)
      G_CheckDemoStatus();
    M_SaveDefaults ();
  }
}

#ifdef SECURE_UID
uid_t stored_euid = -1;
#endif

#define FRAGMENT_SIZE 2350
u8* stream;

void UpdateSound()
{
	/*I_UpdateSound(NULL, stream, FRAGMENT_SIZE);

	TransferSoundData audiotransfer = {
		stream, // Sample address
		FRAGMENT_SIZE,	// Sample length
		11025,  // Sample rate
		127,	// Volume
		64,	// Panning
		1	// Format
	};
	
	playSound(&audiotransfer);*/
}

void StartWifi()
{
#ifdef WIFI_DEBUG
	set_verbosity(VERBOSE_INFO | VERBOSE_ERROR | VERBOSE_TRACE);
#endif

#ifdef WIFI
	{ // send fifo message to initialize the arm7 wifi
		REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR; // enable & clear FIFO
		
		u32 Wifi_pass= Wifi_Init(WIFIINIT_OPTION_USELED);
		REG_IPC_FIFO_TX=0x12345678;
		REG_IPC_FIFO_TX=Wifi_pass;
   	
		*((volatile u16 *)0x0400010E) = 0; // disable timer3
		
//		irqInit(); 
		irqSet(IRQ_TIMER3, Timer_50ms); // setup timer IRQ
		irqEnable(IRQ_TIMER3);
		irqSet(IRQ_FIFO_NOT_EMPTY, arm9_fifo); // setup fifo IRQ
		irqEnable(IRQ_FIFO_NOT_EMPTY);
   	
		REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ; // enable FIFO IRQ
   	
		Wifi_SetSyncHandler(arm9_synctoarm7); // tell wifi lib to use our handler to notify arm7

		// set timer3
		*((volatile u16 *)0x0400010C) = -6553; // 6553.1 * 256 cycles = ~50ms;
		*((volatile u16 *)0x0400010E) = 0x00C2; // enable, irq, 1/256 clock
		
		while(Wifi_CheckInit()==0) { // wait for arm7 to be initted successfully
			while(VCOUNT>192); // wait for vblank
			while(VCOUNT<192);
		}
	} // wifi init complete - wifi lib can now be used!
	
	iprintf("Connecting via WFC data\n");
	{ // simple WFC connect:
		int i;
		Wifi_AutoConnect(); // request connect
		while(1) {
			i=Wifi_AssocStatus(); // check status
			if(i==ASSOCSTATUS_ASSOCIATED) {
				iprintf("Connected successfully!\n");
				break;
			}
			if(i==ASSOCSTATUS_CANNOTCONNECT) {
				iprintf("Could not connect!\n");
				break;
			}
		}
	} // if connected, you can now use the berkley sockets interface to connect to the internet!
	
#ifdef WIFI_DEBUG
	debugger_connect_tcp(192, 168, 1, 105);	//your IP here
	debugger_init();
	user_debugger_update();
#endif
#endif
}

//int main(int argc, const char * const * argv)
int main(int argc, char **argv)
{
  myargc = argc;
  myargv = argv;

  	POWER_CR=POWER_ALL;
	WAIT_CR=0xe800;
	
	irqInit();	// Enable our quintessential vblank interrupt
	irqSet(IRQ_VBLANK, NULL);
	irqEnable(IRQ_VBLANK);
	
	TIMER0_DATA=0;	// Set up the timer
	TIMER1_DATA=0;
	TIMER0_CR=TIMER_DIV_1024;
	TIMER1_CR=TIMER_CASCADE;
 
	DISPLAY_CR=MODE_5_2D | DISPLAY_BG3_ACTIVE;
	vramSetMainBanks(VRAM_A_MAIN_BG_0x6000000, VRAM_B_MAIN_BG_0x6020000, VRAM_C_SUB_BG, VRAM_D_LCD);
 	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	
	SUB_BG0_CR = BG_MAP_BASE(31);
	BG_PALETTE_SUB[255] = RGB15(31,31,31);
	consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);

	BG3_CR = BG_BMP8_512x512;
    BG3_XDX = (320 * 256)/256; //1 << 8;
    BG3_XDY = 0;
    BG3_YDX = 0;
    BG3_YDY = (200*256)/192; // << 8;
    BG3_CX = 0;
    BG3_CY = 0;
	
	lcdSwap();
	
	iprintf("Survived graphics init.\n");

	if (!FAT_InitFiles())
	{
		iprintf("Unable to initialize media device!\n");
	} else {
		iprintf("FAT_InitFiles(): initialized.\n");
	}
	
	setGenericSound(11025, 127, 64, 1);
	stream = malloc(FRAGMENT_SIZE*3*sizeof(u8));
	
  /* Version info */
  lprintf(LO_INFO,"\n");
  PrintVer();

  /* cph - Z_Close must be done after I_Quit, so we register it first. */
  atexit(Z_Close);
  /*
     killough 1/98:

     This fixes some problems with exit handling
     during abnormal situations.

     The old code called I_Quit() to end program,
     while now I_Quit() is installed as an exit
     handler and exit() is called to exit, either
     normally or abnormally. Seg faults are caught
     and the error handler is used, to prevent
     being left in graphics mode or having very
     loud SFX noise because the sound card is
     left in an unstable state.
  */

  Z_Init();                  /* 1/18/98 killough: start up memory stuff first */

  atexit(I_Quit);/*
#ifndef _DEBUG
  signal(SIGSEGV, I_SignalHandler);
#ifdef SIGPIPE
  signal(SIGPIPE, I_SignalHandler); // CPhipps - add SIGPIPE, as this is fatal
#endif
  signal(SIGTERM, I_SignalHandler);
  signal(SIGFPE,  I_SignalHandler);
  signal(SIGILL,  I_SignalHandler);
  signal(SIGINT,  I_SignalHandler);  // killough 3/6/98: allow CTRL-BRK during init
  signal(SIGABRT, I_SignalHandler);
#endif
*/
  /* cphipps - call to video specific startup code */
  I_PreInitGraphics();

  D_DoomMain ();
  return 0;
}
