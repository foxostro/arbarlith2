/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

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

#include "stdafx.h" // Master Header
#include <cmath>
#include "gl.h"
#include "profile.h"

#include "mat4.h"

#include "Tile.h"
#include "Map.h"
#include "World.h"
#include "Object.h"
#include "Creature.h"
#include "Player.h"

#include "Object.h"
#include "Frustum.h"

#include "ListElementTweaker.h"
#include "ToggleWidget.h"
#include "ToggleWidgetText.h"

#include "GameStateEditor.h"

#include "Md3Loader.h"
#include "3dsLoader.h"



namespace Engine {

vec3 Project(const vec3 &p); // stdafx.cpp

const _tstring DEFAULT_MODEL = _T("data/cylinder/model.md3xml");


AnimationController* createAnimationController(const _tstring &fileName)
{
	AnimationController *controller = 0;

	if(!fileName.empty())
	{
		_tstring fileExtension = toLowerCase(File::getExtension(fileName));

		if(fileExtension == _T(".3dsxml"))
		{
			_3dsLoader _3ds;
			controller = _3ds.load(fileName);
		}
		else if(fileExtension == _T(".md3xml"))
		{
			Md3Loader md3;
			controller = md3.load(fileName);
		}
		else
		{
			ERR(_T("Unknown model format! Expected *.md3xml or *.3dsxml; instead got ") + fileName);
		}
	}

	return controller;
}







GEN_ACTOR_RTTI_CPP(Actor, "class Engine::Actor")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Actor::Actor(const OBJECT_ID ID)
: m_ID(ID)
{
	clear();
}

Actor::~Actor(void)
{
	destroy();
}

//////////////////////////////////////////////////////////////////////
// Member functions
//////////////////////////////////////////////////////////////////////

void Actor::clear(void)
{
	zombie				= false;
	m_Mass				= 0.0f;
	topSpeed			= 2.0f;
	m_Scale				= 1.0f;
	m_strName			= _T("Please name this object!");
	cylinderRadius			= 0.0f;
	sphereRadius			= 0.0f;
	isLit				= true;
	m_desiredHeight			= 0.0f;
	showModel			= true;
	solid				= true;
	myEffect			= effect_TEXTURE_LIT; // default
	floating			= false;
	slidOnWall			= false;
	frictionAcceleration		= 32.0f;
	minWalkingVelocity		= 0.5f;
	m_pModel                        = 0;

	hasMoved			= true;
	hasAnimated			= true;
	castShadows			= true;

	orientation.identity();
	validatedPos.zero();
	position.zero();
	velocity.zero();
}

void Actor::destroy(void)
{
	clear();
}

mat4 Actor::toWorldSpace(void) const
{
	vec3 x = getOrientation().getAxisZ();
	vec3 y = getOrientation().getAxisY();
	vec3 z = getOrientation().getAxisX();

	// Build the matrix
	mat4 mat;
	mat.identity();
	mat.setAxisX(x);
	mat.setAxisY(y);
	mat.setAxisZ(z);

	mat4 rotateIt;
	rotateIt.identity();
	rotateIt.rotateY((float)M_PI / 2);

	mat = (mat * rotateIt) * m_Scale;

	// Place the mesh
	mat.setPos(getPos());
	mat.m[15] = 1.0f;

	return mat;
}

void Actor::lookAt(const vec3 &center)
{
	const vec3 z = vec3(position.x - center.x, 0, position.z - center.z).getNormal();
	const vec3 y = vec3(0, 1, 0);
	const vec3 x = y.cross(z);
	orientation.set(vec3(0,0,0), x, y, z);
}

void Actor::LoadModel(const _tstring &fileName)
{
	m_pModel = createAnimationController(fileName);

	// Error check
	if(m_pModel == 0)
	{
		// Failure
		ERR(_tstring(_T("Failed to load model file: ")) + fileName);

		// Try a placeholder instead
		m_pModel = createAnimationController(DEFAULT_MODEL);

		ASSERT(m_pModel!=0,	_T("failed to load model and failed to load placeholder"));

		if(m_pModel==0)
		{
			ERR(_T("failed to load model and failed to load placeholder"));
		}
		else
		{
			m_strModelFilename = DEFAULT_MODEL;
		}
	}
	else
	{
		// Record the model filename
		m_strModelFilename = fileName;
	}

	// Calculate the scale from the desired height and the raw model height
	setHeight(m_desiredHeight);

	// Use the idle animation until another is selected
	ChangeAnimation(_T("idle"));
}

void Actor::Place(const vec3 &pos)
{
	// Set the position
	spawnPoint = validatedPos = position = pos;

	RecordValidatedPos();

	OnPlace();
}

void Actor::OnPlace(void)
{}

void Actor::updateForEditor(float)
{
	sync();

	// Set our elevation to that of the tile we are standing on
	if(!floating && getZone().getMap().onATile(position.x, position.z))
	{
		const Tile& tile = getZone().getMap().getTile(position.x, position.z);
		position.y = tile.getTileHeight();
	}

	// Reset response flags
	hasMoved = true;
	hasAnimated = true;
}

void Actor::update(float milliseconds)
{
	float timeStep = milliseconds/1000.f;

	// Reset response flags
	hasMoved = false;
	hasAnimated = false;
	slidOnWall = false;

	// If there is a model for this object
	if(m_pModel!=0)
	{
		m_pModel->update(milliseconds);
		hasAnimated=true;
	}

	// Eliminate any vertical component of velocity
	velocity.y=0;

	if(velocity.getMagnitude() > FLT_EPSILON)
	{
		// Integrate position over time
		position = position + velocity*timeStep;
		hasMoved = true;
	}
	else
	{
		hasMoved = false;
		velocity.zero();
	}

	// Apply friction
	float frictionalDeltaVelocity = frictionAcceleration*timeStep;
	if(frictionalDeltaVelocity > FLT_EPSILON)
	{
		if(frictionalDeltaVelocity < velocity.getMagnitude())
		{
			velocity = velocity - (velocity.getNormal()*frictionalDeltaVelocity);
		}
		else
		{
			velocity.zero();
		}
	}

	// Set our elevation to that of the tile we are standing on
	if(!floating && getZone().getMap().onATile(position.x, position.z))
	{
		const Tile& tile = getZone().getMap().getTile(position.x, position.z);
		position.y = tile.getTileHeight();
	}

	// Prevent the actor from traveling through walls
	if(isSolid())
	{
		slideAgainstWalls();
	}
}

void Actor::setHeight(float h)
{
	this->m_desiredHeight = h;

	if(m_pModel != 0)
	{
		// Calculate the scale from the desired height and the raw model height
		float height = m_pModel->calculateHeight(0.0f);

		if(height==0)
		{
			TRACE(_tstring(_T("Height was zero, but we'll just pretend its merely tiny: ")) + m_strModelFilename);
			height = 0.01f;
		}

		m_Scale = m_desiredHeight / height;
	}

	// Calculated the model radius
	calculateRadius();
}

void Actor::calculateRadius(void)
{
	// Calculate initial collision detection parameters
	if(m_pModel != 0)
	{
		const float time = 0.0f;

		// Calculate spherical radius
		sphereRadius = getScale() * m_pModel->calculateRadius(time);

		// Calculate Cylindrical radius
		cylinderRadius = getScale() * m_pModel->calculateCylindricalRadius(time);
	}
}

struct collision
{
	vec3 normal;
	vec3 pt;
};


void Actor::DoCollisionDetection(const ActorSet &s)
{
	PROFILE

	m_Collisions = getCollisions(s);
}

void Actor::DoCollisionResponse(void)
{
	PROFILE

	// Exchange collision response messages
	for(list<Actor*>::iterator iter=m_Collisions.begin(); iter!=m_Collisions.end(); ++iter)
	{
		Actor &a = **iter;
		a.OnCollision(*this);
		OnCollision(a);
	}

	// If there was no collision, then the position is valid
	if(m_Collisions.empty())
		RecordValidatedPos();
	else
		RevertToValidatedPos();
}

void Actor::slideAgainstWalls(void)
{
	slidOnWall = false;

	Map &m = getZone().getMap();

	// Get our current position (tile coordinates)
	int x = m.tileX(position.x);
	int z = m.tileX(position.z);

	// Bail out if we aren't even over a tile
	if(!m.onATile(x, z))
		return;

	// Get the bounds of this tile
	float tileSideLength = m.getTileMetersX();
	float wall_left	= tileSideLength  *  (x+0);
	float wall_bottom	= tileSideLength  *  (z+0);
	float wall_right	= tileSideLength  *  (x+1);
	float wall_top	= tileSideLength  *  (z+1);

	// Get the bounds of the actor
	const float FUDGE = 0.7f;
	float r = getCylinderRadius() * FUDGE;
	float actor_left = position.x - r;
	float actor_right = position.x + r;
	float actor_bottom = position.z - r;
	float actor_top = position.z + r;

	/*
	Calculate how far the actor has overlapped each
	edge of the Tile it is standing on
	*/
	float overlapTop = actor_top - wall_top;
	float overlapBottom = wall_bottom - actor_bottom;
	float overlapLeft = wall_left - actor_left;
	float overlapRight = actor_right - wall_right;

	/*
	If the actor would overlap an impassable tile, then adjust the
	position to prevent this from happening.
	*/

	bool passableTop = isNeighborTilePassable(m, x, z+1);
	bool passableBottom = isNeighborTilePassable(m, x, z-1);
	bool passableLeft = isNeighborTilePassable(m, x-1, z);
	bool passableRight = isNeighborTilePassable(m, x+1, z);

	if(overlapTop>0 && !passableTop)
	{
		position.z = wall_top - r;
		slidOnWall = true;
	}

	if(overlapBottom>0 && !passableBottom)
	{
		position.z = wall_bottom + r;
		slidOnWall = true;
	}

	if(overlapLeft>0 && !passableLeft)
	{
		position.x = wall_left + r;
		slidOnWall = true;
	}

	if(overlapRight>0 && !passableRight)
	{
		position.x = wall_right - r;
		slidOnWall = true;
	}

	if(slidOnWall)
	{
		onSlidOnWall();
	}
}

bool Actor::isNeighborTilePassable(const Map &m, int x, int z) const
{
	if(m.onATile(x, z))
	{
		const Tile& tile = m.getTile(x, z);

		bool palatableElevationDifference = false;

		if(floating)
		{

			palatableElevationDifference = getPos().y > tile.getTileHeight();
		}
		else
		{
			palatableElevationDifference = fabsf(getPos().y - tile.getTileHeight()) < 0.5f;
		}

		return(tile.isPassable() && palatableElevationDifference);
	}

	return false;
}

bool Actor::anyCollisions(void) const
{
	return !m_Collisions.empty();
}

list<Actor*> Actor::getCollisions(const ActorSet &s) const
{
	list<Actor*> colliders;

	for(ActorSet::const_iterator iter=s.begin(); iter!=s.end(); ++iter)
	{
		Actor * const a = iter->second;

		if(isCollision(*a))
		{
			colliders.push_front(a);
		}
	}

	return colliders;
}

void Actor::drawTransparentObject(void) const
{}

void Actor::drawObject(void) const
{
	CHECK_GL_ERROR();

	// Color the model while in editor mode
	if(g_Application.getState()==GAME_STATE_EDITOR)
	{
		const EditorToolBar *e = GameStateEditor::GetSingleton().getEditorToolBar();
		glColor4fv((e!=0 && e->getSelectedId() == m_ID) ? red : white);
	}
	else if(!isLit)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
	}

