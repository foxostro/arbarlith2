# Project:          Arbarlith 2
# Project filename: C:\Documents and Settings\arfox\My Documents\arbarlith2\trunk\arbarlith2.cbp
# Compiler used:    GNU GCC Compiler

### Variables used in this Makefile
default_CC=mingw32-gcc.exe
default_CPP=mingw32-g++.exe
default_LD=mingw32-g++.exe
default_LIB=ar.exe
default_RESCOMP=windres.exe

### Compiler/linker options
default_GLOBAL_CFLAGS=
default_PROJECT_CFLAGS= -Wall
default_GLOBAL_LDFLAGS=
default_PROJECT_LDFLAGS=
default_GLOBAL_INCS= -IC:/Program\ Files/CodeBlocks/include -IC:/Program\ Files/CodeBlocks/devpaks/include -IC:/devel/mingw/include
default_PROJECT_INCS=
default_GLOBAL_LIBDIRS= -LC:/Program\ Files/CodeBlocks/lib -LC:/Program\ Files/CodeBlocks/devpaks/lib -LC:/devel/mingw/lib
default_PROJECT_LIBDIRS=
default_GLOBAL_LIBS=
default_PROJECT_LIBS= -lSDL -lopengl32 -lglu32 -lglew32 -lgdi32 -luser32 -lkernel32 -lwinmm -loleaut32 -lDevIL -lILU -lILUT

### Targets compiler flags
default_CFLAGS= $(default_PROJECT_CFLAGS) -fexpensive-optimizations -O3  $(default_GLOBAL_CFLAGS)

### Targets linker flags
default_LDFLAGS= $(default_PROJECT_LDFLAGS) -s $(default_GLOBAL_LDFLAGS)

### Targets include directories
default_INCS= $(default_PROJECT_INCS) $(default_GLOBAL_INCS)

### Targets library directories
default_LIBDIRS= $(default_PROJECT_LIBDIRS) $(default_GLOBAL_LIBDIRS)

### Targets libraries
default_LIBS= $(default_PROJECT_LIBS) $(default_GLOBAL_LIBS)

###############################################################################
#         You shouldn't need to modify anything beyond this point             #
###############################################################################

### Objects used in this Makefile
default_OBJS=.objs/arbarlith2/Boss.o .objs/arbarlith2/Bullet.o .objs/arbarlith2/DetectorProximityMonster.o .objs/arbarlith2/Explosion.o .objs/arbarlith2/GateOpener.o .objs/arbarlith2/Monster.o .objs/arbarlith2/MyPlayer.o .objs/arbarlith2/Powerup.o .objs/arbarlith2/PowerupHeal.o .objs/arbarlith2/PowerupSpell.o .objs/arbarlith2/Spawn.o .objs/arbarlith2/Spell.o .objs/arbarlith2/SpellCasterFSM.o .objs/arbarlith2/SpellFireBall.o .objs/arbarlith2/SpellHeal.o .objs/arbarlith2/SpellIncinerate.o .objs/arbarlith2/Teleporter.o .objs/arbarlith2/TileGate.o .objs/arbarlith2/engine/3dmath.o .objs/arbarlith2/engine/3dsLoader.o .objs/arbarlith2/engine/ActorFactory.o .objs/arbarlith2/engine/ActorSet.o .objs/arbarlith2/engine/AnimationController.o .objs/arbarlith2/engine/BackdropTexture.o .objs/arbarlith2/engine/Blur.o .objs/arbarlith2/engine/BlurEffect.o .objs/arbarlith2/engine/BoundingBox.o .objs/arbarlith2/engine/COLOR.o .objs/arbarlith2/engine/Controller.o .objs/arbarlith2/engine/CycleTextureSelectorState.o .objs/arbarlith2/engine/DebugDisplayToggleKeyDetector.o .objs/arbarlith2/engine/DebugLabel.o .objs/arbarlith2/engine/Detector.o .objs/arbarlith2/engine/DetectorProximity.o .objs/arbarlith2/engine/DetectorProximityPlayer.o .objs/arbarlith2/engine/Dimmer.o .objs/arbarlith2/engine/EditWidget.o .objs/arbarlith2/engine/EditorKeyDetector.o .objs/arbarlith2/engine/EditorToolBar.o .objs/arbarlith2/engine/Effect.o .objs/arbarlith2/engine/EffectManager.o .objs/arbarlith2/engine/Effect_GUI_FFP.o .objs/arbarlith2/engine/Effect_Gouraud_FFP.o .objs/arbarlith2/engine/Effect_Particle_FFP.o .objs/arbarlith2/engine/Effect_Project_Shadows.o .objs/arbarlith2/engine/Effect_Receive_Shadows.o .objs/arbarlith2/engine/Effect_Red_FFP.o .objs/arbarlith2/engine/Effect_TextureReplace_FFP.o .objs/arbarlith2/engine/Effect_Z_Only.o .objs/arbarlith2/engine/FPSDisplayToggleKeyDetector.o .objs/arbarlith2/engine/FileSelectionList.o .objs/arbarlith2/engine/GameStateCredits.o .objs/arbarlith2/engine/GameStateEditor.o .objs/arbarlith2/engine/GameStateMenu.o .objs/arbarlith2/engine/GameStateRun.o .objs/arbarlith2/engine/GameStateSpellMenu.o .objs/arbarlith2/engine/ItemWidget.o .objs/arbarlith2/engine/Key.o .objs/arbarlith2/engine/Light.o .objs/arbarlith2/engine/LightManager.o .objs/arbarlith2/engine/ListElementWidget.o .objs/arbarlith2/engine/ListPaneWidget.o .objs/arbarlith2/engine/Listener.o .objs/arbarlith2/engine/ListenerPrompt.o .objs/arbarlith2/engine/Map.o .objs/arbarlith2/engine/Md3Loader.o .objs/arbarlith2/engine/Menu.o .objs/arbarlith2/engine/MenuGameOver.o .objs/arbarlith2/engine/MenuKeyDetector.o .objs/arbarlith2/engine/MenuOptions.o .objs/arbarlith2/engine/MenuWorldLeadOut.o .objs/arbarlith2/engine/MenuWorldSelection.o .objs/arbarlith2/engine/MessageRouter.o .objs/arbarlith2/engine/ModelLoader.o .objs/arbarlith2/engine/MonsterFSM.o .objs/arbarlith2/engine/MusicEngine.o .objs/arbarlith2/engine/PerformanceLabel.o .objs/arbarlith2/engine/PropertyBag.o .objs/arbarlith2/engine/QuadTreeNode.o .objs/arbarlith2/engine/SDLwindow.o .objs/arbarlith2/engine/ScreenShotTask.o .objs/arbarlith2/engine/SelectionList.o .objs/arbarlith2/engine/Shadow.o .objs/arbarlith2/engine/ShadowManager.o .objs/arbarlith2/engine/SoundSystem.o .objs/arbarlith2/engine/SpellMenuKeyDetector.o .objs/arbarlith2/engine/SplashScreen.o .objs/arbarlith2/engine/StateMachine.o .objs/arbarlith2/engine/StateMachineFactory.o .objs/arbarlith2/engine/Switch.o .objs/arbarlith2/engine/TextureHandle.o .objs/arbarlith2/engine/TextureManager.o .objs/arbarlith2/engine/Tile.o .objs/arbarlith2/engine/ToggleWidget.o .objs/arbarlith2/engine/ToggleWidgetText.o .objs/arbarlith2/engine/ToolBarForEditorTools.o .objs/arbarlith2/engine/Trigger.o .objs/arbarlith2/engine/TriggerParticles.o .objs/arbarlith2/engine/TriggerPrompt.o .objs/arbarlith2/engine/VisualPriority.o .objs/arbarlith2/engine/WaitScreen.o .objs/arbarlith2/engine/animation.o .objs/arbarlith2/engine/application.o .objs/arbarlith2/engine/camera.o .objs/arbarlith2/engine/creature.o .objs/arbarlith2/engine/file.o .objs/arbarlith2/engine/fog.o .objs/arbarlith2/engine/frustum.o .objs/arbarlith2/engine/horizbar.o .objs/arbarlith2/engine/image.o .objs/arbarlith2/engine/item.o .objs/arbarlith2/engine/keyframe.o .objs/arbarlith2/engine/labelwidget.o .objs/arbarlith2/engine/lamp.o .objs/arbarlith2/engine/logstring.o .objs/arbarlith2/engine/mat4.o .objs/arbarlith2/engine/material.o .objs/arbarlith2/engine/mesh.o .objs/arbarlith2/engine/myassert.o .objs/arbarlith2/engine/object.o .objs/arbarlith2/engine/opengl.o .objs/arbarlith2/engine/particle.o .objs/arbarlith2/engine/player.o .objs/arbarlith2/engine/primitivedatatypes.o .objs/arbarlith2/engine/profile.o .objs/arbarlith2/engine/searchfile.o .objs/arbarlith2/engine/stdafx.o .objs/arbarlith2/engine/text.o .objs/arbarlith2/engine/timer.o .objs/arbarlith2/engine/tstring.o .objs/arbarlith2/engine/widget.o .objs/arbarlith2/engine/widgetmanager.o .objs/arbarlith2/engine/windowwidget.o .objs/arbarlith2/engine/world.o .objs/arbarlith2/win32.o 
default_LINKOBJS=$(default_OBJS)
default_DEPS=.deps/arbarlith2/Boss.d .deps/arbarlith2/Bullet.d .deps/arbarlith2/DetectorProximityMonster.d .deps/arbarlith2/Explosion.d .deps/arbarlith2/GateOpener.d .deps/arbarlith2/Monster.d .deps/arbarlith2/MyPlayer.d .deps/arbarlith2/Powerup.d .deps/arbarlith2/PowerupHeal.d .deps/arbarlith2/PowerupSpell.d .deps/arbarlith2/Spawn.d .deps/arbarlith2/Spell.d .deps/arbarlith2/SpellCasterFSM.d .deps/arbarlith2/SpellFireBall.d .deps/arbarlith2/SpellHeal.d .deps/arbarlith2/SpellIncinerate.d .deps/arbarlith2/Teleporter.d .deps/arbarlith2/TileGate.d .deps/arbarlith2/engine/3dmath.d .deps/arbarlith2/engine/3dsLoader.d .deps/arbarlith2/engine/ActorFactory.d .deps/arbarlith2/engine/ActorSet.d .deps/arbarlith2/engine/AnimationController.d .deps/arbarlith2/engine/BackdropTexture.d .deps/arbarlith2/engine/Blur.d .deps/arbarlith2/engine/BlurEffect.d .deps/arbarlith2/engine/BoundingBox.d .deps/arbarlith2/engine/COLOR.d .deps/arbarlith2/engine/Controller.d .deps/arbarlith2/engine/CycleTextureSelectorState.d .deps/arbarlith2/engine/DebugDisplayToggleKeyDetector.d .deps/arbarlith2/engine/DebugLabel.d .deps/arbarlith2/engine/Detector.d .deps/arbarlith2/engine/DetectorProximity.d .deps/arbarlith2/engine/DetectorProximityPlayer.d .deps/arbarlith2/engine/Dimmer.d .deps/arbarlith2/engine/EditWidget.d .deps/arbarlith2/engine/EditorKeyDetector.d .deps/arbarlith2/engine/EditorToolBar.d .deps/arbarlith2/engine/Effect.d .deps/arbarlith2/engine/EffectManager.d .deps/arbarlith2/engine/Effect_GUI_FFP.d .deps/arbarlith2/engine/Effect_Gouraud_FFP.d .deps/arbarlith2/engine/Effect_Particle_FFP.d .deps/arbarlith2/engine/Effect_Project_Shadows.d .deps/arbarlith2/engine/Effect_Receive_Shadows.d .deps/arbarlith2/engine/Effect_Red_FFP.d .deps/arbarlith2/engine/Effect_TextureReplace_FFP.d .deps/arbarlith2/engine/Effect_Z_Only.d .deps/arbarlith2/engine/FPSDisplayToggleKeyDetector.d .deps/arbarlith2/engine/FileSelectionList.d .deps/arbarlith2/engine/GameStateCredits.d .deps/arbarlith2/engine/GameStateEditor.d .deps/arbarlith2/engine/GameStateMenu.d .deps/arbarlith2/engine/GameStateRun.d .deps/arbarlith2/engine/GameStateSpellMenu.d .deps/arbarlith2/engine/ItemWidget.d .deps/arbarlith2/engine/Key.d .deps/arbarlith2/engine/Light.d .deps/arbarlith2/engine/LightManager.d .deps/arbarlith2/engine/ListElementWidget.d .deps/arbarlith2/engine/ListPaneWidget.d .deps/arbarlith2/engine/Listener.d .deps/arbarlith2/engine/ListenerPrompt.d .deps/arbarlith2/engine/Map.d .deps/arbarlith2/engine/Md3Loader.d .deps/arbarlith2/engine/Menu.d .deps/arbarlith2/engine/MenuGameOver.d .deps/arbarlith2/engine/MenuKeyDetector.d .deps/arbarlith2/engine/MenuOptions.d .deps/arbarlith2/engine/MenuWorldLeadOut.d .deps/arbarlith2/engine/MenuWorldSelection.d .deps/arbarlith2/engine/MessageRouter.d .deps/arbarlith2/engine/ModelLoader.d .deps/arbarlith2/engine/MonsterFSM.d .deps/arbarlith2/engine/MusicEngine.d .deps/arbarlith2/engine/PerformanceLabel.d .deps/arbarlith2/engine/PropertyBag.d .deps/arbarlith2/engine/QuadTreeNode.d .deps/arbarlith2/engine/SDLwindow.d .deps/arbarlith2/engine/ScreenShotTask.d .deps/arbarlith2/engine/SelectionList.d .deps/arbarlith2/engine/Shadow.d .deps/arbarlith2/engine/ShadowManager.d .deps/arbarlith2/engine/SoundSystem.d .deps/arbarlith2/engine/SpellMenuKeyDetector.d .deps/arbarlith2/engine/SplashScreen.d .deps/arbarlith2/engine/StateMachine.d .deps/arbarlith2/engine/StateMachineFactory.d .deps/arbarlith2/engine/Switch.d .deps/arbarlith2/engine/TextureHandle.d .deps/arbarlith2/engine/TextureManager.d .deps/arbarlith2/engine/Tile.d .deps/arbarlith2/engine/ToggleWidget.d .deps/arbarlith2/engine/ToggleWidgetText.d .deps/arbarlith2/engine/ToolBarForEditorTools.d .deps/arbarlith2/engine/Trigger.d .deps/arbarlith2/engine/TriggerParticles.d .deps/arbarlith2/engine/TriggerPrompt.d .deps/arbarlith2/engine/VisualPriority.d .deps/arbarlith2/engine/WaitScreen.d .deps/arbarlith2/engine/animation.d .deps/arbarlith2/engine/application.d .deps/arbarlith2/engine/camera.d .deps/arbarlith2/engine/creature.d .deps/arbarlith2/engine/file.d .deps/arbarlith2/engine/fog.d .deps/arbarlith2/engine/frustum.d .deps/arbarlith2/engine/horizbar.d .deps/arbarlith2/engine/image.d .deps/arbarlith2/engine/item.d .deps/arbarlith2/engine/keyframe.d .deps/arbarlith2/engine/labelwidget.d .deps/arbarlith2/engine/lamp.d .deps/arbarlith2/engine/logstring.d .deps/arbarlith2/engine/mat4.d .deps/arbarlith2/engine/material.d .deps/arbarlith2/engine/mesh.d .deps/arbarlith2/engine/myassert.d .deps/arbarlith2/engine/object.d .deps/arbarlith2/engine/opengl.d .deps/arbarlith2/engine/particle.d .deps/arbarlith2/engine/player.d .deps/arbarlith2/engine/primitivedatatypes.d .deps/arbarlith2/engine/profile.d .deps/arbarlith2/engine/searchfile.d .deps/arbarlith2/engine/stdafx.d .deps/arbarlith2/engine/text.d .deps/arbarlith2/engine/timer.d .deps/arbarlith2/engine/tstring.d .deps/arbarlith2/engine/widget.d .deps/arbarlith2/engine/widgetmanager.d .deps/arbarlith2/engine/windowwidget.d .deps/arbarlith2/engine/world.d .deps/arbarlith2/win32.d 

