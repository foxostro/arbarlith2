Arbarlith 2 
============

Home page: <http://www.gamecreation.org/games/arbarlith-ii>

Controls
--------
Arbarlith 2 can be controlled by the keyboard, a gamepad, or a joystick

Keyboard controls:
	Escape enters the game menu
	F11 takes a screen shot
	F1 enters editor mode
	Arrow keys may be used to move around
	Space Bar performs contextual action
	Left-Control enters the spell selection menu
	Left-Shift casts a spell



Credits
-------
Andrew Fox          Project Leader, Programmer, Level Design
                    (and a few 3D models, textures, animations, misc., etc.)
Michael Collins     Programmer
Tom Cauchois        Programmer (SDL code)
Eric Foote          Level Design
John Nesky          Artist (3D Models, textures, animation)
Michael Hanley      Artist (3D Models, textures, animation)
Ning Sung Lee       Artist (GUI artwork)
Justin Lokey        Artist (3D Models)
Chris DeLeon        Sound Effects
Rachel Berkowitz    Musician
Chelsea Hostetter   Writer



Special Thanks
--------------
Mason McCuskey
XML Property Bags
_Game Programming Tricks of the Trade_

Paul Baker <paul@paulsprojects.net>
Various math utilities
http://www.paulsprojects.net

Steve Rabin
"Real-Time In-Game Profiler"
_Game Programming Gems_

David Gurrea <daveghcom@hotmail.com>
Various textures, used within the acceptable uses outlined on his site:
http://www.davegh.com/

You
Thank you for downloading!
Please visit our website http://www.gamecreation.org and check out
our other projects!



Back story
-----------

(Beginning)

Ever since Mael, the God of Chaos, fell at his hand, the young hero Aelyn was
revered as the champion of Arbarlith, the war between men and Gods.  His home
village prospered, and there was peace for a time.

However, the Chaos Rune that Aelyn had used to defeat Mael was gathering
strength within him.  His psyche, once whole, slowly began to be torn apart as
he tried to contain the captured power of the Chaos God that was within him.

Convinced that he was Mael himself, Aelyn set out from the village once again
to battle the demons of his past...

(After Stage 1)

Aelyn surveyed the wrecked body of the demon beast.  Not convinced of his might
he trudged on through a pathway that had opened up in the old, crumbling rock.

The young warrior found himself in a strange world outside of his valley, and he
marked this as the beginning of his new reign as the new God of Chaos.

(After Stage 2)

Mael's power was so strong in Aelyn that even the old gods took notice.  "It is
unheard of," they said, "that a human should bear the power of a god!"  Not to
be bested by a mere human, the gods took Aelyn to their city in the sky to pass
judgment upon him, but Mael's spirit would not be stifled.

(After Stage 3)

Mael claimed the heavenly throne as the old gods lay dead before him.  The sky
rained fire and the seas boiled and turned to blood.  Even the village Aelyn use
to call home was razed by the conflagration.  The world turned to darkness as
Mael's spirit continued to grow stronger.

Though somewhere, in a miniscule corner of his mind, Aelyn's soul flickered
weakly...

+THE END+

(If you Die)

Aelyn's body, unable to support Mael's spirit any longer, collapsed on the cold
floor as he felt the life that was once in him depart.  His eyes slowly closed
as he felt the power of the Rune converge into a small stone, which he held in
his hand, for some poor hapless traveler to be posessed once more...


-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


Steps to build on Mac OS X
---------------------------
NOTE: Tested on 10.5 for the most part. Until a stable SDL release can build
on 10.6, Snow Leopard will not be supported.
1. Download and build other prerequisites:
	$ pushd bootstrap
	$ ./bootstrap.sh
	$ popd
2. Build the executable:
	$ ./scons
3. Run the game:
	$ ./run_arbarlith2
	
	
Steps to build on Ubuntu Linux
------------------------------
NOTE: Tested on Ubuntu Linux 8.04 and 9.04
1. On Ubuntu Linux, some libraries are needed to build GLEW:
		* libx11
		* libxi
		* libxext
		* libxmu
	Grab these on the command line:
		$ sudo aptitude install libx11-dev libxi-dev libxext-dev libxmu-dev
2. Download and build other prerequisites:
	$ pushd bootstrap
	$ ./bootstrap.sh
	$ popd
3. Build the executable:
	$ ./scons
4. Run the game:
	$ ./run_arbarlith2
	

Steps to build in Visual Studio
-------------------------------
NOTE: Tested in Visual Studio 2008 on Windows 7
1. Get Arbarlith 2 Prerequisites for Windows:
	* Boost <http://softlayer.dl.sourceforge.net/project/boost/boost/1.39.0/boost_1_39_0.tar.bz2>
	* SDL <http://www.libsdl.org/release/SDL-devel-1.2.13-VC8.zip>
	* SDL_mixer <http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-devel-1.2.8-VC8.zip>
	* GLEW <http://softlayer.dl.sourceforge.net/project/glew/glew/1.5.1/glew-1.5.1-win32.zip>
	* DevIL <http://softlayer.dl.sourceforge.net/project/openil/DevIL%20Windows%20SDK/1.7.8/DevIL-SDK-x86-1.7.8.zip>
	For your convenience, the correct versions of these prerequisite files have been zipped up in msvc/prereqs-win32.zip.
	Extract the zip file to "msvc\prereqs-win32\"
2. Copy the *.dll files in "msvc\prereqs-win32\bin" to "redist\bin\"
3. Build the Visual Studio Project "msvc\arbarlith2.sln"
4. Run the game with "msvc\run_arbarlith2.bat"
