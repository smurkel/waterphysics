#include "hzpch.h"
#include "WaterPhysicsEngine.h"
#include <glad/glad.h>

namespace Hazel
{
	WaterPhysicsEngine::WaterPhysicsEngine()
	{
		glGenBuffers(1, &b_SSBO);
	}

	glm::vec4 WaterPhysicsEngine::FloatingObjectTorque(Floater* floaters, int nfloaters, glm::mat4& transform)
	{
		cs_FloatingObjectTransform->Bind();
		int m_L = m_Ocean->GetL();
		uint32_t m_N = m_Ocean->GetN();

		std::dynamic_pointer_cast<OpenGLShader>(cs_FloatingObjectTransform)->UploadUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(cs_FloatingObjectTransform)->UploadUniformFloat("u_L", m_L);
		std::dynamic_pointer_cast<OpenGLShader>(cs_FloatingObjectTransform)->UploadUniformInt("u_N", m_N);

		glBindImageTexture(0, _x->GetRendererID(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(1, _y->GetRendererID(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(2, _z->GetRendererID(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, b_SSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, nfloaters * sizeof(Floater), floaters, GL_DYNAMIC_DRAW);
		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, b_SSBO);
		glDispatchCompute(1, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		Floater* _floaters = (Floater*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, nfloaters * sizeof(Floater), bufMask);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glm::vec4 torque_force(0,0,0,0);
		for (int i = 0;i < nfloaters;i++)
		{
			Floater temp = *(_floaters + i);
			torque_force += glm::vec4(temp.x, temp.y, temp.z, temp.w);
			HZ_CORE_INFO("F = {0}", temp.w);
		}
		//HZ_CORE_INFO("Torque: {0}, {1}, {2}, {3}.", torque_force.x, torque_force.y, torque_force.z, torque_force.w);
		return torque_force;
	}

}
