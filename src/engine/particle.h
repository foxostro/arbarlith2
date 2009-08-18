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

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "PropertyBag.h"
#include "vec4.h"
#include "BoundingBox.h"
#include "material.h"

namespace Engine {

/**
Physical body in the particle engine.
Follows some notion of the laws of Physics, respecting acceleration and velocity over time.
*/
class ParticleBody
{
protected:
	/** Position of the body */
	vec3 position;

	/** Initial velocity of the body (meters per second) */
	vec3 initialVelocity;

	/** Initial velocity of the particle ourwards from the center point */
	float initialOutwardVelocity;

	/** Milliseconds since creation*/
	float age;

public:
	/** Initial position of the body */
	vec3 initialPosition;

	/** The acceleration of the body */
	vec3 constantAcceleration;

public:
	/** Destructor */
	virtual ~ParticleBody(void) {}

	/** Default Constructor */
	ParticleBody(void);

	/**
	Copy Constructor
	@param body The object to copy
	*/
	ParticleBody(const ParticleBody &body);

	/**
	Constructor
	@param data Data describing the body
	*/
	ParticleBody(const PropertyBag &data);

	/**
	Loads data from xml and returns success(true) or failure(false)
	@param data Data to describe the body
	@return true if the data loads successfully
	*/
	virtual void load(const PropertyBag &data);

	/**
	Updates the particle position and sets the immediate values for graph related data
	@param deltaTime milliseconds since last update
	*/
	virtual void update(float deltaTime);

	/**
	Gets the position of the Body
	@return position of the Body
	*/
	inline const vec3& getPosition(void) const
	{
		return position;
	}

	/**
	Sets the position of the body
	@param position Position of the body
	@param center Center of the body
	*/
	void setPosition(const vec3 &position, const vec3 &center);

	/**
	Sets the position of the body
	@param position Position of the body
	*/
	void setPosition(const vec3 &position);
};

/**
Represents the curve of some value over time.
The particle engine makes extensive use of the ParticleGraph class to model
arbitrary curves in the graph over time of the properties of particles and particle
systems e.g. colors, size.
*/
class ParticleGraph
{
public:
	/** Min. corner of the graph */
	vec2 min;

	/** Max. corner of the graph */
	vec2 max;

private:
	/** data points */
	vector<Point2> points;

public:
	/** Default Constructor */
	ParticleGraph(void);

	/**
	Constructor
	@param xml data on the curve
	*/
	ParticleGraph(const PropertyBag &xml);

	/**
	Loads graph data from xml
	@param xml data on the curve
	*/
	void load(const PropertyBag &xml);

	/**
	Calculates the value at the specified time
	@param t [0.0, 1.0] representing progress through time on the curve
	*/
	float getValue(float t) const;
};

class ParticleSystem; // Prototype

/** A single particle element. */
class ParticleElement : public ParticleBody
{
public:
	/** This handle could be passed to the System to obtain the material we want to use */
	size_t materialHandle;

	/** ParticleSystem that owns this element */
	ParticleSystem *owner;

	/** Milliseconds that this element will be alive for */
	float lifeSpan;

private:
	/** Name of the particle type */
	string typeName;

	/** Multiplier for the size of the particle */
	float sizeMultiplier;

	/** Size in meters over time */
	ParticleGraph graphSize;

	/** Alpha component over time */
	ParticleGraph graphAlpha;

	/** Red component over time */
	ParticleGraph graphRed;

	/** Green component over time */
	ParticleGraph graphGreen;

	/** Blue component over time */
	ParticleGraph graphBlue;

	/** Particle size component at the immediate moment */
	float graphSizeImmediate;

	/** Particle Alpha component at the immediate moment */
	float graphAlphaImmediate;

	/** Particle Red component at the immediate moment */
	float graphRedImmediate;

	/** Particle Green component at the immediate moment */
	float graphGreenImmediate;

	/** Particle Blue component at the immediate moment */
	float graphBlueImmediate;

public:
	/** Default Constructor */
	ParticleElement(void);

	/**
	Constructor
	@param data Data to describe the element
	@param system Particle system that owns the emitter
	*/
	ParticleElement(const PropertyBag &data, ParticleSystem &system);

	/** Copy Constructor */
	ParticleElement(const ParticleElement &element);

	/**
	Loads particle data
	@param data Data to describe the element
	@param system Particle system that owns the emitter
	*/
	void load(const PropertyBag &data, ParticleSystem &system);

	/**
	Draws Renders the particle
	@param matrix Modelview matrix
	*/
	void draw(const mat4 &matrix) const;

	/**
	Updates the particle element
	@param deltaTime Milliseconds since the last update
	*/
	void update(float deltaTime);

	/**
	Determines whether the particle is dead or not
	@return true if the particle is now dead
	*/
	inline bool isDead(void) const
	{
		return age>lifeSpan;
	}

