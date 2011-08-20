DS Doom 1.2.1
============

DS Doom is a port of PrBoom to the Nintendo DS.
http://www.dsdoom.com

PrBoom is a version of the classic 3D shoot'em'up game Doom, originally
written by id Software.

See the file AUTHORS in this distribution for a list of authors and
other contributors, and a history of the projects PrBoom is derived
from.

dsdoom is made available under the GNU General Public License. See the
file COPYING included in this distribution for details.


Version changes
---------------
1.2.1
	> restore automap zoom controls
	> prevent freeze on changing SFX volume
	> bring up keyboard for save name editing
	> A & B buttons operate as select & back in menus

1.2.0
	> update code for latest libnds, dswifi and libfat
	> now reads files from nds directory with argv supporting cards.

1.1.2
	> Rebuilt with latest libnds, dswifi and libfat
	> fixed warnings
	> zero terminate user string
	> remove dead code
	> move version info to makefile
	> remove banner to allow booting on Supercard
 
1.1.1
	> replaced old gba_nds_fat code with libfat
	> rebuilt with latest toolchain & support libraries

1.1.0
	> Add Wouter Groeneveld's updates
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

DS Doom is a game engine - it provides a program to play Doom levels, but
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
Now included in this archive

Installation
------------

Simply extract the zip to your favourite media device, dsdoom will now read
files from the directory where you put dsdoom.nds when the launcher supports
the argv protocol. For a launcher that supports argv and works on many devices
see http://devkitpro.org/hbmenu. Select Standard Game from the initial menu to
start a single player game.

Using the latest hbmenu you can use .argv files to pass arguments to DS Doom
which gives you the ability to use any PrBoom compatible PWADs and DEHs.
DS Doom will accept most arguments you can pass to PrBoom but we offer no
guarantees that all of them will work as expected. Here are a few of the
PrBoom arguments you can pass.

WAD Options
       -iwad iwadname
              Specifies the location of the IWAD file, typically  doom.wad  or
              doom2.wad (or doom2f.wad). This tells prboom where the main .wad
              file that came with the version of Doom that you own is.

       -file wad1 ...
              Specifies a list of PWAD files to load in addition to  the  IWAD
              file. PWAD files modify the existing Doom game, by adding levels
              or new sounds or graphics. PWAD files are widely  available  for
              download; try ftp.cdrom.com/pub/idgames for starters.

       -deh deh_file
              Tells PrBoom to load the dehacked patch deh_file.

Game Options
       -loadgame {0,1,2,3,4,5,6,7}
              Instructs PrBoom to load the specified saved game immediately.

       -warp { map | epis level }
              Tells  PrBoom  to  begin  a  new game immediately. For Doom 1 or
              Ultimate Doom, you must specify the episode and level number  to
              begin  at  (epis is 1 for Knee-Deep in the Dead, 2 for Shores of
              Hell, 3 for Inferno, 4 for Ultimate Doom; level is between 1 and
              9). For Doom ][ or Final Doom, you must specify the map to begin
              at, which is between 1 and 2 (0 for German Doom ][).

       -skill n
              Tells PrBoom to begin the game at skill level n (1 for ITYTD,  2
              for  Not  Too Rough, 3 for Hurt Me Plenty, 4 for Ultraviolent, 5
              for Nightmare).

       -respawn
              Tells PrBoom that monsters that die should respawn (come back to
              life) after a while. Not for the inexperienced.

       -fast  Tells  PrBoom  to  make all the monsters move  react faster. Not
              for the inexperienced.

       -nomonsters
              Tells PrBoom to include no monsters in the game.


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

DS Doom automatically creates a PRDOOM.CFG configuration file, using default 
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
