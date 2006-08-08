dsdoom 1.0.0
============

dsdoom is a port of PrBoom to the Nintendo DS.

PrBoom is a version of the classic 3D shoot'em'up game Doom, originally
written by id Software.

See the file AUTHORS in this distribution for a list of authors and
other contributors, and a history of the projects PrBoom is derived
from.

dsdoom is made available under the GNU General Public License. See the
file COPYING included in this distribution for details.

Please see the NEWS file included for changes since the previous version.

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

If you don't own any of the Doom games, get the shareware doom1.wad
from Doomworld's shareware download page. You won't be able to play
most add-ons.
http://www.doomworld.com/shareware

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

Wifi connection

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