### The targets of this project
default_BIN=.\arbarlith2\bin\arbarlith2_gcc.exe

.PHONY: all all-before all-custom all-after clean clean-custom distclean distclean-custom depend_default default-before default-after 

all: all-before default all-after


dist:
	@zip arbarlith2.cbp.zip arbarlith2.cbp Makefile arbarlith2\Boss.cpp arbarlith2\Boss.h arbarlith2\Bullet.cpp arbarlith2\Bullet.h arbarlith2\DetectorProximityMonster.cpp arbarlith2\DetectorProximityMonster.h arbarlith2\Explosion.cpp arbarlith2\Explosion.h arbarlith2\GateOpener.cpp arbarlith2\GateOpener.h arbarlith2\Monster.cpp arbarlith2\Monster.h arbarlith2\MyPlayer.cpp arbarlith2\MyPlayer.h arbarlith2\Powerup.cpp arbarlith2\Powerup.h arbarlith2\PowerupHeal.cpp arbarlith2\PowerupHeal.h arbarlith2\PowerupSpell.cpp arbarlith2\PowerupSpell.h arbarlith2\Spawn.cpp arbarlith2\Spawn.h arbarlith2\Spell.cpp arbarlith2\Spell.h arbarlith2\SpellCasterFSM.cpp arbarlith2\SpellCasterFSM.h arbarlith2\SpellFireBall.cpp arbarlith2\SpellFireBall.h arbarlith2\SpellHeal.cpp arbarlith2\SpellHeal.h arbarlith2\SpellIncinerate.cpp arbarlith2\SpellIncinerate.h arbarlith2\Teleporter.cpp arbarlith2\Teleporter.h arbarlith2\TileGate.cpp arbarlith2\TileGate.h arbarlith2\engine\3dmath.cpp arbarlith2\engine\3dmath.h arbarlith2\engine\3dsLoader.cpp arbarlith2\engine\3dsLoader.h arbarlith2\engine\ActorFactory.cpp arbarlith2\engine\ActorFactory.h arbarlith2\engine\ActorSet.cpp arbarlith2\engine\ActorSet.h arbarlith2\engine\AnimationController.cpp arbarlith2\engine\AnimationController.h arbarlith2\engine\Application.h arbarlith2\engine\BackdropTexture.cpp arbarlith2\engine\BackdropTexture.h arbarlith2\engine\Blur.cpp arbarlith2\engine\Blur.h arbarlith2\engine\BlurEffect.cpp arbarlith2\engine\BlurEffect.h arbarlith2\engine\BoundingBox.cpp arbarlith2\engine\BoundingBox.h arbarlith2\engine\COLOR.cpp arbarlith2\engine\COLOR.h arbarlith2\engine\CallbackInterpolator.h arbarlith2\engine\CallbackTask.h arbarlith2\engine\Command.h arbarlith2\engine\Controller.cpp arbarlith2\engine\Controller.h arbarlith2\engine\CycleTextureSelectorState.cpp arbarlith2\engine\CycleTextureSelectorState.h arbarlith2\engine\DebugDisplayToggleKeyDetector.cpp arbarlith2\engine\DebugDisplayToggleKeyDetector.h arbarlith2\engine\DebugLabel.cpp arbarlith2\engine\DebugLabel.h arbarlith2\engine\Detector.cpp arbarlith2\engine\Detector.h arbarlith2\engine\DetectorProximity.cpp arbarlith2\engine\DetectorProximity.h arbarlith2\engine\DetectorProximityPlayer.cpp arbarlith2\engine\DetectorProximityPlayer.h arbarlith2\engine\Dimmer.cpp arbarlith2\engine\Dimmer.h arbarlith2\engine\EditWidget.cpp arbarlith2\engine\EditWidget.h arbarlith2\engine\EditorKeyDetector.cpp arbarlith2\engine\EditorKeyDetector.h arbarlith2\engine\EditorToolBar.cpp arbarlith2\engine\EditorToolBar.h arbarlith2\engine\Effect.cpp arbarlith2\engine\Effect.h arbarlith2\engine\EffectManager.cpp arbarlith2\engine\EffectManager.h arbarlith2\engine\EffectSig.h arbarlith2\engine\Effect_GUI_FFP.cpp arbarlith2\engine\Effect_GUI_FFP.h arbarlith2\engine\Effect_Gouraud_FFP.cpp arbarlith2\engine\Effect_Gouraud_FFP.h arbarlith2\engine\Effect_Particle_FFP.cpp arbarlith2\engine\Effect_Particle_FFP.h arbarlith2\engine\Effect_Project_Shadows.cpp arbarlith2\engine\Effect_Project_Shadows.h arbarlith2\engine\Effect_Receive_Shadows.cpp arbarlith2\engine\Effect_Receive_Shadows.h arbarlith2\engine\Effect_Red_FFP.cpp arbarlith2\engine\Effect_Red_FFP.h arbarlith2\engine\Effect_TextureReplace_FFP.cpp arbarlith2\engine\Effect_TextureReplace_FFP.h arbarlith2\engine\Effect_Z_Only.cpp arbarlith2\engine\Effect_Z_Only.h arbarlith2\engine\FPSDisplayToggleKeyDetector.cpp arbarlith2\engine\FPSDisplayToggleKeyDetector.h arbarlith2\engine\Factory.h arbarlith2\engine\FileSelectionList.cpp arbarlith2\engine\FileSelectionList.h arbarlith2\engine\GameState.h arbarlith2\engine\GameStateCredits.cpp arbarlith2\engine\GameStateCredits.h arbarlith2\engine\GameStateEditor.cpp arbarlith2\engine\GameStateEditor.h arbarlith2\engine\GameStateMenu.cpp arbarlith2\engine\GameStateMenu.h arbarlith2\engine\GameStateRun.cpp arbarlith2\engine\GameStateRun.h arbarlith2\engine\GameStateSpellMenu.cpp arbarlith2\engine\GameStateSpellMenu.h arbarlith2\engine\ItemWidget.cpp arbarlith2\engine\ItemWidget.h arbarlith2\engine\Key.cpp arbarlith2\engine\Key.h arbarlith2\engine\Light.cpp arbarlith2\engine\Light.h arbarlith2\engine\LightManager.cpp arbarlith2\engine\LightManager.h arbarlith2\engine\LinearInterpolator.h arbarlith2\engine\ListElementLabel.h arbarlith2\engine\ListElementTweaker.h arbarlith2\engine\ListElementWidget.cpp arbarlith2\engine\ListElementWidget.h arbarlith2\engine\ListPaneWidget.cpp arbarlith2\engine\ListPaneWidget.h arbarlith2\engine\Listener.cpp arbarlith2\engine\Listener.h arbarlith2\engine\ListenerPrompt.cpp arbarlith2\engine\ListenerPrompt.h arbarlith2\engine\Map.cpp arbarlith2\engine\Map.h arbarlith2\engine\Md3Loader.cpp arbarlith2\engine\Md3Loader.h arbarlith2\engine\Menu.cpp arbarlith2\engine\Menu.h arbarlith2\engine\MenuGameOver.cpp arbarlith2\engine\MenuGameOver.h arbarlith2\engine\MenuKeyDetector.cpp arbarlith2\engine\MenuKeyDetector.h arbarlith2\engine\MenuOptions.cpp arbarlith2\engine\MenuOptions.h arbarlith2\engine\MenuWorldLeadOut.cpp arbarlith2\engine\MenuWorldLeadOut.h arbarlith2\engine\MenuWorldSelection.cpp arbarlith2\engine\MenuWorldSelection.h arbarlith2\engine\Message.h arbarlith2\engine\MessageRouter.cpp arbarlith2\engine\MessageRouter.h arbarlith2\engine\ModelLoader.cpp arbarlith2\engine\ModelLoader.h arbarlith2\engine\MonsterFSM.cpp arbarlith2\engine\MonsterFSM.h arbarlith2\engine\MusicEngine.cpp arbarlith2\engine\MusicEngine.h arbarlith2\engine\PerformanceLabel.cpp arbarlith2\engine\PerformanceLabel.h arbarlith2\engine\PeriodicCallbackTask.h arbarlith2\engine\PropertyBag.cpp arbarlith2\engine\PropertyBag.h arbarlith2\engine\QuadTreeNode.cpp arbarlith2\engine\QuadTreeNode.h arbarlith2\engine\SDLwindow.cpp arbarlith2\engine\SDLwindow.h arbarlith2\engine\ScreenShotTask.cpp arbarlith2\engine\ScreenShotTask.h arbarlith2\engine\SelectionList.cpp arbarlith2\engine\SelectionList.h arbarlith2\engine\Shadow.cpp arbarlith2\engine\Shadow.h arbarlith2\engine\ShadowManager.cpp arbarlith2\engine\ShadowManager.h arbarlith2\engine\SoundSystem.cpp arbarlith2\engine\SoundSystem.h arbarlith2\engine\SpellMenuKeyDetector.cpp arbarlith2\engine\SpellMenuKeyDetector.h arbarlith2\engine\SplashScreen.cpp arbarlith2\engine\SplashScreen.h arbarlith2\engine\StateMachine.cpp arbarlith2\engine\StateMachine.h arbarlith2\engine\StateMachineFactory.cpp arbarlith2\engine\StateMachineFactory.h arbarlith2\engine\StringSelectionList.h arbarlith2\engine\Switch.cpp arbarlith2\engine\Switch.h arbarlith2\engine\TextureHandle.cpp arbarlith2\engine\TextureHandle.h arbarlith2\engine\TextureManager.cpp arbarlith2\engine\TextureManager.h arbarlith2\engine\Tile.cpp arbarlith2\engine\Tile.h arbarlith2\engine\ToggleWidget.cpp arbarlith2\engine\ToggleWidget.h arbarlith2\engine\ToggleWidgetText.cpp arbarlith2\engine\ToggleWidgetText.h arbarlith2\engine\ToolBarForEditorTools.cpp arbarlith2\engine\ToolBarForEditorTools.h arbarlith2\engine\Trigger.cpp arbarlith2\engine\Trigger.h arbarlith2\engine\TriggerParticles.cpp arbarlith2\engine\TriggerParticles.h arbarlith2\engine\TriggerPrompt.cpp arbarlith2\engine\TriggerPrompt.h arbarlith2\engine\TweakerWidget.h arbarlith2\engine\VisualPriority.cpp arbarlith2\engine\VisualPriority.h arbarlith2\engine\WaitScreen.cpp arbarlith2\engine\WaitScreen.h arbarlith2\engine\animation.cpp arbarlith2\engine\animation.h arbarlith2\engine\application.cpp arbarlith2\engine\camera.cpp arbarlith2\engine\camera.h arbarlith2\engine\creature.cpp arbarlith2\engine\creature.h arbarlith2\engine\file.cpp arbarlith2\engine\file.h arbarlith2\engine\fog.cpp arbarlith2\engine\fog.h arbarlith2\engine\frustum.cpp arbarlith2\engine\frustum.h arbarlith2\engine\gl.h arbarlith2\engine\horizbar.cpp arbarlith2\engine\horizbar.h arbarlith2\engine\il.h arbarlith2\engine\image.cpp arbarlith2\engine\image.h arbarlith2\engine\item.cpp arbarlith2\engine\item.h arbarlith2\engine\keyframe.cpp arbarlith2\engine\keyframe.h arbarlith2\engine\labelwidget.cpp arbarlith2\engine\labelwidget.h arbarlith2\engine\lamp.cpp arbarlith2\engine\lamp.h arbarlith2\engine\logstring.cpp arbarlith2\engine\logstring.h arbarlith2\engine\macros.h arbarlith2\engine\mat4.cpp arbarlith2\engine\mat4.h arbarlith2\engine\material.cpp arbarlith2\engine\material.h arbarlith2\engine\mesh.cpp arbarlith2\engine\mesh.h arbarlith2\engine\misc.h arbarlith2\engine\myassert.cpp arbarlith2\engine\myassert.h arbarlith2\engine\object.cpp arbarlith2\engine\object.h arbarlith2\engine\opengl.cpp arbarlith2\engine\opengl.h arbarlith2\engine\particle.cpp arbarlith2\engine\particle.h arbarlith2\engine\player.cpp arbarlith2\engine\player.h arbarlith2\engine\primitivedatatypes.cpp arbarlith2\engine\primitivedatatypes.h arbarlith2\engine\profile.cpp arbarlith2\engine\profile.h arbarlith2\engine\random.h arbarlith2\engine\searchfile.cpp arbarlith2\engine\searchfile.h arbarlith2\engine\singleton.h arbarlith2\engine\stdafx.cpp arbarlith2\engine\stdafx.h arbarlith2\engine\task.h arbarlith2\engine\text.cpp arbarlith2\engine\text.h arbarlith2\engine\timer.cpp arbarlith2\engine\timer.h arbarlith2\engine\tstring.cpp arbarlith2\engine\tstring.h arbarlith2\engine\vec4.h arbarlith2\engine\widget.cpp arbarlith2\engine\widget.h arbarlith2\engine\widgetmanager.cpp arbarlith2\engine\widgetmanager.h arbarlith2\engine\windowwidget.cpp arbarlith2\engine\windowwidget.h arbarlith2\engine\world.cpp arbarlith2\engine\world.h arbarlith2\stdafx.h arbarlith2\win32.cpp 


