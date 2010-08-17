/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Modified to use SDL windowing and get rid of ControlData February 2006
by Tom Cauchois
E-Mail: mailto:tcauchoi@andrew.cmu.edu

Copyright (c) 2003-2007,2009,2010 Game Creation Society
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Game Creation Society nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE Game Creation Society ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE Game Creation Society BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "stdafx.h"
#include "gl.h"
#include "SDLwindow.h"
#include "EffectManager.h"
#include "Application.h"
#include "WaitScreen.h"
#include "SplashScreen.h"
#include "world.h"
#include "Md3Loader.h"

#include "ScreenShotTask.h"
#include "EditorKeyDetector.h"
#include "MenuKeyDetector.h"
#include "SpellMenuKeyDetector.h"
#include "DebugDisplayToggleKeyDetector.h"
#include "FPSDisplayToggleKeyDetector.h"

#include "GameStateRun.h"
#include "GameStateEditor.h"
#include "GameStateMenu.h"
#include "GameStateSpellMenu.h"
#include "GameStateCredits.h"
#include "Controller.h"

#include "profile.h"
#include "PeriodicCallbackTask.h"

/* Include the DevIL headers */
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

namespace Engine {

Application* g_pApplication = 0; // TODO: remove the global Application object

Application::Application(void)
: defaultSetupFileName("setup.xml")
{
	clear();
}

Application::~Application(void)
{
	stop();
}

void Application::loadFonts(void)
{
	fontSmall.setup("data/fonts/proggy.xml");
	fontLarge.setup("data/fonts/narkism.xml");
}

void Application::start(void)
{
	PROFILE;

    TRACE("Starting application...");

    /* Preferably, the share directory is specified in an environment variable.
	 * We'll set this to our working directory.
	 * XXX: We don't want to write files to the share directory!
	 */
	{
		const char * share;

		share = getenv("ARBARLITH2_SHARE");

		if(!share) {
			setWorkingDirectory(getApplicationDirectory());
		} else {
			setWorkingDirectory(share);
		}
	}

	// Parse the setup files
	loadXmlConfigFiles();

	// Initialize APIs
	startOpenGL();
	startDevIL();

	// Create the effect manager
	EffectManager::GetSingleton().create();

	// Load the font setup file
	loadFonts();

	// Load the key bindings
	TRACE("Loading controller bindings...");
	new Controller();
	TRACE("...Loaded controller bindings");

	// Seed the random number generator
	TRACE("Seeding the random number generator...");
	srand(SDL_GetTicks());
	TRACE("...Seeded the random number generator");

	// Do the splash screen
	{
		TRACE("Doing splash screen...");
		SplashScreen splashScreen;
		splashScreen.doSplash(5000);
		TRACE("...Finished with splash screen");
	}

	// Initialize the wait screen
	new WaitScreen();
	g_WaitScreen.Render();
	TRACE("Wait screen started.");

	// Create the frame timer
	TRACE("Creating the frame timer...");
	fme = new NeHe::Frame();
	TRACE("...Created the frame timer");

	// Create the sound manager
	soundSystem = new SoundSystem();
	soundSystem->create();
	TRACE("Sound system initialized");

	// Prepare handlers for various key press events
	addTask(new ScreenShotTask);
	TRACE("Started screen shot task");

	addTask(new EditorKeyDetector);
	TRACE("Started editor hotkey task");

	addTask(new MenuKeyDetector);
	TRACE("Started menu hotkey task");

	addTask(new SpellMenuKeyDetector);
	TRACE("Started spell-menu hotkey task");

	addTask(new DebugDisplayToggleKeyDetector(*this));
	TRACE("Started debug-info hotkey task");

	addTask(new FPSDisplayToggleKeyDetector);
	TRACE("Started FPS-display hotkey task");


	// Create a task to handle the GUI
	new WidgetManager;
	TRACE("Started the game GUI task");
	g_WaitScreen.Render();

	// Start the profiler system
#ifdef ENABLE_PROFILER
	Profiler::ProfileInit();
	addTask(makePeriodicCallbackTask(500.0f,
                                     &Profiler::ProfileDumpOutputToBuffer));
	TRACE("Started the game profiler task");
	g_WaitScreen.Render();
#endif

	// set up the game states
	TRACE("Creating game state objects...");
	{
		states[GAME_STATE_RUN] = new GameStateRun(*this);
		TRACE("...created GameStateRun...");

		states[GAME_STATE_EDITOR] = new GameStateEditor(*this);
		TRACE("...created GameStateEditor...");

		states[GAME_STATE_MENU] = new GameStateMenu(*this);
		TRACE("...created GameStateMenu...");

		states[GAME_STATE_SPELL_MENU] = new GameStateSpellMenu(*this);
		TRACE("...created GameStateSpellMenu...");

		states[GAME_STATE_CREDITS] = new GameStateCredits(*this);
		TRACE("...created GameStateCredits...");
	}
	TRACE("...finished (Creating game state objects)");

	TRACE("Entering the menu game state...");
	gameState = GAME_STATE_MENU;
	states[GAME_STATE_MENU]->onEnter();
	TRACE("...finished (Entering the menu game state)");

	// Complete
	TRACE("...Finished starting up");
	g_WaitScreen.Render();
}

void Application::run(void)
{
	TRACE("Running");
	g_WaitScreen.Render();

	while(!g_Input.Quit)
	{
		GameState *state = states[gameState];

		ASSERT(0!=fme, "fme was null");
		ASSERT(0!=state, "state was null");

		// update the frame timer
		fme->Update();
		float frameLength = (float)min(fme->getLength(), (unsigned int)70);

		// Update the current game state
		state->update(frameLength);

		// update all tasks
		for(std::list<Task*>::const_iterator i=tasks.begin();
            i!=tasks.end(); ++i)
		{
			updateTask(*i, frameLength);
		}

		// Take out the garbage
        // TODO: this really only needs to be run periodically?
		tasks = pruneDeadTasks(tasks);

		// pump
		glFlush();
		g_Window.Flip();
		g_Input.Pump();

		// check for window resizing
		if(g_Input.ResizePending())
		{
			g_Window.Resize(g_Input.ResizeWidth(), g_Input.ResizeHeight());
		}
	}
}

list<Task*> Application::pruneDeadTasks(list<Task*> tasks)
{
	list<Task*>::iterator iter = tasks.begin();

	while(iter != tasks.end())
	{
		if(static_cast<Task*>(*iter)->dead)
		{
			delete(*iter);
			iter = tasks.erase(iter);
		}
		else
			iter++;
	}

	return tasks;
}

void Application::stop(void)
{
	TRACE("Shutting down application...");
	g_WaitScreen.Render();

    for(list<Task*>::iterator iter = tasks.begin(); iter != tasks.end(); ++iter)
        delete(*iter);
    tasks.clear();
    TRACE("Deleted tasks");

    delete soundSystem;
    TRACE("Destroyed the sound system");

    g_GUI.Destroy();
    TRACE("Destroyed the GUI");

    OpenGL::Destroy();
    TRACE("Destroyed the OpenGL context");

    Controller::Destroy();
    TRACE("Destroyed the key binding manager");

    WaitScreen::Destroy();
    TRACE("Destroyed the Wait screen");

	clear();

	TRACE("...shutdown completed");
}

void Application::clear(void)
{
	unlockedWorld = 1;

	menuMusic = "data/music/Rachel01.mp3";
	soundEnabled = true;

	graphicsMode=LIGHTING_ENABLED;
	useParticleEffects=true;
	useBlurEffects=true;

	textureFilter = 1;
	aniostropy = 4.0f;

	displayDebugData=false;
	displayFPS=false;

	mouseSensitivity=100.0f;

	soundSystem = 0;
	fme = 0;
	world = 0;

	tasks.clear();
}

void Application::startDevIL()
{
	TRACE("Initializing DevIL...");

	TRACE("Linked against IL with IL_VERSION of " + itoa(IL_VERSION));
	TRACE("Linked against ILU with ILU_VERSION of " + itoa(ILU_VERSION));
	TRACE("Linked against ILUT with ILUT_VERSION of " + itoa(ILUT_VERSION));

	// First, check the DevIL image library's version
	if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		FAIL("IL_VERSION_NUM is different than expected!");
	}

