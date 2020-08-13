#pragma once


#include "Library.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/KeyEvent.h"

#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Entity/Model.h"

#include "Hazel/Ocean/WaterPhysicsEngine.h"
#include "Hazel/Entity/RigidBody.h"

namespace Hazel
{

	class Boat
	{
	public:
		Boat();
		void SetModel(Ref<Hazel::Model>& model) { m_Model = model; }
		void SetShader(Ref<Hazel::Shader>& shader) { m_Shader = shader; }

		void Render();
		void Render(glm::mat4& transform); // to be deleted

		Ref<Model> GetModel() { return m_Model; }
		Ref<Shader> GetShader() { return m_Shader; }
	public:
		void Steer(bool ccw);
		void OnUpdate(Timestep ts, WaterPhysicsEngine& WPE);

		
		glm::vec2 GetDirection() { return m_Direction; }
		glm::mat4 GetTranform() { return m_Transform; }
	private:
		// rendering
		Ref<Hazel::Model> m_Model;
		Ref<Hazel::Shader> m_Shader;
		// vars
		glm::mat3 I = glm::mat3(15.2416, 0.0000, -0.0000, 0.0000, 21.2211, -0.0000, -0.0000, -0.0000, 15.2416);
		float M = 28.8000;
		RigidBody rb = RigidBody(I, M);
		glm::mat4 m_Transform = glm::translate(glm::mat4(rb.R), rb.x);
		// Boat will have a RigidBody object
		

		// old
		glm::vec3 m_Direction = { 0.0, 0.0, 1.0 };		// Direction
		float m_DirectionAngle = 0.0;					// Direction as an angle (used only upon Steer() to calculate the new direction)
		
		glm::vec3 m_Normal = { 0.0, 1.0, 0.0 };			// Normal
		float m_Maneuverability = 2.5;					// Maneuverability
		

	};

	class BoatController
	{
	public:
		BoatController(Boat& boat);

		Boat& GetBoat() { return m_Boat; }
		void SetBoat(Boat& boat) { m_Boat = boat; }

		void OnEvent(Event& e);
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
	private:
		Boat& m_Boat;
	};

}