	// At visual priorities less than 0.0, the object is not rendered at all
	if(
		(m_pModel!=0)
			&&
		(
			showModel
				||
			(g_Application.getState()==GAME_STATE_EDITOR || g_Application.displayDebugData)
		)
	  )
	{
		glPushMatrix();
		glMultMatrixf(toWorldSpace());
		m_pModel->draw();
		glPopMatrix();
	}

	if(g_Application.getState()!=GAME_STATE_EDITOR && !isLit)
	{
		glPopAttrib();
	}

	CHECK_GL_ERROR();
}

void Actor::drawObjectToDepthBuffer(void) const
{
	CHECK_GL_ERROR();

	if(m_pModel!=0)
	{
		glPushMatrix();
		glMultMatrixf(toWorldSpace());
		m_pModel->draw();
		glPopMatrix();
	}

	CHECK_GL_ERROR();
}

void Actor::drawObjectDebugData(void) const
{
	const vec3 base = getPos() + vec3(0, getHeight(), 0);

	glPushMatrix();
		glTranslatef(base.x, base.y, base.z);
		g_Application.fontSmall.write(getName(), white, FONT_SIZE_NORMAL, true);

		glTranslatef(0.0f, 0.2f, 0.0f);
		g_Application.fontSmall.write(getTypeString(), white, FONT_SIZE_NORMAL, true);
	glPopMatrix();
}