	if(ilGetInteger(ILU_VERSION_NUM) < ILU_VERSION)
	{
		FAIL("ILU_VERSION_NUM is different than expected!");
	}

	if(ilGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		FAIL("ILUT_VERSION_NUM is different than expected!");
	}

	/*
	Then, Initialize the DevIL image library
	The docs say this step isn't necessary as long as we use a shared
	library when linking to DevIL.  However, it suggests we do it anyway
	to ease a few sorts of particular problems.
	*/
	ilInit();
	iluInit();
	ilutInit();

	TRACE("...DevIL initialized successfully");
}

void Application::startOpenGL()
{
	PropertyBag xml, window;

	bool fullscreen	= false; // whether to set fullscreen
	int width       = 800;   // dimensions of window
	int height      = 600;   // dimensions of window
	int depth       = 32;    // bits per pixel

	const string setupFileName = pathAppend(getAppDataDirectory(),
                                              defaultSetupFileName);

	if(File::isFileOnDisk(setupFileName))
	{
		xml.loadFromFile(setupFileName);
	}
	else
	{
		xml.loadFromFile(defaultSetupFileName);
	}

	if(xml.exists("window")) {
		xml.get("window", window);
		window.get("width", width);
		window.get("height", height);
		window.get("depth", depth);
		window.get("fullscreen", fullscreen);
	}

	// Create an OpenGL context
	new SDLWindow(*this);
	g_Window.Create("Game Creation Society",
					width,
					height,
					SDLWindow::R8G8B8A8,
					24,
					fullscreen);
	new OpenGL(width, height);
	TRACE("Created OpenGL context and window");

	if(fullscreen)
	{
		SDL_ShowCursor(0);
	}
}

