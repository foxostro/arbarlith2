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
: m_vPosition(0,0,0),
initialVelocity(0,0,0),
m_Age(0),
m_vInitialPosition(0,0,0),
m_vConstantAcceleration(0,0,0),
center(0,0,0),
initialRadialVelocity(0)
{}

ParticleBody::ParticleBody(CPropBag &Bag)
: m_vPosition(0,0,0),
initialVelocity(0,0,0),
m_Age(0),
m_vInitialPosition(0,0,0),
m_vConstantAcceleration(0,0,0),
center(0,0,0),
initialRadialVelocity(0)
{
	LoadXml(Bag);
}

ParticleBody::ParticleBody(const ParticleBody &body)
: m_vPosition(body.m_vPosition),
initialVelocity(body.initialVelocity),
m_Age(body.m_Age),
m_vInitialPosition(body.m_vInitialPosition),
m_vConstantAcceleration(body.m_vConstantAcceleration),
center(body.center),
initialRadialVelocity(body.initialRadialVelocity)
{}

bool ParticleBody::LoadXml(CPropBag &Bag)
{
	Bag.Get(_T("position"),				&m_vInitialPosition);
	Bag.Get(_T("acceleration"),			&m_vConstantAcceleration);
	Bag.Get(_T("initialRadialVelocity"), initialRadialVelocity);

	// The Body starts, of course, at the initial position
	m_vPosition = m_vInitialPosition;

	// Reset age to zero
	m_Age = 0.0;

	return true;
}

void ParticleBody::Update(float dTime)
{
	m_Age += dTime;

	float Time = m_Age / 1000.0f;

	m_vPosition = (m_vConstantAcceleration * 0.5f * SQR(Time)) +
		          (initialVelocity * Time) +
				   m_vInitialPosition;
}

void ParticleBody::setPosition(const vec3 &position, const vec3 &center)
{
	m_vInitialPosition = position;
	this->center = center;

	initialVelocity = vec3(m_vInitialPosition-center).getNormal() * initialRadialVelocity * FRAND_RANGE(0.8f, 1.2f);
}

ParticleGraph::ParticleGraph(void)
: m_MinX(0.0f),
  m_MaxX(0.0f),
  m_MinY(0.0f),
  m_MaxY(0.0f)
{}

ParticleGraph::ParticleGraph(CPropBag &)
: m_MinX(0.0f),
  m_MaxX(0.0f),
  m_MinY(0.0f),
  m_MaxY(0.0f)
{}

bool ParticleGraph::LoadXml(CPropBag &Bag)
{
	m_Points.clear();

	m_MinX = 0.0f;
	m_MaxX = 0.0f;
	m_MinY = 0.0f;
	m_MaxY = 0.0f;

	size_t nPoints = Bag.GetNumInstances(_T("point"));

	for(size_t i=0; i<nPoints; ++i)
	{
		CPropBag PointBag;
		Point2 pt;

		Bag.Get(_T("point"), PointBag, (int)i);
		PointBag.Get(_T("x"), pt.x);
		PointBag.Get(_T("y"), pt.y);

		m_Points.push_back(pt);

		if(pt.x < m_MinX) m_MinX = pt.x;
		if(pt.y < m_MinY) m_MinY = pt.y;
		if(pt.x > m_MaxX) m_MaxX = pt.x;
		if(pt.y > m_MaxY) m_MaxY = pt.y;
	}

	return true;
}

float ParticleGraph::GetValue(float x)
{
	size_t nSize=m_Points.size();

	if(nSize == 0)
	{
		return 0.0f;
	}
	else if(nSize == 1) // If there is only one point
	{
		return (m_Points[0].y);
	}
	else
	{
		for(size_t i=0; i<nSize-1; ++i)
		{
			Point2 one = m_Points[i+0];
			Point2 two = m_Points[i+1];

			if(x >= one.x && x <= two.x)
			{
				float slope = (two.y - one.y) / (two.x - one.x);
				float value = slope*(x - one.x) + one.y;

				return value;
			}
		}
	}

	return 0.0f;
}