PropertyBag Actor::save(void) const
{
	PropertyBag Bag;
	PropertyBag dataFile;

	if(editorDataFile.empty())
	{
		Bag.add(_T("type"), getTypeString());
	}
	else
	{
		// Save a reference to the data file
		Bag.add(_T("file"), editorDataFile);

		// We will make comparisons to stored data
		dataFile.Load(editorDataFile);
	}

	saveTidy(Bag, dataFile);

	return Bag;
}

bool Actor::saveTidy(PropertyBag &Bag, PropertyBag &dataFile) const
{
	saveTag(Bag, dataFile, _T("height"),			m_desiredHeight);
	saveTag(Bag, dataFile, _T("mass"),				m_Mass);
	saveTag(Bag, dataFile, _T("speed"),				topSpeed);
	saveTag(Bag, dataFile, _T("model"),				m_strModelFilename);
	saveTag(Bag, dataFile, _T("name"),				m_strName);
	saveTag(Bag, dataFile, _T("castShadows"),			castShadows);
	saveTag(Bag, dataFile, _T("solid"),				solid);
	saveTag(Bag, dataFile, _T("showModel"),			showModel);
	saveTag(Bag, dataFile, _T("floating"),			floating);
	saveTag(Bag, dataFile, _T("look"),				orientation.getAxisZ());
	saveTag(Bag, dataFile, _T("up"),				orientation.getAxisY());
	saveTag(Bag, dataFile, _T("frictionAcceleration"),	frictionAcceleration);

	Bag.add(_T("pos"), position);

	return true;
}