	inline const string& getName(void) const
	{
		return typeName;
	}

	inline float getSizeMultiplier(void) const
	{
		return sizeMultiplier;
	}

	inline float getAge(void) const
	{
		return age;
	}

	inline float setSizeMultiplier(float x)
	{
		return(sizeMultiplier = x);
	}

	inline float setLifeSpan(float x)
	{
		return(lifeSpan = x);
	}
};

/**
Emits particle for a ParticleSystem.
Creates particles in the parent ParticleSystem according to rules loaded from xml.
*/
class ParticleEmitter
{
private:
	/** The particle system that owns this emitter */
	ParticleSystem *owner;

	/** Particle template for all Elements emitted by this object */
	ParticleElement particleTemplate;

	/** The probability of a particle being created drops to zero between the hot spot radius and the falloff radius. */
	float radiusFalloff;

	/** Rate of emission over time */
	ParticleGraph graphEmissionRate;

	/** Size multiplier on emission over time */
	ParticleGraph graphSizeMultiplier;

	/** Elements are given a life span when they are spawned, however, this may change over time */
	ParticleGraph graphLifeSpan;

	/** Age of the emitter */
	float age;

	/** Life span of the emitter */
	float lifeSpan;

	/** If true, then the emitter will repeat after its lifespan has expired */
	bool looping;

	/**
	The number of times (remaining) to allow a looping emitter to repeat.
	If this is negative, then the emitter will continue to repeat forever.
	*/
	int numberOfLifeCycles;

public:
	/**
	Constructor
	@param data Data to describe the behavior of the emitter
	@param owner ParticleSystem that owns the emitter
	*/
	ParticleEmitter(const PropertyBag &data, ParticleSystem &owner);

	/** Copy Constructor */
	ParticleEmitter(const ParticleEmitter &emitter);

	/** Assignment Operator */
	ParticleEmitter &operator=(const ParticleEmitter &emitter);

	/**
	Updates the emitter over time
	@param deltaTime Milliseconds since the last update
	*/
	void update(float deltaTime);

	/** Kills the emitter */
	void kill(void);

	/**
	Determines whether the emitter is dead or alive.
	@return true if the emitter is dead
	*/
	bool isDead(void) const;

private:
	/** Emits a particle */
	void emitParticle(void);
};

/**
System of particles and particle emitters.
Manages a collection of particle emitters and a fixed number of particles (per system)\
spawned from those emitters.
*/
class ParticleSystem : public ParticleBody
{
private:
	/** Particle-Template's Name -> Particle-Template */
	map<string, ParticleElement> templatesByName;

	/** Particle materials available for a template to specify */
	vector<Material> materials;

	/** Particle emitters managed by the ParticleSystem */
	vector<ParticleEmitter> emitters;

	/** Hard maximum number of particles that may be maintained by the ParticleSystem */
	size_t maxNumberOfParticles;

	/** Behavior to follow when max. particles are in use and we want to spawn a new particle */
	enum
	{
		IGNORE_EMISSION,
		REPLACE_RANDOM
	} emissionBehavior;

	/** Array of particles allocated for the ParticleSystem */
	ParticleElement **elements;

public:
	/** Destructor */
	virtual ~ParticleSystem(void);

	/** Default Constructor */
	ParticleSystem(void);

	/**
	Constructor
	@param fileName File to load data from
	*/
	ParticleSystem(const string &fileName);

	/**
	Constructor
	@param data Data describing the system
	*/
	ParticleSystem(PropertyBag &data);

	/** Copy Constructor */
	ParticleSystem(const ParticleSystem &system);

	/**
	Loads data from xml and returns success(true) or failure(false)
	@param data Data to describe the body
	@return true if the data loads successfully
	*/
	virtual void load(PropertyBag &data);

	/** Draws the particles in the system */
	void draw(void) const;

	/**
	Updates the particle system
	@param deltaTime Milliseconds since the last update
	*/
	void update(float deltaTime);

	/**
	Spawns a new particle
	@param element Particle to copy when spawning the new one
	*/
	void spawn(const ParticleElement &element);

	/**
	Retrieves a particle template given its name
	@param name Name of the particle template
	*/
	const ParticleElement& getTemplate(const string &name);

	/**
	Gets the material handle when given a material name
	@param name Name of the material
	@return handle to the material
	*/
	size_t getMaterialHandle(const string &name) const;

	/**
	Retrieves a material given its handle
	@param materialHandle Handle of the material
	*/
	Material& getMaterial(size_t materialHandle);

	/** Polls the emitters and returns true if all emitters are dead, else returns false */
	bool isDead(void) const;

	/** Kills each particle emitter */
	void kill(void);

private:
	/** Destroy and free all particle elements */
	void destroyElements(void);
};

} // namespace Engine

#endif
