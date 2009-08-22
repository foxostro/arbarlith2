/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

2006 Richard Halstead
E-mail: mailto:rhalstea@andrew.cmu.edu

Copyright © 2006-2007 Game Creation Society
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



#ifndef ACTOR_H
#define ACTOR_H

#include "mat4.h"
#include "tstring.h"
#include "ActorFactory.h"
#include "EffectSig.h"
#include "Message.h"
#include "AnimationController.h"

namespace Engine {

class ActorSet;
class World;
class Map;
class ListPaneWidget;

/**
A basic object in the game world.  An Actor has interfaces to present a
visual representation of itself and to interact on a very basic level with
the other objects in the game world.  For example, the Actor class provide
a very simple collision detection mechanism and an event handling mechanism
that exists for a more advanced system to build upon.
*/
class Actor
{
public:
	GEN_RTTI(Actor, "class Engine::Actor")

public:
	/**
	Constructor
	@param ID The unique ID of the object
	@param zone The starting zone of the object
	*/
	Actor(OBJECT_ID ID);

	/** Destructor */
	virtual ~Actor(void);

	/** Set everything to defaults */
	virtual void clear(void);

	/** Free all allocated memory, cleanup */
	virtual void destroy(void);

	/**
	Saves the object state to an XML data source
	@return The XML data source returned
	*/
	virtual PropertyBag save(void) const;

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/**
	Loads the object state
	@param fileName Name of the file to load
	*/
	void loadFromFile(const string &fileName);

	/**
	Gets the distance in the XZ-plane that the actots are from one another
	@param a The first actor
	@param b The second actor
	@return XZ distance
	*/
	static float getDistance(const Actor *a, const Actor &b)
	{
		return vec3(a->getPos().x-b.getPos().x, 0, a->getPos().z-b.getPos().z).getMagnitude();
	}

	/**
	Gets the distance in the XZ-plane that the actots are from one another
	@param a The first actor
	@param b The second actor
	@return XZ distance
	*/
	static float getDistance(const Actor &a, const Actor &b)
	{
		return getDistance(&a, b);
	}

	/** Transforms and renders the object */
	virtual void drawObject(void) const;

	/** Transforms and renders the transparent parts of the object */
	virtual void drawTransparentObject(void) const;

	/** Quickly render the object into the depth buffer */
	virtual void drawObjectToDepthBuffer(void) const;

	/** Used by drawObject to render Debug data */
	virtual void drawObjectDebugData(void) const;

	/**
	Gets the transformation matrix from model-space into world-space
	@return transformation to world-space
	*/
	virtual mat4 toWorldSpace(void) const;

	/**
	Gets the validated position of the object
	@return world-space position
	*/
	inline const vec3& getValidatedPos(void) const
	{
		return validatedPos;
	}

	/**
	Updates the object without displaying it
	@param deltaTime milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

	/**
	Updates the object for the editor mode of the engine.  Does not display the Actor.
	@param deltaTime milliseconds since the last tick
	*/
	virtual void updateForEditor(float deltaTime);

	/**
	Perform collision detection
	@param s the set of actors to test for collision against
	*/
	virtual void DoCollisionDetection(const ActorSet &s);

	/**
	Perform collision response using the m_Collisions list
	*/
	virtual void DoCollisionResponse(void);

	/**
	Determines whether a collision had been previously detected between this object and another.
	@param id The ID of the possible collider
	@return true if a collision had previously been detected, false otherwise
	*/
	virtual bool wasCollision(OBJECT_ID id);

	/**
	Determines whether a given actor is in close proximity to us
	@param actor the actor in question
	@return true if the specified actor is in proximity to us
	*/
	virtual bool isInProximity(OBJECT_ID actor) const;

	/**
	Determines whether a given actor is in close proximity to us
	@param actor the actor in question
	@param triggerRadius Radius to define "proximity" as
	@return true if the specified actor is in proximity to us
	*/
	virtual bool isInProximity(OBJECT_ID actor, float triggerRadius) const;