clean:
	-@if exist $(default_BIN) del $(default_BIN)
	-@if exist ".objs\" rmdir ".objs\" /S /Q

distclean:
	-@if exist $(default_BIN) del $(default_BIN)
	-@if exist ".objs\" rmdir ".objs\" /S /Q
	-@if exist ".objs\" rmdir ".deps\" /S /Q

depend_default_DIRS:
	-@if not exist ".deps\." mkdir ".deps"
	-@if not exist ".deps\arbarlith2\." mkdir ".deps\arbarlith2"
	-@if not exist ".deps\arbarlith2\engine\." mkdir ".deps\arbarlith2\engine"

depend_default: depend_default_DIRS $(default_DEPS)

depend: depend_default

default_DIRS:
	-@if not exist ".objs\." mkdir ".objs"
	-@if not exist ".objs\arbarlith2\." mkdir ".objs\arbarlith2"
	-@if not exist ".objs\arbarlith2\engine\." mkdir ".objs\arbarlith2\engine"
	-@if not exist ".\." mkdir "."
	-@if not exist ".\arbarlith2\." mkdir ".\arbarlith2"
	-@if not exist ".\arbarlith2\bin\." mkdir ".\arbarlith2\bin"

default: depend_default default_DIRS default-before $(default_BIN) default-after

$(default_BIN): $(default_LINKOBJS)
	$(default_LD) $(default_LIBDIRS) -o $(default_BIN) $(default_LINKOBJS) $(default_LDFLAGS) $(default_LIBS) -mwindows


.deps/arbarlith2/Boss.d: arbarlith2/Boss.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/Boss.d -MT .objs/arbarlith2/Boss.o $(default_INCS) arbarlith2/Boss.cpp

.objs/arbarlith2/Boss.o: .deps/arbarlith2/Boss.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/Boss.cpp -o .objs/arbarlith2/Boss.o

.deps/arbarlith2/Bullet.d: arbarlith2/Bullet.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/Bullet.d -MT .objs/arbarlith2/Bullet.o $(default_INCS) arbarlith2/Bullet.cpp

.objs/arbarlith2/Bullet.o: .deps/arbarlith2/Bullet.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/Bullet.cpp -o .objs/arbarlith2/Bullet.o

.deps/arbarlith2/DetectorProximityMonster.d: arbarlith2/DetectorProximityMonster.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/DetectorProximityMonster.d -MT .objs/arbarlith2/DetectorProximityMonster.o $(default_INCS) arbarlith2/DetectorProximityMonster.cpp

.objs/arbarlith2/DetectorProximityMonster.o: .deps/arbarlith2/DetectorProximityMonster.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/DetectorProximityMonster.cpp -o .objs/arbarlith2/DetectorProximityMonster.o

.deps/arbarlith2/Explosion.d: arbarlith2/Explosion.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/Explosion.d -MT .objs/arbarlith2/Explosion.o $(default_INCS) arbarlith2/Explosion.cpp

.objs/arbarlith2/Explosion.o: .deps/arbarlith2/Explosion.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/Explosion.cpp -o .objs/arbarlith2/Explosion.o

.deps/arbarlith2/GateOpener.d: arbarlith2/GateOpener.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/GateOpener.d -MT .objs/arbarlith2/GateOpener.o $(default_INCS) arbarlith2/GateOpener.cpp

.objs/arbarlith2/GateOpener.o: .deps/arbarlith2/GateOpener.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/GateOpener.cpp -o .objs/arbarlith2/GateOpener.o

.deps/arbarlith2/Monster.d: arbarlith2/Monster.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/Monster.d -MT .objs/arbarlith2/Monster.o $(default_INCS) arbarlith2/Monster.cpp

.objs/arbarlith2/Monster.o: .deps/arbarlith2/Monster.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/Monster.cpp -o .objs/arbarlith2/Monster.o

.deps/arbarlith2/MyPlayer.d: arbarlith2/MyPlayer.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/MyPlayer.d -MT .objs/arbarlith2/MyPlayer.o $(default_INCS) arbarlith2/MyPlayer.cpp

.objs/arbarlith2/MyPlayer.o: .deps/arbarlith2/MyPlayer.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/MyPlayer.cpp -o .objs/arbarlith2/MyPlayer.o

.deps/arbarlith2/Powerup.d: arbarlith2/Powerup.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/Powerup.d -MT .objs/arbarlith2/Powerup.o $(default_INCS) arbarlith2/Powerup.cpp

