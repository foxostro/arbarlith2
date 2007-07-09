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

#include "propbag.h"
#include "3dmath.h"
#include "vec4.h"
#include "mesh.h" // BoundingBox

namespace Engine {

		class ParticleSystem; // Prototype definition

		/** A body that follows the laws of physics */
		class ParticleBody
		{
		protected:
			/** Position of the body */
			vec3 m_vPosition;

			vec3 initialVelocity;

			/** Milliseconds since creation*/
			float m_Age;

		public:
			vec3 m_vInitialPosition;
			vec3 m_vConstantAcceleration;
			vec3 center;
			float initialRadialVelocity;

		public:
			/** Constructor */
			ParticleBody();

			/**
			Constructor
			@param Bag Reference to a loaded xml data class
			*/
			ParticleBody(CPropBag &Bag);

			/**
			Loads data from xml and returns success(true) or failure(false)
			@param Bag Reference to a loaded xml data class
			@return true if the data loads successfully
			*/
			bool LoadXml(CPropBag &Bag);

			/**
			Copy Constructor
			@param body The object to copy
			*/
			ParticleBody(const ParticleBody &body);

			///  Name: Body::Update
			///  Desc: Updates the particle position and sets the immediate values for graph related data
			/// Param: float dTime  --  Delta time (milliseconds) since last update
			virtual void Update(float dTime);

			///  Name: Body::getPosition
			///  Desc: Returns the position of the Body
			/// Param: void
			const vec3& getPosition() const { return m_vPosition; }

			void setPosition(const vec3 &position, const vec3 &center);
		};

		// Records a list of 2D points and can calculate the graph's value for any x-value
		class ParticleGraph
		{
		private:
			vector<Point2> m_Points;

		public:
			///  Name: Graph::Graph
			///  Desc: Constructor
			/// Param: void
			ParticleGraph(void);

			///  Name: Graph::Graph
			///  Desc: Constructor
			/// Param: CPropBag &Bag  --  Reference to a loaded xml data class
			ParticleGraph(CPropBag &Bag);

			///  Name: Graph::LoadXml
			///  Desc: Loads graph data from xml and returns success(true) or failure(false)
			/// Param: CPropBag &Bag  --  Reference to a loaded xml data class
			bool LoadXml(CPropBag &Bag);

			///  Name: Graph::GetValue
			///  Desc: Returns the value of the graph when x equals the given value.
			/// Param: float x  --  Arbitrary x value on graph and within its domain.
			float GetValue(float x);

			float m_MinX, m_MaxX;
			float m_MinY, m_MaxY;
		};

		// A particle
		class ParticleElement : public ParticleBody
		{
		public:
			/** This handle could be passed to the System to obtain the material we want to use */
			size_t m_nMaterialHandle;

			/** ParticleSystem that owns this element */
			ParticleSystem *m_pOwner;

			/** Milliseconds that this element will be alive for */
			float m_LifeSpan;

		private:
			// Name of the particle type
			_tstring m_strName;

			// Rotation of the particle versus time
			// Please note that no angular kinematics are performed, this data
			// is only for rendering purposes
			bool m_bRandomRotationDirection; // Choose rotation direction apon creation randomly?
			bool m_bRotationDirection; // Left or right?

			float m_SizeMul;

			// Graph related data
			ParticleGraph m_RotationGraph; // Rotation in radians over time
			ParticleGraph m_SizeGraph;     // Size in meters over time
			ParticleGraph m_AlphaGraph;    // Alpha component over time
			ParticleGraph m_RedGraph;      // Red component over time
			ParticleGraph m_GreenGraph;    // Green component over time
			ParticleGraph m_BlueGraph;     // Blue component over time

			// All graph related data must have an immediate value for the current cycle.
			float m_RotationImmediate;
			float m_SizeImmediate;
			float m_AlphaImmediate;
			float m_RedImmediate;
			float m_GreenImmediate;
			float m_BlueImmediate;

			// Billboard vertices
			vec3 A, B, C, D;

		public:
			/** Destructor */
			virtual ~ParticleElement(void)
			{}

			///  Name: Element::Element
			///  Desc: Constructor
			/// Param: void
			ParticleElement(void);

			///  Name: Element::Element
			///  Desc: Constructor
			/// Param: CPropBag &Bag             --  Reference to a loaded xml data class
			///        Particle::System &system  --  Reference yo the particle system that owns this element
			ParticleElement(CPropBag &Bag, ParticleSystem &system);

			///  Name: Element::Element
			///  Desc: Copy Constructor
			ParticleElement(const ParticleElement &element);

