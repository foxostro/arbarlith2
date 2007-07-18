/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Modified to use SDL windowing and get rid of ControlData February 2006 by Tom Cauchois
E-Mail: mailto:tcauchoi@andrew.cmu.edu

Copyright © 2003-2007 Game Creation Society
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
#include "World.h"
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


/*
Include the DevIL headers
*/
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

namespace Engine {

Application::Application(void)
: defaultSetupFileName(_T("setup.xml"))
{
	clear();
	start();
	run();
}

Application::~Application(void)
{
	stop();
}

void Application::loadFonts(void)
{
	fontSmall.setup(_T("data/fonts/proggy.xml"));
	fontLarge.setup(_T("data/fonts/narkism.xml"));
}

void Application::start(void)
{
	PROFILE

	TRACE(_T("Starting application..."));

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
	new Controller();

	// Initialize the splash screen
	TRACE(_T("Loading splash screen..."));
	new SplashScreen();
	g_SplashScreen.doSplash(5000);
	TRACE(_T("...Completed loading splash screen"));

	// Initialize the wait screen
	new WaitScreen();
	g_WaitScreen.Render();
	TRACE(_T("Wait screen started."));

	// Create the frame timer
	fme = new NeHe::Frame;

	// Create the sound manager
	addTask(soundSystem = new SoundSystem);
	g_WaitScreen.Render();
	TRACE(_T("Sound system initialized"));


	// Prepare handlers for various key press events
	addTask(new ScreenShotTask);
	TRACE(_T("Started screen shot task"));

	addTask(new EditorKeyDetector);
	TRACE(_T("Started editor hotkey task"));

	addTask(new MenuKeyDetector);
	TRACE(_T("Started menu hotkey task"));

	addTask(new SpellMenuKeyDetector);
	TRACE(_T("Started spell-menu hotkey task"));

	addTask(new DebugDisplayToggleKeyDetector(*this));
	TRACE(_T("Started debug-info hotkey task"));

	addTask(new FPSDisplayToggleKeyDetector);
	TRACE(_T("Started FPS-display hotkey task"));


	// Create a task to handle the GUI
	new WidgetManager;
	TRACE(_T("Started the game GUI task"));
	g_WaitScreen.Render();

	// Start the profiler system
#ifdef ENABLE_PROFILER
	Profiler::ProfileInit();
	addTask(makePeriodicCallbackTask(500.0f, &Profiler::ProfileDumpOutputToBuffer));
	TRACE(_T("Started the game profiler task"));
	g_WaitScreen.Render();
#endif

	// set up the game states
	TRACE(_T("Creating game state objects..."));
	{
		states[GAME_STATE_RUN] = new GameStateRun(*this);
		TRACE(_T("...created GameStateRun..."));

		states[GAME_STATE_EDITOR] = new GameStateEditor(*this);
		TRACE(_T("...created GameStateEditor..."));

		states[GAME_STATE_MENU] = new GameStateMenu(*this);
		TRACE(_T("...created GameStateMenu..."));

		states[GAME_STATE_SPELL_MENU] = new GameStateSpellMenu(*this);
		TRACE(_T("...created GameStateSpellMenu..."));

		states[GAME_STATE_CREDITS] = new GameStateCredits(*this);
		TRACE(_T("...created GameStateCredits..."));
	}
	TRACE(_T("...finished (Creating game state objects)"));

	TRACE(_T("Entering the menu game state..."));
	gameState = GAME_STATE_MENU;
	states[GAME_STATE_MENU]->onEnter();
	TRACE(_T("...finished (Entering the menu game state)"));

	// Complete
	TRACE(_T("...Finished starting up"));
	g_WaitScreen.Render();
}

void Application::run(void)
{
	TRACE(_T("Running"));
	g_WaitScreen.Render();

	while(!g_Input.Quit)
	{
		GameState *state = states[gameState];

		ASSERT(0!=fme, _T("fme was null"));
		ASSERT(0!=state, _T("state was null"));

		// update the frame timer
		fme->Update();
		float frameLength = fme->GetLength()>500.0f ? 500.0f : fme->GetLength();

		// Update the current game state
		state->update(frameLength);

		// update all tasks
		for(std::list<Task*>::const_iterator i=tasks.begin(); i!=tasks.end(); ++i)
		{
			ASSERT(0!=*i, _T("*i was null"));
			updateTask(*i, frameLength);
		}

		// Take out the garbage
		tasks = pruneDeadTasks(tasks); // TODO: this really only needs to be run periodically?

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
	TRACE(_T("Shutting down application..."));
	g_WaitScreen.Render();


	// Delete all tasks
	for_each(tasks.begin(), tasks.end(), bind(delete_arg(), _1));
	tasks.clear();
	g_WaitScreen.Render();


	// Destroy the GUI
	g_GUI.Destroy();


	// Destroy the OpenGL window
	OpenGL::Destroy();
	g_WaitScreen.Render();


	// Delete the key binding manager
	Controller::Destroy();
	g_WaitScreen.Render();


	// Delete the waitscreen
	WaitScreen::Destroy();


	// Finish it up
	clear();

	TRACE(_T("...shutdown completed"));
}

void Application::clear(void)
{
	unlockedWorld = 1;

	menuMusic = _T("data/music/Rachel01.mp3");
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
	TRACE(_T("Initializing DevIL..."));

	// First, check the DevIL image library's version
	if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		FAIL(_T("IL_VERSION_NUM version is different than expected!"));
	}

	if(ilGetInteger(ILU_VERSION_NUM) < ILU_VERSION)
	{
		FAIL(_T("IL_VERSION_NUM version is different than expected!"));
	}

	if(ilGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		FAIL(_T("IL_VERSION_NUM version is different than expected!"));
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

	TRACE(_T("...DevIL initialized successfully"));
}

void Application::startOpenGL()
{
	PropertyBag xml, window;



	bool fullscreen	= false; // whether to set fullscreen
	int width       = 800;   // dimensions of window
	int height      = 600;   // dimensions of window
	int depth       = 32;    // bits per pixel




	// Load the setup file
	const _tstring setupFileName = pathAppend(getAppDataDirectory(), defaultSetupFileName);

	if(File::isFileOnDisk(setupFileName))
	{
		xml.loadFromFile(setupFileName);
	}
	else
	{
		xml.loadFromFile(defaultSetupFileName);
	}



	// Get window properties
	if(xml.getSym(window))
	{
		window.getSym(width);
		window.getSym(height);
		window.getSym(depth);
		window.getSym(fullscreen);
	}




	// Create an instance of OpenGL
	new SDLWindow(*this);
	g_Window.Create(_T("Game Creation Society"),
					width,
					height,
					SDLWindow::R8G8B8A8,
					24,
					fullscreen);
	new OpenGL(width, height);
	TRACE(_T("Created OpenGL context and window"));



	if(fullscreen)
	{
		// Hide the mouse cursor
		SDL_ShowCursor(0);
	}
}

void Application::saveXmlConfigFiles(void)
{
	// Load the engine setup file
	PropertyBag BaseBag, FogBag, PerfBag, window;

	// Save the mouse sensitivity value
	BaseBag.addSym(mouseSensitivity);

	BaseBag.addSym(unlockedWorld);
	BaseBag.addSym(menuMusic);
	BaseBag.addSym(soundEnabled);

	// Save settings
	PerfBag.addSym(useParticleEffects);
	PerfBag.addSym(useBlurEffects);
	PerfBag.addSym(textureFilter);
	PerfBag.addSym(aniostropy);

	BaseBag.add(_T("performance"), PerfBag);

	// Save the window settings too
	window.add(_T("width"), (int)g_Window.GetWidth());
	window.add(_T("height"), (int)g_Window.GetHeight());
	window.add(_T("depth"), (int)g_Window.GetColorDepth());
	window.add(_T("fullscreen"), (bool)g_Window.GetFullscreen());
	BaseBag.addSym(window);

	// We'll save settings to the home directory
	_tstring setupFileName = pathAppend(getAppDataDirectory(), defaultSetupFileName);
	BaseBag.saveToFile(setupFileName);

	TRACE(_T("Config files saved"));
}

void Application::loadXmlConfigFiles(void)
{
	// Load the engine setup file
	PropertyBag BaseBag, FogBag, PerfBag;



	// Load the setup file
	_tstring setupFileName = pathAppend(getAppDataDirectory(), defaultSetupFileName);

	if(File::isFileOnDisk(setupFileName))
	{
		BaseBag.loadFromFile(setupFileName);
	}
	else
	{
		BaseBag.loadFromFile(defaultSetupFileName);
	}

	BaseBag.getSym(mouseSensitivity);

	BaseBag.getSym(unlockedWorld);
	BaseBag.getSym(menuMusic);
	BaseBag.getSym(soundEnabled);

	// Load settings
	BaseBag.get(_T("performance"), PerfBag);
	PerfBag.getSym(useParticleEffects);
	PerfBag.getSym(useBlurEffects);
	PerfBag.getSym(textureFilter);
	PerfBag.getSym(aniostropy);

	if(!supportsAniostropy && textureFilter==2)
		textureFilter = 1;

	TRACE(_T("Config files parsed."));
}

Task* Application::addTask(Task *task)
{
	ASSERT(task!=0, _T("cannot add a null pointer for a task. (OK to continue)"));
	if(task) tasks.push_back(task);
	return task;
}

void Application::enterWorld(int worldNum)
{
	ASSERT(worldNum>=0 && worldNum<3, _T("Parameter \'worldNum\' is invalid: ") + itoa(worldNum));
	ASSERT(worldNum<=unlockedWorld,   _T("Parameter \'worldNum\' specifies a world that is currently locked: #") + itoa(worldNum)
		                        + _T(" while only worlds #0 through #") + itoa(unlockedWorld) + _T("are unlocked."));

	const _tstring worlds[] =
	{
		_T("data/zones/World1.xml"),
		_T("data/zones/World2.xml"),
		_T("data/zones/World3.xml")
	};

	const _tstring &worldFileName = worlds[worldNum];

	TRACE(_T("Starting the game from file: ") + worldFileName);

	delete world;
	world = new World;
	world->loadFromFile(worldFileName);

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
	TRACE(_T("Releasing game state resources"));
	g_WaitScreen.Render();

	for(map<GAME_STATE,GameState*>::iterator i = states.begin(); i != states.end(); ++i)
		(i->second)->release();

	TRACE(_T("Releasing game world resources"));
	g_WaitScreen.Render();

	if(world!=0)
		world->release();

	fontLarge.release();
	fontSmall.release();

	g_TextureMgr.release();
}

void Application::reaquire(void)
{
	g_TextureMgr.reaquire();

	fontLarge.reaquire();
	fontSmall.reaquire();

	TRACE(_T("Reaquiring game world resources"));
	g_WaitScreen.Render();

	if(world!=0)
		world->reaquire();

	TRACE(_T("Reaquiring game state resources"));
	g_WaitScreen.Render();

	for(map<GAME_STATE,GameState*>::iterator i = states.begin(); i != states.end(); ++i)
		(i->second)->reaquire();
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

}; // namespace