ParticleElement::ParticleElement(void)
: ParticleBody(),
  m_nMaterialHandle(0),
  m_pOwner(0),
  m_LifeSpan(0.0f),
  m_bRandomRotationDirection(false),
  m_bRotationDirection(true),
  m_SizeMul(1.0f),
  m_RotationImmediate(0.0f),
  m_SizeImmediate(0.0f),
  m_AlphaImmediate(0.0f),
  m_RedImmediate(0.0f),
  m_GreenImmediate(0.0f),
  m_BlueImmediate(0.0f)
{}

ParticleElement::ParticleElement(CPropBag &Bag, ParticleSystem &system)
: ParticleBody(Bag),
  m_nMaterialHandle(0),
  m_pOwner(&system),
  m_LifeSpan(1000.0f),
  m_bRandomRotationDirection(false),
  m_bRotationDirection(true),
  m_SizeMul(1.0f),
  m_RotationImmediate(0.0f),
  m_SizeImmediate(0.0f),
  m_AlphaImmediate(0.0f),
  m_RedImmediate(0.0f),
  m_GreenImmediate(0.0f),
  m_BlueImmediate(0.0f)
{
	LoadXml(Bag, system);
}

ParticleElement::ParticleElement(const ParticleElement &element)
: ParticleBody(element),
  m_nMaterialHandle(element.m_nMaterialHandle),
  m_pOwner(element.m_pOwner),
  m_LifeSpan(element.m_LifeSpan),
  m_strName(element.m_strName),
  m_bRandomRotationDirection(element.m_bRandomRotationDirection),
  m_bRotationDirection(element.m_bRotationDirection),
  m_SizeMul(element.m_SizeMul),
  m_RotationGraph(element.m_RotationGraph),
  m_SizeGraph(element.m_SizeGraph),
  m_AlphaGraph(element.m_AlphaGraph),
  m_RedGraph(element.m_RedGraph),
  m_GreenGraph(element.m_GreenGraph),
  m_BlueGraph(element.m_BlueGraph),
  m_RotationImmediate(element.m_RotationImmediate),
  m_SizeImmediate(element.m_SizeImmediate),
  m_AlphaImmediate(element.m_AlphaImmediate),
  m_RedImmediate(element.m_RedImmediate),
  m_GreenImmediate(element.m_GreenImmediate),
  m_BlueImmediate(element.m_BlueImmediate)
{}

bool ParticleElement::LoadXml(CPropBag &Bag, ParticleSystem &system)
{
	m_pOwner = &system;

	CPropBag RotationGraph, SizeGraph, AlphaGraph, RedGraph, GreenGraph, BlueGraph;

	// Reset defaults
	m_LifeSpan = 1000.0f;

	// Get misc. data
	Bag.Get(_T("name"),                      m_strName);
	Bag.Get(_T("random rotation direction"), m_bRandomRotationDirection);
	Bag.Get(_T("rotation direction"),        m_bRotationDirection);

	// Load material data
	_tstring strMatName;
	Bag.Get(_T("material"), strMatName);
	m_nMaterialHandle = m_pOwner->GetMaterialHandle(strMatName); // Have the system give us the material's handle

	// Load graph data
	Bag.Get(_T("rotation"), RotationGraph);
	Bag.Get(_T("size"),     SizeGraph);
	Bag.Get(_T("alpha"),    AlphaGraph);
	Bag.Get(_T("red"),      RedGraph);
	Bag.Get(_T("green"),    GreenGraph);
	Bag.Get(_T("blue"),     BlueGraph);

	m_RotationGraph.LoadXml(RotationGraph);
	m_SizeGraph.LoadXml(SizeGraph);
	m_AlphaGraph.LoadXml(AlphaGraph);
	m_RedGraph.LoadXml(RedGraph);
	m_GreenGraph.LoadXml(GreenGraph);
	m_BlueGraph.LoadXml(BlueGraph);

	// Gets the first values for the graph data
	m_RotationImmediate = m_RotationGraph.GetValue(0.0f);
	m_SizeImmediate     = m_SizeGraph.GetValue(0.0f);
	m_AlphaImmediate    = m_AlphaGraph.GetValue(0.0f);
	m_RedImmediate      = m_RedGraph.GetValue(0.0f);
	m_GreenImmediate    = m_GreenGraph.GetValue(0.0f);
	m_BlueImmediate     = m_BlueGraph.GetValue(0.0f);

	// Load physics data
	ParticleBody::LoadXml(Bag);

	return true;
}