			/// Name: operator=
			/// Desc: Assignment Operator
			ParticleElement &operator=(const ParticleElement &element);

			///  Name: Element::LoadXml
			///  Desc: Loads data from xml and returns success (true) or failure (false)
			/// Param: CPropBag &Bag             --  Reference to a loaded xml data class
			///        Particle::System &system  --  Reference yo the particle system that owns this element
			bool LoadXml(CPropBag &Bag, ParticleSystem &system);

			/// Name: Element::PrepareForRender
			///  Desc: Calculates the billboard vertices for Particle::Element::Render
			/// Param: float x0                --  matrix[0] - matrix[1]
			///        float x1                --  matrix[0] + matrix[1]
			///        float y0                --  matrix[4] - matrix[5]
			///        float y1                --  matrix[4] + matrix[5]
			///        float z0                --  matrix[8] - matrix[9]
			///        float z1                --  matrix[8] + matrix[9]
			///             (matrix[16] represents the modelview matrix)
			void PrepareForRender(float x0, float x1, float y0, float y1, float z0, float z1);

			///  Name: Element::Render
			///  Desc: Renders the particle
			void Render();

			///  Name: Element::Update
			///  Desc: Updates the particle position and sets the immediate values for graph related data
			/// Param: float dTime  --  Delta time (milliseconds) since last update
			void Update(float dTime);

			///  Name: Element::IsDead
			///  Desc: Is the particle dead?
			/// Param: void
			inline bool IsDead(void)
			{
				return m_Age>m_LifeSpan;
			}

			///  Name: Element::GetName
			///  Desc: Returns the name of the particle
			/// Param: void
			inline _tstring GetName(void) { return m_strName; }

			///  Name: Element::SizeMul
			///  Desc: Returns a reference to the size multiplier
			inline float& SizeMul() { return m_SizeMul; }

			///  Name: Element::GetAge
			///  Desc: Returns the age of the particle
			inline float GetAge() { return m_Age; }
		};

		// Creates particles in the parent System accordign to rules loaded from xml.
		class ParticleEmitter
		{
		private:
			// Owner system
			ParticleSystem *m_pOwner;

			// Particle template for all Elements emitted by this object
			ParticleElement m_Template;

			// Rotation of the emitter versus time
			// Please note that no angular kinematics are performed, this data
			// is only for rendering purposes
			bool m_bRandomRotationDirection; // Choose rotation direction apon creation randomly?
			bool m_bRotationDirection;       // Left or right?
			vec3 m_vRotationAxis;        // Rotation axis
			ParticleGraph m_RotationGraph;           // Rotation in radians over time
			float m_RotationImmediate;       // Immediate rotation in radians for this cycle

			// Emission data
			float m_HotspotRadius;         // Radius of sphere where there is a full probability of a particle
			float m_FalloffRadius;         // Radius of area where probabilty drops to zero.
			ParticleGraph m_EmissionRateGraph;     // Rate of emission over time
			float m_EmissionRateImmediate; // Immediate Rate of emission

			// Particle stats on emission
			ParticleGraph m_SpeedGraph;     // Speed in direction of emitter on emission over time
			float m_SpeedImmediate; // Immediate Speed in direction of emitter on emission over time

			ParticleGraph m_SizeMulGraph;     // Size multiplier on emission over time
			float m_SizeMulImmediate; // Immediate Size multiplier on emmision

			ParticleGraph m_LifeSpanGraph;     // Element life span on emission over time
			float m_LifeSpanImmediate; // immediate Element life span on emission

			// Whether it loops or not, how many times does it loop.
			float m_Time;
			float m_Length;
			bool m_bLooping;
			int m_iNumCycles; // Number of cycles remaining. (If < 0, then infinite)

		public:
			///  Name: Emitter::Emitter
			///  Desc: Constructor
			/// Param: CPropBag &Bag  --  Reference to a loaded xml data class
			ParticleEmitter(CPropBag &Bag, ParticleSystem &Owner);

			///  Name: Emitter::Emitter
			///  Desc: Copy Constructor
			ParticleEmitter(const ParticleEmitter &emitter);

			/// Name: operator=
			/// Desc: Assignment Operator
			ParticleEmitter &operator=(const ParticleEmitter &emitter);

			///  Name: Emitter::Update
			///  Desc: Updates the emitter
			/// Param: float dTime  --  Delta time (milliseconds) since last update
			void Update(float dTime);

