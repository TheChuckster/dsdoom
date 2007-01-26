dsdoom 1.1.0
============

dsdoom is a port of PrBoom to the Nintendo DS.
http://www.jefklak.com/Guides/Doom-DS

PrBoom is a version of the classic 3D shoot'em'up game Doom, originally
written by id Software.

See the file AUTHORS in this distribution for a list of authors and
other contributors, and a history of the projects PrBoom is derived
from.

dsdoom is made available under the GNU General Public License. See the
file COPYING included in this distribution for details.


Version changes
---------------

1.1.0
	> Enabled automap mode on the lower DS Screen! stats are displayed 
	  on the lower screen too. Option can be set in config file.
		- Automap set to autofollow
		- Zoom IN/OUT: hold B and press L/R accordingly
	> Implemented Power save mode! Closing the DS will set DS Doom in 
	    - Engine stops running and game will be 'paused'
		- Screen and Sound system powered down to save battery life
		- LED blinks to indicate save mode
	> Disabled LED blinking when playing.
	> On DS Lite, Change Brightness in-game! hold B and press SELECT.
	> Displays loading/saving messages (can take a while)
	> Options/General menu:
		- cheat code option 
		- console mode option
		- swap DS screens option
		- All options can be set manually in CFG!
	> Options menu: fixed Y/N (press select/A to emulate Y key)
	> Exit Doom code, powers off the DS device
	> NDS icon attached (G6, DSX, ... cartridges)
	> Saving: automatically label slot with profile (select a save 
	  slot with SElECT. When the name appears, press SELECT again or A 
	  to confirm - this will save the game.
	> In DOOM II, add regular shotgun to weapon cycling pool
	> Other minor fixes and tweaks

	
1.0.0 (Initial Release)
    > DSDoom loads any Prboom-compatible IWAD file.
    > Single-player is playable at very smooth framerates.
    > Multiplayer network play is possible using DS wifi and prboom_server.exe.
    > Configuration file.
    > Sound effects work with stereo panning.
	

Game data - WADs
----------------

(This section is aimed at people not familiar with Doom and the
data files it uses.)

dsdoom is a game engine - it provides a program to play Doom levels, but
it doesn't include any levels itself. More importantly, you need all the
sounds, sprites, and other graphics that make up the Doom environment.
So to play dsdoom, you need one of the main Doom date files from id
Software - either doom.wad, doom2.wad, tnt.wad or plutonia.wad from one
of the commercial Doom games, or the shareware doom1.wad. This file
is called the IWAD.

If you don't own any of the Doom games, get the doom1.wad from shareware
version of doom on The Dos Games Archive. You won't be able to play most
add-ons.

http://www.dosgamesarchive.com/download/game/7
Mirror: http://www.jefklak.com/Files/Doom

Installation
------------

Simply extract the zip to the root of your favourite media device, dsdoom
has been tested to work on M3:SD and GBAMP so far. Start dsdoom.nds in the
manner used by your media device. Select Standard Game from the initial
menu to start a single player game.


Controls are simple.

Use the directional pad for movement.
Hold the X button to sprint.
The Y button toggles through your weapon inventory.
The A button fires.
The B button is the "use" key for opening doors, etc.
The R and L buttons allow you to strafe.
The Start button brings up the main menu.
The Select button chooses options in the menus.
Hold B and press L or R to zoom in and out on the automap.
Press Start and Select at once to alter console mode.


Configuration
-------------

dsdoom automatically creates a PRDOOM.CFG configuration file, using default 
values. Options set in the setup screen will be saved automatically. 
A couple of DS specific variables can be manually edited:

	gen_screen_swap (0/1) - Setting this to 1 swaps both DS screens
	gen_console_enable (0/1) - Disables automap and shows console window
	gen_cheat_enable (0/1) - Enables Cheats (god, all ammo/keys).

As for now, there are a lot of 'garbage' options which can be altered 
but will not be used in the main game. These are originally used in PrBoom. 
WARNING - don't try to delete one of those entries, dsdoom won't start!


Wifi connection
---------------

To play online using wifi, you're going to want to have a PC with port 5030
opened/forwarded. Run prboom_server.exe with the number of players specified
using the -N command line parameter. There are other parameters for such
things as maps and coop; check out the prboom_server documentation. The
server waits until the given number of players connect and then the game
begins.
 
Now that you have the server running, you will want to set up the DSes. Make
sure that WFC data is set in firmware on the DSes. Next, edit the prboom.cfg
file on the card and change the server "name:port" line under misc. You are
ready to start. Launch dsdoom.nds and choose the network game option in the
initial menu.
