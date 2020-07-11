#pragma once

#include "Hazel/Hazel.h"
#include "imgui/imgui.h"

#include "Hazel/Ocean/Ocean.h"
#include "Hazel/Ocean/WaterPhysicsEngine.h"
#include "Hazel/Entity/Boat.h"

class TestEnvironment : public Hazel::Layer
{
public:
	TestEnvironment();

	void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
private:
	bool OnKeyPressed(Hazel::KeyPressedEvent& e);
	Hazel::CameraController m_CameraController;
	Hazel::Ocean m_Ocean;
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VA;
	Hazel::Ref<Hazel::Texture2D> m_Texture;
	glm::vec4 c_Emissive;
	glm::vec4 c_Ambient;
	glm::vec4 c_Diffuse;
	glm::vec4 c_Specular;

	Hazel::Library m_Library;
	Hazel::Boat m_Boat;
	Hazel::BoatController m_BoatController = Hazel::BoatController::BoatController(m_Boat);

	Hazel::WaterPhysicsEngine m_WPE;
};