.objs/arbarlith2/Powerup.o: .deps/arbarlith2/Powerup.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/Powerup.cpp -o .objs/arbarlith2/Powerup.o

.deps/arbarlith2/PowerupHeal.d: arbarlith2/PowerupHeal.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/PowerupHeal.d -MT .objs/arbarlith2/PowerupHeal.o $(default_INCS) arbarlith2/PowerupHeal.cpp

.objs/arbarlith2/PowerupHeal.o: .deps/arbarlith2/PowerupHeal.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/PowerupHeal.cpp -o .objs/arbarlith2/PowerupHeal.o

.deps/arbarlith2/PowerupSpell.d: arbarlith2/PowerupSpell.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/PowerupSpell.d -MT .objs/arbarlith2/PowerupSpell.o $(default_INCS) arbarlith2/PowerupSpell.cpp

.objs/arbarlith2/PowerupSpell.o: .deps/arbarlith2/PowerupSpell.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/PowerupSpell.cpp -o .objs/arbarlith2/PowerupSpell.o

.deps/arbarlith2/Spawn.d: arbarlith2/Spawn.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/Spawn.d -MT .objs/arbarlith2/Spawn.o $(default_INCS) arbarlith2/Spawn.cpp

.objs/arbarlith2/Spawn.o: .deps/arbarlith2/Spawn.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/Spawn.cpp -o .objs/arbarlith2/Spawn.o

.deps/arbarlith2/Spell.d: arbarlith2/Spell.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/Spell.d -MT .objs/arbarlith2/Spell.o $(default_INCS) arbarlith2/Spell.cpp

.objs/arbarlith2/Spell.o: .deps/arbarlith2/Spell.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/Spell.cpp -o .objs/arbarlith2/Spell.o

.deps/arbarlith2/SpellCasterFSM.d: arbarlith2/SpellCasterFSM.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/SpellCasterFSM.d -MT .objs/arbarlith2/SpellCasterFSM.o $(default_INCS) arbarlith2/SpellCasterFSM.cpp

.objs/arbarlith2/SpellCasterFSM.o: .deps/arbarlith2/SpellCasterFSM.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/SpellCasterFSM.cpp -o .objs/arbarlith2/SpellCasterFSM.o

.deps/arbarlith2/SpellFireBall.d: arbarlith2/SpellFireBall.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/SpellFireBall.d -MT .objs/arbarlith2/SpellFireBall.o $(default_INCS) arbarlith2/SpellFireBall.cpp

.objs/arbarlith2/SpellFireBall.o: .deps/arbarlith2/SpellFireBall.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/SpellFireBall.cpp -o .objs/arbarlith2/SpellFireBall.o

.deps/arbarlith2/SpellHeal.d: arbarlith2/SpellHeal.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/SpellHeal.d -MT .objs/arbarlith2/SpellHeal.o $(default_INCS) arbarlith2/SpellHeal.cpp

.objs/arbarlith2/SpellHeal.o: .deps/arbarlith2/SpellHeal.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/SpellHeal.cpp -o .objs/arbarlith2/SpellHeal.o

.deps/arbarlith2/SpellIncinerate.d: arbarlith2/SpellIncinerate.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/SpellIncinerate.d -MT .objs/arbarlith2/SpellIncinerate.o $(default_INCS) arbarlith2/SpellIncinerate.cpp

.objs/arbarlith2/SpellIncinerate.o: .deps/arbarlith2/SpellIncinerate.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/SpellIncinerate.cpp -o .objs/arbarlith2/SpellIncinerate.o

.deps/arbarlith2/Teleporter.d: arbarlith2/Teleporter.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/Teleporter.d -MT .objs/arbarlith2/Teleporter.o $(default_INCS) arbarlith2/Teleporter.cpp

.objs/arbarlith2/Teleporter.o: .deps/arbarlith2/Teleporter.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/Teleporter.cpp -o .objs/arbarlith2/Teleporter.o

.deps/arbarlith2/TileGate.d: arbarlith2/TileGate.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/TileGate.d -MT .objs/arbarlith2/TileGate.o $(default_INCS) arbarlith2/TileGate.cpp

.objs/arbarlith2/TileGate.o: .deps/arbarlith2/TileGate.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/TileGate.cpp -o .objs/arbarlith2/TileGate.o

.deps/arbarlith2/engine/3dmath.d: arbarlith2/engine/3dmath.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/3dmath.d -MT .objs/arbarlith2/engine/3dmath.o $(default_INCS) arbarlith2/engine/3dmath.cpp

.objs/arbarlith2/engine/3dmath.o: .deps/arbarlith2/engine/3dmath.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/3dmath.cpp -o .objs/arbarlith2/engine/3dmath.o

.deps/arbarlith2/engine/3dsLoader.d: arbarlith2/engine/3dsLoader.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/3dsLoader.d -MT .objs/arbarlith2/engine/3dsLoader.o $(default_INCS) arbarlith2/engine/3dsLoader.cpp

.objs/arbarlith2/engine/3dsLoader.o: .deps/arbarlith2/engine/3dsLoader.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/3dsLoader.cpp -o .objs/arbarlith2/engine/3dsLoader.o

.deps/arbarlith2/engine/ActorFactory.d: arbarlith2/engine/ActorFactory.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ActorFactory.d -MT .objs/arbarlith2/engine/ActorFactory.o $(default_INCS) arbarlith2/engine/ActorFactory.cpp

.objs/arbarlith2/engine/ActorFactory.o: .deps/arbarlith2/engine/ActorFactory.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ActorFactory.cpp -o .objs/arbarlith2/engine/ActorFactory.o

.deps/arbarlith2/engine/ActorSet.d: arbarlith2/engine/ActorSet.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ActorSet.d -MT .objs/arbarlith2/engine/ActorSet.o $(default_INCS) arbarlith2/engine/ActorSet.cpp

.objs/arbarlith2/engine/ActorSet.o: .deps/arbarlith2/engine/ActorSet.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ActorSet.cpp -o .objs/arbarlith2/engine/ActorSet.o

.deps/arbarlith2/engine/AnimationController.d: arbarlith2/engine/AnimationController.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/AnimationController.d -MT .objs/arbarlith2/engine/AnimationController.o $(default_INCS) arbarlith2/engine/AnimationController.cpp

.objs/arbarlith2/engine/AnimationController.o: .deps/arbarlith2/engine/AnimationController.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/AnimationController.cpp -o .objs/arbarlith2/engine/AnimationController.o

.deps/arbarlith2/engine/BackdropTexture.d: arbarlith2/engine/BackdropTexture.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/BackdropTexture.d -MT .objs/arbarlith2/engine/BackdropTexture.o $(default_INCS) arbarlith2/engine/BackdropTexture.cpp

.objs/arbarlith2/engine/BackdropTexture.o: .deps/arbarlith2/engine/BackdropTexture.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/BackdropTexture.cpp -o .objs/arbarlith2/engine/BackdropTexture.o

.deps/arbarlith2/engine/Blur.d: arbarlith2/engine/Blur.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Blur.d -MT .objs/arbarlith2/engine/Blur.o $(default_INCS) arbarlith2/engine/Blur.cpp

.objs/arbarlith2/engine/Blur.o: .deps/arbarlith2/engine/Blur.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Blur.cpp -o .objs/arbarlith2/engine/Blur.o

.deps/arbarlith2/engine/BlurEffect.d: arbarlith2/engine/BlurEffect.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/BlurEffect.d -MT .objs/arbarlith2/engine/BlurEffect.o $(default_INCS) arbarlith2/engine/BlurEffect.cpp

.objs/arbarlith2/engine/BlurEffect.o: .deps/arbarlith2/engine/BlurEffect.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/BlurEffect.cpp -o .objs/arbarlith2/engine/BlurEffect.o

.deps/arbarlith2/engine/BoundingBox.d: arbarlith2/engine/BoundingBox.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/BoundingBox.d -MT .objs/arbarlith2/engine/BoundingBox.o $(default_INCS) arbarlith2/engine/BoundingBox.cpp

.objs/arbarlith2/engine/BoundingBox.o: .deps/arbarlith2/engine/BoundingBox.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/BoundingBox.cpp -o .objs/arbarlith2/engine/BoundingBox.o

.deps/arbarlith2/engine/COLOR.d: arbarlith2/engine/COLOR.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/COLOR.d -MT .objs/arbarlith2/engine/COLOR.o $(default_INCS) arbarlith2/engine/COLOR.cpp

.objs/arbarlith2/engine/COLOR.o: .deps/arbarlith2/engine/COLOR.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/COLOR.cpp -o .objs/arbarlith2/engine/COLOR.o

.deps/arbarlith2/engine/Controller.d: arbarlith2/engine/Controller.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Controller.d -MT .objs/arbarlith2/engine/Controller.o $(default_INCS) arbarlith2/engine/Controller.cpp

.objs/arbarlith2/engine/Controller.o: .deps/arbarlith2/engine/Controller.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Controller.cpp -o .objs/arbarlith2/engine/Controller.o

.deps/arbarlith2/engine/CycleTextureSelectorState.d: arbarlith2/engine/CycleTextureSelectorState.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/CycleTextureSelectorState.d -MT .objs/arbarlith2/engine/CycleTextureSelectorState.o $(default_INCS) arbarlith2/engine/CycleTextureSelectorState.cpp

.objs/arbarlith2/engine/CycleTextureSelectorState.o: .deps/arbarlith2/engine/CycleTextureSelectorState.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/CycleTextureSelectorState.cpp -o .objs/arbarlith2/engine/CycleTextureSelectorState.o

.deps/arbarlith2/engine/DebugDisplayToggleKeyDetector.d: arbarlith2/engine/DebugDisplayToggleKeyDetector.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/DebugDisplayToggleKeyDetector.d -MT .objs/arbarlith2/engine/DebugDisplayToggleKeyDetector.o $(default_INCS) arbarlith2/engine/DebugDisplayToggleKeyDetector.cpp

.objs/arbarlith2/engine/DebugDisplayToggleKeyDetector.o: .deps/arbarlith2/engine/DebugDisplayToggleKeyDetector.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/DebugDisplayToggleKeyDetector.cpp -o .objs/arbarlith2/engine/DebugDisplayToggleKeyDetector.o

.deps/arbarlith2/engine/DebugLabel.d: arbarlith2/engine/DebugLabel.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/DebugLabel.d -MT .objs/arbarlith2/engine/DebugLabel.o $(default_INCS) arbarlith2/engine/DebugLabel.cpp

.objs/arbarlith2/engine/DebugLabel.o: .deps/arbarlith2/engine/DebugLabel.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/DebugLabel.cpp -o .objs/arbarlith2/engine/DebugLabel.o

.deps/arbarlith2/engine/Detector.d: arbarlith2/engine/Detector.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Detector.d -MT .objs/arbarlith2/engine/Detector.o $(default_INCS) arbarlith2/engine/Detector.cpp

.objs/arbarlith2/engine/Detector.o: .deps/arbarlith2/engine/Detector.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Detector.cpp -o .objs/arbarlith2/engine/Detector.o

.deps/arbarlith2/engine/DetectorProximity.d: arbarlith2/engine/DetectorProximity.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/DetectorProximity.d -MT .objs/arbarlith2/engine/DetectorProximity.o $(default_INCS) arbarlith2/engine/DetectorProximity.cpp