	/**
	Determines whether any object is in proximity to this object and returns the ID of that object
	@param id When isAnythingInProximity returns true, this returns the ID of the object
	@return true if some object was in proximity, false otherwise
	*/
	virtual bool isAnythingInProximity(OBJECT_ID &id) const;

	/**
	Determines whether any object is in proximity to this object and returns the ID of that object
	@param id When isAnythingInProximity returns true, this returns the ID of the object
	@param triggerRadius Radius to define "proximity" as
	@return true if some object was in proximity, false otherwise
	*/
	virtual bool isAnythingInProximity(OBJECT_ID &id, float triggerRadius) const;

	/**
	Determines whether any object is in proximity to this object and returns the ID of that object
	@param s Only considers objects of the given actor set
	@param id When isAnythingInProximity returns true, this returns the ID of the object
	@return true if some object was in proximity, false otherwise
	*/
	virtual bool isAnythingInProximity(const ActorSet &s, OBJECT_ID &id) const;

	/**
	Determines whether any object is in proximity to this object and returns the ID of that object
	@param s Only considers objects of the given actor set
	@param id When isAnythingInProximity returns true, this returns the ID of the object
	@param triggerRadius Radius to define "proximity" as
	@return true if some object was in proximity, false otherwise
	*/
	virtual bool isAnythingInProximity(const ActorSet &s, OBJECT_ID &id, float triggerRadius) const;

	/**
	Called on the event that a message is received by the object
	@param message The message received
	*/
	virtual void OnMessage(Message_s message);

	/**
	Called on the event that the received message is a collision
	@param actor The colliding actor
	*/
	virtual void OnCollision(Actor &actor);

	/**
	Place the object at a point in space
	@param pos The point to place the object at
	*/
	virtual void Place(const vec3 &pos);

	/**
	Called in the event that the object is placed
	It is possible that our World has been changed in the process
	*/
	virtual void OnPlace(void);

	/**
	Gets the position of the object
	@return world-space position
	*/
	inline const vec3& getPos(void) const
	{
		return position;
	}

	/**
	Gets the orientation of the Actor
	@return Orthonormal basis
	*/
	inline const mat4& getOrientation(void) const
	{
		return orientation;
	}

	/**
	Gets the orientation of the Actor
	@return Orthonormal basis
	*/
	inline void setOrientation(const mat4& orientation)
	{
		this->orientation = orientation;
	}

	/**
	Sets he height of the object
	@param height The new height of the object
	*/
	virtual void setHeight(float height);

	/** Gets the height of the character */
	inline float getHeight(void) const
	{
		return m_desiredHeight;
	}

	/**
	Causes the object to look at a point in space
	@param center The point in space the the object is looking at
	@param up The up-vector of the object
	*/
	virtual void lookAt(const vec3 &center);

	/**
	Get the radius of the smallest sphere that can completly enclose the object
	@return radius
	*/
	inline float getSphereRadius(void) const
	{
		return sphereRadius;
	}

	/**
	Get the radius of the smallest cylinder that can completly enclose the object
	@return radius
	*/
	inline float getCylinderRadius(void) const
	{
		return cylinderRadius;
	}

	/**	Recalculate radius of the model	*/
	virtual void calculateRadius(void);

	/**
	Get the object's top speed
	@return speed
	*/
	inline float getTopSpeed(void) const
	{
		return topSpeed;
	}

	/**
	Set the object's top speed
	@param x The new speed
	*/
	inline void setTopSpeed(float x)
	{
		topSpeed = x;
	}

	/**
	Gets the mass of the object
	@return The mass of the object
	*/
	inline float getMass(void) const
	{
		return m_Mass;
	}

	/**
	Gets the scale of the object
	@return The scale of the object
	*/
	inline float getScale(void) const
	{
		return m_Scale;
	}

	/**
	Loads a model for the object
	@param fileName The file name of a model's XML definition
	*/
	virtual void LoadModel(const string &fileName);

	/**
	Requests that the animation be changed to the specified animation at the next available opportunity
	@param name		The name of the new animation
	@param speed	The speed multiplier for the animation
	*/
	virtual bool ChangeAnimation(const string &name, float speed=1.0f);

