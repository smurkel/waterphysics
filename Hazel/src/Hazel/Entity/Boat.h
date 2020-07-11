#pragma once


#include "Library.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/KeyEvent.h"

#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Entity/Model.h"

#include "Hazel/Ocean/WaterPhysicsEngine.h"

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

		glm::vec3 GetPosition() { return m_Position; }
		glm::vec2 GetDirection() { return m_Direction; }
		float GetVelocity() { return m_Velocity; }

		void SetVelocity(float velocity) { m_Velocity = velocity; }

	private:
		// rendering
		Ref<Hazel::Model> m_Model;
		Ref<Hazel::Shader> m_Shader;
		// vars
		glm::mat4 m_Transform = glm::mat4(1.0);
		// gamevars
		// 2020 06 29
		glm::vec3 m_Position = { 0.0, 0.0, 0.0 };		// P
		float m_Velocity = 0.0;							// V
		float m_VelocityVertical = 0.0;					// V_y
		glm::vec3 m_AngularMomentum = { 0.0, 0.0, 0.0 };// L
		float m_Weight = 9 * 10;							// m


		// old
		glm::vec3 m_Direction = { 0.0, 0.0, 1.0 };		// Direction
		float m_DirectionAngle = 0.0;					// Direction as an angle (used only upon Steer() to calculate the new direction)
		
		glm::vec3 m_Normal = { 0.0, 1.0, 0.0 };			// Normal
		float m_Maneuverability = 2.5;					// Maneuverability
		

		Floater* m_Floaters;
		int m_nFloaters;

		void SetFloaters();
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