.objs/arbarlith2/engine/DetectorProximity.o: .deps/arbarlith2/engine/DetectorProximity.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/DetectorProximity.cpp -o .objs/arbarlith2/engine/DetectorProximity.o

.deps/arbarlith2/engine/DetectorProximityPlayer.d: arbarlith2/engine/DetectorProximityPlayer.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/DetectorProximityPlayer.d -MT .objs/arbarlith2/engine/DetectorProximityPlayer.o $(default_INCS) arbarlith2/engine/DetectorProximityPlayer.cpp

.objs/arbarlith2/engine/DetectorProximityPlayer.o: .deps/arbarlith2/engine/DetectorProximityPlayer.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/DetectorProximityPlayer.cpp -o .objs/arbarlith2/engine/DetectorProximityPlayer.o

.deps/arbarlith2/engine/Dimmer.d: arbarlith2/engine/Dimmer.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Dimmer.d -MT .objs/arbarlith2/engine/Dimmer.o $(default_INCS) arbarlith2/engine/Dimmer.cpp

.objs/arbarlith2/engine/Dimmer.o: .deps/arbarlith2/engine/Dimmer.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Dimmer.cpp -o .objs/arbarlith2/engine/Dimmer.o

.deps/arbarlith2/engine/EditWidget.d: arbarlith2/engine/EditWidget.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/EditWidget.d -MT .objs/arbarlith2/engine/EditWidget.o $(default_INCS) arbarlith2/engine/EditWidget.cpp

.objs/arbarlith2/engine/EditWidget.o: .deps/arbarlith2/engine/EditWidget.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/EditWidget.cpp -o .objs/arbarlith2/engine/EditWidget.o

.deps/arbarlith2/engine/EditorKeyDetector.d: arbarlith2/engine/EditorKeyDetector.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/EditorKeyDetector.d -MT .objs/arbarlith2/engine/EditorKeyDetector.o $(default_INCS) arbarlith2/engine/EditorKeyDetector.cpp

.objs/arbarlith2/engine/EditorKeyDetector.o: .deps/arbarlith2/engine/EditorKeyDetector.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/EditorKeyDetector.cpp -o .objs/arbarlith2/engine/EditorKeyDetector.o

.deps/arbarlith2/engine/EditorToolBar.d: arbarlith2/engine/EditorToolBar.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/EditorToolBar.d -MT .objs/arbarlith2/engine/EditorToolBar.o $(default_INCS) arbarlith2/engine/EditorToolBar.cpp

.objs/arbarlith2/engine/EditorToolBar.o: .deps/arbarlith2/engine/EditorToolBar.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/EditorToolBar.cpp -o .objs/arbarlith2/engine/EditorToolBar.o

.deps/arbarlith2/engine/Effect.d: arbarlith2/engine/Effect.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect.d -MT .objs/arbarlith2/engine/Effect.o $(default_INCS) arbarlith2/engine/Effect.cpp

.objs/arbarlith2/engine/Effect.o: .deps/arbarlith2/engine/Effect.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect.cpp -o .objs/arbarlith2/engine/Effect.o

.deps/arbarlith2/engine/EffectManager.d: arbarlith2/engine/EffectManager.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/EffectManager.d -MT .objs/arbarlith2/engine/EffectManager.o $(default_INCS) arbarlith2/engine/EffectManager.cpp

.objs/arbarlith2/engine/EffectManager.o: .deps/arbarlith2/engine/EffectManager.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/EffectManager.cpp -o .objs/arbarlith2/engine/EffectManager.o

.deps/arbarlith2/engine/Effect_GUI_FFP.d: arbarlith2/engine/Effect_GUI_FFP.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect_GUI_FFP.d -MT .objs/arbarlith2/engine/Effect_GUI_FFP.o $(default_INCS) arbarlith2/engine/Effect_GUI_FFP.cpp

.objs/arbarlith2/engine/Effect_GUI_FFP.o: .deps/arbarlith2/engine/Effect_GUI_FFP.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect_GUI_FFP.cpp -o .objs/arbarlith2/engine/Effect_GUI_FFP.o

.deps/arbarlith2/engine/Effect_Gouraud_FFP.d: arbarlith2/engine/Effect_Gouraud_FFP.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect_Gouraud_FFP.d -MT .objs/arbarlith2/engine/Effect_Gouraud_FFP.o $(default_INCS) arbarlith2/engine/Effect_Gouraud_FFP.cpp

.objs/arbarlith2/engine/Effect_Gouraud_FFP.o: .deps/arbarlith2/engine/Effect_Gouraud_FFP.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect_Gouraud_FFP.cpp -o .objs/arbarlith2/engine/Effect_Gouraud_FFP.o

.deps/arbarlith2/engine/Effect_Particle_FFP.d: arbarlith2/engine/Effect_Particle_FFP.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect_Particle_FFP.d -MT .objs/arbarlith2/engine/Effect_Particle_FFP.o $(default_INCS) arbarlith2/engine/Effect_Particle_FFP.cpp

.objs/arbarlith2/engine/Effect_Particle_FFP.o: .deps/arbarlith2/engine/Effect_Particle_FFP.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect_Particle_FFP.cpp -o .objs/arbarlith2/engine/Effect_Particle_FFP.o

.deps/arbarlith2/engine/Effect_Project_Shadows.d: arbarlith2/engine/Effect_Project_Shadows.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect_Project_Shadows.d -MT .objs/arbarlith2/engine/Effect_Project_Shadows.o $(default_INCS) arbarlith2/engine/Effect_Project_Shadows.cpp

.objs/arbarlith2/engine/Effect_Project_Shadows.o: .deps/arbarlith2/engine/Effect_Project_Shadows.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect_Project_Shadows.cpp -o .objs/arbarlith2/engine/Effect_Project_Shadows.o

.deps/arbarlith2/engine/Effect_Receive_Shadows.d: arbarlith2/engine/Effect_Receive_Shadows.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect_Receive_Shadows.d -MT .objs/arbarlith2/engine/Effect_Receive_Shadows.o $(default_INCS) arbarlith2/engine/Effect_Receive_Shadows.cpp

.objs/arbarlith2/engine/Effect_Receive_Shadows.o: .deps/arbarlith2/engine/Effect_Receive_Shadows.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect_Receive_Shadows.cpp -o .objs/arbarlith2/engine/Effect_Receive_Shadows.o

.deps/arbarlith2/engine/Effect_Red_FFP.d: arbarlith2/engine/Effect_Red_FFP.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect_Red_FFP.d -MT .objs/arbarlith2/engine/Effect_Red_FFP.o $(default_INCS) arbarlith2/engine/Effect_Red_FFP.cpp

.objs/arbarlith2/engine/Effect_Red_FFP.o: .deps/arbarlith2/engine/Effect_Red_FFP.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect_Red_FFP.cpp -o .objs/arbarlith2/engine/Effect_Red_FFP.o

.deps/arbarlith2/engine/Effect_TextureReplace_FFP.d: arbarlith2/engine/Effect_TextureReplace_FFP.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect_TextureReplace_FFP.d -MT .objs/arbarlith2/engine/Effect_TextureReplace_FFP.o $(default_INCS) arbarlith2/engine/Effect_TextureReplace_FFP.cpp

.objs/arbarlith2/engine/Effect_TextureReplace_FFP.o: .deps/arbarlith2/engine/Effect_TextureReplace_FFP.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect_TextureReplace_FFP.cpp -o .objs/arbarlith2/engine/Effect_TextureReplace_FFP.o

.deps/arbarlith2/engine/Effect_Z_Only.d: arbarlith2/engine/Effect_Z_Only.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Effect_Z_Only.d -MT .objs/arbarlith2/engine/Effect_Z_Only.o $(default_INCS) arbarlith2/engine/Effect_Z_Only.cpp

.objs/arbarlith2/engine/Effect_Z_Only.o: .deps/arbarlith2/engine/Effect_Z_Only.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Effect_Z_Only.cpp -o .objs/arbarlith2/engine/Effect_Z_Only.o

.deps/arbarlith2/engine/FPSDisplayToggleKeyDetector.d: arbarlith2/engine/FPSDisplayToggleKeyDetector.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/FPSDisplayToggleKeyDetector.d -MT .objs/arbarlith2/engine/FPSDisplayToggleKeyDetector.o $(default_INCS) arbarlith2/engine/FPSDisplayToggleKeyDetector.cpp

.objs/arbarlith2/engine/FPSDisplayToggleKeyDetector.o: .deps/arbarlith2/engine/FPSDisplayToggleKeyDetector.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/FPSDisplayToggleKeyDetector.cpp -o .objs/arbarlith2/engine/FPSDisplayToggleKeyDetector.o

.deps/arbarlith2/engine/FileSelectionList.d: arbarlith2/engine/FileSelectionList.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/FileSelectionList.d -MT .objs/arbarlith2/engine/FileSelectionList.o $(default_INCS) arbarlith2/engine/FileSelectionList.cpp

.objs/arbarlith2/engine/FileSelectionList.o: .deps/arbarlith2/engine/FileSelectionList.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/FileSelectionList.cpp -o .objs/arbarlith2/engine/FileSelectionList.o

.deps/arbarlith2/engine/GameStateCredits.d: arbarlith2/engine/GameStateCredits.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/GameStateCredits.d -MT .objs/arbarlith2/engine/GameStateCredits.o $(default_INCS) arbarlith2/engine/GameStateCredits.cpp

.objs/arbarlith2/engine/GameStateCredits.o: .deps/arbarlith2/engine/GameStateCredits.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/GameStateCredits.cpp -o .objs/arbarlith2/engine/GameStateCredits.o

.deps/arbarlith2/engine/GameStateEditor.d: arbarlith2/engine/GameStateEditor.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/GameStateEditor.d -MT .objs/arbarlith2/engine/GameStateEditor.o $(default_INCS) arbarlith2/engine/GameStateEditor.cpp

.objs/arbarlith2/engine/GameStateEditor.o: .deps/arbarlith2/engine/GameStateEditor.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/GameStateEditor.cpp -o .objs/arbarlith2/engine/GameStateEditor.o

.deps/arbarlith2/engine/GameStateMenu.d: arbarlith2/engine/GameStateMenu.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/GameStateMenu.d -MT .objs/arbarlith2/engine/GameStateMenu.o $(default_INCS) arbarlith2/engine/GameStateMenu.cpp

.objs/arbarlith2/engine/GameStateMenu.o: .deps/arbarlith2/engine/GameStateMenu.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/GameStateMenu.cpp -o .objs/arbarlith2/engine/GameStateMenu.o

.deps/arbarlith2/engine/GameStateRun.d: arbarlith2/engine/GameStateRun.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/GameStateRun.d -MT .objs/arbarlith2/engine/GameStateRun.o $(default_INCS) arbarlith2/engine/GameStateRun.cpp

.objs/arbarlith2/engine/GameStateRun.o: .deps/arbarlith2/engine/GameStateRun.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/GameStateRun.cpp -o .objs/arbarlith2/engine/GameStateRun.o

.deps/arbarlith2/engine/GameStateSpellMenu.d: arbarlith2/engine/GameStateSpellMenu.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/GameStateSpellMenu.d -MT .objs/arbarlith2/engine/GameStateSpellMenu.o $(default_INCS) arbarlith2/engine/GameStateSpellMenu.cpp