	/**
	Requests that the animation be changed to the specified animation at the next available opportunity
	@param handle	The handle of the new animation
	@param speed	The speed multiplier for the animation
	*/
	virtual bool ChangeAnimation(size_t handle, float speed=1.0f);

	/**
	@param name The name of the current animation
	@return the handle to the animation
	*/
	inline size_t getAnimationHandle(const string &name)
	{
		ASSERT(m_pModel!=0, "Actor has no model");
		return m_pModel->getAnimationHandle(name);
	}

	/**
	@param handle The handle of the animation
	@return the handle to the animation
	*/
	inline float getAnimationLength(size_t handle) const
	{
		ASSERT(m_pModel!=0, "Actor has no model");
		return m_pModel->getAnimation(handle).getLength();
	}

	/**
	Get the name of the object
	@return name
	*/
	inline const string &getName(void) const
	{
		return m_strName;
	}

	/** Returns to the last validated transformation */
	inline void RevertToValidatedPos(void)
	{
		position = validatedPos;
	}

	/** Records the current position as a valid transformation */
	inline void RecordValidatedPos(void)
	{
		validatedPos = position;
	}

	/**
	Notify the creature that it has been moved to a new zone
	@param zone The zone that the creature has been moved into
	*/
	inline void setZone(World *zone)
	{
		ASSERT(zone!=0, "myZone was NULL");
		myZone = zone;
	}

	/**
	Gets the zone that the creature believes that it is located within
	@return The World
	*/
	inline World& getZone(void)
	{
		ASSERT(myZone!=0, "myZone was NULL");

		return(*myZone);
	}

	/**
	Gets the zone that the creature believes that it is located within
	@return The World
	*/
	inline const World& getZone(void) const
	{
		ASSERT(myZone!=0, "myZone was NULL");

		return(*myZone);
	}

	/**
	Allow access to the model without allowing the child
	class to change the pointer.
	@return The animation controller
	*/
	inline AnimationController * getModel(void)
	{
		return m_pModel;
	}

	/**
	Allow access to the model without allowing the child
	class to change the pointer.
	@return The animation controller
	*/
	inline const AnimationController * getModel(void) const
	{
		return m_pModel;
	}

	/**
	Indicates that the object is solid.  Solid objects may not pass through other solid objects.
	@return true when solid
	*/
	inline bool isSolid(void) const
	{
		return solid;
	}

	/** Indicates that the actor will cast shadows */
	virtual bool doesCastShadows(void) const;

	/**
	Sets up the actor properties pane that will be visible in the map editor
	@param pane The actor properties pane
	*/
	virtual void createToolBar(ListPaneWidget *pane);

	/**
	Determines whether any collisions with other objects were recorded last tick
	@return true if there were any collisions with other objects last tick
	*/
	bool anyCollisions(void) const;

	/**
	Gets the original spawn point of the object
	@return vec3
	*/
	inline const vec3& getSpawnPoint(void) const
	{
		return spawnPoint;
	}

protected:
	/**
	Tests for a collision with another object
	@param object The object to test collision with
	@return true when the object has collided with this object
	*/
	virtual bool isCollision(const Actor &object) const;

	/**
	Tests for collisions between this object and all objects in the given actor set
	@return list of all objects this object is colliding with
	*/
	virtual list<Actor*> getCollisions(const ActorSet &set) const;

	/** Records all collisions in the previous tick */
	list<Actor*> m_Collisions;

	/**
	Determines if the actor can cross into the tile from its current position
	@param m The map
	@param x Tile X of the neighboring tile
	@param z Tile Z of the neighboring tile
	*/
	bool isNeighborTilePassable(const Map &m, int x, int z) const;

	/** Cause the object to slide off of walls when collided with them */
	void slideAgainstWalls(void);

	/** Called in the event that the actor slid against a wall */
	virtual void onSlidOnWall(void){}

	/**
	Loads a list of strings from XML
	@param xml XML data
	@param name The name of the item set
	@param list The list items to return
	*/
	virtual void loadList(const PropertyBag& xml, const string& name, vector<string>& strList);