			///  Name: Emitter::IsDead
			///  Desc: Is the particle dead?
			/// Param: void
			inline bool IsDead(void)
			{
				if(m_bLooping == false)
				{
					return(m_Time>m_Length);
				}
				else
				{
					return(m_iNumCycles==0); // if it is negative, then the pfx should be kept alive indefinitely
				}
			}

			///  Name: Emitter::Kill
			///  Desc: Kills the emitter
			/// Param: none
			void Kill()
			{
				m_Time = m_Length + 1.0f; // Run past the length of the animation
				m_iNumCycles = 0;         // Do not loop
			}

			/**
			Gets the falloff radius of the emitter
			@return the falloff radius of the emitter
			*/
			float getFalloffRadius(void) const
			{
				return m_FalloffRadius;
			}
		};

		// Contains a list of particle emitters and manages all particles created by them.
		class ParticleSystem : public ParticleBody
		{
		public:
			enum EMIT_MAX_BEHAVIOR
			{
				EMIT_WAIT,
				EMIT_REPLACE_OLDEST,
				EMIT_REPLACE_RANDOM
			};

		private:
			// List of particle templates loaded at creation
			std::multimap<_tstring, ParticleElement> m_Templates;

			// List of materials loaded at creation
			vector<Material> m_Materials;

			// Holds the current frame of animation for all materials
			vector<float> m_Frames;

			// List of all emitters managed by the System
			vector<ParticleEmitter> m_Emitters;

			// Particles in the system
			size_t m_nMaxParticles;
			ParticleElement **m_ppElements; // Array of dynamically allocated particles
			ParticleElement **m_ppSorted; // Particles sorted by z depth
			EMIT_MAX_BEHAVIOR m_Behavior;

		public:
			///  Name: System::System
			///  Desc: Constructor
			/// Param: void
			ParticleSystem(void)
			: ParticleBody(),
			  m_nMaxParticles(0),
			  m_ppElements(NULL),
			  m_Behavior(EMIT_WAIT)
			{}

			///  Name: System::System
			///  Desc: Constructor
			/// Param: _tstring strFile  --  File to load xml from
			ParticleSystem(_tstring strFile)
			: ParticleBody(),
			  m_nMaxParticles(0),
			  m_ppElements(NULL),
			  m_Behavior(EMIT_WAIT)
			{
				CPropBag Bag;
				Bag.Load(strFile);
				LoadXml(Bag);
			}

			///  Name: System::System
			///  Desc: Constructor
			/// Param: CPropBag &Bag  --  Reference to a loaded xml data class
			ParticleSystem(CPropBag &Bag);

			///  Name: System::System
			///  Desc: Copy Constructor
			ParticleSystem(const ParticleSystem &system);

			/** Destructor */
			virtual ~ParticleSystem(void);

			///  Name: System::LoadXml
			///  Desc: Loads data from xml and returns success (true) or failure (false)
			/// Param: CPropBag &Bag  --  Reference to a loaded xml data class
			bool LoadXml(CPropBag &Bag);

			///  Name: System::Render
			///  Desc: Renders the system
			/// Param: void
			void Render(void);

			///  Name: System::Update
			///  Desc: Updates the system
			/// Param: float dTime  --  Delta time (milliseconds) since last update
			void Update(float dTime);

			///  Name: System::Spawn
			///  Desc: Spawns a new particle
			/// Param: const Element &element  --  The particle to copy
			void Spawn(const ParticleElement &element);

			///  Name: System::GetTemplate
			///  Desc: Returns a copy of the template element specified
			/// Param: _tstring strName  --  Name of the template element
			ParticleElement GetTemplate(_tstring strName);

			///  Name: System::GetMaterialHandle
			///  Desc: Returns a handle to the material specified
			/// Param: _tstring strName  --  Name of the material
			size_t GetMaterialHandle(const _tstring &strName) const;

			///  Name: System::GetMaterial
			///  Desc: Returns the material specified
			/// Param: size_t nHandle  --  Handle of the material
			Material& GetMaterial(size_t nHandle);

			///  Name: System::IsDead
			///  Desc: Polls the emitters and returns true if all emitters are dead, else returns false
			/// Param: none
			bool IsDead();

			///  Name: System::Kill
			///  Desc: Kills each particle emitter
			/// Param: none
			void Kill();

			/**
			Retrieves the maximum falloff radius of the particle system
			@return falloff radius
			*/
			float getRadius(void) const;

			void setPosition(const vec3 &position)
			{
				m_vInitialPosition = position;
				center = position;
				initialVelocity.zero();
			}
		};

} // namespace Engine

typedef vector<Engine::ParticleSystem*> PARTICLE_SYSTEMS;

#endif
