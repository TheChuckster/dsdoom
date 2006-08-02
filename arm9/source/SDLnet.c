/*
    SDL_net:  An example cross-platform network library for use with SDL
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

/* $Id: SDLnet.c 2207 2006-04-20 16:48:25Z slouken $ */

#include <nds.h>
#include <string.h>

#include "SDLnetsys.h"
#include "SDL_net.h"

#define DEBUG_NET

/* Since the UNIX/Win32/BeOS code is so different from MacOS,
   we'll just have two completely different sections here.
*/
static int SDLNet_started = 0;

#include <signal.h>

static __inline__ u16 SDL_Swap16(u16 x)
{
	return((x<<8)|(x>>8));
} 
 
static __inline__ u32 SDL_Swap32(u32 x) {
	return((x<<24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x>>24));
}

/* Initialize/Cleanup the network API */
int  SDLNet_Init(void)
{
	if ( !SDLNet_started ) {
		/* SIGPIPE is generated when a remote socket is closed */
		void (*handler)(int);
		handler = signal(SIGPIPE, SIG_IGN);
		if ( handler != SIG_DFL ) {
			signal(SIGPIPE, handler);
		}
	}
	++SDLNet_started;
	return(0);
}
void SDLNet_Quit(void)
{
	if ( SDLNet_started == 0 ) {
		return;
	}
	if ( --SDLNet_started == 0 ) {
		/* Restore the SIGPIPE handler */
		void (*handler)(int);
		handler = signal(SIGPIPE, SIG_DFL);
		if ( handler != SIG_IGN ) {
			signal(SIGPIPE, handler);
		}
	}
}

/* Resolve a host name and port to an IP address in network form */
int SDLNet_ResolveHost(IPaddress *address, const char *host, u16 port)
{
	int retval = 0;

	// Perform the actual host resolution
	if ( host == NULL ) {
		address->host = INADDR_ANY;
	} else {
		address->host = inet_addr(host);
		if ( address->host == INADDR_NONE ) {
			struct hostent *hp;

			hp = gethostbyname(host);
			if ( hp ) {
				memcpy(&address->host, hp->h_addr_list[0], hp->h_length);
			} else {
				retval = -1;
			}
		}
	}
	address->port = SDL_SwapBE16(port);
	//address->port = port;

	/* Return the status */
	return(retval);
}

/* Resolve an ip address to a host name in canonical form.
   If the ip couldn't be resolved, this function returns NULL,
   otherwise a pointer to a static buffer containing the hostname
   is returned.  Note that this function is not thread-safe.
*/
/* Written by Miguel Angel Blanch.
 * Main Programmer of Arianne RPG.
 * http://come.to/arianne_rpg
 */
const char *SDLNet_ResolveIP(IPaddress *ip)
{
/*	struct hostent *hp;

	hp = gethostbyaddr((char *)&ip->host, 4, AF_INET);
	if ( hp != NULL ) {
		return hp->h_name;
	}*/
  	return NULL;
}