	/**
	Saves a list of strings to XML
	@param xml XML data
	@param name The name of the item set
	@param list The list items to return
	*/
	virtual void saveList(PropertyBag& xml, const string& name, const vector<string>& strList) const;

	/**
	Saves a tag, but only if the default data is different
	@param xml The XMl data source
	@param editorData The editor data source (default data)
	@param tagName The name of the tag
	@param data The data to save
	*/
	template<class T>
	bool saveTag(PropertyBag &xml, PropertyBag &editorData, const string &tagName, T data) const
	{
		T defaultValue;
		bool contains = editorData.get(tagName, defaultValue);
		if(!contains || (contains && defaultValue!=data))
		{
			xml.add(tagName, data);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool saveTag(PropertyBag &xml, PropertyBag &editorData, const string &tagName, string data) const
	{
		string defaultValue;
		bool contains = editorData.get(tagName, defaultValue);
		if(!contains || (contains && defaultValue!=data))
		{
			xml.add(tagName, data);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool saveTag(PropertyBag &xml, PropertyBag &editorData, const string &tagName, vec4 data) const
	{
		vec4 defaultValue;
		bool contains = editorData.get(tagName, defaultValue);
		if(!contains || (contains && defaultValue!=data))
		{
			xml.add(tagName, data);
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(PropertyBag &xml, PropertyBag &dataFile) const;

	/**
	When the toolbar is active data values may be changed at
	any moment.  This function takes the data for the Actor
	and synchronizes the Actor's state with it.  For example,
	the appropriate model is reloaded and the scale is
	recalculated.
	*/
	virtual void sync(void);

public:
	/** Unique ID of the object*/
	OBJECT_ID m_ID;

	/** In editor mode, this is the data file that object originates from */
	string editorDataFile;

	/** Determines whether or not the model is displayed outside of editor mode */
	bool showModel;

	/** Indicates that the manager may delete us */
	bool zombie;

	/** The actor has moved within the last tick */
	bool hasMoved;

	/** The actor has animated within the last tick */
	bool hasAnimated;

protected:
	/** When true, the actor will be floating off the surface of the ground beneath him */
	bool floating;

	/** This is the effect sig for the effect that will be applied to the model */
	effect_sig myEffect;

	/** The desired height of the object after scaling */
	float m_desiredHeight;

	/** The actual radius of the object's bounding cylinder */
	float cylinderRadius;

	/** The actual radius of the object's bounding sphere */
	float sphereRadius;

	/** The name of the object */
	string m_strName;

	/** The mass of the object in kilograms */
	float m_Mass;

	/** The top speed of the object in meters-per-second */
	float topSpeed;

	/** The scale factor necessary to scale the object to the desired radius */
	float m_Scale;

	/** The file name of the model loaded for this actor */
	string m_strModelFilename;

	/** The model that has been loaded for this actor. (memory-managed by the model loader's cache) */
	AnimationController *m_pModel;

	/** Indicates the object should cast shadows */
	bool castShadows;

	/** Indicates the object should be lit */
	bool isLit;

	/** The object is solid and may not pass through other solid objects. */
	bool solid;

	/** Magnitude of acceleration from friction */
	float frictionAcceleration;

	/** Minimum velocity at which the walking animation will play */
	float minWalkingVelocity;

	/** velocity of the actor. */
	vec3 velocity;

	/** Position of the actor. */
	vec3 position;

private:
	/** The realm that the object is currently located within */
	World *myZone;

	/** Position of the actor. Garaunteed to result in a valid, non-colliding position */
	vec3 validatedPos;

	/** Orientation of the object */
	mat4 orientation;

	/** The spawn point of the object */
	vec3 spawnPoint;

	/** Indicates that we slid on a wall in the previous tick */
	bool slidOnWall;

	/**
	Extract the angle about the Y-Axis from the orientation matrix
	@return radians about the Y-Axis
	*/
	float getRealAngleY(void) const;
};

} // namespace Engine

#endif