void Application::saveXmlConfigFiles(void)
{
	TRACE("Saving configuration file...");

	PropertyBag BaseBag, FogBag, PerfBag, window;

	// Save the mouse sensitivity value
	BaseBag.add("mouseSensitivity", mouseSensitivity);

	BaseBag.add("unlockedWorld", unlockedWorld);
	BaseBag.add("menuMusic", menuMusic);
	BaseBag.add("soundEnabled", soundEnabled);

	// Save settings
	PerfBag.add("useParticleEffects", useParticleEffects);
	PerfBag.add("useBlurEffects", useBlurEffects);
	PerfBag.add("textureFilter", textureFilter);
	PerfBag.add("aniostropy", aniostropy);

	BaseBag.add("performance", PerfBag);

	// Save the window settings too
	window.add("width", (int)g_Window.GetWidth());
	window.add("height", (int)g_Window.GetHeight());
	window.add("depth", (int)g_Window.GetColorDepth());
	window.add("fullscreen", (bool)g_Window.GetFullscreen());
	BaseBag.add("window", window);

	// We'll save settings to the home directory
    BaseBag.saveToFile(pathAppend(getAppDataDirectory(),
                                  defaultSetupFileName));

	TRACE("...configuration file saved");
}

void Application::loadXmlConfigFiles(void)
{
	PropertyBag BaseBag, FogBag, PerfBag;

	string setupFileName = pathAppend(getAppDataDirectory(),
                                        defaultSetupFileName);

	if(File::isFileOnDisk(setupFileName))
	{
		BaseBag.loadFromFile(setupFileName);
	}
	else
	{
		BaseBag.loadFromFile(defaultSetupFileName);
	}

	BaseBag.get("mouseSensitivity", mouseSensitivity);

	BaseBag.get("unlockedWorld", unlockedWorld);
	BaseBag.get("menuMusic", menuMusic);
	BaseBag.get("soundEnabled", soundEnabled);

	// Load settings
	BaseBag.get("performance", PerfBag);
	PerfBag.get("useParticleEffects", useParticleEffects);
	PerfBag.get("useBlurEffects", useBlurEffects);
	PerfBag.get("textureFilter", textureFilter);
	PerfBag.get("aniostropy", aniostropy);

	if(!supportsAniostropy && textureFilter==2)
		textureFilter = 1;

	TRACE("Config files parsed.");
}

Task* Application::addTask(Task *task)
{
	ASSERT(task!=0, "task was NULL");
	tasks.push_back(task);
	return task;
}

void Application::enterWorld(int worldNum)
{
	ASSERT(worldNum>=0 && worldNum<3,
    "Parameter \'worldNum\' is invalid: " + itoa(worldNum));

	ASSERT(worldNum<=unlockedWorld,
    "Parameter \'worldNum\' specifies a world that is currently locked: #"
    + itoa(worldNum) + " while only worlds #0 through #"
    + itoa(unlockedWorld) + "are unlocked.");

	const string worlds[] =
	{
		"data/zones/World1.xml",
		"data/zones/World2.xml",
		"data/zones/World3.xml"
	};

	const string &worldFileName = worlds[worldNum];

	if(world)
	{
		TRACE("Deleting the old World");
		delete world;
	}

	TRACE("Allocating a new World");
	world = new World;

	TRACE("Starting the game from file: " + worldFileName);
	world->loadFromFile(worldFileName);

	TRACE("Continuing game now");
	changeGameState(GAME_STATE_RUN);
}

void Application::unlockWorld(int worldToUnlock)
{
	if(unlockedWorld < worldToUnlock)
		unlockedWorld = worldToUnlock;

	if(unlockedWorld > 2)
		unlockedWorld = 3;
}

void Application::updateTask(Task *task, float deltaTime)
{
	if(task && !task->dead && !task->paused)
		task->update(deltaTime);
}

Camera& Application::getCamera(void)
{
	return camera;
}

void Application::release(void)
{
	TRACE("Releasing game state resources...");
	g_WaitScreen.Render();

	for(map<GAME_STATE,GameState*>::iterator i = states.begin();
        i != states.end(); ++i)
    {
		(i->second)->release();
    }

	TRACE("...Releasing game world resources...");

	if(world!=0) world->release();
	fontLarge.release();
	fontSmall.release();
    g_TextureMgr.release();

    TRACE("...completely released");
}

void Application::reaquire(void)
{
	g_TextureMgr.reaquire();
	fontLarge.reaquire();
	fontSmall.reaquire();

	TRACE("Reaquiring game world resources");
	g_WaitScreen.Render();

	if(world!=0) world->reaquire();

	TRACE("Reaquiring game state resources");

    for(map<GAME_STATE,GameState*>::iterator i = states.begin();
        i != states.end(); ++i)
    {
        (i->second)->reaquire();
    }
}

void Application::changeGameState(GAME_STATE newState)
{
	if(newState != gameState)
	{
		states[gameState]->onExit();
		gameState = newState;
		states[gameState]->onEnter();
	}
}

} // namespace Engine