bool Actor::LoadXml(PropertyBag &Bag)
{
	// Free any used memory and destroy the old object
	destroy();

	// Allow an external data file
	if(Bag.get(_T("file"), editorDataFile))
	{
		TRACE(_tstring(_T("Loading external data file: ")) + editorDataFile);
		Bag.LoadMerge(editorDataFile);
		TRACE(_tstring(_T("Done loading external data file: ")) + editorDataFile);
	}

	// kept to support previous versions of the file format
	if(Bag.get(_T("radius"), m_desiredHeight)) m_desiredHeight*=2.0f;

	// Load the object data
	Bag.get(_T("height"),			m_desiredHeight);
	Bag.get(_T("mass"),			m_Mass);
	Bag.get(_T("speed"),			topSpeed);
	Bag.get(_T("name"),			m_strName);

	Bag.getSym(showModel);
	Bag.getSym(solid);
	Bag.getSym(floating);
	Bag.getSym(frictionAcceleration);

	// Set the orientation matrix
	vec3 zAxis (0,0,1), yAxis(0,1,0), xAxis(1,0,0);
	Bag.get(_T("look"), &zAxis);
	xAxis = yAxis.cross(zAxis).getNormal();

	orientation.setAxisZ(zAxis);
	orientation.setAxisY(yAxis);
	orientation.setAxisX(xAxis);
	orientation.setPos(vec3(0,0,0));

	// Set the spawn point and location of the object
	Bag.get(_T("pos"), &position);
	spawnPoint = validatedPos = position;

	ASSERT(getZone().getMap().onATile(position.x, position.z), _T("position is outside of the bounds of the map"));

	// Load model data from xml, then load the specified resources
	if(Bag.get(_T("model"), m_strModelFilename) == true)
	{
		LoadModel(m_strModelFilename);
	}

	// Is the object supposed to be lit?
	Bag.get(_T("isLit"),		isLit);
	Bag.get(_T("castShadows"),	castShadows);

	// Return with success
	return true;
}