.objs/arbarlith2/engine/GameStateSpellMenu.o: .deps/arbarlith2/engine/GameStateSpellMenu.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/GameStateSpellMenu.cpp -o .objs/arbarlith2/engine/GameStateSpellMenu.o

.deps/arbarlith2/engine/ItemWidget.d: arbarlith2/engine/ItemWidget.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ItemWidget.d -MT .objs/arbarlith2/engine/ItemWidget.o $(default_INCS) arbarlith2/engine/ItemWidget.cpp

.objs/arbarlith2/engine/ItemWidget.o: .deps/arbarlith2/engine/ItemWidget.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ItemWidget.cpp -o .objs/arbarlith2/engine/ItemWidget.o

.deps/arbarlith2/engine/Key.d: arbarlith2/engine/Key.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Key.d -MT .objs/arbarlith2/engine/Key.o $(default_INCS) arbarlith2/engine/Key.cpp

.objs/arbarlith2/engine/Key.o: .deps/arbarlith2/engine/Key.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Key.cpp -o .objs/arbarlith2/engine/Key.o

.deps/arbarlith2/engine/Light.d: arbarlith2/engine/Light.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Light.d -MT .objs/arbarlith2/engine/Light.o $(default_INCS) arbarlith2/engine/Light.cpp

.objs/arbarlith2/engine/Light.o: .deps/arbarlith2/engine/Light.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Light.cpp -o .objs/arbarlith2/engine/Light.o

.deps/arbarlith2/engine/LightManager.d: arbarlith2/engine/LightManager.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/LightManager.d -MT .objs/arbarlith2/engine/LightManager.o $(default_INCS) arbarlith2/engine/LightManager.cpp

.objs/arbarlith2/engine/LightManager.o: .deps/arbarlith2/engine/LightManager.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/LightManager.cpp -o .objs/arbarlith2/engine/LightManager.o

.deps/arbarlith2/engine/ListElementWidget.d: arbarlith2/engine/ListElementWidget.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ListElementWidget.d -MT .objs/arbarlith2/engine/ListElementWidget.o $(default_INCS) arbarlith2/engine/ListElementWidget.cpp

.objs/arbarlith2/engine/ListElementWidget.o: .deps/arbarlith2/engine/ListElementWidget.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ListElementWidget.cpp -o .objs/arbarlith2/engine/ListElementWidget.o

.deps/arbarlith2/engine/ListPaneWidget.d: arbarlith2/engine/ListPaneWidget.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ListPaneWidget.d -MT .objs/arbarlith2/engine/ListPaneWidget.o $(default_INCS) arbarlith2/engine/ListPaneWidget.cpp

.objs/arbarlith2/engine/ListPaneWidget.o: .deps/arbarlith2/engine/ListPaneWidget.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ListPaneWidget.cpp -o .objs/arbarlith2/engine/ListPaneWidget.o

.deps/arbarlith2/engine/Listener.d: arbarlith2/engine/Listener.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Listener.d -MT .objs/arbarlith2/engine/Listener.o $(default_INCS) arbarlith2/engine/Listener.cpp

.objs/arbarlith2/engine/Listener.o: .deps/arbarlith2/engine/Listener.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Listener.cpp -o .objs/arbarlith2/engine/Listener.o

.deps/arbarlith2/engine/ListenerPrompt.d: arbarlith2/engine/ListenerPrompt.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ListenerPrompt.d -MT .objs/arbarlith2/engine/ListenerPrompt.o $(default_INCS) arbarlith2/engine/ListenerPrompt.cpp

.objs/arbarlith2/engine/ListenerPrompt.o: .deps/arbarlith2/engine/ListenerPrompt.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ListenerPrompt.cpp -o .objs/arbarlith2/engine/ListenerPrompt.o

.deps/arbarlith2/engine/Map.d: arbarlith2/engine/Map.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Map.d -MT .objs/arbarlith2/engine/Map.o $(default_INCS) arbarlith2/engine/Map.cpp

.objs/arbarlith2/engine/Map.o: .deps/arbarlith2/engine/Map.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Map.cpp -o .objs/arbarlith2/engine/Map.o

.deps/arbarlith2/engine/Md3Loader.d: arbarlith2/engine/Md3Loader.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Md3Loader.d -MT .objs/arbarlith2/engine/Md3Loader.o $(default_INCS) arbarlith2/engine/Md3Loader.cpp

.objs/arbarlith2/engine/Md3Loader.o: .deps/arbarlith2/engine/Md3Loader.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Md3Loader.cpp -o .objs/arbarlith2/engine/Md3Loader.o

.deps/arbarlith2/engine/Menu.d: arbarlith2/engine/Menu.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Menu.d -MT .objs/arbarlith2/engine/Menu.o $(default_INCS) arbarlith2/engine/Menu.cpp

.objs/arbarlith2/engine/Menu.o: .deps/arbarlith2/engine/Menu.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Menu.cpp -o .objs/arbarlith2/engine/Menu.o

.deps/arbarlith2/engine/MenuGameOver.d: arbarlith2/engine/MenuGameOver.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/MenuGameOver.d -MT .objs/arbarlith2/engine/MenuGameOver.o $(default_INCS) arbarlith2/engine/MenuGameOver.cpp

.objs/arbarlith2/engine/MenuGameOver.o: .deps/arbarlith2/engine/MenuGameOver.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/MenuGameOver.cpp -o .objs/arbarlith2/engine/MenuGameOver.o

.deps/arbarlith2/engine/MenuKeyDetector.d: arbarlith2/engine/MenuKeyDetector.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/MenuKeyDetector.d -MT .objs/arbarlith2/engine/MenuKeyDetector.o $(default_INCS) arbarlith2/engine/MenuKeyDetector.cpp

.objs/arbarlith2/engine/MenuKeyDetector.o: .deps/arbarlith2/engine/MenuKeyDetector.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/MenuKeyDetector.cpp -o .objs/arbarlith2/engine/MenuKeyDetector.o

.deps/arbarlith2/engine/MenuOptions.d: arbarlith2/engine/MenuOptions.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/MenuOptions.d -MT .objs/arbarlith2/engine/MenuOptions.o $(default_INCS) arbarlith2/engine/MenuOptions.cpp

.objs/arbarlith2/engine/MenuOptions.o: .deps/arbarlith2/engine/MenuOptions.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/MenuOptions.cpp -o .objs/arbarlith2/engine/MenuOptions.o

.deps/arbarlith2/engine/MenuWorldLeadOut.d: arbarlith2/engine/MenuWorldLeadOut.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/MenuWorldLeadOut.d -MT .objs/arbarlith2/engine/MenuWorldLeadOut.o $(default_INCS) arbarlith2/engine/MenuWorldLeadOut.cpp

.objs/arbarlith2/engine/MenuWorldLeadOut.o: .deps/arbarlith2/engine/MenuWorldLeadOut.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/MenuWorldLeadOut.cpp -o .objs/arbarlith2/engine/MenuWorldLeadOut.o

.deps/arbarlith2/engine/MenuWorldSelection.d: arbarlith2/engine/MenuWorldSelection.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/MenuWorldSelection.d -MT .objs/arbarlith2/engine/MenuWorldSelection.o $(default_INCS) arbarlith2/engine/MenuWorldSelection.cpp

.objs/arbarlith2/engine/MenuWorldSelection.o: .deps/arbarlith2/engine/MenuWorldSelection.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/MenuWorldSelection.cpp -o .objs/arbarlith2/engine/MenuWorldSelection.o

.deps/arbarlith2/engine/MessageRouter.d: arbarlith2/engine/MessageRouter.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/MessageRouter.d -MT .objs/arbarlith2/engine/MessageRouter.o $(default_INCS) arbarlith2/engine/MessageRouter.cpp

.objs/arbarlith2/engine/MessageRouter.o: .deps/arbarlith2/engine/MessageRouter.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/MessageRouter.cpp -o .objs/arbarlith2/engine/MessageRouter.o

.deps/arbarlith2/engine/ModelLoader.d: arbarlith2/engine/ModelLoader.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ModelLoader.d -MT .objs/arbarlith2/engine/ModelLoader.o $(default_INCS) arbarlith2/engine/ModelLoader.cpp

.objs/arbarlith2/engine/ModelLoader.o: .deps/arbarlith2/engine/ModelLoader.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ModelLoader.cpp -o .objs/arbarlith2/engine/ModelLoader.o

.deps/arbarlith2/engine/MonsterFSM.d: arbarlith2/engine/MonsterFSM.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/MonsterFSM.d -MT .objs/arbarlith2/engine/MonsterFSM.o $(default_INCS) arbarlith2/engine/MonsterFSM.cpp

.objs/arbarlith2/engine/MonsterFSM.o: .deps/arbarlith2/engine/MonsterFSM.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/MonsterFSM.cpp -o .objs/arbarlith2/engine/MonsterFSM.o

.deps/arbarlith2/engine/MusicEngine.d: arbarlith2/engine/MusicEngine.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/MusicEngine.d -MT .objs/arbarlith2/engine/MusicEngine.o $(default_INCS) arbarlith2/engine/MusicEngine.cpp

.objs/arbarlith2/engine/MusicEngine.o: .deps/arbarlith2/engine/MusicEngine.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/MusicEngine.cpp -o .objs/arbarlith2/engine/MusicEngine.o

.deps/arbarlith2/engine/PerformanceLabel.d: arbarlith2/engine/PerformanceLabel.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/PerformanceLabel.d -MT .objs/arbarlith2/engine/PerformanceLabel.o $(default_INCS) arbarlith2/engine/PerformanceLabel.cpp

.objs/arbarlith2/engine/PerformanceLabel.o: .deps/arbarlith2/engine/PerformanceLabel.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/PerformanceLabel.cpp -o .objs/arbarlith2/engine/PerformanceLabel.o

.deps/arbarlith2/engine/PropertyBag.d: arbarlith2/engine/PropertyBag.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/PropertyBag.d -MT .objs/arbarlith2/engine/PropertyBag.o $(default_INCS) arbarlith2/engine/PropertyBag.cpp

.objs/arbarlith2/engine/PropertyBag.o: .deps/arbarlith2/engine/PropertyBag.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/PropertyBag.cpp -o .objs/arbarlith2/engine/PropertyBag.o

.deps/arbarlith2/engine/QuadTreeNode.d: arbarlith2/engine/QuadTreeNode.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/QuadTreeNode.d -MT .objs/arbarlith2/engine/QuadTreeNode.o $(default_INCS) arbarlith2/engine/QuadTreeNode.cpp

.objs/arbarlith2/engine/QuadTreeNode.o: .deps/arbarlith2/engine/QuadTreeNode.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/QuadTreeNode.cpp -o .objs/arbarlith2/engine/QuadTreeNode.o

.deps/arbarlith2/engine/SDLwindow.d: arbarlith2/engine/SDLwindow.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/SDLwindow.d -MT .objs/arbarlith2/engine/SDLwindow.o $(default_INCS) arbarlith2/engine/SDLwindow.cpp

.objs/arbarlith2/engine/SDLwindow.o: .deps/arbarlith2/engine/SDLwindow.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/SDLwindow.cpp -o .objs/arbarlith2/engine/SDLwindow.o

.deps/arbarlith2/engine/ScreenShotTask.d: arbarlith2/engine/ScreenShotTask.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ScreenShotTask.d -MT .objs/arbarlith2/engine/ScreenShotTask.o $(default_INCS) arbarlith2/engine/ScreenShotTask.cpp