ParticleElement &ParticleElement::operator=(const ParticleElement &element)
{
	m_vPosition             = element.m_vPosition;
	initialRadialVelocity	= element.initialRadialVelocity;
	initialVelocity         = element.initialVelocity;
	center                  = element.center;
	m_vConstantAcceleration = element.m_vConstantAcceleration;

	m_AlphaGraph               = element.m_AlphaGraph;
	m_AlphaImmediate           = element.m_AlphaImmediate;
	m_BlueGraph                = element.m_BlueGraph;
	m_BlueImmediate            = element.m_BlueImmediate;
	m_bRandomRotationDirection = element.m_bRandomRotationDirection;
	m_bRotationDirection       = element.m_bRotationDirection;
	m_GreenGraph               = element.m_GreenGraph;
	m_GreenImmediate           = element.m_GreenImmediate;
	m_LifeSpan                 = element.m_LifeSpan;
	m_RedGraph                 = element.m_RedGraph;
	m_RedImmediate             = element.m_RedImmediate;
	m_RotationGraph            = element.m_RotationGraph;
	m_RotationImmediate        = element.m_RotationImmediate;
	m_SizeGraph                = element.m_SizeGraph;
	m_SizeImmediate            = element.m_SizeImmediate;
	m_strName                  = element.m_strName;
	m_pOwner                   = element.m_pOwner;
    m_SizeMul                  = element.m_SizeMul;
	m_nMaterialHandle          = element.m_nMaterialHandle;

	return(*this);
}

void ParticleElement::PrepareForRender(float x0, float x1, float y0, float y1, float z0, float z1)
{
	const vec3 pos(m_vPosition);

	float radius = m_SizeMul*m_SizeImmediate;

	A = vec3(pos.x - x1*radius, pos.y - y1*radius, pos.z - z1*radius);
	B = vec3(pos.x + x0*radius, pos.y + y0*radius, pos.z + z0*radius);
	C = vec3(pos.x + x1*radius, pos.y + y1*radius, pos.z + z1*radius);
	D = vec3(pos.x - x0*radius, pos.y - y0*radius, pos.z - z0*radius);
}

