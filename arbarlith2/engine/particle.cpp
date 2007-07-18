/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2004-2007 Game Creation Society
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

#include "random.h"
#include "particle.h"

namespace Engine {

/*
When set to 1, ParticleGraph objects draw a line between the
initial and ending values without consideration for intermediate values.
*/
#define ENABLE_CURVED_GRAPHS (1)

/**
Get a vector pointing in a random direction
@param length The length of the vector
@return The random vector
*/
vec3 GetRandomVector(float length)
{
	float x = FRAND_RANGE(-1.0f,+1.0f);
	float y = FRAND_RANGE(-1.0f,+1.0f);
	float z = FRAND_RANGE(-1.0f,+1.0f);

	vec3 vector(x,y,z);

	vector.normalize();

	return( vector * length );
}

typedef ParticleElement* ELEMENT_PTR;

ParticleBody::ParticleBody(void)
: position(0,0,0),
initialVelocity(0,0,0),
age(0),
initialPosition(0,0,0),
constantAcceleration(0,0,0),
center(0,0,0),
initialRadialVelocity(0)
{}

ParticleBody::ParticleBody(PropertyBag &Bag)
: position(0,0,0),
initialVelocity(0,0,0),
age(0),
initialPosition(0,0,0),
constantAcceleration(0,0,0),
center(0,0,0),
initialRadialVelocity(0)
{
	load(Bag);
}

ParticleBody::ParticleBody(const ParticleBody &body)
: position(body.position),
initialVelocity(body.initialVelocity),
age(body.age),
initialPosition(body.initialPosition),
constantAcceleration(body.constantAcceleration),
center(body.center),
initialRadialVelocity(body.initialRadialVelocity)
{}

void ParticleBody::load(PropertyBag &Bag)
{
	Bag.get(_T("position"),              &initialPosition);
	Bag.get(_T("acceleration"),          &constantAcceleration);
	Bag.get(_T("initialRadialVelocity"),  initialRadialVelocity);

	position = initialPosition; // the Body starts, of course, at the initial position

	age = 0.0f;
}

void ParticleBody::update(float dTime)
{
	age += dTime;

	float Time = age / 1000.0f;

	position = (constantAcceleration * 0.5f * SQR(Time)) +
		          (initialVelocity * Time) +
				   initialPosition;
}

void ParticleBody::setPosition(const vec3 &position, const vec3 &center)
{
	initialPosition = position;
	this->center = center;

	initialVelocity = vec3(initialPosition-center).getNormal() * initialRadialVelocity * FRAND_RANGE(0.8f, 1.2f);
}

void ParticleBody::setPosition(const vec3 &position)
{
	initialPosition = position;
	this->center = position;

	initialVelocity = vec3(0,0,0);
}

ParticleGraph::ParticleGraph(void)
{
	points.clear();

	min = vec2(0,0);
	max = vec2(0,0);
}

ParticleGraph::ParticleGraph(PropertyBag & xml)
{
	load(xml);
}

void ParticleGraph::load(const PropertyBag &xml)
{
	points.clear();

	min = vec2(0,0);
	max = vec2(0,0);

	const size_t numberOfPoints = xml.getNumInstances(_T("point"));
	for(size_t i=0; i<numberOfPoints; ++i)
	{
		PropertyBag PointBag;
		Point2 pt;

		xml.get(_T("point"), PointBag, i);
		PointBag.get(_T("x"), pt.x);
		PointBag.get(_T("y"), pt.y);

		points.push_back(pt);

		if(pt.x < min.x) min.x = pt.x;
		if(pt.y < min.y) min.y = pt.y;
		if(pt.x > max.x) max.x = pt.x;
		if(pt.y > max.y) max.y = pt.y;
	}
}

float ParticleGraph::getValue(float t) const
{
	ASSERT(t >= 0.0f && t <= 1.0f, _T("Parameter \'t\' is invalid: ") + ftoa(t));

	size_t numberOfPoints = points.size();

	if(numberOfPoints == 0)
	{
		return 0.0f;
	}

	if(numberOfPoints == 1)
	{
		return (points[0].y);
	}

	for(size_t i=0; i<numberOfPoints-1; ++i)
	{
		const Point2 &one = points[i+0];
		const Point2 &two = points[i+1];

		if(t >= one.x && t <= two.x)
		{
			float slope = (two.y - one.y) / (two.x - one.x);
			float value = slope * (t - one.x) + one.y;

			return value;
		}
	}

	return 0.0f;
}

ParticleElement::ParticleElement(void)
: ParticleBody(),
  materialHandle(0),
  owner(0),
  lifeSpan(0.0f),
  sizeMultiplier(1.0f),
  graphSizeImmediate(0.0f),
  graphAlphaImmediate(0.0f),
  graphRedImmediate(0.0f),
  graphGreenImmediate(0.0f),
  graphBlueImmediate(0.0f)
{}

ParticleElement::ParticleElement(PropertyBag &Bag, ParticleSystem &system)
: ParticleBody(Bag),
  materialHandle(0),
  owner(&system),
  lifeSpan(1000.0f),
  sizeMultiplier(1.0f),
  graphSizeImmediate(0.0f),
  graphAlphaImmediate(0.0f),
  graphRedImmediate(0.0f),
  graphGreenImmediate(0.0f),
  graphBlueImmediate(0.0f)
{
	load(Bag, system);
}

ParticleElement::ParticleElement(const ParticleElement &element)
: ParticleBody(element),
  materialHandle(element.materialHandle),
  owner(element.owner),
  lifeSpan(element.lifeSpan),
  typeName(element.typeName),
  sizeMultiplier(element.sizeMultiplier),
  graphSize(element.graphSize),
  graphAlpha(element.graphAlpha),
  graphRed(element.graphRed),
  graphGreen(element.graphGreen),
  graphBlue(element.graphBlue),
  graphSizeImmediate(element.graphSizeImmediate),
  graphAlphaImmediate(element.graphAlphaImmediate),
  graphRedImmediate(element.graphRedImmediate),
  graphGreenImmediate(element.graphGreenImmediate),
  graphBlueImmediate(element.graphBlueImmediate)
{}

void ParticleElement::load(PropertyBag &Bag, ParticleSystem &system)
{
	owner = &system;

	PropertyBag RotationGraph, SizeGraph, AlphaGraph, RedGraph, GreenGraph, BlueGraph;

	lifeSpan = 1000.0f; // Reset defaults

	Bag.get(_T("name"), typeName);

	// Load material data
	_tstring strMatName;
	Bag.get(_T("material"), strMatName);
	materialHandle = owner->getMaterialHandle(strMatName); // Have the system give us the material's handle

	// Load graph data
	Bag.get(_T("size"),     SizeGraph);
	Bag.get(_T("alpha"),    AlphaGraph);
	Bag.get(_T("red"),      RedGraph);
	Bag.get(_T("green"),    GreenGraph);
	Bag.get(_T("blue"),     BlueGraph);

	graphSize   .  load(SizeGraph);
	graphAlpha  .  load(AlphaGraph);
	graphRed    .  load(RedGraph);
	graphGreen  .  load(GreenGraph);
	graphBlue   .  load(BlueGraph);

	// Gets the initial values from the graph data
	graphSizeImmediate     = graphSize   .  getValue(0.0f);
	graphAlphaImmediate    = graphAlpha  .  getValue(0.0f);
	graphRedImmediate      = graphRed    .  getValue(0.0f);
	graphGreenImmediate    = graphGreen  .  getValue(0.0f);
	graphBlueImmediate     = graphBlue   .  getValue(0.0f);

	ParticleBody::load(Bag);
}

void ParticleElement::draw(const mat4 &matrix) const
{
	vec3 A, B, C, D; // billboard vertices

	// Set up the billboard material
	{
		ASSERT(owner!=0, _T("owner was null"));

		const Material &material = owner->getMaterial(materialHandle);
		material.bind();
		glBlendFunc(GL_SRC_ALPHA, (material.glow) ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
	}

	// Build the billboard vertices
	{
		float radius = sizeMultiplier*graphSizeImmediate;

		float x0 = matrix[0] - matrix[1];
		float x1 = matrix[0] + matrix[1];
		float y0 = matrix[4] - matrix[5];
		float y1 = matrix[4] + matrix[5];
		float z0 = matrix[8] - matrix[9];
		float z1 = matrix[8] + matrix[9];

		A = vec3(position.x - x1*radius, position.y - y1*radius, position.z - z1*radius);
		B = vec3(position.x + x0*radius, position.y + y0*radius, position.z + z0*radius);
		C = vec3(position.x + x1*radius, position.y + y1*radius, position.z + z1*radius);
		D = vec3(position.x - x0*radius, position.y - y0*radius, position.z - z0*radius);
	}

	// Render the billboard
	{
		glColor4f(graphRedImmediate, graphGreenImmediate, graphBlueImmediate, graphAlphaImmediate);

		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(A.x, A.y, A.z);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(B.x, B.y, B.z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(C.x, C.y, C.z);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(D.x, D.y, D.z);

		glEnd();
	}
}

void ParticleElement::update(float deltaTime)
{
	if(isDead()) return;

	ASSERT(lifeSpan!=0.0f, _T("m_LifeSpan==0.0, and causes division by zero."));

	const float percent = age / lifeSpan;

	graphSizeImmediate     = graphSize       .   getValue(percent);
	graphAlphaImmediate    = graphAlpha      .   getValue(percent);
	graphRedImmediate      = graphRed        .   getValue(percent);
	graphGreenImmediate    = graphGreen      .   getValue(percent);
	graphBlueImmediate     = graphBlue       .   getValue(percent);

	ParticleBody::update(deltaTime);
}

ParticleEmitter::ParticleEmitter(PropertyBag &Bag, ParticleSystem &Owner)
: owner(&Owner),
  radiusFalloff(1.0f),
  graphSpeedImmediate(0.0f),
  graphSizeMultiplierImmediate(0.0f),
  graphLifeSpanImmediate(0.0),
  age(0.0f),
  lifeSpan(0.0f),
  looping(false),
  numberOfLifeCycles(0)
{
	ASSERT(owner!=0, _T("owner was null"));

	// Get the particle template
	_tstring templateName;
	Bag.get(_T("template"), templateName);
	particleTemplate = owner->getTemplate(templateName);

	PropertyBag Rate;
	Bag.get(_T("rate"), Rate);
	graphEmissionRate.load(Rate);

	PropertyBag Speed;
	Bag.get(_T("speed"), Speed);
	graphSpeed.load(Speed);
	graphSpeedImmediate = graphSpeed.getValue(0.0f);

	PropertyBag SizeMul;
	Bag.get(_T("sizemul"), SizeMul);
	graphSizeMultiplier.load(SizeMul);
	graphSizeMultiplierImmediate = graphSizeMultiplier.getValue(0.0f);

	PropertyBag LifeBag;
	Bag.get(_T("lifespan"), LifeBag);
	graphLifeSpan.load(LifeBag);
	graphLifeSpanImmediate = graphLifeSpan.getValue(0.0f);
	ASSERT(graphLifeSpanImmediate!=0.0f, _T("m_LifeSpan==0.0, and causes division by zero."));

	Bag.get(_T("falloff"),                   radiusFalloff);
	Bag.get(_T("length"),                    lifeSpan);
	Bag.get(_T("looping"),                   looping);
	Bag.get(_T("cycles"),                    numberOfLifeCycles);
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter &emitter)
: owner(emitter.owner),
  particleTemplate(emitter.particleTemplate),
  radiusFalloff(emitter.radiusFalloff),
  graphEmissionRate(emitter.graphEmissionRate),
  graphSpeed(emitter.graphSpeed),
  graphSpeedImmediate(emitter.graphSpeedImmediate),
  graphSizeMultiplier(emitter.graphSizeMultiplier),
  graphSizeMultiplierImmediate(emitter.graphSizeMultiplierImmediate),
  graphLifeSpan(emitter.graphLifeSpan),
  graphLifeSpanImmediate(emitter.graphLifeSpanImmediate),
  age(emitter.age),
  lifeSpan(emitter.lifeSpan),
  looping(emitter.looping),
  numberOfLifeCycles(emitter.numberOfLifeCycles)
{
	ASSERT(owner!=0, _T("owner was null"));
}

ParticleEmitter &ParticleEmitter::operator=(const ParticleEmitter &emitter)
{
	ASSERT(emitter.owner!=0, _T("emitter.owner was null"));

	owner                          =   emitter.owner;
	particleTemplate               =   emitter.particleTemplate;
	radiusFalloff                  =   emitter.radiusFalloff;
	graphEmissionRate              =   emitter.graphEmissionRate;
	age                            =   emitter.age;
	lifeSpan                       =   emitter.lifeSpan;
	looping                        =   emitter.looping;
	numberOfLifeCycles             =   emitter.numberOfLifeCycles;
	graphSpeedImmediate            =   emitter.graphSpeedImmediate;
	graphSpeed                     =   emitter.graphSpeed;
	graphSizeMultiplierImmediate   =   emitter.graphSizeMultiplierImmediate;
	graphSizeMultiplier            =   emitter.graphSizeMultiplier;
	graphLifeSpanImmediate         =   emitter.graphLifeSpanImmediate;
	graphLifeSpan                  =   emitter.graphLifeSpan;

	return(*this);
}

void ParticleEmitter::update(float deltaTime)
{
	ASSERT(owner!=0, _T("owner was null"));
	ASSERT(lifeSpan!=0.0f, _T("lifeSpan==0 -> will cause division by zero"));

	age += deltaTime;

	if(looping && age>lifeSpan)
	{
		age = 0.0f;

		if(numberOfLifeCycles>0)
			numberOfLifeCycles--;
	}

	if(isDead())
	{
		return;
	}

	const float percent = age / lifeSpan;

	graphSpeedImmediate          = graphSpeed          .   getValue(percent);
	graphSizeMultiplierImmediate = graphSizeMultiplier .   getValue(percent);
	graphLifeSpanImmediate       = graphLifeSpan       .   getValue(percent);

	ASSERT(graphLifeSpanImmediate!=0.0f, _T("m_LifeSpanImmediate==0.0, and causes division by zero."));

	size_t numberOfEmittedParticles = (size_t)ceil(graphEmissionRate.getValue(percent));
	for(size_t i=0; i < numberOfEmittedParticles; ++i)
	{
		ParticleElement particle(particleTemplate);

		particle.setSizeMultiplier(graphSizeMultiplierImmediate);

		particle.setLifeSpan(graphLifeSpanImmediate);

		/*
		Chooses a point at a random distance away from the emitter position, and a random direction.
		The random distance follows a Gaussian distribution as radius increases.
		*/
		particle.setPosition(   owner->getPosition() + GetRandomVector(   radiusFalloff * (1 - powf((float)M_E, -SQR(FRAND_RANGE(0,2))))   )   );

		owner->spawn(particle);
	}
}

bool ParticleEmitter::isDead(void) const
{
	return (!looping) ? (age>lifeSpan) : (numberOfLifeCycles==0);
}

void ParticleEmitter::kill(void)
{
	age = lifeSpan + 1.0f; // Run past the length of the animation
	numberOfLifeCycles = 0; // Do not loop
}

ParticleSystem::ParticleSystem(PropertyBag &Bag)
: ParticleBody(Bag),
  maxNumberOfParticles(0),
  emissionBehavior(IGNORE_EMISSION),
  elements(0)
{
	load(Bag);
}

ParticleSystem::ParticleSystem(const ParticleSystem &system)
: ParticleBody(system),
  emitters(system.emitters),
  maxNumberOfParticles(system.maxNumberOfParticles),
  emissionBehavior(system.emissionBehavior),
  elements(0)
{
	// Allocate the m_pElements array
	elements = new ELEMENT_PTR[maxNumberOfParticles];
	ASSERT(elements!=0,  _T("ParticleSystem::System  ->  Null Pointer: m_ppElements."));
	memset(elements, 0, sizeof(ELEMENT_PTR) * maxNumberOfParticles); // Zero the element pointers in the array

	// Copy the particle data
	for(size_t i=0; i<maxNumberOfParticles; ++i)
	{
		elements[i] = NULL;

		if(system.elements[i]) // Assumes that system.m_ppElements[i] is a valid pointer
		{
			elements[i] = new ParticleElement(*system.elements[i]);
			ASSERT(elements[i]!=NULL,  _T("ParticleSystem::ParticleSystem  ->  Null Pointer: m_ppElements[i]."));
		}
	}
}

ParticleSystem::ParticleSystem( const _tstring &fileName )
: ParticleBody(),
  maxNumberOfParticles(0),
  emissionBehavior(IGNORE_EMISSION),
  elements(0)
{
	PropertyBag Bag;
	Bag.loadFromFile(fileName);
	load(Bag);
}

ParticleSystem::ParticleSystem(void)
: ParticleBody(),
  maxNumberOfParticles(0),
  emissionBehavior(IGNORE_EMISSION),
  elements(0)
{}

ParticleSystem::~ParticleSystem(void)
{
	destroyElements();
}

void ParticleSystem::destroyElements(void)
{
	for(size_t i=0; elements!=0 && i<maxNumberOfParticles; ++i)
	{
		delete(elements[i]);
	}

	delete [] elements;
}

void ParticleSystem::load(PropertyBag &Bag)
{
	// Clear the old system
	destroyElements();
	emitters.clear();
	templatesByName.clear();
	materials.clear();

	// Get the number of tags to expect
	const size_t nMaterials = Bag.getNumInstances(_T("material"));
	const size_t nTemplates = Bag.getNumInstances(_T("template"));
	const size_t nEmitters = Bag.getNumInstances(_T("emitter"));

	ASSERT(nMaterials>0, _T("particle system does not specify any materials"));
	ASSERT(nTemplates>0, _T("particle system does not specify any templates"));
	ASSERT(nEmitters>0, _T("particle system does not specify any emitters"));

	// Get the max number of particles
	Bag.get(_T("max"), maxNumberOfParticles);
	ASSERT(maxNumberOfParticles>0, _T("particle system does not give maxNumberOfParticles>0"));

	// Allocate the m_pElements array
	elements = new ELEMENT_PTR[maxNumberOfParticles];
	ASSERT(elements!=0, _T("elements was null"));
	memset(elements, 0, sizeof(ELEMENT_PTR) * maxNumberOfParticles);

	// Load the materials from XML for particles
	for(size_t i=0; i<nMaterials; ++i)
	{
		PropertyBag MatBag;
		Material mat;
		_tstring name, imageFilename;

		// Get material data
		Bag.get(_T("material"), MatBag, i);
		MatBag.get(_T("name"), name);
		MatBag.get(_T("image"), imageFilename);
		MatBag.get(_T("glow"), mat.glow);

		// Create the material
		mat.setName(name);
		mat.loadTexture(imageFilename, 0);

		// Record it.
		materials.push_back(mat);
	}

	// Load the particle templates
	for(size_t i=0; i<nTemplates; ++i)
	{
		PropertyBag data;
		Bag.get(_T("template"), data, i);
		ParticleElement element(data, *this);
		templatesByName.insert(make_pair(element.getName(), element));
	}

	// Load the emitters
	for(size_t i=0; i<nEmitters; ++i)
	{
		PropertyBag EmitterBag;
		Bag.get(_T("emitter"), EmitterBag, i);
		ParticleEmitter emitter(EmitterBag, *this);
		emitters.push_back(emitter);
	}

	// Load the base class
	ParticleBody::load(Bag);

	ASSERT(!materials.empty(),       _T("after loading, there are no particle materials in system"));
	ASSERT(!templatesByName.empty(), _T("after loading, there are no particle templates in emitter"));
	ASSERT(!emitters.empty(),        _T("after loading, there are no particle emitters in system"));
}

void ParticleSystem::draw(void) const
{
	ASSERT(elements!=0, _T("Cannot draw particles because none were allocated!"));

	float matrix[16] = {0};

	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

	glPushMatrix();
	glDepthMask(GL_FALSE);

		for(size_t i = 0; i<maxNumberOfParticles; ++i)
		{
			if(elements[i] && !elements[i]->isDead())
				elements[i]->draw(matrix);
		}

	glDepthMask(GL_TRUE);
	glPopMatrix();
}

void ParticleSystem::update(float dTime)
{
	for(size_t i=0; i<emitters.size(); ++i)
		emitters[i].update(dTime);

	ASSERT(elements!=0, _T("Cannot update particles: no particle storage has even been allocated!"));

	for(size_t i=0; i<maxNumberOfParticles; ++i)
	{
		if(!elements[i] || (elements[i] && elements[i]->isDead()))
		{
			delete(elements[i]);
			elements[i] = 0;
		}
		else
		{
			elements[i]->update(dTime);
		}
	}

	ParticleBody::update(dTime);
}

void ParticleSystem::spawn(const ParticleElement &element)
{
	ASSERT(elements!=0, _T("Cannot spawn particle: no particle storage was allocated!"));

	for(size_t i=0; i<maxNumberOfParticles; ++i)
	{
		if(elements[i] == 0)
		{
			elements[i] = new ParticleElement(element);
			return;
		}
	}

	if(emissionBehavior == REPLACE_RANDOM)
	{
		size_t i = IRAND_RANGE(0, maxNumberOfParticles-1);

		delete(elements[i]);
		elements[i] = new ParticleElement(element);

		return;
	}
}

const ParticleElement& ParticleSystem::getTemplate(const _tstring &name)
{
	ASSERT(templatesByName.find(name)!=templatesByName.end(), _T("Particle template could not be found: ") + name);

	return(templatesByName.find(name)->second);
}

size_t ParticleSystem::getMaterialHandle(const _tstring &materialName) const
{
	for(size_t i=0; i<materials.size(); ++i)
	{
		if(materials[i].getName() == materialName)
		{
			return i;
		}
	}

	FAIL(_T("Particle material could not be found: ") + materialName);
	return 0;
}

Material& ParticleSystem::getMaterial(size_t materialHandle)
{
	ASSERT(materialHandle < materials.size(), _T("Parameter \'materialHandle\' out of bounds!"));

	return materials[materialHandle];
}

bool ParticleSystem::isDead(void) const
{
	for(size_t i=0; i<emitters.size(); ++i)
	{
		if(!emitters[i].isDead()) return false;
	}

	return true;
}

void ParticleSystem::kill(void)
{
	for(size_t i=0; i<emitters.size(); ++i)
	{
		emitters[i].kill();
	}

	for(size_t i=0; i<maxNumberOfParticles; ++i)
	{
		delete(elements[i]);
		elements[i] = 0;
	}
}

} // namespace Engine