bool Actor::ChangeAnimation(const _tstring &name, float speed)
{
	ASSERT(m_pModel!=0, _T("Actor::ChangeAnimation  ->  Null Pointer: m_pModel."));

	if(m_pModel != 0)
	{
		return m_pModel->requestAnimationChange(name, speed);
	}
	else
	{
		return false;
	}
}

bool Actor::ChangeAnimation(size_t nIdx, float Speed)
{
	ASSERT(m_pModel!=0, _T("Actor::ChangeAnimation  ->  Null Pointer: m_pModel."));

	if(m_pModel)
	{
		return m_pModel->requestAnimationChange(nIdx, Speed);
	}
	else
	{
		return true;
	}
}

#ifdef _DEBUG
void Actor::OnMessage(Message_s Msg)
{
	ASSERT(m_ID == Msg.m_Recipient, _T("Actor::OnMessage  ->  Message was mailed to the wrong object."));
}
#else
void Actor::OnMessage(Message_s){}
#endif

bool Actor::wasCollision(OBJECT_ID id)
{
	for(list<Actor*>::const_iterator iter=m_Collisions.begin(); iter != m_Collisions.end(); ++iter)
	{
		Actor *actor = *iter;

		ASSERT(actor!=0, _T("actor was null"));

		if(actor->m_ID==id)
		{
			return true;
		}
	}

	return false;
}

void Actor::OnCollision(Actor &)
{}

void Actor::createToolBar(ListPaneWidget *pane)
{
	ASSERT(pane!=0, _T("The actor pane was expected to have been created before now!"));

	// Create a new tool bar
	pane->addElement(new ToggleWidgetText(		_T("Cast Shadows"),	&castShadows));
	pane->addElement(new ToggleWidgetText(		_T("Lit"),			&isLit));
	pane->addElement(new ListElementTweakerString(	_T("Name"),			&m_strName));
	pane->addElement(new ListElementTweaker<float>(	_T("Height (m)"),		&m_desiredHeight));
	pane->addElement(new ToggleWidgetText(		_T("Visible"),		&showModel));
	/*pane->addElement(new ListElementTweakerString(_T("Model"),		&m_strModelFilename));*/
	pane->addElement(new ListElementTweakerXML(	_T("Position"),		&position));
}

void Actor::sync(void)
{
	setHeight(m_desiredHeight);

	validatedPos = position;
}

float Actor::getRealAngleY(void) const
{
	const vec3 axis(1, 0, 0);
	const vec3 x = getOrientation().getAxisX().getNormal();

	// Get the angle between the absolute x-axis and the local x-axis
	return acosf(axis.dot(x));
}

void Actor::saveList(PropertyBag& xml, const _tstring& name, const vector<_tstring>& list) const
{
	PropertyBag bag;

	for(vector<_tstring>::const_iterator iter=list.begin(); iter!=list.end(); ++iter)
	{
		bag.add(_T("file"), (*iter));
	}

	xml.add(name, bag);
}

void Actor::loadList(PropertyBag& xml, const _tstring& name, vector<_tstring>& list)
{
	PropertyBag bag;

	if(xml.get(name, bag))
	{
		const size_t numberOfFiles = bag.getNumInstances(_T("file"));
		for(size_t i=0; i<numberOfFiles; ++i)
		{
			_tstring fileName;
			bag.get(_T("file"), fileName, i);
			list.push_back(fileName);
		}
	}
}

