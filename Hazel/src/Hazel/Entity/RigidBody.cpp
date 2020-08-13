#include "hzpch.h"
#include "RigidBody.h"

namespace Hazel
{
	RigidBody::RigidBody(glm::mat3 Ibody, float mass)
		: I(glm::inverse(Ibody)), m(mass)
	{
	}
	void RigidBody::Update(glm::vec3 F, glm::vec3 T, float dt)
	{
		// F = force, T = torque, dt = interval
		// I update X and R first, then P and L. Not sure if this is the best way to go.
		// source: https://www.cs.cmu.edu/~baraff/sigcourse/notesd1.pdf (page 15)
		x += P / m * dt;

		// Matrix rotation
		_I = R * I * glm::transpose(R); // I is already: inverse(Ibody). (see constructor. _I is thus already I inverse.)
		w = _I * L;
		//R += Cross(w) * R * dt;
		// OR
		// Quaternion rotation
		q += Quaternion(0, (w / 2.0f)) * q * dt;
		R = q.toMatrix();

		P += F * dt;
		L += T * dt;

		// Drag
		//P[1] -= P[1] * 0.05;
		//L[0] -= L[0] * 0.05;
		//L[1] -= L[1] * 0.05;
		//L[2] -= L[2] * 0.05;
		
	}

	glm::mat3 RigidBody::Cross(glm::vec3 v)
	{
		return glm::mat3(0, v[2], -v[1], -v[2], 0, v[0], v[1], -v[0], 0);
	}

}