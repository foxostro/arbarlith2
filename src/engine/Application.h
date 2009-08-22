/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Modified to use SDL windowing and get rid of ControlData
February 2006 by Tom Cauchois
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

#ifndef APP_H
#define APP_H

#include "myassert.h"
#include "text.h"
#include "opengl.h"
#include "task.h"
#include "widgetmanager.h"
#include "SoundSystem.h"
#include "TextureManager.h"
#include "camera.h"

namespace Engine {

/** Game states */
enum GAME_STATE
{
	GAME_STATE_RUN,
	GAME_STATE_EDITOR,
	GAME_STATE_MENU,
	GAME_STATE_SPELL_MENU,
	GAME_STATE_CREDITS
};

// Class Prototypes
class SDLWindow;
class GameState;
class Actor;
class World;

/**
The Application class may be considered the entry point of the application.
While the main or WinMain functions (depending on the OS we are compiling for)
are used to enter the application from the Operating System kernel, the only
purpose they serve is to immediately instantiate an Application type object.
*/
class Application
{
public:
	/** Default Constructor */
	Application(void);

	/** Destructor */
	virtual ~Application(void);

	/**
	Allows access to the sound system.
	@return Returns a pointer to the sound system.
	*/
	SoundSystem& getSoundSystem(void)
	{
		ASSERT(soundSystem!=0, "soundSystem was NULL");
		return(*soundSystem);
	}

	/**
	Allows access to the texture manager
	@return Returns a pointer to the texture manager
	*/
	TextureManager& getTextureManager(void)
	{
		return(m_Tex);
	}

	/** Run the game loop */
	void run(void);

	/** Initialization code to run before the game loop */
	void start(void);

	/** Shutdown code to run after the game loop exits */
	void stop(void);

	/** Reset the state of the application to a "just-constructed" state */
	void clear(void);

	/**
	Gets the mouse sensitivity setting
	@return mouse sensitivity setting
	*/
	float getMouseSensitivity(void) const
	{
		return mouseSensitivity;
	}

	/** Saves the configuration settings */
	void saveXmlConfigFiles(void);

	/**
	Adds the task to the list of live tasks.
	@param task A game task. The task will be memory managed by the kernel
	@return Handle to the task
	*/
	virtual Task* addTask(Task *task);

	/**
	Gets the frame rate (FPS)
	@return FPS
	*/
	unsigned int getFrameRate(void) const
	{
		ASSERT(fme!=0, "fme was NULL");
		return fme->getFPS();
	}

	/**
	Gets the current game state
	@return Game State
	*/
	GAME_STATE getState(void) const
	{
		return gameState;
	}

	/**
	Enters a new game state
	@param newState the new game state
	*/
	void changeGameState(GAME_STATE newState);

	/**
	Allows access to the camera
	@return Returns a pointer to the camera
	*/
	virtual Camera& getCamera(void);

	/** Release assets */
	virtual void release(void);

	/** Reaquire assets */
	virtual void reaquire(void);

	/**
	Gets the game world
	@return the game world
	*/
	inline World& getWorld(void)
	{
		ASSERT(world!=0, "world was null");
		return(*world);
	}

	/**
	Gets the game world
	@return the game world
	*/
	inline const World& getWorld(void) const
	{
		ASSERT(world!=0, "world was null");
		return(*world);
	}

	/**
	Determines whether a game world has been loaded
	@return true if the world has been loaded
	*/
	inline bool isWorldLoaded(void) const
	{
		return world!=0;
	}

	/**
	Loads the specified game world
	Sets the application into the RUN state so the world may be played
	@param worldNum Starts counting at 0 with world #1
	*/
	void enterWorld(int worldNum);

	/**
	Unlocks the specified game world and all preceding worlds
	@param worldToUnlock Starts counting at 0 with world #1
	*/
	void unlockWorld(int worldToUnlock);

private:
	/** Start OpenGL */
	void startOpenGL(void);

	/** Start DevIL */
	void startDevIL(void);

	/** Load the configuration files for the game */
	void loadXmlConfigFiles(void);

	/** Load the font data files */
	void loadFonts(void);

	/**
	Frees and removes dead tasks from the task queue
	@param tasks The task queue
	@return the updated task list
	*/
	static list<Task*> pruneDeadTasks(list<Task*> tasks);

	/**
	Update a single task
	@param task the task to update
	@param deltaTime The milliseconds since the last update
	*/
	static void updateTask(Task *task, float deltaTime);

public:
	/**
	Indicates the highest numbered world that is currently unlocked.
	Starts counting at 1 with world #1, not zero
	*/
	int unlockedWorld;

	/** true if the sound is not muted */
	bool soundEnabled;

	/** Indicates the music file that we would like to play for the menu */
	string menuMusic;

	/** Performance setting to control the technique for rendering the game world */
	enum
	{
		SHADOWS_AND_LIGHTING_ENABLED,
		LIGHTING_ENABLED,
		SHADOWS_AND_LIGHTING_DISABLED
	} graphicsMode;

	/** Performance setting to control whether particle effects are rendered */
	bool useParticleEffects;

	/**
	Performance setting to control which texture filter to use
	0 - bilinear filter
	1 - trilinear filter
	2 - aniostropic filter
	*/
	int textureFilter;

	/** Indicates that the game may use blur effects */
	bool useBlurEffects;

	/** Performance setting to control the level of aniostropy when aniostropic filtersare used */
	float aniostropy;

	/** Indicates that debug data should be displayed */
	bool displayDebugData;

	/** Indicates that FPS label should be displayed */
	bool displayFPS;

	/** Proggy Font */
	TextWriter fontSmall;

	/** Tahoma Font */
	TextWriter fontLarge;

private:
	/** the frame timer */
	NeHe::Frame *fme;

	/** Sound engine */
	SoundSystem *soundSystem;

	/**	The list of live tasks */
	list<Task*> tasks;

	/** the texture manager */
	TextureManager m_Tex;

	/** Mouse sensitivity setting */
	float mouseSensitivity;

	/** Filename for the settings file */
	string defaultSetupFileName;

	/** The active game state */
	GAME_STATE gameState;

	/** Available game states */
	map<GAME_STATE, GameState*> states;

	/** The camera for the client perspective */
	Camera camera;

	/** the game world */
	World *world;
};

extern Engine::Application* g_pApplication;

} // namespace Engine

// Proxies
#define g_Application   (*Engine::g_pApplication)
#define g_SoundSystem	(Engine::g_pApplication->getSoundSystem())
#define g_TextureMgr	(Engine::g_pApplication->getTextureManager())
#define g_Camera	(Engine::g_pApplication->getCamera())
#define g_Window	(Engine::SDLWindow::GetSingleton())
#define g_Input		(Engine::SDLWindow::GetSingleton().input)


#endif