bool Actor::doesCastShadows(void) const
{
	return castShadows;
}

bool Actor::isInProximity(OBJECT_ID actor, float triggerRadius) const
{
	const ActorSet &s = getZone().getObjects();

	if(zombie || !s.isMember(actor))
		return false;

	const Actor &a = s.get(actor);

	if(!a.zombie)
	{
		vec3 delta = getPos() - a.getPos();
		delta.y=0;

		// The minimum distance before a trigger event occurs
		float minDist = (triggerRadius + a.getCylinderRadius());

		// The distance separating the two cylinders
		float realDist = delta.getMagnitude() - minDist;

		// Has an intersection occurred?
		if(realDist < 0.0f)
		{
			return true;
		}
	}

	return false;
}

bool Actor::isInProximity(OBJECT_ID actor) const
{
	return isInProximity(actor, getCylinderRadius());
}

bool Actor::isAnythingInProximity(OBJECT_ID &handle) const
{
	return isAnythingInProximity(getZone().getObjects(), handle);
}

bool Actor::isAnythingInProximity(OBJECT_ID &handle, float triggerRadius) const
{
	return isAnythingInProximity(getZone().getObjects(), handle, triggerRadius);
}

bool Actor::isAnythingInProximity(const ActorSet &s, OBJECT_ID &handle, float triggerRadius) const
{
	for(ActorSet::const_iterator i = s.begin(); i!=s.end(); ++i)
	{
		OBJECT_ID id = i->first;

		if(id != m_ID && isInProximity(id, triggerRadius))
		{
			handle = id; // return the ID
			return true;
		}
	}

	return false;
}

bool Actor::isAnythingInProximity(const ActorSet &s, OBJECT_ID &handle) const
{
	return isAnythingInProximity(s, handle, getCylinderRadius());
}

}; // namespace










#include "Trigger.h"
#include "../Bullet.h" // TODO: HACK that we even need this!
#include "Player.h" // TODO: HACK that we even need this!

namespace Engine{
bool Actor::isCollision(const Actor &a) const
{
	if(a.m_ID == m_ID) return false; // Cannot collide with self
	if(a.zombie || zombie) return false; // Cannot collide with zombies
	if(!a.isSolid() || !isSolid()) return false; // Cannot collide with non-solid objects

	float minDist = (getCylinderRadius() + a.getCylinderRadius()) * 0.70f;
	float realDist = getDistance(this, a) - minDist;

	if(realDist > 0.0f)
	{
		// an intersection assuredly did NOT occur
		return false;
	}

/*********************/

	/*
	This totally violates the rules and principles of OOP and complicates the game engine.
	However, it works around bugs that arise with whether Actor.isCollision(Bullet)
	or Bullet.isCollision(Actor) is called and etc.

	TODO: Add to Actor a list of objects that the objec is allowed to pass through with no collisions
	*/

	// If we are a bullet
	const Arbarlith2::Bullet *bulletWe = dynamic_cast<const Arbarlith2::Bullet*>(this);
	if(bulletWe!=0)
	{
		OBJECT_ID owner = bulletWe->getOwner();

		// if they are the owner, then we shouldn't collide with them
		return(a.m_ID != owner);
	}

	// If they are a bullet
	const Arbarlith2::Bullet *bulletThey = dynamic_cast<const Arbarlith2::Bullet*>(&a);
	if(bulletThey!=0)
	{
		OBJECT_ID owner = bulletThey->getOwner();

		// if we are the owner, then they shouldn't collide with us
		return(m_ID != owner);
	}

/*********************/

	// If we are a player and they are a player
	const Player *playerWe = dynamic_cast<const Player*>(this);
	const Player *playerThey = dynamic_cast<const Player*>(&a);

	if(playerWe!=0 && playerThey!=0)
	{
		return false;
	}

/*********************/

	return true;
}

}; // namespace