.objs/arbarlith2/engine/ScreenShotTask.o: .deps/arbarlith2/engine/ScreenShotTask.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ScreenShotTask.cpp -o .objs/arbarlith2/engine/ScreenShotTask.o

.deps/arbarlith2/engine/SelectionList.d: arbarlith2/engine/SelectionList.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/SelectionList.d -MT .objs/arbarlith2/engine/SelectionList.o $(default_INCS) arbarlith2/engine/SelectionList.cpp

.objs/arbarlith2/engine/SelectionList.o: .deps/arbarlith2/engine/SelectionList.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/SelectionList.cpp -o .objs/arbarlith2/engine/SelectionList.o

.deps/arbarlith2/engine/Shadow.d: arbarlith2/engine/Shadow.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Shadow.d -MT .objs/arbarlith2/engine/Shadow.o $(default_INCS) arbarlith2/engine/Shadow.cpp

.objs/arbarlith2/engine/Shadow.o: .deps/arbarlith2/engine/Shadow.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Shadow.cpp -o .objs/arbarlith2/engine/Shadow.o

.deps/arbarlith2/engine/ShadowManager.d: arbarlith2/engine/ShadowManager.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ShadowManager.d -MT .objs/arbarlith2/engine/ShadowManager.o $(default_INCS) arbarlith2/engine/ShadowManager.cpp

.objs/arbarlith2/engine/ShadowManager.o: .deps/arbarlith2/engine/ShadowManager.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ShadowManager.cpp -o .objs/arbarlith2/engine/ShadowManager.o

.deps/arbarlith2/engine/SoundSystem.d: arbarlith2/engine/SoundSystem.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/SoundSystem.d -MT .objs/arbarlith2/engine/SoundSystem.o $(default_INCS) arbarlith2/engine/SoundSystem.cpp

.objs/arbarlith2/engine/SoundSystem.o: .deps/arbarlith2/engine/SoundSystem.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/SoundSystem.cpp -o .objs/arbarlith2/engine/SoundSystem.o

.deps/arbarlith2/engine/SpellMenuKeyDetector.d: arbarlith2/engine/SpellMenuKeyDetector.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/SpellMenuKeyDetector.d -MT .objs/arbarlith2/engine/SpellMenuKeyDetector.o $(default_INCS) arbarlith2/engine/SpellMenuKeyDetector.cpp

.objs/arbarlith2/engine/SpellMenuKeyDetector.o: .deps/arbarlith2/engine/SpellMenuKeyDetector.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/SpellMenuKeyDetector.cpp -o .objs/arbarlith2/engine/SpellMenuKeyDetector.o

.deps/arbarlith2/engine/SplashScreen.d: arbarlith2/engine/SplashScreen.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/SplashScreen.d -MT .objs/arbarlith2/engine/SplashScreen.o $(default_INCS) arbarlith2/engine/SplashScreen.cpp

.objs/arbarlith2/engine/SplashScreen.o: .deps/arbarlith2/engine/SplashScreen.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/SplashScreen.cpp -o .objs/arbarlith2/engine/SplashScreen.o

.deps/arbarlith2/engine/StateMachine.d: arbarlith2/engine/StateMachine.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/StateMachine.d -MT .objs/arbarlith2/engine/StateMachine.o $(default_INCS) arbarlith2/engine/StateMachine.cpp

.objs/arbarlith2/engine/StateMachine.o: .deps/arbarlith2/engine/StateMachine.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/StateMachine.cpp -o .objs/arbarlith2/engine/StateMachine.o

.deps/arbarlith2/engine/StateMachineFactory.d: arbarlith2/engine/StateMachineFactory.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/StateMachineFactory.d -MT .objs/arbarlith2/engine/StateMachineFactory.o $(default_INCS) arbarlith2/engine/StateMachineFactory.cpp

.objs/arbarlith2/engine/StateMachineFactory.o: .deps/arbarlith2/engine/StateMachineFactory.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/StateMachineFactory.cpp -o .objs/arbarlith2/engine/StateMachineFactory.o

.deps/arbarlith2/engine/Switch.d: arbarlith2/engine/Switch.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Switch.d -MT .objs/arbarlith2/engine/Switch.o $(default_INCS) arbarlith2/engine/Switch.cpp

.objs/arbarlith2/engine/Switch.o: .deps/arbarlith2/engine/Switch.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Switch.cpp -o .objs/arbarlith2/engine/Switch.o

.deps/arbarlith2/engine/TextureHandle.d: arbarlith2/engine/TextureHandle.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/TextureHandle.d -MT .objs/arbarlith2/engine/TextureHandle.o $(default_INCS) arbarlith2/engine/TextureHandle.cpp

.objs/arbarlith2/engine/TextureHandle.o: .deps/arbarlith2/engine/TextureHandle.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/TextureHandle.cpp -o .objs/arbarlith2/engine/TextureHandle.o

.deps/arbarlith2/engine/TextureManager.d: arbarlith2/engine/TextureManager.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/TextureManager.d -MT .objs/arbarlith2/engine/TextureManager.o $(default_INCS) arbarlith2/engine/TextureManager.cpp

.objs/arbarlith2/engine/TextureManager.o: .deps/arbarlith2/engine/TextureManager.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/TextureManager.cpp -o .objs/arbarlith2/engine/TextureManager.o

.deps/arbarlith2/engine/Tile.d: arbarlith2/engine/Tile.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Tile.d -MT .objs/arbarlith2/engine/Tile.o $(default_INCS) arbarlith2/engine/Tile.cpp

.objs/arbarlith2/engine/Tile.o: .deps/arbarlith2/engine/Tile.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Tile.cpp -o .objs/arbarlith2/engine/Tile.o

.deps/arbarlith2/engine/ToggleWidget.d: arbarlith2/engine/ToggleWidget.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ToggleWidget.d -MT .objs/arbarlith2/engine/ToggleWidget.o $(default_INCS) arbarlith2/engine/ToggleWidget.cpp

.objs/arbarlith2/engine/ToggleWidget.o: .deps/arbarlith2/engine/ToggleWidget.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ToggleWidget.cpp -o .objs/arbarlith2/engine/ToggleWidget.o

.deps/arbarlith2/engine/ToggleWidgetText.d: arbarlith2/engine/ToggleWidgetText.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ToggleWidgetText.d -MT .objs/arbarlith2/engine/ToggleWidgetText.o $(default_INCS) arbarlith2/engine/ToggleWidgetText.cpp

.objs/arbarlith2/engine/ToggleWidgetText.o: .deps/arbarlith2/engine/ToggleWidgetText.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ToggleWidgetText.cpp -o .objs/arbarlith2/engine/ToggleWidgetText.o

.deps/arbarlith2/engine/ToolBarForEditorTools.d: arbarlith2/engine/ToolBarForEditorTools.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/ToolBarForEditorTools.d -MT .objs/arbarlith2/engine/ToolBarForEditorTools.o $(default_INCS) arbarlith2/engine/ToolBarForEditorTools.cpp

.objs/arbarlith2/engine/ToolBarForEditorTools.o: .deps/arbarlith2/engine/ToolBarForEditorTools.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/ToolBarForEditorTools.cpp -o .objs/arbarlith2/engine/ToolBarForEditorTools.o

.deps/arbarlith2/engine/Trigger.d: arbarlith2/engine/Trigger.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/Trigger.d -MT .objs/arbarlith2/engine/Trigger.o $(default_INCS) arbarlith2/engine/Trigger.cpp

.objs/arbarlith2/engine/Trigger.o: .deps/arbarlith2/engine/Trigger.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/Trigger.cpp -o .objs/arbarlith2/engine/Trigger.o

.deps/arbarlith2/engine/TriggerParticles.d: arbarlith2/engine/TriggerParticles.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/TriggerParticles.d -MT .objs/arbarlith2/engine/TriggerParticles.o $(default_INCS) arbarlith2/engine/TriggerParticles.cpp

.objs/arbarlith2/engine/TriggerParticles.o: .deps/arbarlith2/engine/TriggerParticles.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/TriggerParticles.cpp -o .objs/arbarlith2/engine/TriggerParticles.o

.deps/arbarlith2/engine/TriggerPrompt.d: arbarlith2/engine/TriggerPrompt.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/TriggerPrompt.d -MT .objs/arbarlith2/engine/TriggerPrompt.o $(default_INCS) arbarlith2/engine/TriggerPrompt.cpp

.objs/arbarlith2/engine/TriggerPrompt.o: .deps/arbarlith2/engine/TriggerPrompt.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/TriggerPrompt.cpp -o .objs/arbarlith2/engine/TriggerPrompt.o

.deps/arbarlith2/engine/VisualPriority.d: arbarlith2/engine/VisualPriority.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/VisualPriority.d -MT .objs/arbarlith2/engine/VisualPriority.o $(default_INCS) arbarlith2/engine/VisualPriority.cpp

.objs/arbarlith2/engine/VisualPriority.o: .deps/arbarlith2/engine/VisualPriority.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/VisualPriority.cpp -o .objs/arbarlith2/engine/VisualPriority.o

.deps/arbarlith2/engine/WaitScreen.d: arbarlith2/engine/WaitScreen.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/WaitScreen.d -MT .objs/arbarlith2/engine/WaitScreen.o $(default_INCS) arbarlith2/engine/WaitScreen.cpp

.objs/arbarlith2/engine/WaitScreen.o: .deps/arbarlith2/engine/WaitScreen.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/WaitScreen.cpp -o .objs/arbarlith2/engine/WaitScreen.o

.deps/arbarlith2/engine/animation.d: arbarlith2/engine/animation.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/animation.d -MT .objs/arbarlith2/engine/animation.o $(default_INCS) arbarlith2/engine/animation.cpp

.objs/arbarlith2/engine/animation.o: .deps/arbarlith2/engine/animation.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/animation.cpp -o .objs/arbarlith2/engine/animation.o

.deps/arbarlith2/engine/application.d: arbarlith2/engine/application.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/application.d -MT .objs/arbarlith2/engine/application.o $(default_INCS) arbarlith2/engine/application.cpp

.objs/arbarlith2/engine/application.o: .deps/arbarlith2/engine/application.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/application.cpp -o .objs/arbarlith2/engine/application.o

.deps/arbarlith2/engine/camera.d: arbarlith2/engine/camera.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/camera.d -MT .objs/arbarlith2/engine/camera.o $(default_INCS) arbarlith2/engine/camera.cpp

.objs/arbarlith2/engine/camera.o: .deps/arbarlith2/engine/camera.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/camera.cpp -o .objs/arbarlith2/engine/camera.o

.deps/arbarlith2/engine/creature.d: arbarlith2/engine/creature.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/creature.d -MT .objs/arbarlith2/engine/creature.o $(default_INCS) arbarlith2/engine/creature.cpp

.objs/arbarlith2/engine/creature.o: .deps/arbarlith2/engine/creature.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/creature.cpp -o .objs/arbarlith2/engine/creature.o

.deps/arbarlith2/engine/file.d: arbarlith2/engine/file.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/file.d -MT .objs/arbarlith2/engine/file.o $(default_INCS) arbarlith2/engine/file.cpp

.objs/arbarlith2/engine/file.o: .deps/arbarlith2/engine/file.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/file.cpp -o .objs/arbarlith2/engine/file.o

