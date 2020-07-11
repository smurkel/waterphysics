#pragma once

#include "Ocean.h"
#include "Platform/OpenGL/OpenGLShader.h"


#include <glm/gtc/matrix_transform.hpp>
#include "Hazel/Renderer/Shader.h"

namespace Hazel
{

	struct Floater
	{
		float x, y, z, w;
	};

	class WaterPhysicsEngine
	{
	public:
		WaterPhysicsEngine();

		glm::vec4 FloatingObjectTorque(Floater* floaters, int nfloaters, glm::mat4& transform);

		void SetOcean(Ocean* ocean) { m_Ocean = ocean; _x = ocean->GetDisplacementMap(0); _y = ocean->GetDisplacementMap(1); _z = ocean->GetDisplacementMap(2); }
	private:
		Ref<Shader> cs_FloatingObjectTransform = Shader::Create("assets/shaders/compute/BoatTransform.glsl");
		uint32_t b_SSBO;

	public:
		OceanTexture* _x = 0;
		OceanTexture* _y = 0;
		OceanTexture* _z = 0;
		Hazel::Ocean* m_Ocean;
	};

	

}