void ParticleElement::Render(void)
{
	glColor4f(m_RedImmediate, m_GreenImmediate, m_BlueImmediate, m_AlphaImmediate);
	Material &material = m_pOwner->GetMaterial(m_nMaterialHandle);
	material.bind();
	if(material.glow)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render the billboard
	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(A.x, A.y, A.z);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(B.x, B.y, B.z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(C.x, C.y, C.z);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(D.x, D.y, D.z);

	glEnd();
}

void ParticleElement::Update(float dTime)
{
	if(IsDead() == false)
	{
		ParticleBody::Update(dTime);

		ASSERT(m_LifeSpan!=0.0, _T("m_LifeSpan==0.0, and causes division by zero."));

		float Percent = m_Age / m_LifeSpan;

		m_RotationImmediate = m_RotationGraph.GetValue(Percent);
		m_SizeImmediate     = m_SizeGraph.GetValue(Percent);
		m_AlphaImmediate    = m_AlphaGraph.GetValue(Percent);
		m_RedImmediate      = m_RedGraph.GetValue(Percent);
		m_GreenImmediate    = m_GreenGraph.GetValue(Percent);
		m_BlueImmediate     = m_BlueGraph.GetValue(Percent);
	}
}

ParticleEmitter::ParticleEmitter(CPropBag &Bag, ParticleSystem &Owner)
: m_pOwner(&Owner),
  m_bRandomRotationDirection(false),
  m_bRotationDirection(true),
  m_vRotationAxis(0.0f, 1.0f, 0.0f),
  m_RotationImmediate(0.0f),
  m_HotspotRadius(1.0f),
  m_FalloffRadius(1.0f),
  m_EmissionRateImmediate(0.0f),
  m_SpeedImmediate(0.0f),
  m_SizeMulImmediate(0.0f),
  m_LifeSpanImmediate(0.0),
  m_Time(0.0f),
  m_Length(0.0f),
  m_bLooping(false),
  m_iNumCycles(0)
{
	ASSERT(m_pOwner!=NULL, _T("ParticleEmitter::Emitter  ->  Null Pointer: m_pOwner."));

	// Get the particle template
	_tstring strTemplate;
	Bag.Get(_T("template"), strTemplate);
	m_Template = m_pOwner->GetTemplate(strTemplate);

	CPropBag Rate;
	Bag.Get(_T("rate"), Rate);
	m_EmissionRateGraph.LoadXml(Rate);
	m_EmissionRateImmediate = m_EmissionRateGraph.GetValue(0.0f);

	CPropBag Speed;
	Bag.Get(_T("speed"), Speed);
	m_SpeedGraph.LoadXml(Speed);
	m_SpeedImmediate = m_SpeedGraph.GetValue(0.0f);

	CPropBag SizeMul;
	Bag.Get(_T("sizemul"), SizeMul);
	m_SizeMulGraph.LoadXml(SizeMul);
	m_SizeMulImmediate = m_SizeMulGraph.GetValue(0.0f);

	CPropBag LifeBag;
	Bag.Get(_T("lifespan"), LifeBag);
	m_LifeSpanGraph.LoadXml(LifeBag);
	m_LifeSpanImmediate = m_LifeSpanGraph.GetValue(0.0f);
	ASSERT(m_LifeSpanImmediate!=0.0, _T("m_LifeSpan==0.0, and causes division by zero."));

	Bag.Get(_T("random rotation direction"), m_bRandomRotationDirection);
	Bag.Get(_T("rotation direction"),        m_bRotationDirection);
	Bag.Get(_T("rotation axis"),            &m_vRotationAxis);
	Bag.Get(_T("hotspot"),                   m_HotspotRadius);
	Bag.Get(_T("falloff"),                   m_FalloffRadius);
	Bag.Get(_T("length"),                    m_Length);
	Bag.Get(_T("looping"),                   m_bLooping);
	Bag.Get(_T("cycles"),                    m_iNumCycles);
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter &emitter)
: m_pOwner(emitter.m_pOwner),
  m_Template(emitter.m_Template),
  m_bRandomRotationDirection(emitter.m_bRandomRotationDirection),
  m_bRotationDirection(emitter.m_bRotationDirection),
  m_vRotationAxis(emitter.m_vRotationAxis),
  m_RotationGraph(emitter.m_RotationGraph),
  m_RotationImmediate(emitter.m_RotationImmediate),
  m_HotspotRadius(emitter.m_HotspotRadius),
  m_FalloffRadius(emitter.m_FalloffRadius),
  m_EmissionRateGraph(emitter.m_EmissionRateGraph),
  m_EmissionRateImmediate(emitter.m_EmissionRateImmediate),
  m_SpeedGraph(emitter.m_SpeedGraph),
  m_SpeedImmediate(emitter.m_SpeedImmediate),
  m_SizeMulGraph(emitter.m_SizeMulGraph),
  m_SizeMulImmediate(emitter.m_SizeMulImmediate),
  m_LifeSpanGraph(emitter.m_LifeSpanGraph),
  m_LifeSpanImmediate(emitter.m_LifeSpanImmediate),
  m_Time(emitter.m_Time),
  m_Length(emitter.m_Length),
  m_bLooping(emitter.m_bLooping),
  m_iNumCycles(emitter.m_iNumCycles)
{
	ASSERT(m_pOwner!=0, _T("m_pOwner was null"));
}

ParticleEmitter &ParticleEmitter::operator=(const ParticleEmitter &emitter)
{
	m_pOwner                        = emitter.m_pOwner;
	m_Template                      = emitter.m_Template;
	m_bRandomRotationDirection      = emitter.m_bRandomRotationDirection;
	m_bRotationDirection            = emitter.m_bRotationDirection;
	m_vRotationAxis                 = emitter.m_vRotationAxis;
	m_RotationGraph                 = emitter.m_RotationGraph;
	m_RotationImmediate             = emitter.m_RotationImmediate;
	m_HotspotRadius                 = emitter.m_HotspotRadius;
	m_FalloffRadius                 = emitter.m_FalloffRadius;
	m_EmissionRateGraph             = emitter.m_EmissionRateGraph;
	m_Time                          = emitter.m_Time;
	m_Length                        = emitter.m_Length;
	m_bLooping                      = emitter.m_bLooping;
	m_iNumCycles                    = emitter.m_iNumCycles;
	m_EmissionRateImmediate         = emitter.m_EmissionRateImmediate;
	m_SpeedImmediate                = emitter.m_SpeedImmediate;
	m_SpeedGraph                    = emitter.m_SpeedGraph;
	m_SizeMulImmediate              = emitter.m_SizeMulImmediate;
	m_SizeMulGraph                  = emitter.m_SizeMulGraph;
	m_LifeSpanImmediate             = emitter.m_LifeSpanImmediate;
	m_LifeSpanGraph                 = emitter.m_LifeSpanGraph;

	ASSERT(m_pOwner!=0, _T("m_pOwner was null"));

	return(*this);
}

void ParticleEmitter::Update(float dTime)
{
	ASSERT(m_Length!=0.0, _T("m_Length==0.0, and causes division by zero."));

	m_Time += dTime;

	// Reset, if looping
	if(m_bLooping && m_Time>m_Length)
	{
		m_Time = 0.0f;

		if(m_iNumCycles>0)
			m_iNumCycles--;
	}

	if(!IsDead())
	{
		ASSERT(m_pOwner!=0, _T("m_pOwner was null"));

		// Update immediate graph values
		float Percent = m_Time / m_Length;
		m_EmissionRateImmediate = m_EmissionRateGraph.GetValue(Percent);
		m_SpeedImmediate		= m_SpeedGraph.GetValue(Percent);
		m_SizeMulImmediate		= m_SizeMulGraph.GetValue(Percent);
		m_LifeSpanImmediate		= m_LifeSpanGraph.GetValue(Percent);
		ASSERT(m_LifeSpanImmediate!=0.0, _T("m_LifeSpanImmediate==0.0, and causes division by zero."));

		// Spawn new particles
		size_t nEmits = (size_t)ceil(m_EmissionRateImmediate);

		for(size_t i=0; i < nEmits; ++i)
		{
			ParticleElement particle(m_Template);

			vec3 origin = m_pOwner->getPosition();

			// Ignore m_HotSpot
			// Very much like a normal distribution
			float x = FRAND_RANGE(0,2);
			float y = (1 - powf((float)M_E, -SQR(x))) * m_FalloffRadius;
			particle.setPosition(origin + GetRandomVector(y), origin);

			// Set other relevant data members
			particle.SizeMul()  = m_SizeMulImmediate;
			particle.m_LifeSpan = m_LifeSpanImmediate;

			// Spawn the particle
			m_pOwner->Spawn(particle);
		}
	}
}

ParticleSystem::ParticleSystem(CPropBag &Bag)
: ParticleBody(Bag),
  m_nMaxParticles(0),
  m_ppElements(NULL),
  m_Behavior(EMIT_WAIT)
{
	LoadXml(Bag);
}

ParticleSystem::ParticleSystem(const ParticleSystem &system)
: ParticleBody(system),
  m_Emitters(system.m_Emitters),
  m_nMaxParticles(system.m_nMaxParticles),
  m_ppElements(NULL),
  m_Behavior(system.m_Behavior)
{
	// Allocate the m_pElements array
	m_ppElements = new ELEMENT_PTR[m_nMaxParticles];
	ASSERT(m_ppElements!=NULL,  _T("ParticleSystem::System  ->  Null Pointer: m_ppElements."));
	memset(m_ppElements, 0, sizeof(ELEMENT_PTR) * m_nMaxParticles); // Zero the element pointers in the array

	// Copy the particle data
	for(size_t i=0; i<m_nMaxParticles; ++i)
	{
		m_ppElements[i] = NULL;

		if(system.m_ppElements[i]) // Assumes that system.m_ppElements[i] is a valid pointer
		{
			m_ppElements[i] = new ParticleElement(*system.m_ppElements[i]);
			ASSERT(m_ppElements[i]!=NULL,  _T("ParticleSystem::ParticleSystem  ->  Null Pointer: m_ppElements[i]."));
		}
	}
}

ParticleSystem::~ParticleSystem(void)
{
	if(m_ppElements)
	{
		for(size_t i=0; i<m_nMaxParticles; ++i)
		{
			delete(m_ppElements[i]);
		}

		delete[](m_ppElements);
	}
}

bool ParticleSystem::LoadXml(CPropBag &Bag)
{
	int iTemp=0;

	// Clear the old system
	m_Emitters.clear();
	m_Templates.clear();
	m_Materials.clear();
	m_Frames.clear();

	// Delete all particles
	if(m_ppElements)
	{
		for(size_t i=0; i<m_nMaxParticles; ++i)
		{
			delete(m_ppElements[i]);
		}

		delete[](m_ppElements);
	}

	// Get the number of tags to expect
	int nMaterials = Bag.GetNumInstances(_T("material"));
	int nTemplates = Bag.GetNumInstances(_T("template"));
	int nEmitters = Bag.GetNumInstances(_T("emitter"));

	ASSERT(nMaterials>0, _T("particle system does not specify any materials"));
	ASSERT(nTemplates>0, _T("particle system does not specify any templates"));
	ASSERT(nEmitters>0, _T("particle system does not specify any emitters"));

	// Get the max number of particles
	Bag.Get(_T("max"), iTemp);
	m_nMaxParticles = (size_t)iTemp;
	ASSERT(m_nMaxParticles>0, _T("particle system does not give m_nMaxParticles>0"));

	// Allocate the m_pElements array
	m_ppElements = new ELEMENT_PTR[m_nMaxParticles];
	ASSERT(m_ppElements!=NULL, _T("Null Pointer: m_ppElements."));
	memset(m_ppElements, 0, sizeof(ELEMENT_PTR) * m_nMaxParticles);

	// Load the materials from XML for particles
	for(int i=0; i<nMaterials; ++i)
	{
		CPropBag MatBag;
		Material mat;
		_tstring name, imageFilename;

		// Get material data
		Bag.Get(_T("material"), MatBag, i);
		MatBag.Get(_T("name"), name);
		MatBag.Get(_T("image"), imageFilename);
		MatBag.Get(_T("glow"), mat.glow);

		// Create the material
		mat.setName(name);
		mat.loadTexture(imageFilename, 0);

		// Record it.
		m_Materials.push_back(mat);
		m_Frames.push_back(0);
	}

	// Load the particle templates
	for(int i=0; i<nTemplates; ++i)
	{
		CPropBag ElBag;

		// Load the particle template
		Bag.Get(_T("template"), ElBag, i);
		ParticleElement element(ElBag, *this);

		// Record it.
		m_Templates.insert(make_pair(element.GetName(), element));
	}

	// Load the emitters
	for(int i=0; i<nEmitters; ++i)
	{
		CPropBag EmitterBag;

		Bag.Get(_T("emitter"), EmitterBag, i);
		ParticleEmitter emitter(EmitterBag, *this);

		m_Emitters.push_back(emitter);
	}

	// Load the base class
	ParticleBody::LoadXml(Bag);

	ASSERT(!m_Materials.empty(), _T("after loading, there are no particle materials in system"));
	ASSERT(!m_Templates.empty(), _T("after loading, there are no particle templates in emitter"));
	ASSERT(!m_Emitters.empty(), _T("after loading, there are no particle emitters in system"));

	return true;
}

void ParticleSystem::Render(void)
{
	vec3 vPos = getPosition();

	glPushMatrix();

	if(m_ppElements)
	{
		float matrix[16];

		glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

		float x0 = matrix[0] - matrix[1];
		float x1 = matrix[0] + matrix[1];

		float y0 = matrix[4] - matrix[5];
		float y1 = matrix[4] + matrix[5];

		float z0 = matrix[8] - matrix[9];
		float z1 = matrix[8] + matrix[9];

		// Prepare particles
		for(size_t i = 0; i<m_nMaxParticles; ++i)
		{
			if(m_ppElements[i])
			{
				if(m_ppElements[i]->IsDead() == false)
				{
					m_ppElements[i]->PrepareForRender(x0, x1, y0, y1, z0, z1);
				}
			}
		} // end for

		m_ppSorted = m_ppElements;

		// For each particle
		glDepthMask(GL_FALSE);
		for(size_t i = 0; i<m_nMaxParticles; ++i)
		{
			if(m_ppSorted[i])
			{
				if(m_ppSorted[i]->IsDead() == false)
				{
					m_ppSorted[i]->Render();
				}
			}
		}

		glDepthMask(GL_TRUE);
	}

	glPopMatrix();
}

void ParticleSystem::Update(float dTime)
{
	// Update the system as a physical body
	ParticleBody::Update(dTime);

	// Process all the emitters
	for(size_t i=0; i<m_Emitters.size(); ++i)
	{
		m_Emitters[i].Update(dTime);
	}

	// Process all the particles
	if(m_ppElements)
	{
		for(size_t i=0; i<m_nMaxParticles; ++i)
		{
			if(m_ppElements[i])
			{
				if(m_ppElements[i]->IsDead() == false)
				{
					m_ppElements[i]->Update(dTime);
				}
				else
				{
					ParticleElement *pParticle = m_ppElements[i];
					delete(pParticle);
					m_ppElements[i] = 0;
				}
			}
		} // end for
	}
}

void ParticleSystem::Spawn(const ParticleElement &element)
{
	float Oldest=0.0f;
	size_t iOldest=0;

	if(m_ppElements)
	{
		size_t i;
		for(i=0; i<m_nMaxParticles; ++i)
		{
			if(m_ppElements[i] == NULL)
			{
				m_ppElements[i] = new ParticleElement(element);
				ASSERT(m_ppElements[i]!=NULL, _T("ParticleSpawn::Spawn  ->  Null Pointer: m_ppElements[i]."));
				return;
			}
			else
			{
				// Find the oldest particle in case we need to EMIT_REPLACE_OLDEST
				if(m_ppElements[i]->GetAge() > Oldest)
				{
					Oldest = m_ppElements[i]->GetAge();
					iOldest = i;
				}
			}
		} // end for

		if(m_Behavior == EMIT_REPLACE_OLDEST)
		{
			delete(m_ppElements[iOldest]);
			m_ppElements[iOldest] = new ParticleElement(element);
			ASSERT(m_ppElements[iOldest]!=NULL, _T("ParticleSpawn::Spawn  ->  Replace Oldest; Null Pointer: m_ppElements[iOldest]."));
			return;
		}
		else if(m_Behavior == EMIT_REPLACE_RANDOM)
		{
			i = IRAND_RANGE(0, m_nMaxParticles-1);

			delete(m_ppElements[i]);
			m_ppElements[i] = new ParticleElement(element);
			ASSERT(m_ppElements[i]!=NULL, _T("ParticleSpawn::Spawn  ->  Replace Random; Null Pointer: m_ppElements[i]."));
			return;
		}
	}
}

ParticleElement ParticleSystem::GetTemplate(_tstring strName)
{
	std::multimap<_tstring, ParticleElement>::iterator iter = m_Templates.find(strName);

	ASSERT(iter!=m_Templates.end(), _T("ParticleSystem::GetTemplate  ->  Invalid Iterator"));

	return(iter->second);
}

size_t ParticleSystem::GetMaterialHandle(const _tstring &strName) const
{
	for(size_t i=0; i<m_Materials.size(); ++i)
	{
		if(m_Materials[i].getName() == strName)
		{
			return i;
		}
	}

	return 0;
}

Material& ParticleSystem::GetMaterial(size_t nHandle)
{
	ASSERT(nHandle < m_Materials.size(), _T("ParticleSystem::GetTemplate  ->  Index out of bounds."));

	return m_Materials[nHandle];
}

bool ParticleSystem::IsDead()
{
	bool bResult=false;

	// If all of the emitters are dead, then we are dead too.
	for(size_t i=0; i<m_Emitters.size(); ++i)
	{
		ParticleEmitter &emitter = m_Emitters[i];

		bResult &= emitter.IsDead();
	}

	return bResult;
}

float ParticleSystem::getRadius(void) const
{
	float radius = 0;

	for(size_t i=0; i<m_Emitters.size(); ++i)
	{
		radius = max(radius, m_Emitters[i].getFalloffRadius());
	}

	return radius;
}

void ParticleSystem::Kill()
{
	size_t i=0;

	for(i=0; i<m_Emitters.size(); ++i)
	{
		m_Emitters[i].Kill();
	}

	// Kill all particles here
	for(i=0; i<m_nMaxParticles; ++i)
	{
		ParticleElement *pParticle = m_ppElements[i];
		delete(pParticle);
		m_ppElements[i] = 0;
	}
}

}; // namespace
