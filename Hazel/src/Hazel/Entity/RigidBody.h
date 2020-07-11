#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "Hazel/Entity/Model.h"

namespace Hazel
{

	class RigidBody
	// Physics implementation only! Leave rendering and such to the owner.
	{
	public:
		RigidBody(PhysicsProbe* probes, float n_probes) { m_Probes = probes; m_ProbeCount = n_probes; }

		void CalculateMomentOfInertia();

	public:
		// point-particle
		glm::vec3	P	= { 0.0, 0.0, 0.0 };	// center of mass position
		glm::vec3	V	= { 0.0, 0.0, 0.0 };	// center of mass velocity
		float		m	= 0;					// total mass

		// rigid-body
		glm::vec3	w	= { 0.0, 0.0, 0.0 };	// angular velocity
		glm::mat3	I	= glm::mat3(1.0);		// moment of inertia

		// bookkeeping
		PhysicsProbe*	m_Probes;
		float			m_ProbeCount;
	};

}