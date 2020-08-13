#include "hzpch.h"
#include "Boat.h"

#include "Hazel/Input.h"
#include "Hazel/Keycodes.h"


namespace Hazel
{
	Boat::Boat()
	{
	}

	void Boat::Render()
	{
		Renderer::Submit(this->GetShader(), this->GetModel()->GetVA(), this->GetModel()->GetTexture(), m_Transform);
	}

	void Boat::Render(glm::mat4& transform)
	{
		Renderer::Submit(this->GetShader(), this->GetModel()->GetVA(), this->GetModel()->GetTexture(), transform);
	}

	void Boat::OnUpdate(Timestep ts, WaterPhysicsEngine& WPE)
	{
		PhysicsProbeOutput net = WPE.FloatingObjectTorque(this->GetModel()->GetProbes(), this->GetModel()->GetProbeCount(), m_Transform, rb);
		
		rb.Update(net.force, net.torque, ts);

		m_Transform = glm::translate(glm::mat4(rb.R), glm::vec3(rb.x));
	}

	void Boat::Steer(bool ccw)
	{
		if (ccw)
		{
			m_DirectionAngle += m_Maneuverability;
		}
		else
		{
			m_DirectionAngle -= m_Maneuverability;
		}
		float cos = glm::cos(glm::radians(m_DirectionAngle));
		float sin = glm::sin(glm::radians(m_DirectionAngle));
		glm::vec3 newDirection;
		m_Direction.x = sin;
		m_Direction.z = cos;
	}


	// BOAT CONTROLLER

	BoatController::BoatController(Boat& boat)
		: m_Boat(boat)
	{

	}

	void BoatController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(BoatController::OnKeyPressed));
	}

	bool BoatController::OnKeyPressed(KeyPressedEvent& e)
	{
		glm::vec2 temp = m_Boat.GetDirection();
		//HZ_CORE_INFO("GET: m_Direction.x = {0}, m_Direction.y = {1}", temp.x, temp.y);
		if (e.GetKeyCode() == HZ_KEY_A)
		{
			m_Boat.Steer(true);
			return true;
		}
		else if (e.GetKeyCode() == HZ_KEY_D)
		{
			m_Boat.Steer(false);
			return true;
		}

		if (e.GetKeyCode() == HZ_KEY_W)
		{
			// hard coded max velocity
		}
		else if (e.GetKeyCode() == HZ_KEY_S)
		{
			// hard coded min velocity
			
		}
		return false;
	}
	
	

}