.deps/arbarlith2/engine/fog.d: arbarlith2/engine/fog.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/fog.d -MT .objs/arbarlith2/engine/fog.o $(default_INCS) arbarlith2/engine/fog.cpp

.objs/arbarlith2/engine/fog.o: .deps/arbarlith2/engine/fog.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/fog.cpp -o .objs/arbarlith2/engine/fog.o

.deps/arbarlith2/engine/frustum.d: arbarlith2/engine/frustum.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/frustum.d -MT .objs/arbarlith2/engine/frustum.o $(default_INCS) arbarlith2/engine/frustum.cpp

.objs/arbarlith2/engine/frustum.o: .deps/arbarlith2/engine/frustum.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/frustum.cpp -o .objs/arbarlith2/engine/frustum.o

.deps/arbarlith2/engine/horizbar.d: arbarlith2/engine/horizbar.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/horizbar.d -MT .objs/arbarlith2/engine/horizbar.o $(default_INCS) arbarlith2/engine/horizbar.cpp

.objs/arbarlith2/engine/horizbar.o: .deps/arbarlith2/engine/horizbar.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/horizbar.cpp -o .objs/arbarlith2/engine/horizbar.o

.deps/arbarlith2/engine/image.d: arbarlith2/engine/image.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/image.d -MT .objs/arbarlith2/engine/image.o $(default_INCS) arbarlith2/engine/image.cpp

.objs/arbarlith2/engine/image.o: .deps/arbarlith2/engine/image.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/image.cpp -o .objs/arbarlith2/engine/image.o

.deps/arbarlith2/engine/item.d: arbarlith2/engine/item.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/item.d -MT .objs/arbarlith2/engine/item.o $(default_INCS) arbarlith2/engine/item.cpp

.objs/arbarlith2/engine/item.o: .deps/arbarlith2/engine/item.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/item.cpp -o .objs/arbarlith2/engine/item.o

.deps/arbarlith2/engine/keyframe.d: arbarlith2/engine/keyframe.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/keyframe.d -MT .objs/arbarlith2/engine/keyframe.o $(default_INCS) arbarlith2/engine/keyframe.cpp

.objs/arbarlith2/engine/keyframe.o: .deps/arbarlith2/engine/keyframe.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/keyframe.cpp -o .objs/arbarlith2/engine/keyframe.o

.deps/arbarlith2/engine/labelwidget.d: arbarlith2/engine/labelwidget.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/labelwidget.d -MT .objs/arbarlith2/engine/labelwidget.o $(default_INCS) arbarlith2/engine/labelwidget.cpp

.objs/arbarlith2/engine/labelwidget.o: .deps/arbarlith2/engine/labelwidget.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/labelwidget.cpp -o .objs/arbarlith2/engine/labelwidget.o

.deps/arbarlith2/engine/lamp.d: arbarlith2/engine/lamp.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/lamp.d -MT .objs/arbarlith2/engine/lamp.o $(default_INCS) arbarlith2/engine/lamp.cpp

.objs/arbarlith2/engine/lamp.o: .deps/arbarlith2/engine/lamp.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/lamp.cpp -o .objs/arbarlith2/engine/lamp.o

.deps/arbarlith2/engine/logstring.d: arbarlith2/engine/logstring.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/logstring.d -MT .objs/arbarlith2/engine/logstring.o $(default_INCS) arbarlith2/engine/logstring.cpp

.objs/arbarlith2/engine/logstring.o: .deps/arbarlith2/engine/logstring.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/logstring.cpp -o .objs/arbarlith2/engine/logstring.o

.deps/arbarlith2/engine/mat4.d: arbarlith2/engine/mat4.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/mat4.d -MT .objs/arbarlith2/engine/mat4.o $(default_INCS) arbarlith2/engine/mat4.cpp

.objs/arbarlith2/engine/mat4.o: .deps/arbarlith2/engine/mat4.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/mat4.cpp -o .objs/arbarlith2/engine/mat4.o

.deps/arbarlith2/engine/material.d: arbarlith2/engine/material.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/material.d -MT .objs/arbarlith2/engine/material.o $(default_INCS) arbarlith2/engine/material.cpp

.objs/arbarlith2/engine/material.o: .deps/arbarlith2/engine/material.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/material.cpp -o .objs/arbarlith2/engine/material.o

.deps/arbarlith2/engine/mesh.d: arbarlith2/engine/mesh.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/mesh.d -MT .objs/arbarlith2/engine/mesh.o $(default_INCS) arbarlith2/engine/mesh.cpp

.objs/arbarlith2/engine/mesh.o: .deps/arbarlith2/engine/mesh.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/mesh.cpp -o .objs/arbarlith2/engine/mesh.o

.deps/arbarlith2/engine/myassert.d: arbarlith2/engine/myassert.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/myassert.d -MT .objs/arbarlith2/engine/myassert.o $(default_INCS) arbarlith2/engine/myassert.cpp

.objs/arbarlith2/engine/myassert.o: .deps/arbarlith2/engine/myassert.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/myassert.cpp -o .objs/arbarlith2/engine/myassert.o

.deps/arbarlith2/engine/object.d: arbarlith2/engine/object.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/object.d -MT .objs/arbarlith2/engine/object.o $(default_INCS) arbarlith2/engine/object.cpp

.objs/arbarlith2/engine/object.o: .deps/arbarlith2/engine/object.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/object.cpp -o .objs/arbarlith2/engine/object.o

.deps/arbarlith2/engine/opengl.d: arbarlith2/engine/opengl.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/opengl.d -MT .objs/arbarlith2/engine/opengl.o $(default_INCS) arbarlith2/engine/opengl.cpp

.objs/arbarlith2/engine/opengl.o: .deps/arbarlith2/engine/opengl.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/opengl.cpp -o .objs/arbarlith2/engine/opengl.o

.deps/arbarlith2/engine/particle.d: arbarlith2/engine/particle.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/particle.d -MT .objs/arbarlith2/engine/particle.o $(default_INCS) arbarlith2/engine/particle.cpp

.objs/arbarlith2/engine/particle.o: .deps/arbarlith2/engine/particle.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/particle.cpp -o .objs/arbarlith2/engine/particle.o

.deps/arbarlith2/engine/player.d: arbarlith2/engine/player.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/player.d -MT .objs/arbarlith2/engine/player.o $(default_INCS) arbarlith2/engine/player.cpp

.objs/arbarlith2/engine/player.o: .deps/arbarlith2/engine/player.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/player.cpp -o .objs/arbarlith2/engine/player.o

.deps/arbarlith2/engine/primitivedatatypes.d: arbarlith2/engine/primitivedatatypes.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/primitivedatatypes.d -MT .objs/arbarlith2/engine/primitivedatatypes.o $(default_INCS) arbarlith2/engine/primitivedatatypes.cpp

.objs/arbarlith2/engine/primitivedatatypes.o: .deps/arbarlith2/engine/primitivedatatypes.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/primitivedatatypes.cpp -o .objs/arbarlith2/engine/primitivedatatypes.o

.deps/arbarlith2/engine/profile.d: arbarlith2/engine/profile.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/profile.d -MT .objs/arbarlith2/engine/profile.o $(default_INCS) arbarlith2/engine/profile.cpp

.objs/arbarlith2/engine/profile.o: .deps/arbarlith2/engine/profile.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/profile.cpp -o .objs/arbarlith2/engine/profile.o

.deps/arbarlith2/engine/searchfile.d: arbarlith2/engine/searchfile.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/searchfile.d -MT .objs/arbarlith2/engine/searchfile.o $(default_INCS) arbarlith2/engine/searchfile.cpp

.objs/arbarlith2/engine/searchfile.o: .deps/arbarlith2/engine/searchfile.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/searchfile.cpp -o .objs/arbarlith2/engine/searchfile.o

.deps/arbarlith2/engine/stdafx.d: arbarlith2/engine/stdafx.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/stdafx.d -MT .objs/arbarlith2/engine/stdafx.o $(default_INCS) arbarlith2/engine/stdafx.cpp

.objs/arbarlith2/engine/stdafx.o: .deps/arbarlith2/engine/stdafx.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/stdafx.cpp -o .objs/arbarlith2/engine/stdafx.o

.deps/arbarlith2/engine/text.d: arbarlith2/engine/text.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/text.d -MT .objs/arbarlith2/engine/text.o $(default_INCS) arbarlith2/engine/text.cpp

.objs/arbarlith2/engine/text.o: .deps/arbarlith2/engine/text.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/text.cpp -o .objs/arbarlith2/engine/text.o

.deps/arbarlith2/engine/timer.d: arbarlith2/engine/timer.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/timer.d -MT .objs/arbarlith2/engine/timer.o $(default_INCS) arbarlith2/engine/timer.cpp

.objs/arbarlith2/engine/timer.o: .deps/arbarlith2/engine/timer.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/timer.cpp -o .objs/arbarlith2/engine/timer.o

.deps/arbarlith2/engine/tstring.d: arbarlith2/engine/tstring.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/tstring.d -MT .objs/arbarlith2/engine/tstring.o $(default_INCS) arbarlith2/engine/tstring.cpp

.objs/arbarlith2/engine/tstring.o: .deps/arbarlith2/engine/tstring.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/tstring.cpp -o .objs/arbarlith2/engine/tstring.o

.deps/arbarlith2/engine/widget.d: arbarlith2/engine/widget.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/widget.d -MT .objs/arbarlith2/engine/widget.o $(default_INCS) arbarlith2/engine/widget.cpp

.objs/arbarlith2/engine/widget.o: .deps/arbarlith2/engine/widget.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/widget.cpp -o .objs/arbarlith2/engine/widget.o

.deps/arbarlith2/engine/widgetmanager.d: arbarlith2/engine/widgetmanager.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/widgetmanager.d -MT .objs/arbarlith2/engine/widgetmanager.o $(default_INCS) arbarlith2/engine/widgetmanager.cpp

.objs/arbarlith2/engine/widgetmanager.o: .deps/arbarlith2/engine/widgetmanager.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/widgetmanager.cpp -o .objs/arbarlith2/engine/widgetmanager.o

.deps/arbarlith2/engine/windowwidget.d: arbarlith2/engine/windowwidget.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/windowwidget.d -MT .objs/arbarlith2/engine/windowwidget.o $(default_INCS) arbarlith2/engine/windowwidget.cpp

.objs/arbarlith2/engine/windowwidget.o: .deps/arbarlith2/engine/windowwidget.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/windowwidget.cpp -o .objs/arbarlith2/engine/windowwidget.o

.deps/arbarlith2/engine/world.d: arbarlith2/engine/world.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/engine/world.d -MT .objs/arbarlith2/engine/world.o $(default_INCS) arbarlith2/engine/world.cpp

.objs/arbarlith2/engine/world.o: .deps/arbarlith2/engine/world.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/engine/world.cpp -o .objs/arbarlith2/engine/world.o

.deps/arbarlith2/win32.d: arbarlith2/win32.cpp
	$(default_CPP) -MM $(default_CFLAGS) -MF .deps/arbarlith2/win32.d -MT .objs/arbarlith2/win32.o $(default_INCS) arbarlith2/win32.cpp

.objs/arbarlith2/win32.o: .deps/arbarlith2/win32.d
	$(default_CPP) $(default_CFLAGS) $(default_INCS) -c arbarlith2/win32.cpp -o .objs/arbarlith2/win32.o



